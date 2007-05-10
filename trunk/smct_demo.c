 /*
 * smct_demo - run a client program, via port 80. View and work with
 *             smct formats.
 *
 * Copyright (C) Paulino Huerta Sanchez
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */
#include<signal.h>
#include<stdio.h>
#include<sys/wait.h>
#include<sys/socket.h>
#include<string.h>
#include<fcntl.h>
#include<netdb.h>
#include<netinet/in.h>
#include<sys/time.h>
int comunica(void);
void cargar_file(char *,char *,char *,char *);
void para_zombis(int sig);
void say_who_called(struct sockaddr_in *addrp);
void mostrar_preg_resp(char **preg,char **resp,char *text,char *npreg,int desde,int hasta);
void  fun_saq(char **,int,char **);
void  fun_examen(char **,int,char **);
void  fun_ficha(char **,int,char **);
void  fun_array(char **);
void  fun_enviar(char **);
char  *obtener_preg(char **,char *,char **,FILE *,FILE *);
void  fun_test(char **,char *,char *,int);
void liberar(char **);
void grabar_resp(char **,char *,int);
void devolver(char **);
void salir(int sig);
void grabar_temporal(char *);
//void ejecuta_funciones(char *env_resp,int,int);

void ejecuta_funciones(char *env_resp,int,int,struct timeval,int,
                    struct sockaddr_in, struct sockaddr_in, int,int);
void ejecuta_coor(int fp,struct timeval value,int val_size,
                 struct sockaddr_in saddr,struct sockaddr_in faddr,
		 int faddrlen,int pid);
int primario=0;
int pag_actual=0;
int tpag=0;
int salir_bucle=0;
		       
