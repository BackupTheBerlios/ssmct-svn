 /*
 * smct - run a client program, simple multiple choice test
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
void para_zombis(int sig);
void say_who_called(struct sockaddr_in *addrp);
void mostrar_preg_resp(char **preg,char **resp,char *text,char *npreg,int desde,int hasta);
void  fun_saq(char **,int,char **);
void  fun_examen(char **,int,char **);
void  fun_ficha(char **,int,char **);
void  fun_array(char **);
void  fun_enviar(char **);
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
  struct timeval value;
  long msglen,n;
  char buf[70000],s9[60],buf1[250];
 int cont,cont_resp,act,ind,b,j,i,ini, pid,desde,hasta,top,argumento,cont_preg;
 int hay_preg,id;
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
 signal(SIGUSR1,salir);
 signal(SIGCHLD,para_zombis);
 value.tv_sec=0;
 value.tv_usec=100000;
 val_size=sizeof(value);
 chdir("./ssmct.d");
  len_error=sizeof(error);
  tipo_pres=una_opcion=tipo_note=examen=saq=corregido=ficha=passwd=cordinado=0;
  primario=0;
  fp1=popen("greq -t'Hola, primero conectamos' -e'Dir. IP de tu servidor' -ePuerto -d 14500 -c 'Ver la corección de un Trabajo' -c 'Ver temas de clase' -c 'Con password'","r");
  if(!fp1) {
     puts("NO pudo ejecutar grep, sale");
     exit(1);
  }
  p=fgets(sp,500,fp1);
  if(!p) {
    puts("No leyo. Sale");
    exit(1);
  }
  pclose(fp1);
  sscanf(sp,"%s\v%s\v%s\v%s\v%s",servidor,puerto,correg,temas,pass);
  if(*correg=='T')
    corregido=1;
  if(*pass=='T')
    passwd=1;
  if(*temas=='T')
    ficha_entry=1;
  printf("El servidor es: %s\nEl puerto es %s - corregido es: %d - passwd es: %d\n",servidor,puerto,corregido,passwd);
  if(corregido) {
   if(!passwd) {
     fp1=popen("greq -t'Identificación' -e'Apellidos y Nombre'","r");
   }
   if(passwd) {
     fp1=popen("greq -t'Identificación' -e'Apellidos y Nombre' -E'Password'","r");
   }
   if(!fp1) {
       puts("NO pudo ejecutar grep, sale");
       exit(1);
   }
    p=fgets(sp,500,fp1);
    if(!p) {
      puts("No leyo. Sale");
      exit(1);
    }
    pclose(fp1);
    strcpy(buf1," \v");
    strcat(buf1,sp);
  }
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
  fp=socket(AF_INET,SOCK_DGRAM,0);
  if(corregido) {
    n=sendto(fp,buf1,strlen(buf1),0,(struct sockaddr *)&saddr,sizeof(saddr));

    //if(setsockopt(fp,SOL_SOCKET,SO_RCVTIMEO,&value,val_size) != 0 )
    //  perror("setsockopt() fallo");
    while((msglen=recvfrom(fp,buf,70000,0,(struct sockaddr *)&faddr,(int*)&faddrlen))==-1) {
       n=sendto(fp,buf1,strlen(buf1),0,(struct sockaddr *)&saddr,sizeof(saddr));
       //puts("ENVIE CORREGIDO");
     }

  //msglen=recvfrom(fp,buf,70000,0,(struct sockaddr *)&faddr,(int *)&faddrlen);
    buf[msglen]='\0';
    printf("Recibidos: %ld\n",msglen);
    fd=open("/tmp/FILE",O_WRONLY|O_CREAT|O_TRUNC,0644);
    if(fd==-1) {perror("no puede escribir en tmp"); exit(1); }
    write(fd,buf,msglen);
    close(fd);
// Abrimos de lectura para obtener los datos y presentarlos
   fp0=fopen("/tmp/FILE","r");
   if(fp0==NULL) {puts ("no puede abrir file en /tmp"); exit(1); }
// leemos de /tmp/FILE,  primera línea título
    p=fgets(tit,500,fp0);
    if(p==NULL) {
      printf("El servidor no envía la corrección 1\n");
      unlink("/tmp/FILE");
      exit(1);
    }
// lemos segunda línea: titulo 2
    p=fgets(tit2,500,fp0);
    if(p==NULL) {
      printf("El servidor no envía la corrección 2\n");
      unlink("/tmp/FILE");
      exit(1);
    }
// leer el resto del /tmp/FILE y concatenar en string preguntas
    while(fgets(st,BUFSIZ,fp0)) {
       strcat(preguntas,st);
    }
    tit[strlen(tit)-1]='\0';
    tit2[strlen(tit2)-1]='\0';
    sprintf(tit1,"%s%65s%65s","-X",tit2," ");
    pid=fork();
    if(pid==0) {
       execlp("/usr/local/bin/greq","/usr/local/bin/greq","-t",tit,
             tit1,"-Z",preguntas,NULL);
       puts("no ejecuto exec");
     }
     if(pid>0) {
       wait(NULL);
       unlink("/tmp/FILE");
       exit(0);
     }
  }
  else {
    if(setsockopt(fp,SOL_SOCKET,SO_RCVTIMEO,&value,val_size) != 0 )
      perror("setsockopt() fallo");
  while(1) {
    n=sendto(fp,"\0",1,0,(struct sockaddr *)&saddr,sizeof(saddr));
    for(i=1;i<=2;i++) {
     while((msglen=recvfrom(fp,buf,70000,0,(struct sockaddr *)&faddr,(int*)&faddrlen))==-1) {
       n=sendto(fp,"\0",1,0,(struct sockaddr *)&saddr,sizeof(saddr));
       //puts("ENVIE");
           
     }
     if( (msglen==1)&&(*buf=='\0') ) { // es saq
       saq=1;
       break;
     }
     if( (msglen==1)&&(*buf=='f') ) { // es ficha
       ficha=1;
       if(!ficha_entry) {
         printf("-Temas de Clase- es lo que toca\n");
         unlink("/tmp/FILE1");         
         exit(1);
       }
       break;
     }
     if( (msglen==1)&&(*buf=='\v') ) { // es examen
       examen=1;
       break;
     }
     if( (msglen==1)&&(*buf=='\t') ) { // es notebook
       tipo_note=1;
       break;
     }
     if( (msglen==1)&&(*buf=='\f') ) { // es CordinadoNote
       cordinado=1;
       primario=comunica();
       break;
     }
     if( (msglen==1)&&(*buf=='u') ) { // es ua_opcion
       una_opcion=1;
       break;
     }

//
     sprintf(s9,"%s%d","/tmp/FILE",i);
     fd=open(s9,O_WRONLY|O_CREAT|O_TRUNC,0644);
     if(fd==-1) {perror("no puede escribir en tmp"); exit(1); }
     write(fd,buf,msglen);
     close(fd);
    }
    if( (!saq) && (!examen) && (!ficha) && (!una_opcion) && (!tipo_note) &&
        (!cordinado)) {
     msglen=recvfrom(fp,buf,70000,0,(struct sockaddr *)&faddr,(int *)&faddrlen);
     if(msglen<=25) {
      for(t=j=i=0;i<msglen;i++,j++) {
       if(buf[i]!=' '&& buf[i]!='\0')
         st_arg[j]=buf[i];
       else {
        st_arg[j]='\0';
        ar_arg[t++]=(char *)malloc(strlen(st_arg)+1);
        strcpy(ar_arg[t-1],st_arg);
        j=-1;
       }
      }
      ar_arg[t]=NULL;      
     }
    if(msglen>=25) {
     fd=open("/tmp/FILE3",O_WRONLY|O_CREAT|O_TRUNC,0644);
     if(fd==-1) {perror("no puede escribir en tmp"); exit(1); }
     write(fd,buf,msglen);
     close(fd);
     tipo_pres=1;
    // break;
    }
   }

  // "contamos preguntas del fichero"
 fp0=fopen("/tmp/FILE1","r");
 if(fp0==NULL) {puts (" * No puede abrir file en /tmp  *"); continue;}
 else 
  break;
} // el while(1)
} // el else
 hay_preg=0;
 while(fgets(st,BUFSIZ,fp0)) {
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
   perror("greq");
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
 if(tipo_pres || una_opcion || tipo_note || cordinado) {
    // ejecuta funciones en un hijo;
    strcpy(env_resp,nombre);
    if(tipo_note || cordinado)
       id=1;
    else
       id=0;
    if(tipo_pres||tipo_note || cordinado)
      strcat(env_resp,"\v");
    else
      strcat(env_resp,":");
    ejecuta_funciones(env_resp,id,fp,value,val_size,saddr,faddr,
                  faddrlen,cordinado);
    //ejecuta_funciones(env_resp,id,cordinado);
    // enviar
   n=sendto(fp,env_resp,strlen(env_resp)+1,0,(struct sockaddr *)&saddr,sizeof(saddr));
   msglen=recvfrom(fp,buf,70000,0,(struct sockaddr *)&faddr,(int *)&faddrlen);
   buf[msglen]='\0';
   if(strcmp(buf,"recv")==0)
     printf("Recibida respuesta del servidor, OK\n");
   else
     printf("Recibí:: %s\n",buf);
   exit(0);
 }
 if(saq || examen || ficha) {
   fun_array(ar_preg);
   if(saq) {
     fun_saq(ar_preg,hay_preg,respuestas);
   }
   if(examen)
     fun_examen(ar_preg,hay_preg,respuestas);
   if(ficha)
     fun_ficha(ar_preg,hay_preg,respuestas);
   strcpy(env_resp,nombre);
   if(examen)
     strcat(env_resp,"\v");
   else
     strcat(env_resp,"\n");
   grabar_resp(respuestas,env_resp,examen) ;
   grabar_temporal(env_resp);
   n=sendto(fp,env_resp,strlen(env_resp)+1,0,(struct sockaddr *)&saddr,sizeof(saddr));
   msglen=recvfrom(fp,buf,70000,0,(struct sockaddr *)&faddr,(int *)&faddrlen);
   buf[msglen]='\0';
   if(strcmp(buf,"recv")==0)
     printf("Recibida respuesta del servidor, OK\n");
   devolver(ar_preg);
   devolver(respuestas);
   unlink("/tmp/FILE");
   unlink("/tmp/FILE1");
   unlink("/tmp/FILE2");
   exit(1);
 }
 top=14;
 aplicar=0;
 desde=1;
 argumento=atoi(ar_arg[0]);
 fp0=fopen("/tmp/FILE1","r");
 fp1=fopen("/tmp/FILE2","r");
 st[0]=s1[0]=s2[0]=stp[0]='\0';
 cont=cont_preg=ini=act=b=0;
 cont_resp=0;
 ind=6;
   ar[0]=(char*)"/usr/local/bin/greq";
   ar[1]=(char*) "-t";
   ar[2]=(char *)"Mi Programa";
   ar[4]=(char*)"-z";
   ar[5]=(char*) st ;
while(1){
 st[0]='\0';
 while(p=fgets(s,300,fp0)) {
   strcat(st,s);
   strcat(stp,s);
   if(s[0]=='\n') {
     cont++;
     ar_preg[cont_preg]=(char *)malloc(strlen(stp)+1);
     strcpy(ar_preg[cont_preg],stp);
     cont_preg++;
     *stp='\0';
     while(p1=fgets(sop,300,fp1)) {
          if(sop[0]!='\n') {
            ar_sop[act]=(char *)malloc(strlen(sop)+1);
            strcpy(ar_sop[act],sop);
            act++;
          }
          if(sop[0]=='\n') {
             ar[ind++]=(char*)"-m";
             b++;
             sprintf(etiq,"%s %d","Resp. ",b);
             ar[ind++]=(char *)malloc(strlen(etiq)+1);
             strcpy(ar[ind-1],etiq);
             sop_desde[cont_preg-1]=ini;
             sop_hasta[cont_preg-1]=act-1;
             for(;ini<act;ini++)
               ar[ind++]=ar_sop[ini];
             
             break;
          }
     }

     if(cont==argumento) {
       break;
     }
   }
 }
 if(p==NULL&&cont==0)
   break;
 sprintf(tit,"-X%60s %s  %d-%d / %d%50s"," ","Preguntas  ",desde,desde+cont-1,hay_preg," ");
 ar[3]=(char*) tit;
 ar[5]=(char*) st ;
 ar[ind]=NULL;
 ind=6;
 if(pipe(pi)==-1) {puts("error en pipe"); exit(1); }
 pid=fork();
 if(pid==-1) {puts("error en fork"); exit(1); }
 if(pid==0) { 
   fflush(stdout);
   close(pi[0]);
   dup2(pi[1],1);
   
   execvp(ar[0],ar);
   
   perror("ejecutando exec");
   // con error
 }
 desde=desde+cont;
 cont=0;
 close(pi[1]);
 s2[0]='\0';
 n=read(pi[0],s2,BUFSIZ);
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
 respuestas[cont_resp]=NULL;
 wait(NULL);
}
ar_preg[cont_preg]=NULL;
if(ar_arg[1]!=NULL) {
 if(ar_arg[1][0]=='v') {
   while(1) {
     *preg=*text='\0';   
     mostrar_preg_resp(ar_preg,respuestas,text,preg,0,cont_preg-1);
     if(pipe(pi)==-1) {puts("error en pipe"); exit(1); }
     pid=fork();
     if(pid==0) {
        close(pi[0]);
        dup2(pi[1],1);
        sprintf(tit,"%s","Verificar todos las respuestas ");
        sprintf(tit2,"%s%80s%80s","-X","Tus Respuestas"," ");
         execlp("/usr/local/bin/greq","/usr/local/bin/greq","-t",tit,
           tit2,"-z",text,"-bModificar preg. ","-r",
          preg,"-cAplicar",NULL);
       puts("no ejecuto exec");
     }
     close(pi[1]);
     s2[0]='\0';
     n=read(pi[0],s2,BUFSIZ);
     s2[n]='\0';
     p2=s2;
     if(*s2=='\0')
       exit(1);
     for(i=0;p2[i]!='\0';i++) {
       if(p2[i]=='\v') {
         p2[i]='\0';
         if(p2[0]!='\0')
           resp_mod=atoi(p2);
         p2+=i+1;
         i=-1;
       }
     }
     if(strcmp(p2,"TRUE")==0) {
       aplicar=1;
     }
     wait(NULL);
     if(aplicar) {
       if(pipe(pi)==-1) {puts("error en pipe"); exit(1); }
       pid=fork();
       if(pid==0) {
         close(pi[0]);
         dup2(pi[1],1);
	 *text=*descarte='\0';
	 mostrar_preg_resp(ar_preg,respuestas,text,descarte,resp_mod-1,resp_mod-1);
         sprintf(tit,"%s%80s %d%80s","-X","Pregunta",resp_mod," "); 
         ar[3]=(char *)tit;
         ar[5]=(char *)text;
         ind=6;
         ar[ind++]=(char*)"-m";
         sprintf(etiq,"%s %d","Resp. ",resp_mod);
         ar[ind++]=(char *)malloc(strlen(etiq)+1);
         strcpy(ar[ind-1],etiq);
         for(i=sop_desde[resp_mod-1];i<=sop_hasta[resp_mod-1];i++) {
             ar[ind++]=ar_sop[i];             
         }
         ar[ind++]="-bModificar preg. ";
         ar[ind++]="-r";
         ar[ind++]=preg;
         ar[ind++]="-cAplicar";
         ar[ind]=NULL;
         execvp(ar[0],ar);
         perror("No ejecuta exec");
       }
       close(pi[1]);
       s2[0]='\0';
       n=read(pi[0],s2,BUFSIZ);
       s2[n]='\0';
       if(*s2=='\0')
         exit(1);
       p2=s2;
       cont_resp=resp_mod-1;
       for(i=0;p2[i]!='\0';i++) {
         if(p2[i]=='\v')
           p2[i]=' ';
         if(p2[i]=='\n') {
           p2[i]='\0';
           free(respuestas[cont_resp]);
           respuestas[cont_resp]=(char *)malloc(strlen(p2)+1);
           strcpy(respuestas[cont_resp],p2);
           break;
         }
       }
       wait(NULL);
       aplicar=0;
    } // el if (aplicar)
    else {
      break;
    }
   } // el while
  } //un if
 } // el otro
 strcpy(env_resp,nombre);
 strcat(env_resp,"\n");
 grabar_resp(respuestas,env_resp,0) ;
     //mostrar_preg_resp(ar_preg,respuestas,text,preg,0,cont_preg-1);
// enviar
  grabar_temporal(env_resp);
  n=sendto(fp,env_resp,strlen(env_resp)+1,0,(struct sockaddr *)&saddr,sizeof(saddr));
   msglen=recvfrom(fp,buf,70000,0,(struct sockaddr *)&faddr,(int *)&faddrlen);
   buf[msglen]='\0';
   if(strcmp(buf,"recv")==0)
     printf("Recibida respuesta del servidor, OK\n");
 devolver(ar_preg);
 devolver(respuestas);
 unlink("/tmp/FILE");
 unlink("/tmp/FILE1");
 unlink("/tmp/FILE2");
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
  if(cordinado && !primario) {
     ejecuta_coor(fp,value,val_size,saddr,faddr,faddrlen,pid);
  }
  n=read(pi[0],s2,60000);
  wait(NULL);
  s2[n]='\0';
  if(*s2=='\0')
    exit(1);
  strcat(env_resp,s2);
}

void ejecuta_coor(int fp,struct timeval value,int val_size,
                 struct sockaddr_in saddr,struct sockaddr_in faddr,
		 int faddrlen,int pid) {
    char buf[100];
    FILE *fp1;
    int msglen;
    int n;
  //  if(setsockopt(fp,SOL_SOCKET,SO_RCVTIMEO,&value,val_size) != 0 )
      //perror("setsockopt() fallo");
    fp1=fopen("/tmp/.smct_pag","w");
    fprintf(fp1,"%d",tpag);
    fclose(fp1);
    n=sendto(fp,"\f",1,0,(struct sockaddr *)&saddr,sizeof(saddr));
  while(!salir_bucle) {
    *buf='\0';
    if((msglen=recvfrom(fp,buf,100,0,(struct sockaddr *)&faddr,(int*)&faddrlen))==-1) {
        perror("En ejecuta_coor");
	exit(1);
    }
        buf[msglen]='\0';
	sscanf(buf,"%d",&pag_actual);
	if(pag_actual>tpag) {
	   fflush(stderr);
	   tpag=pag_actual;
           fp1=fopen("/tmp/.smct_pag","w");
	   fprintf(fp1,"%d",tpag);
           fclose(fp1);
	}
	sleep(1);
        n=sendto(fp,"\f",1,0,(struct sockaddr *)&saddr,sizeof(saddr));
        if(kill(pid,0)==-1) {
           salir_bucle=1;
	}
   }
    if((msglen=recvfrom(fp,buf,100,0,(struct sockaddr *)&faddr,(int*)&faddrlen))==-1) {
        perror("En recvfrom");
	exit(1);
    }
    *buf='\0';
}

//

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
      puts("no ha podido grabar resuestas temporal");
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
   sprintf(tit,"-n'%65s %s  %d / %d%60s%c%s'%s"," ","Pregunta:  ",
              i+1,hay_preg," ",car,"Tu Respuesta"," -Z  \"");
   strcat(ejecuta,tit);
   strcat(ejecuta,ar_preg[i]);
   strcat(ejecuta,"\"");
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
   strcpy(respuestas[i],una_contesta);
}
respuestas[i]=NULL;
}
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
 // cargar respuesta en array
  respuestas[i]=(char *)malloc(strlen(resp1)+1);
  strcpy(respuestas[i],resp1);
  if(larga) {
       ar_preg[i]=ar_preg[i]-2;
  }
    
}
//
respuestas[i]=NULL;
}

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

       
int comunica(void) {
    int id_ssmct;
    FILE *fp;
    char s[100];
    fp=fopen("/tmp/.ssmct_pid","r");
    if(!fp) {
      return 0;
    }
    fgets(s,100,fp);
    sscanf(s,"%d",&id_ssmct);
    //printf("El id de ssmct es: %d\n",id_ssmct);
    fclose(fp);
//    unlink("/tmp/.ssmct_pid");
    return id_ssmct;
}

void para_zombis(int sig){
    while(waitpid(-1,0,WNOHANG)>0);
}
void salir(int sig) {
  salir_bucle=1;
}
