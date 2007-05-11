/* this file is part of ssmct 
 * see ssmct.c for copyright information
 */
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
char *mostrar_corregir(char **,char *,char **,char *);
long dame_trabajo(FILE *fp,char *un_trabajo,long pos);
void devolver(char**);
int trabajo_array(char **ar_resp,char *un_trabajo);
char *obtener_nombre(char *s,char *nombre,char *el_nom);
void mostrar_pregunta_num(char **ar_resp,int num_preg,char *tit2);
void generar_preguntas_resp(char **ar_preg,char **ar_resp,char *preguntas);
void corregir(char *b,char *c,char *s);

int main(int argc, char **argv) {
struct sockaddr_in faddr;
int faddrlen,sock_fp;
int pi[2], pid,primero,termina,no_mas,port,pos,pos_final,hay_resp;
long n;
char *p, *fin, *ps, s2[BUFSIZ],s[BUFSIZ],nombre[250],st[250],text[70000],
     tit[1500],tit2[1500],tit0[1500], st_envio[70000], nombre1[250];
char corr_trabajo[250], salida[BUFSIZ], un_trabajo[BUFSIZ];
char resp_trabajo[100],nom_a_buscar[100], trabajo[100], host[100];
FILE *fp;
char op, op_trabajo[100],tit1[500];
int i,seguir,examen;
  char text1[BUFSIZ],s1[BUFSIZ], *fin1,*fin2, *ar_preg[500], preguntas[70000], *ar_resp[500];
  char *ar_correctas[500];
	  
  FILE *fp1,*fp2;
 examen=i=0;
 *st_envio=*text1='\0';
 p=strchr(argv[3],':');
 *p='\0';
 strcpy(host,argv[3]);
 port=atoi(p+1);
 strcpy(trabajo,argv[1]);
 strcpy(nombre,argv[2]);
 strcpy(corr_trabajo,"corr_resp_");
 strcat(corr_trabajo,trabajo);
 if(strcmp(argv[4],"examen")==0){
   examen=1;
   ar_preg[0]=NULL;
 }
 if(examen) {
   fp1=fopen(trabajo,"r");
   if(fp1==NULL) {printf("no puede abrir %s\n",trabajo); exit(1);}
 }
 fp=fopen(corr_trabajo,"r");
 if(fp==NULL) {printf("El trabajo no ha sido corregido\n");exit(1);}
 
 if(examen) {
  while(1) { // Hasta acabar todas las preguntas
     while(fin1=fgets(s1,BUFSIZ,fp1)) { // cargamos las preguntas
	if(*s1=='\n')
	  break;
	strcat(text1,s1);
     }
     if(fin1==NULL)
       break;
     ar_preg[i++]=(char *)malloc(strlen(text1)+1);
     strcpy(ar_preg[i-1],text1);
     *text1='\0';
  }
  fclose(fp1);
  ar_preg[i]=NULL;
 }// esto es si era examen, presentación no lleva preguntas
//
// Generamos array de respuestas para "nombre", debemos buscar sus
// respuestas en fichero resp_trabajo
//
//
  *salida='\0';
  pos=0;
  pos_final=fseek(fp,0L,SEEK_END);
  pos_final=ftell(fp);
  while(pos < pos_final - 1) { // quedan registros por leer
    pos=dame_trabajo(fp,un_trabajo,pos);
    hay_resp=trabajo_array(ar_resp,un_trabajo); // tengo un_trabajo en
                     // memoria- ar_resp[0] es datos,
                    //  ar_resp[1] primera respuesta
    p=mostrar_corregir(ar_resp,nombre,ar_preg,st_envio);
    if(p)
      pos_final=-1; // para salir del bucle
    else
      devolver(ar_resp);
  }
  faddrlen=sizeof(faddr);
  faddr.sin_family=AF_INET;
  faddr.sin_port=htons(port);
  inet_aton(host,&faddr.sin_addr);
  sock_fp=socket(AF_INET,SOCK_DGRAM,0);
  if(pos_final==-1) {
      n=sendto(sock_fp,st_envio,strlen(st_envio),0,(struct sockaddr *)&faddr,sizeof(faddr));
      devolver(ar_resp);
  }
  else {
   sprintf(tit1,"%s\n%s %s\n","Respuestas","Respuestas de:",nombre);
   strcat(st_envio,tit1);
   strcat(st_envio,"No hay un trabajo con ese nombre");
   n=sendto(sock_fp,st_envio,strlen(st_envio),0,(struct sockaddr *)&faddr,sizeof(faddr));
  }
}