main(int argc, char **argv) {
  struct sockaddr_in faddr,saddr;
  struct hostent *hp;
  int fp,faddrlen,fd,corregido,tipo_pres,examen,saq,error,len_error,val_size;
  int una_opcion,tipo_note,cordinado;
  int ficha,ficha_entry,passwd;
  char nombre[250],servidor[250],puerto[30], correg[250],pass[250],temas[250];
  char tipo[500];
  struct timeval value;
  long msglen,n;
  int ret,leidos;
  char peticion[500],respuesta[64000],res[64000],name_file[50];
  char buf[70000],s9[60],buf1[250];
 int cont,cont_resp,act,ind,b,j,i,ini, pid,desde,hasta,top,argumento,cont_preg;
 int hay_preg,id,tipo_test;
 char *p3,*p2,*p,*p1,sop[300],s2[BUFSIZ],s1[BUFSIZ],st[BUFSIZ],s[300],tit[500],op[5][100], tit1[600], tit2[600], preguntas[60000];
 char etiq[200],text[BUFSIZ*2],stp[BUFSIZ];
 char *ar[300],*ar_sop[1500], *respuestas[300], preg[BUFSIZ],*ar_preg[200];
 int aplicar,resp_mod;
 char env_resp[60000],sp[500];
 char descarte[300], una_contesta[BUFSIZ];
int sop_hasta[500],sop_desde[500];
 char st_arg[100]; int t;  char *ar_arg[20];
 
 FILE *fp0,*fp1,*fp2;
 int pi[2];
 signal(SIGCHLD,para_zombis);
 value.tv_sec=0;
 value.tv_usec=100000;
 val_size=sizeof(value);
 strcpy(name_file,"/tmp/FILE1");
  len_error=sizeof(error);
  tipo_pres=una_opcion=tipo_note=examen=saq=corregido=ficha=passwd=cordinado=0;
  tipo_test=primario=0;
  fp1=popen("greq -t'Hola, primero conectamos' -e'Dir. IP de tu servidor' -d ssmct.berlios.de -ePuerto -d 80 -m 'Tipo Trabajo' Test Examen Presentación Fichas/Cuestionario Resp.Simples UnaOpción Notebook","r");
  if(!fp1) {
     puts("NO pudo ejecutar grep, sale");
     exit(1);
  }
  p=fgets(sp,500,fp1);
  if(!p) {
    puts("No leyó. Sale");
    exit(1);
  }
  pclose(fp1);
  
  sscanf(sp,"%s\v%s\v%s",servidor,puerto,tipo);
  // Averiguar host
 hp=gethostbyname(servidor);
 if(hp==NULL) {
  printf("No existe ese nombre de servidor\nPuedes probar con la dirección IP");  exit(1);
 }
  bzero(&saddr,sizeof(saddr));
  bcopy(hp->h_addr,(struct sockaddr *)&saddr.sin_addr, hp->h_length);           
  //
  faddrlen=sizeof(saddr);
  saddr.sin_family=AF_INET;
  saddr.sin_port=htons(atoi(puerto));
  // inet_aton(servidor,&saddr.sin_addr);
  fp=socket(AF_INET,SOCK_STREAM,0);
  ret=connect( fp, (struct sockaddr *) &saddr, faddrlen);
  if(ret==-1) {
     perror("No conecta con el servidor");
     exit(1);
  }
  switch(*tipo) {
    case 'E': examen=1;
              break;
    case 'T': tipo_test=1;
              break;
    case 'F': ficha=1;
              break;
    case 'N': tipo_note=1;
              break;
    case 'P': tipo_pres=1;
              break;
    case 'U': una_opcion=1;
              break;
    case 'R': saq=1;
              break;
    default : exit(1);
  }
  if(tipo_test) {
    sprintf(peticion,"GET http://ssmct.berlios.de/test HTTP/1.0\n\n");
    strcpy(name_file,"/tmp/FILE0");
  }
  if(examen) {
    sprintf(peticion,"GET http://ssmct.berlios.de/examen HTTP/1.0\n\n");
  }
  if(saq) {
    sprintf(peticion,"GET http://ssmct.berlios.de/saq HTTP/1.0\n\n");
  }
  if(una_opcion) {
    sprintf(peticion,"GET http://ssmct.berlios.de/unaOpcion HTTP/1.0\n\n");
  }
  if(tipo_pres) {
    sprintf(peticion,"GET http://ssmct.berlios.de/presentacion HTTP/1.0\n\n");
    strcpy(name_file,"/tmp/FILE0");
  }
  if(ficha) {
    sprintf(peticion,"GET http://ssmct.berlios.de/ficha HTTP/1.0\n\n");
  }
  if(tipo_note) {
    sprintf(peticion,"GET http://ssmct.berlios.de/notebook HTTP/1.0\n\n");
  }
  write(fp,peticion,strlen(peticion));
  while(1) {
   leidos=read(fp,res,64000);
   if(leidos==0) {
     break;
   }
   res[leidos]='\0';
   strcat(respuesta,res);
  }
  p=strstr(respuesta,"\n\r\n");
  fd=open(name_file,O_WRONLY|O_CREAT|O_TRUNC,0644);
  if(fd==-1) {perror("no puede escribir en tmp"); exit(1); }
  write(fd,p+3,strlen(p+3));
  close(fd);
  // "contamos preguntas del fichero"
 fp0=fopen(name_file,"r");
 if(fp0==NULL) {puts (" * No puede abrir file en /tmp  *");exit(1); }
 hay_preg=0;
 while(fgets(st,BUFSIZ,fp0)) {
         printf("%s",st);
	 if(*st=='\n')
             hay_preg++;	
 }
 fclose(fp0);
 
 if(pipe(pi)==-1) {puts("error en pipe"); exit(1); }
 pid=fork();
 if(pid==-1) {puts("error en fork"); exit(1); }
 if(pid==0) { 
   fflush(stdout);
   close(pi[0]);
   dup2(pi[1],1);
         execlp("/usr/local/bin/greq","/usr/local/bin/greq","-t",
           "Identificación",
           "-eApellidos y Nombre",
          NULL);
 }
 close(pi[1]);
 s2[0]='\0';
 n=read(pi[0],s2,BUFSIZ);
 wait(NULL);
 s2[n]='\0';
 if(*s2=='\0') {
   exit(1);
 }
 strcpy(nombre,s2);
 if(una_opcion) {
    // ejecuta funciones en un hijo;
    sprintf(env_resp,"\n::%s",nombre);
    id=0;
    strcat(env_resp,":");
    ejecuta_funciones(env_resp,id,fp,value,val_size,saddr,faddr,
                  faddrlen,cordinado);
    grabar_temporal(env_resp);
    exit(1); 
 }
 if(tipo_note) {
    sprintf(env_resp,"::%s",nombre);
    id=1;
    strcat(env_resp,"\v");
    ejecuta_funciones(env_resp,id,fp,value,val_size,saddr,faddr,
                  faddrlen,cordinado);
    grabar_temporal(env_resp);
    exit(1); 
 }
 if(tipo_pres) {
    sprintf(env_resp,"::%s\v",nombre);
    id=0;
    cargar_file(name_file,"/tmp/FILE1","/tmp/FILE2","/tmp/FILE3");
    ejecuta_funciones(env_resp,id,fp,value,val_size,saddr,faddr,
                  faddrlen,cordinado);
    grabar_temporal(env_resp);
    exit(1); 
 }
 if(tipo_test) {
    sprintf(env_resp,"\n::%s\n",nombre);
    id=0;
    cargar_file(name_file,"/tmp/FILE1","/tmp/FILE2",NULL);
    fun_test(respuestas,"/tmp/FILE1","/tmp/FILE2",hay_preg/2);
    grabar_resp(respuestas,env_resp,examen) ;
    grabar_temporal(env_resp);
    exit(1); 
 }
 if(ficha) {
   fun_array(ar_preg);
   fun_ficha(ar_preg,hay_preg,respuestas);
   sprintf(env_resp,"\n::%s",nombre);
   strcat(env_resp,"\n");
   grabar_resp(respuestas,env_resp,examen) ;
   grabar_temporal(env_resp);
   devolver(ar_preg);
   devolver(respuestas);
   exit(1);
 }
 if(examen) {
   fun_array(ar_preg);
   fun_examen(ar_preg,hay_preg,respuestas);
   sprintf(env_resp,"::%s\v",nombre);
   grabar_resp(respuestas,env_resp,examen) ;
   grabar_temporal(env_resp);
   devolver(ar_preg);
   devolver(respuestas);
   //unlink("/tmp/FILE");
   //unlink("/tmp/FILE1");
   //unlink("/tmp/FILE2");
 }
 if(saq) {
   fun_array(ar_preg);
   fun_saq(ar_preg,hay_preg,respuestas);
   sprintf(env_resp,"\n::%s\n",nombre);
   grabar_resp(respuestas,env_resp,examen) ;
   grabar_temporal(env_resp);
   devolver(ar_preg);
   devolver(respuestas);
   exit(1);
 }
}

