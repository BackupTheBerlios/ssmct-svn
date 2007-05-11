/* this file is part of ssmct 
 * see ssmct.c for copyright information
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
void generar_preguntas_resp(char **ar_preg,char **ar_resp,char *preguntas,char **ar_correctas,char *tipo);
void corregir(char *b,char *c,char *s,char *tipo);
void enviar(char *st_envio,char **nombre);

int main(int argc, char **argv) {
struct sockaddr_in faddr;
int faddrlen,sock_fp,no_tal,fd;
int pi[2], pid,primero,termina,no_mas,port;
long n;
char *p, *fin, *ps, s2[BUFSIZ],s[BUFSIZ],nombre[250],st[250],text[70000],
     tit[1500],tit2[1500],tit0[1500], st_envio[70000], nombre1[250];
char resp_trabajo[100],nom_a_buscar[100], trabajo[100], host[100];
char tipo[250],*past,*past1;
FILE *fp;
char op, op_trabajo[100];
int i,j,seguir,ficha, hay_bytes,nbytes;
  char text1[BUFSIZ],s1[BUFSIZ], *fin1,*fin2, *ar_preg[500], preguntas[70000], *ar_resp[500];
  char *ar_correctas[500],*pbuf;
	  
  FILE *fp1,*fp2;
// recibir nombre por dialogo
//
// Si hay que cargar opciones este es el momento, junto con las preguntas.
//
// en este cargamos las preguntas
//
 ficha=no_tal=0;
 *st_envio='\0';
 pbuf=NULL;
 p=strchr(argv[4],':');
 *p='\0';
 strcpy(host,argv[4]);
 strcpy(tipo,argv[5]);
 port=atoi(p+1);
 strcpy(trabajo,argv[1]);
 strcpy(nombre,argv[2]);
 strcpy(resp_trabajo,"resp_");
 strcpy(op_trabajo,"op_");
 strcat(resp_trabajo,trabajo);
 strcat(op_trabajo,trabajo);
 fd=open(trabajo,O_RDONLY); // Vemos si es ficha
 if(fd==-1) {
   printf("No puede abrir %s\n",trabajo);
   exit(1);
 }
 hay_bytes=lseek(fd,0,SEEK_END);
 pbuf=(char *)malloc(hay_bytes+1);
 if(pbuf==NULL) {
   printf("No hay memoria\n");
   exit(1);
 }
 lseek(fd,0,SEEK_SET);
 nbytes=read(fd,pbuf,hay_bytes);
 close(fd);
 if(nbytes!=hay_bytes) {
   printf("No ha leido de %s\n",trabajo);
   exit(1);
 }
 pbuf[nbytes]='\0';
 if((past=strstr(pbuf,"\n\n*\n\n"))!=NULL) { // es ficha
    ficha=1;
    ar_preg[0]=past+5;
    past=past+5;
    for(j=1;*past!='\0';j++) {
       if((past1=strstr(past,"\n\n"))!=NULL) {
          *(past1+1)='\0';
          ar_preg[j]=past1+2;
          past=past1+2;
       }
    }
    ar_preg[j-1]=NULL;
 }
 else {       // No es ficha
   free(pbuf);
   pbuf=NULL;
 }
if(!ficha) { 
 fp1=fopen(trabajo,"r");
 if(fp1==NULL) {printf("no puede abrir %s\n",trabajo); exit(1);}

//
// Aqui mismo vemos si se grabaron respuestas, si no se sale con mensaje
//
   fp2=fopen(resp_trabajo,"r");
   if(fp2==NULL) {printf("Ni siquiera el trabajo está realizado\n");exit(1);}
   fin1=fgets(s1,BUFSIZ,fp2);
   if(fin1!=NULL && *s1!='\n'){
     fclose(fp2);
   }
   else {
     fputs("\n\n   * * * Primero:  Genere las Respuestas * * *\n\n",stdout);
     exit(1);
   }
//
// Aqui por opciones
//
 if(argv[3][0]=='1'){ //hay que juntar a cada pregunta con su opciones
  fp2=fopen(op_trabajo,"r");
  if(fp2==NULL) {printf("No puede abrir fichero %s\n",op_trabajo); exit(1);}
 }
 i=0;
 while(1) {
  while(fin1=fgets(s1,BUFSIZ,fp1)) {
	if(*s1=='\n')
	  break;
	strcat(text1,s1);
  }
  if(fin1==NULL)
    break;
  if(argv[3][0]=='1') { // le agregamos las correspondientes opciones
    while(fin2=fgets(s1,BUFSIZ,fp2)) {
	if(*s1=='\n')
	  break;
	strcat(text1,s1);
    }
    if(fin2==NULL)
       break;
  }
  ar_preg[i++]=(char *)malloc(strlen(text1)+1);
  strcpy(ar_preg[i-1],text1);
  *text1='\0';
 }
 ar_preg[i]=NULL;
}  //    acaba if(!ficha) ////
//
// cargamos las respuestas
//
fp=fopen(resp_trabajo,"r");
if(fp==NULL) {puts("No puedo abrir file");exit(1);}
i=0;
  while(fin=fgets(s,BUFSIZ,fp)) {
	if(*s=='\n')
	  break;
	ps=strchr(s,':');
	if (ps==NULL) { puts("RARO"); puts (s);exit(1); };
	ar_correctas[i++]=(char *)malloc(strlen(ps+1)+1);
	strcpy(ar_correctas[i-1],ps+1);
  }
  ar_correctas[i]=NULL;
//
// Generamos array de respuestas para "nombre", debemos buscar sus
// respuestas en fichero resp_trabajo
//
//
while(1) { 
  p=fgets(s,BUFSIZ,fp);
  if(p==NULL) {
        printf("No hatal nombre: %s\n",nombre);
        no_tal=1;
        break;
  }
  p[strlen(p)-1]='\0';
  p=strstr(s,nombre);
  if(p!=NULL) {
     for(--p; *p!=':'; --p);	
     ++p;
      // aqui se ha encontrado p apunta al nombre
     strcpy(nombre1,p);
     break;
   }
   // Adelantar un registro ya que no es el buscado: implica leer varias lín.
  while((p=fgets(s,BUFSIZ,fp)) && *s!='\n') ;
  if(p==NULL)  {
        printf("No hay tal nombre: %s\n",nombre);
        no_tal=1;
        break;
  }
}
 if(!no_tal) {
//
// Cargamos ahora las respuestas del encontrado.
  i=0;
  while(fin=fgets(s,BUFSIZ,fp)) {
	if(*s=='\n')
	  break;
	ps=strchr(s,':');
	if (ps==NULL) { puts("RARO"); puts (s);exit(1); };
	ar_resp[i++]=(char *)malloc(strlen(ps+1)+1);
	strcpy(ar_resp[i-1],ps+1);
  }
  ar_resp[i]=NULL;
//
  sprintf(tit,"%s\n%s %s\n","Respuestas","Respuestas de:",nombre1);
  strcat(st_envio,tit);
  generar_preguntas_resp(ar_preg,ar_resp,preguntas,ar_correctas,tipo);
  strcat(st_envio,preguntas);
 }
 else {
  sprintf(tit,"%s\n%s %s\n","Respuestas","Respuestas de:",nombre);
  strcat(st_envio,"No hay un trabajo con ese nombre");
 }
//
// enviar datagrama

//
  faddrlen=sizeof(faddr);
  faddr.sin_family=AF_INET;
  faddr.sin_port=htons(port);
  inet_aton(host,&faddr.sin_addr);
  sock_fp=socket(AF_INET,SOCK_DGRAM,0);
  n=sendto(sock_fp,st_envio,strlen(st_envio),0,(struct sockaddr *)&faddr,sizeof(faddr));
   //printf("muestra_corr: Enviados %ld bytes\n",n);
//
 if(pbuf!=NULL) {
   free(pbuf);
 }
}
 
void generar_preguntas_resp(char **a,char **b,char *preg,char **correctas,char *tipo) {
  char string[BUFSIZ];
  while(*a) {
   strcat(preg,*a);
   strcat(preg,"\nRespuesta: ");
   strcat(preg,*b);
   *string='\0';
   corregir(*b,*correctas,string,tipo);
   strcat(preg,string);
   strcat(preg,"\n\n");
   a++; b++; correctas++;
  }
}
void corregir(char *b,char *c,char *s,char *tipo) {
// las respuesta (en b) viene en un string:
//   se trata de separar estos campos. FS=":"
// para ello usamos strtok
    char *p,*p1,st[BUFSIZ];

     strcpy(st,c);
     p=strtok(st,":");
     p1=strchr(c,':'); // para modificar c y mostrar una sola respuesta si era
                       // saq
     if(*tipo=='T') {  // es tipo test
       if(strcmp(c,b)==0) {
          strcpy(s,"|[b]CORRECTA[b]|");
       }
       else {
           strcpy(s,"|[r]INCORRECTA[r]|");
           strcat(s,"\nLa Respuesta es: ");
           c[strlen(c)-1]='\0';
           strcat(s,c);
       }
     }
     else { // es saq
        while(p!=NULL) {
           if(p[strlen(p)-1]=='\n')
               p[strlen(p)-1]='\0';
           if(b[strlen(b)-1]=='\n')
               b[strlen(b)-1]='\0';
	   if(strcmp(p,b)==0) {
		strcpy(s,"|[b]CORRECTA[b]|");
                strcat(s,"\nRespuesta del profesor: ");
                strncat(s,c,p1-c); // para que se vea sola una de
                break;
           }
	   else {
                p=strtok(NULL,":");
           }
         }
         if(p==NULL) {
		strcpy(s,"|[r]INCORRECTA[r]|");
                strcat(s,"\nLa Respuesta es: ");
                if(p1!=NULL) {
                   strncat(s,c,p1-c); // para que se vea sola una de
                                     //  las respuestas.
                }
                else
                   strcat(s,c);
          }
     }
}
