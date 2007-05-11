/* this file is part of ssmct 
 * see ssmct.c for copyright information
 */
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
void generar_preguntas_resp(char **ar_preg,char **ar_resp,char *preguntas,char **ar_correctas);
void corregir(char *b,char *c,char *s);
void de_quien(char *s2);

int main(int argc, char **argv) {
int pi[2], pid,primero,termina,no_mas;
long n;
char *p, *fin, *ps, s2[BUFSIZ],s[BUFSIZ],nombre[250],st[250],text[70000],tit[1500],tit2[1500],tit0[1500], arg_nombre[250];
char resp_trabajo[100],nom_a_buscar[100] ,trabajo[100];
FILE *fp;
int i,seguir;
  char text1[BUFSIZ],s1[BUFSIZ], *fin1, *ar_preg[500], preguntas[70000], *ar_resp[500];
  char *ar_correctas[500];
	  
  FILE *fp1;
// recibir nombre por dialogo
        if(pipe(pi)==-1) {puts("error en pipe"); exit(1); }
        pid=fork();
        if(pid==-1) {puts("error en fork"); exit(1); }
        if(pid==0) { 
          close(pi[0]);
          dup2(pi[1],1);
          *st='\0';
            execlp("/usr/local/bin/greq","/usr/local/bin/greq","-t",
            "Confirmas? ",
            "-eNombre del TTrabajo",  
            "-eTrabajo del alumno/a",  
            NULL);
            printf("EEEEEEEEEEEEEEEEEEEEEEEEEE\n");
        }
        close(pi[1]);
        s2[0]='\0';
        n=read(pi[0],s2,BUFSIZ);
        s2[n]='\0';
        if(*s2=='\0')
           exit(1);
strcpy(resp_trabajo,"resp_");
p=strchr(s2,'\v');
*p='\0';
strcat(resp_trabajo,s2);
strcpy(nombre,p+1);

        strcpy(arg_nombre,"a=");
        strcat(arg_nombre,nombre);	
        if(pipe(pi)==-1) {puts("error en pipe"); exit(1); }
        pid=fork();
        if(pid==-1) {puts("error en fork"); exit(1); }
        if(pid==0) { 
          close(pi[0]);
          dup2(pi[1],1);
	  sprintf(st,"-d%s",p);
            execlp("/usr/bin/awk","/usr/bin/awk","-f","muestra.awk",
			    arg_nombre,resp_trabajo,NULL);
            printf("EEEEEEEEEEEEEEEEEEEEEEEEEE\n");
        }
        close(pi[1]);
        s2[0]='\0';
        n=read(pi[0],s2,BUFSIZ);
	printf("%s\n",s2);


  
// 
  //generar_preguntas_resp(ar_preg,ar_resp,preguntas,ar_correctas);
  
        if(pipe(pi)==-1) {puts("error en pipe"); exit(1); }
        pid=fork();
        if(pid==-1) {puts("error en fork"); exit(1); }
        if(pid==0) { 
          close(pi[0]);
          dup2(pi[1],1);
          sprintf(tit,"%s","Respuestas");
          sprintf(tit2,"%s%80s %s %80s","-X","Respuestas de:",nombre," ");
          sprintf(tit0,"%s%80s %85s","-X","Preguntas"," ");
          execlp("/usr/local/bin/greq","/usr/local/bin/greq","-t",tit,
           tit2,"-z",s2,NULL);
           //"-X","-z",preguntas,NULL);
          puts("no ejecuto exec");
        }

        close(pi[1]);
        s2[0]='\0';
        n=read(pi[0],s2,BUFSIZ);
        s2[n]='\0';
        //if(*s2=='\0')
           //exit(1);
       wait(&termina);
        if((termina>>8)==1) // ha cancelado
         {
           exit(1);
         }
	printf("SALGO\n");
	exit(1);
        fp=fopen(resp_trabajo,"r");
        if(fp==NULL) {puts("No puedo abrir file");exit(1);}
        i=0;
        while(fin=fgets(s,BUFSIZ,fp)) {
	  if(*s=='\n')
	     break;
        }
}
void generar_preguntas_resp(char **a,char **b,char *preg,char **correctas) {
  char string[100];
  while(*a) {
   strcat(preg,*a);
   strcat(preg,"\nRespuesta: ");
   strcat(preg,*b);
   *string='\0';
   corregir(*b,*correctas,string);
   strcat(preg,string);
   strcat(preg,"\n");
   a++; b++; correctas++;
  }
}
void corregir(char *b,char *c,char *s) {
	if(strcmp(b,c)==0)
		strcpy(s,"[CORRECTA]");
	else
		strcpy(s,"[---INCORRECTA---]");
}
void de_quien(char *s2) {
  int n,pid, pi[2];
        if(pipe(pi)==-1) {puts("error en pipe"); exit(1); }
        pid=fork();
        if(pid==-1) {puts("error en fork"); exit(1); }
        if(pid==0) { 
          close(pi[0]);
          dup2(pi[1],1);
            execlp("/usr/local/bin/greq","/usr/local/bin/greq","-t",
            "Confirmas? ",
            "-eEl trabajo de quien?",
            NULL);
            printf("EEEEEEEEEEEEEEEEEEEEEEEEEE\n");
        }
        close(pi[1]);
        s2[0]='\0';
        n=read(pi[0],s2,BUFSIZ);
        s2[n]='\0';
printf("mmmmmmmmm--%s--mmmmmmmm\n",s2);
        if(*s2=='\0')
           exit(1);
}