void fun_test(char **respuestas, char *name1, char *name2, int hay_preg) {
  char *ar[300],preg[BUFSIZ*4],*ar_sop[20], tit[500], s2[BUFSIZ],*p2,etiq[500];
  FILE *fp0,*fp1;
  int i,n,pid,cont,pi[2],cont_resp;
  
  s2[0]='\0';
  cont_resp=0;
  cont=0;
  fp0=fopen(name1,"r");
  fp1=fopen(name2,"r");
  if(!fp0 || !fp1) {
    printf("Error al abrir Fichero en /tmp\n");
    exit(1);
  }
  ar[0]=(char*)"/usr/local/bin/greq";
  ar[1]=(char*) "-t";
  ar[2]=(char *)"Mi Programa";
  ar[3]=(char *) tit;
  ar[4]=(char*)"-z";
  ar[5]=(char *) preg;
  ar[6]=(char*)"-m";
  ar[7]=(char *) etiq;
  *preg='\0';
  while(obtener_preg(ar,preg,ar_sop,fp0,fp1)) {
     cont++;
     sprintf(tit,"-X%60s %s  %d / %d%50s"," ","Preguntas  ",cont,hay_preg," ");
     sprintf(etiq,"%s %d","Resp. ",cont);
     if(pipe(pi)==-1) {puts("error en pipe"); exit(1); }
     pid=fork();
     if(pid==-1) {puts("error en fork"); exit(1); }
     if(pid==0) { 
       close(pi[0]);
       dup2(pi[1],1);
       execvp(ar[0],ar);
       printf("EEEE\n");
      // con error
     }
     close(pi[1]);
     s2[0]='\0';
     n=read(pi[0],s2,BUFSIZ);
     wait(NULL);
     s2[n]='\0';
     p2=s2;
     if(*s2=='\0') {
       unlink("/tmp/FILE1");
       unlink("/tmp/FILE2");
       exit(1);
     }
     for(i=0;p2[i]!='\0';i++) {
       if(p2[i]=='\v')
         p2[i]=' ';
       if(p2[i]=='\n') {
         p2[i]='\0';
         respuestas[cont_resp]=(char *)malloc(strlen(p2)+1);
         strcpy(respuestas[cont_resp],p2);
         p2+=i+1;
         i=-1;
         cont_resp++;
       }
     }
  //liberar(ar_sop);
  //liberar(ar+8);
     *preg='\0';
 }
 respuestas[cont_resp]=NULL;
}

