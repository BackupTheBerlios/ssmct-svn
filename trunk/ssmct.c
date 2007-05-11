/*
 * ssmct - run a server program, simple multiple choice test
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
#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<sys/socket.h>
#include<string.h>
#include<netinet/in.h>
#include<fcntl.h>
#include<sys/wait.h>
//void para_fin(int sig);
void para_coor(int sig);
void para_zombis(int sig);
void say_who_called(struct sockaddr_in *addrp,char *host_port);
void graba_linea(struct sockaddr_in *addrp,int,int,int*,int);
FILE *fpd;
int I=0;

main(int argc, char **argv) {
  struct sockaddr_in saddr,faddr;
  int pid,fp,saddrlen,faddrlen,fd1,fd2,tipo_pres,tipo_test,examen,resp_simples;
  int una_opcion,tipo_note,cordinado,num_proc;
  int fd3;
  long bytes1,bytes2,bytes3,n,msglen;
  char buf[60000],buf1[60000],buf2[60000],buf3[60000],*p,sp[500], nombre_t[100],
       trabajo[250],nombre[250], host_port[250], nombre_corr[100], buf4[50];
  char puerto[100], tipo[100];
  char nombre_op[100], nombre_a[100],nombre_b[100],nombre_resp[100]; 
  char verif[25],por_vez[25], opcion[25],con_op[25],correc[25];
  char *cp, directorio[PATH_MAX];
  FILE *fp1,*fp2;
  FILE *fp5, *fpid;
  int ret,permite_ver, es_primero,ficha;
  long hay_reg;
  char *mensa="En este momento el servidor no muestra CORRECCIONES\n";
es_primero=ficha=examen=tipo_pres=tipo_note=tipo_test=resp_simples=una_opcion=0;
  cordinado=0;
//
signal(SIGUSR1,para_coor);
signal(SIGCHLD,para_zombis);
//
//ret=chdir("./bin");
//fclose(fp5);
if(argc>1) {
 if(strcmp(argv[1],"-show")==0) {
   execlp("Show","Show",NULL);
   printf("No pudo ejecutar -show\n");
 }
}
fp1=popen("greq -t'Hola, Vamos a ponerlo en marcha' -e'Puerto' -d 14500 \
                -e'Nombre del Trabajo', -e'Cuántas preg. a la vez' -d 1 \
                -e'Con Verificación al final' -d n \
                -e'Ver la Corrección' -d n \
                -e'Con opciones' -d n \
                -m 'Tipo Trabajo' Test Examen Presentación Fichas/Cuestionario Resp.Simples UnaOpción Notebook CoordinadoNotebook\
                -p'Cómo usar ssmct y Garantía: ssmct  -show'","r");
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
  sscanf(sp,"%s\v%s\v%s\v%s\v%s\v%s\v%s",puerto,nombre_t,por_vez,verif,
                                         correc,con_op,tipo);
  strcpy(nombre_op,"op_");
  strcpy(nombre_resp,"resp_");
  strcat(nombre_op,nombre_t);
  strcat(nombre_resp,nombre_t);
  strcpy(opcion,por_vez);
  if(*verif=='s' || *verif=='S') {
    strcat(opcion," v");
  }
  if(*tipo=='T')
    tipo_test=1;
  if(*tipo=='N')
    tipo_note=1;
  if(*tipo=='C')
    cordinado=1;
  if(*tipo=='P')
    tipo_pres=1;
  if(*tipo=='U')
    una_opcion=1;
  if(*tipo=='E')
    examen=1;
  if(*tipo=='R')
    resp_simples=1;
  if(*tipo=='F')
    ficha=1;
  if(*correc=='s' || *correc=='S') {
     permite_ver=1;
     // permitimos ver cooreccion de trabajo, debemos recibir el nombre
     // del interesado
  }
  else {
     permite_ver=0;
  }
  if(*con_op=='s' || *con_op=='S') {
    strcpy(con_op,"1");
  }
  else {
    strcpy(con_op,"0");
  }
  if(cordinado) {
    num_proc=getpid();
    fpid=fopen("/tmp/.ssmct_pid","w");
    if(fpid) {
       fprintf(fpid,"%d",num_proc);
       fclose(fpid);
    }
    else {
     perror("grabación del proc.id");
     exit(1);
    }
  }
  printf("El puerto es: %s\nEl trabajo es %s\nVerifica: %s\nCuantas %s\nResp: %s\nop: %s\nfin: %s\nTipo Test: %s\n",
             puerto,nombre_t,verif,por_vez,nombre_resp,nombre_op,opcion,tipo);
  faddrlen=saddrlen=sizeof(saddr);
  saddr.sin_family=AF_INET;
  saddr.sin_port=htons(atoi(puerto));
  saddr.sin_addr.s_addr=INADDR_ANY;
  fp=socket(AF_INET,SOCK_DGRAM,0);
  bind(fp,(struct sockaddr *)&saddr,saddrlen);
  fd1=open(nombre_t,O_RDONLY); 
  if(fd1==-1) {
      perror("al abrir fichero ");
      exit(1);
  }
  if(tipo_test) { 
    fd2=open(nombre_op,O_RDONLY); 
    if(fd1==-1||fd2==-1){
      perror("al abrir fichero ");
      exit(1);
    }
  }
  if(tipo_pres) { 
    strcpy(nombre_a,nombre_t);
    strcpy(nombre_b,nombre_t);
    strcat(nombre_a,".a");
    strcat(nombre_b,".b");
    fd2=open(nombre_a,O_RDONLY); 
    fd3=open(nombre_b,O_RDONLY); 
    if(fd1==-1||fd2==-1||fd3==-1){
      perror("al abrir fichero ");
      exit(1);
    }
  }
  bytes1=read(fd1,buf1,60000);
  if (bytes1==-1) {
   perror("al leer de file");
   exit(1);
  }
  if(tipo_test) {
    bytes2=read(fd2,buf2,60000);
    if (bytes2==-1) {
      perror("al leer de file");
      exit(1);
    }
  }
  if(tipo_pres) {
    bytes2=read(fd2,buf2,60000);
    if (bytes2==-1) {
      perror("al leer de file");
      exit(1);
    }
    bytes3=read(fd3,buf3,60000);
    if (bytes3==-1) {
      perror("al leer de file");
      exit(1);
    }
  }
  fpd=fopen(nombre_resp,"a");
  if(fpd==NULL) {
     puts("NO puede abrir respuestas");
     exit(1);
  }
  hay_reg=ftell(fpd);
  if(hay_reg==0)
    es_primero=1;
 while(1) {
 *buf='\0';
 msglen=recvfrom(fp,buf,60000,0,(struct sockaddr *)&faddr,(int *)&faddrlen);
 buf[msglen]='\0';
 puts("-------RREECCIIIIBIII------");
 printf("%c%c%c%c%c\n",buf[0],buf[1],buf[2],buf[3],buf[4]);
 puts("-------RREECCIIIIBIII------");

   if(*buf==' '&&buf[1]=='\v') { // solicita trabajo corregido
      // ejeuta corregido
      if(!permite_ver) {
        n=sendto(fp,mensa,strlen(mensa),0,(struct sockaddr *)&faddr,sizeof(faddr));
	puts("Solicitud de ver correcciones Rechazada");
        continue;
      }
     /* Abrir claves y buscar en segundo campo (la clave)
      */  
      pid=fork();
      if(pid==0) {
         say_who_called(&faddr,host_port);
         if(tipo_test || resp_simples || ficha) {
	    execlp("muestra_corr","muestra_corr",nombre_t,buf+2,con_op,
                host_port,tipo,NULL);
	    puts("No ejecuta muestra_corr");
         }
         if(examen) {
	    execlp("muestra_corr1","muestra_corr1",nombre_t,buf+2,
                host_port,"examen",NULL);
	    puts("No ejecuta muestra_corr1");
         }
         if(tipo_pres) {
	    execlp("muestra_corr1","muestra_corr1",nombre_t,buf+2,
                host_port,"tipo_pres",NULL);
	    puts("No ejecuta muestra_corr1");
         }
      }
   }
   if(*buf=='\0') {
      //printf("..+++++%c+++++..\n",*buf1);
      n=sendto(fp,buf1,bytes1,0,(struct sockaddr *)&faddr,sizeof(faddr));
      if(n==-1)
        perror("Al enviar: ");
      printf("Enviados %ld caract.",n);
      fflush(stdout);
      usleep(3000);
      if(tipo_test || tipo_pres) {
        usleep(3000);
        n=sendto(fp,buf2,bytes2,0,(struct sockaddr *)&faddr,sizeof(faddr));
        printf("Enviados %ld caract..",n);
        usleep(3000);
        if(tipo_test) {
          n=sendto(fp,opcion,strlen(opcion)+1,0,(struct sockaddr *)&faddr,sizeof(faddr));
        }
        else {
          n=sendto(fp,buf3,bytes3,0,(struct sockaddr *)&faddr,sizeof(faddr));
        }
        printf("++++Enviados %ld caract...",n);
      }
      if(examen){
          n=sendto(fp,"\v",1,0,(struct sockaddr *)&faddr,sizeof(faddr));
          printf("-////Enviados %ld caract....",n);
      }
      if(cordinado){
          n=sendto(fp,"\f",1,0,(struct sockaddr *)&faddr,sizeof(faddr));
          printf("-////Enviados CordinadoNotebook %ld caract....",n);
      }
      if(tipo_note){
          n=sendto(fp,"\t",1,0,(struct sockaddr *)&faddr,sizeof(faddr));
          printf("-////Enviados Notebook %ld caract....",n);
      }
      if(una_opcion){
          n=sendto(fp,"u",1,0,(struct sockaddr *)&faddr,sizeof(faddr));
          printf("....Enviados %ld caract....",n);
      }
      if(ficha){
          n=sendto(fp,"f",1,0,(struct sockaddr *)&faddr,sizeof(faddr));
          printf("-..//// ///..Enviados %ld caract....",n);
      }
      if(resp_simples) {
        fflush(stdout);
        n=sendto(fp,"\0",1,0,(struct sockaddr *)&faddr,sizeof(faddr));
        printf("\nEnviados %ld caract........",n);
      }
      say_who_called(&faddr,host_port);
      fflush(stdout);
    }
    // si cordinado
    if(*buf=='\f') {
        sprintf(buf4,"%d",I);
       n=sendto(fp,buf4,strlen(buf4),0,(struct sockaddr *)&faddr,sizeof(faddr));
        printf("\nEnvio pag %s \n",buf4);
	fflush(stdout);
    }
    //
    if(*buf!='\0' && *buf!=' ' && *(buf+1) != '\v' && *buf != '\f') {
       // ha recibido las respuestas
      if(cordinado) {
	 printf("voy\n");
         graba_linea(&faddr,examen,tipo_pres,&es_primero,1);
	 printf("vuelvo\n");
      }
      else
         graba_linea(&faddr,examen,tipo_pres,&es_primero,tipo_note);
      buf[msglen]='\0';
      fprintf(fpd,"%s",buf);
      fflush(fpd);
      n=sendto(fp,"recv",4,0,(struct sockaddr *)&faddr,sizeof(faddr));
    }
 } // el while
 return (1);
}
void graba_linea(struct sockaddr_in *addrp,int examen,int tipo_pres,int *es_primero,int tipo_note){
   char *host;
   int port;
   host=(char *)inet_ntoa(addrp->sin_addr);
   port=ntohs(addrp->sin_port);
   if(examen||tipo_pres||tipo_note) {
     if(!(*es_primero))
       fprintf(fpd,"\f%s:%d:",host,port);
     else {
       fprintf(fpd,"%s:%d:",host,port);
       *es_primero=0;
     }
   }
   if(!examen && !tipo_pres && !tipo_note) {
     fprintf(fpd,"\n%s:%d:",host,port);
   }
   fflush(fpd);
}
void say_who_called(struct sockaddr_in *addrp,char *host_port){
   char *host;
   int port;
   host=(char *)inet_ntoa(addrp->sin_addr);
   port=ntohs(addrp->sin_port);
   sprintf(host_port,"%s:%d",host,port);
}
//
void para_coor(int sig) {
   I++;
}
//
void para_zombis(int sig) {
    while(waitpid(-1,0,WNOHANG)>0);
}