char * mostrar_corregir(char **ar_resp,char *nom,char **ar_preg,char *st_envio){
   char el_nom[250],nombre[250],*p,*p1, tit2[500],tit1[250], s2[BUFSIZ];
   int i;
   char preguntas[70000];
   p=obtener_nombre(ar_resp[0],nom,el_nom);
   if(p==NULL)
     return NULL;
   i=0;
   sprintf(tit1,"%s\n%s %s\n","Respuestas","Respuestas de:",el_nom);
   strcat(st_envio,tit1);
   generar_preguntas_resp(ar_preg,ar_resp,preguntas);
   strcat(st_envio,preguntas);
   return(st_envio);
//
// enviar datagrama (el contenido está en preguntas
// esto lo hace el main como útima tarea.
//
}
char *obtener_nombre(char *s,char *nombre,char *el_nom) {
   char *p,*p1;
   p=p1=NULL;
   p=strchr(s,':');   // p+1 apunta al segundo campo
   p1=strchr(p+1,':'); // p1+1  apunta al tercer campo
   p=p1+1;
   if(*nombre!='\0') {
     p1=strstr(p,nombre);
     if(p1==NULL)
        return NULL;
   }
   strcpy(el_nom,p);
   return el_nom;
}

//

void mostrar_pregunta_num(char **ar_resp,int num_preg,char *tit2) {
   int pid, pi[2],n,termina;
   char s2[BUFSIZ];
     if(pipe(pi)==-1) {puts("error en pipe"); exit(1); }
     pid=fork();
     if(pid==-1) {puts("error en fork"); exit(1); }
     if(pid==0) {
        close(pi[0]);
        dup2(pi[1],1);
        execlp("/usr/local/bin/greq","/usr/local/bin/greq","-t",
                "Podemos Corregir los Trabajos",tit2,"-z",ar_resp[num_preg],
                "-bCorregir con", "-r" "r\v-r\vr+\vr\vb\vn",NULL);
        printf("EMMEEMMEEMMEEMM\n");
     }
     close(pi[1]);
     s2[0]='\0';
     n=read(pi[0],s2,BUFSIZ);
     s2[n]='\0';
     wait(&termina);
     if(*s2=='\0') {
        exit(1);      
     }
     printf("Así he corregido:\n");
     printf("++++%s++++\n",s2);
}

void devolver(char **a) {
    if(*a) // hay solo un requerimiento
      free(*a);
}
long dame_trabajo(FILE *fp,char *un_trabajo,long pos) {
    int pos_ini, i, car;
    i=0;
    pos_ini=fseek(fp,pos,SEEK_SET);
    while((car=fgetc(fp))!='\f' ) {
      if(car==EOF)
         break;
      un_trabajo[i++]=car;
    }
    un_trabajo[i]='\0';
    return (ftell(fp));
}
int trabajo_array(char **ar_resp,char *un_trabajo) {
      char *p,*p1;
      int i;
      i=0;
      p=(char *)malloc(strlen(un_trabajo)+1);
      strcpy(p,un_trabajo);
      ar_resp[i]=p;
      while(1) {
         p1=strchr(ar_resp[i],'\v');
         if(p1==NULL)
            break;
         ar_resp[++i]=p1+1;
         *p1='\0';
      }
      ar_resp[++i]=NULL;
      return i;
}

void generar_preguntas_resp(char **a,char **b,char *preg) {
  char *p,*p1,string[BUFSIZ];
  int hay;
  p1=NULL;
  b++;
  if(a[0]!=NULL){
    while(*a) {
      strcat(preg,*a);
      strcat(preg,"\n|[b]Respuesta: [b]|\n");
      p=strchr(*b,':');
      p1=NULL;
      //while(p && (*(p+1)>'9' || *(p+1)<'0')) {
	 //p1=p;
         //p=strchr(p+1,':');
      //}
      hay=1;
      while(p&&hay) {
        hay=0;
        p1=p;
        if(p[1]=='r'&&p[2]==':') 
	  hay=1;
        if(p[1]=='r'&&p[2]=='+'&&p[3]==':') 
	  hay=1;
        if(p[1]=='r'&&p[2]=='-'&&p[3]==':') 
	  hay=1;
        if(p[1]=='b'&&p[2]==':') 
	  hay=1;
        if(p[1]=='n'&&p[2]==':') 
	  hay=1;
        if(hay) {
          p=strchr(p+1,':');
	  hay=1;
	}
      }
      if(p1==NULL) {
        strcat(preg,"** No Corregido **");
      }
      else
        strcat(preg,p1+1);
      strcat(preg,"\n\n");
      a++; b++; 
   }
 }
 else {
    while(*b){
      p=strchr(*b,':');
      while(p&&(p[1]=='r' || p[1]=='b' || p[1]=='n')&&(p[2]==':' || p[3]==':'))
         p=strchr(p+1,':');
      strcat(preg,p+1);
      strcat(preg,"\n|[b]---[b]|\n");
      b++; 
    }
 }
}