void liberar(char **ar){ 
   while(*ar) {
     free(ar);
     ar++;
   }
}

char *obtener_preg(char **ar,char *preg,char **ar_sop,FILE *fp0,FILE *fp1) {
   char *p, *p1, s[BUFSIZ],sop[BUFSIZ],stp[BUFSIZ],etiq[500];
   int cont, cont_preg,act,ini,in,b,ind;
   cont=ini=cont_preg=act=b=0;
   ind=8;
   *preg='\0';
   while( (p=fgets(s,BUFSIZ,fp0)) && *s!='\n' ) {
     strcat(preg,s);
   }
   if(p==NULL) {  //  fin de fichero
     return NULL;
   }
   while( (p1=fgets(sop,BUFSIZ,fp1)) && *sop!='\n' ) {
      ar_sop[act]=(char *)malloc(strlen(sop)+1);
      strcpy(ar_sop[act],sop);
      act++;
   }
   ar_sop[act]=NULL;
   for(;ini<act;ini++)
      ar[ind++]=ar_sop[ini];
   ar[ind]=NULL;
   return preg;
}

//
//
void fun_ficha(char **ar_preg,int hay_preg,char **respuestas) {
int hay,desde,i,j;
char ejecuta[BUFSIZ],una_contesta[BUFSIZ],tit[300], *p, contesta[500], resp1[500];
FILE *fp1;
char car='\v';
int hay_pprof;
hay_pprof=hay_preg;
for(i=0;ar_preg[i]!=NULL;i++) {
   if((strlen(ar_preg[i])==2) && (*ar_preg[i]=='*')) {
     hay_pprof=i;
     break;
   }
}
for(i=0,j=1;i<hay_pprof;i++,j++) {
   strcpy(ejecuta,"greq -t'Preguntas' ");
   sprintf(tit,"-X'%65s %s  %d / %d%65s'%s"," ","Tema:  ",
              j,hay_pprof/2," "," -Z\"");
   strcat(ejecuta,tit);
   strcat(ejecuta,ar_preg[i]);
   strcat(ejecuta,"\" -a -o'Respuesta'");
   fp1=popen(ejecuta,"r");
   if(!fp1) {
     puts("NO pudo ejecutar grep, sale");
     exit(1);
   }
   pclose(fp1);
   strcpy(ejecuta,"greq -t'Preguntas' ");
   //
   sprintf(tit,"-N'%65s %s  %d / %d%60s%c%s'%s"," ","Tema:  ",
              j,hay_pprof/2," ",car,"Respondemos"," -Z  \"");
   strcat(ejecuta,tit);
   strcat(ejecuta,ar_preg[i]);
   strcat(ejecuta,"\" -Z \"");
   strcat(ejecuta,ar_preg[++i]);
   strcat(ejecuta,"\"");
	      
   /*
   strcat(ejecuta,tit);
   strcat(ejecuta,ar_preg[i]);
   strcat(ejecuta,"\" -X'Respondemos' -Z \"");
   strcat(ejecuta,ar_preg[++i]);
   strcat(ejecuta,"\"");
   */
   fp1=popen(ejecuta,"r");
   if(!fp1) {
     puts("NO pudo ejecutar grep, sale");
     exit(1);
   }
   pclose(fp1);
 }
 desde=hay_pprof+1;
 hay=hay_preg - desde;
 fun_saq(ar_preg+desde,hay,respuestas);
}

void fun_saq(char **ar_preg,int hay_preg,char **respuestas) {
int i,j,larga;
char ejecuta[BUFSIZ],tit[300], *p, contesta[BUFSIZ], resp1[BUFSIZ];
FILE *fp1;

for(i=0;ar_preg[i]!=NULL;i++) {
   strcpy(ejecuta,"greq -t'Preguntas' ");
   sprintf(tit,"-X'%60s %s  %d / %d%50s'%s"," ","Pregunta:  ",
              i+1,hay_preg," "," -z \"");
   strcat(ejecuta,tit);
   larga=0;
   if(ar_preg[i][0]=='*' && ar_preg[i][1]==' ') {
       // ajustar
       ar_preg[i]=&(ar_preg[i][2]);
       larga=1;
   }
   strcat(ejecuta,ar_preg[i]);
   if(larga)
      strcat(ejecuta,"\" -e'^Tu respuesta'");
   else
      strcat(ejecuta,"\" -e'Tu respuesta'");
   fp1=popen(ejecuta,"r");
   if(!fp1) {
     puts("NO pudo ejecutar greq, sale");
     exit(1);
   }
  p=fgets(contesta,BUFSIZ,fp1);
  if(!p) {
    puts("Cancelado");
    unlink("/tmp/FILE1");
    exit(1);
  }
  pclose(fp1);
  for(j=0;contesta[j];j++) {
    if(isupper(contesta[j]))
      contesta[j]+=32;
    if(contesta[j]==':')
      contesta[j]=' ';
    if(contesta[j]=='"')
      contesta[j]=' ';
  }
  for(j-=1;j>=0&&(contesta[j]==' '||contesta[j]=='\t');j--);
  contesta[j+1]='\0'; 
  strcpy(resp1,contesta);
  printf("*%s*\n",resp1);
 // cargar respuesta en array
  respuestas[i]=(char *)malloc(strlen(resp1)+1);
  strcpy(respuestas[i],resp1);
  if(larga) {
       ar_preg[i]=ar_preg[i]-2;
       printf("!!!!%s!!!\n",ar_preg[i]);
  }
    
}
//
respuestas[i]=NULL;
}

void mostrar_preg_resp(char **preg,char **resp,char *text,char *npreg,int desde,int hasta) {
	int i;
	char s[200];
	for(i=desde;i<=hasta;i++) {
		strcat(text,preg[i]);
                strcat(text,"\nTu respuesta es: ");
                strcat(text,resp[i]);
                strcat(text,"\n\n");
		sprintf(s,"%d\v",i+1);
		strcat(npreg,s);
	}
}
void grabar_resp(char **resp,char *s,int examen) {
    int i,j;
    char s1[50000];
    FILE *fp;
    for(i=0;resp[i]!=NULL;i++) {
      for(j=0;resp[i][j]==' ';j++);
      if(!examen)
        sprintf(s1,"%d:%s\n",i+1,resp[i]+j);
      if(examen) {
        if(i==0)
           sprintf(s1,"%d:%s\n",i+1,resp[i]+j);
        else
           sprintf(s1,"%c%d:%s\n",'\v',i+1,resp[i]+j);
      }
      strcat(s,s1);
    }
}
void grabar_temporal(char *s) {
    FILE *fp;
    if((fp=fopen("temporal","w"))!=NULL)
      fprintf(fp,"%s",s);
    else {
      puts("no ha podido grabar respuestas temporal");
    }
    fclose(fp);
}
void devolver(char **ar) {
	int i=0;
	while(ar[i]) {
	   free(ar[i++]);
	}
}
void say_who_called(struct sockaddr_in *addrp){
   char *host;
   int port,n;
   host=(char *)inet_ntoa(addrp->sin_addr);
   port=ntohs(addrp->sin_port);
   printf("  from:  %s:%d\n",host,port);
}

void fun_examen(char **ar_preg,int hay_preg,char **respuestas) {
int i,j;
char ejecuta[BUFSIZ],una_contesta[50000],tit[300], *p, contesta[BUFSIZ];
FILE *fp1;
char car='\v';

for(i=0;ar_preg[i]!=NULL;i++) {
   strcpy(ejecuta,"greq -t'Preguntas' ");
   //sprintf(tit,"-X'%60s %s  %d / %d%50s'%s"," ","Pregunta:  ",
              //i+1,hay_preg," "," -z \"");
   sprintf(tit,"-n'%65s %s  %d / %d%60s%c%s'%s"," ","Pregunta:  ",
              i+1,hay_preg," ",car,"Tu Respuesta"," -Z  \"");
   strcat(ejecuta,tit);
   strcat(ejecuta,ar_preg[i]);
   strcat(ejecuta,"\"");
   printf("***%s***\n",ejecuta);
   fflush(stdout);
   //strcat(ejecuta,"\" -x'Tu respuesta'");
   fp1=popen(ejecuta,"r");
   if(!fp1) {
     puts("NO pudo ejecutar grep, sale");
     exit(1);
   }
   *una_contesta='\0';
   p=fgets(contesta,BUFSIZ,fp1);
   if(p!=NULL){
        strcat(una_contesta,contesta+1);
        while((p=fgets(contesta,BUFSIZ,fp1))!=NULL) 
           strcat(una_contesta,contesta);
   }
   pclose(fp1);
 // cargar respuesta en array
   respuestas[i]=(char *)malloc(strlen(una_contesta)+1);
   printf("--%d--%d\n",i,strlen(una_contesta));
   strcpy(respuestas[i],una_contesta);
}
respuestas[i]=NULL;
}
//

int  contar_preguntas() {
  char st[BUFSIZ];
  FILE* fp0;
  int hay_preg=0;
  
  // "contamos preguntas del fichero"
 fp0=fopen("/tmp/FILE1","r");
 if(fp0==NULL) {puts ("no pueede abrir file en /tmp"); exit(1); }
 while(fgets(st,BUFSIZ,fp0)) {
	 if(*st=='\n')
             hay_preg++;	
 }
 fclose(fp0);
 return hay_preg;
}

void  fun_array(char **ar_preg) {
  char st[BUFSIZ],s[BUFSIZ], *p;
  FILE* fp0;
  int cont_preg=0;
  fp0=fopen("/tmp/FILE1","r");
  if(fp0==NULL) {puts ("no puee abrir file en /tmp"); exit(1); }
  st[0]='\0';
  while(1){
   while(p=fgets(s,BUFSIZ,fp0)) {
     if(s[0]=='\n') {
       ar_preg[cont_preg]=(char *)malloc(strlen(st)+1);
       strcpy(ar_preg[cont_preg],st);
       cont_preg++;
       *st='\0';
     }
     else
       strcat(st,s);
   }
   if(p==NULL) 
     break;
  }
  ar_preg[cont_preg]=NULL;
}

void ejecuta_funciones(char *env_resp,int id,int fp,struct timeval value,
                     int val_size, struct sockaddr_in saddr,
		     struct sockaddr_in faddr, int faddrlen,int cordinado) {
  int n,pi[2],pid;
  char s2[59800];
  if(pipe(pi)==-1) {puts("error en pipe"); exit(1); }
  pid=fork();
  if(pid==-1) {puts("error en fork"); exit(1); }
  if(pid==0) { 
    fflush(stdout);
    close(pi[0]);
    dup2(pi[1],1);
    if(env_resp[strlen(env_resp)-1]=='\v') {
       if(id==1) {
         if(cordinado){
	    if(!primario) {
               execlp("notebook_co","notebook_co","/tmp/FILE1",NULL);  
	    }
	    else
               execlp("notebook_pr","notebook_pr","/tmp/FILE1",NULL);  
	 }
	 else
            execlp("notebook","notebook","/tmp/FILE1",NULL);  
       }
       else {
 execlp("funciones","funciones","/tmp/FILE1","/tmp/FILE2","/tmp/FILE3",NULL);  
       }
    }
    else {
         execlp("/usr/local/bin/test","test","/tmp/FILE1",NULL);
    }
 printf("Error: No ejecuta funciones ó test ó note ó noteCord ó noteprimario");
    exit(1);
  }
  close(pi[1]);
  s2[0]='\0';
  n=read(pi[0],s2,60000);
  wait(NULL);
  s2[n]='\0';
  if(*s2=='\0')
    exit(1);
  strcat(env_resp,s2);
}

void cargar_file(char *f_origen,char *f_des1,char *f_des2,char *f_des3) {
    int cont;
    char s[500],s1[500];
    FILE *fp0,*fp1,*fp2,*fp3;
    char *separa="---------------";
    fp3=NULL;
    cont=0;
    fp0=fopen(f_origen,"r");
    if(fp0==NULL) {
      perror("Abriendo fichero origen");
      exit(1);
    }
    fp1=fopen(f_des1,"w");
    if(fp1==NULL) {
      perror("Abriendo fichero destino 1");
      exit(1);
    }
    fp2=fopen(f_des2,"w");
    if(fp2==NULL) {
      perror("Abriendo fichero destino 2");
      exit(1);
    }
    if(f_des3) {
      fp3=fopen(f_des3,"w");
      if(fp3==NULL) {
        perror("Abriendo fichero destino 3");
        exit(1);
      }
    }
    while(fgets(s,500,fp0)) {
      strcpy(s1,s);
      s1[15]='\0';
      if(strcmp(s1,separa)==0) {
        fgets(s,500,fp0);
        cont++;
      }
      if(cont==0)
         fprintf(fp1,"%s",s);
      if(cont==1)
         fprintf(fp2,"%s",s);
      if(cont==2)
         fprintf(fp3,"%s",s);
    }
    fclose(fp0);fclose(fp1);fclose(fp2);
    if(fp3) {
      fclose(fp3);
    }
}

void para_zombis(int sig){
    while(waitpid(-1,0,WNOHANG)>0);
}
