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
char *p, *fin, *ps, s2[BUFSIZ],s[BUFSIZ],nombre[250],st[250],text[70000],tit[1500],tit2[1500],tit0[1500];
char resp_trabajo[100],nom_a_buscar[100] ,trabajo[100];
FILE *fp;
char op, op_trabajo[100];
int i,seguir;
  char text1[BUFSIZ],s1[BUFSIZ], *fin1,*fin2, *ar_preg[500], preguntas[70000], *ar_resp[500];
  char *ar_correctas[500];
	  
  FILE *fp1,*fp2;
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
            "-eNombre del Trabajo",  
            "-cPoner Opciones",
            NULL);
            printf("EEEEEEEEEEEEEEEEEEEEEEEEEE\n");
        }
        close(pi[1]);
        s2[0]='\0';
        n=read(pi[0],s2,BUFSIZ);
        s2[n]='\0';
        if(*s2=='\0')
           exit(1);
        for(i=0;s2[i]!='\0'&&s2[i]!='\v';i++);
        if(s2[i]=='\v') {
          s2[i]='\0';
          strcpy(resp_trabajo,"resp_");
          strcat(resp_trabajo,s2);
          strcpy(trabajo,s2);
          op=s2[i+1];
          if(op=='T') {
            strcpy(op_trabajo,"op_");
            strcat(op_trabajo,trabajo);
          }
        }
        else { exit(1); }
        i=0;


//
// Si hay que cargar opciones este es el momento, junto con las preguntas.
//
// en este cargamos las preguntas
//
 fp1=fopen(trabajo,"r");
 if(fp1==NULL) exit(1);
//
// Aqui mismo vemos si se grabaron respuestas, si no se sale con mensaje
//
   fp2=fopen(resp_trabajo,"r");
   if(fp2==NULL) exit(1);
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
 if(op=='T'){ //hay que juntar a cada pregunta con su opciones
  fp2=fopen(op_trabajo,"r");
  if(fp2==NULL) exit(1);
 }
 while(1) {
  while(fin1=fgets(s1,BUFSIZ,fp1)) {
	if(*s1=='\n')
	  break;
	strcat(text1,s1);
  }
  if(fin1==NULL)
    break;
  if(op=='T') { // le agregamos las correspondientes opciones
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
no_mas=0;
while(1) {
  de_quien(s2);
 // ya tenemos quien hay que buscar
  strcpy(nom_a_buscar,s2);
  primero=1;
while(1) {
  while(!primero&&(fin=fgets(s,BUFSIZ,fp))) {
	if(*s=='\n') {
	  break;
        }
  }
  primero=0;
  if(fin==NULL) {
    //Mostrar dialog
    puts("No hay nadie con ese nombre");
    fp=fopen(resp_trabajo,"r");
    if(fp==NULL) {puts("No puedo abrir file");exit(1);}
    while(fin=fgets(s,BUFSIZ,fp)) {
      if(*s=='\n')
         break;
    }
    no_mas=1;
    break;
  }
  p=fgets(s,BUFSIZ,fp);
  if(p==NULL)
	exit(1);
  p[strlen(p)-1]='\0';

  p=strstr(s,nom_a_buscar);
  
  if(p!=NULL) {
	// mostrar y pedir confirmacion
        for(--p; *p!=':'; --p);	
	++p;
        if(pipe(pi)==-1) {puts("error en pipe"); exit(1); }
        pid=fork();
        if(pid==-1) {puts("error en fork"); exit(1); }
        if(pid==0) { 
          close(pi[0]);
          dup2(pi[1],1);
	  sprintf(st,"-d%s",p);
            execlp("/usr/local/bin/greq","/usr/local/bin/greq","-t",
            "Confirmas? ",
            "-eApellidos y Nombre",st,
            "-cSeguir Buscando",
            NULL);
            printf("EEEEEEEEEEEEEEEEEEEEEEEEEE\n");
        }
        close(pi[1]);
        s2[0]='\0';
        n=read(pi[0],s2,BUFSIZ);
        s2[n]='\0';
        if(*s2=='\0')
           exit(1);
        seguir=0;
        for(i=0;s2[i]!='\0';i++) {
           if(s2[i]!='\v')
             nombre[i]=s2[i];
           else {
             nombre[i]='\0';
             if(s2[++i]=='T')
               seguir=1;
             break;
           }
        }
        if(!seguir)
          break;
  }
 }
/*
  char text1[BUFSIZ],s1[BUFSIZ], *fin1, *ar_preg[500], preguntas[70000];
  FILE *fp1;
*/
if(no_mas) {
  no_mas=0;
}
else {
 *preguntas=*text1='\0';
 i=0;
/*
*/
  *text='\0';
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
  generar_preguntas_resp(ar_preg,ar_resp,preguntas,ar_correctas);
  
        if(pipe(pi)==-1) {puts("error en pipe"); exit(1); }
        pid=fork();
        if(pid==-1) {puts("error en fork"); exit(1); }
        if(pid==0) { 
          close(pi[0]);
          dup2(pi[1],1);
          sprintf(tit,"%s","Respuestas");
          sprintf(tit2,"%s%65s %s %65s","-X","Respuestas de:",nombre," ");
          sprintf(tit0,"%s%80s %85s","-X","Preguntas"," ");
          execlp("/usr/local/bin/greq","/usr/local/bin/greq","-t",tit,
           tit2,"-Z",preguntas,NULL);
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
        fp=fopen(resp_trabajo,"r");
        if(fp==NULL) {puts("No puedo abrir file");exit(1);}
        i=0;
        while(fin=fgets(s,BUFSIZ,fp)) {
	  if(*s=='\n')
	     break;
        }
}// el else
}
return 1;
}
void generar_preguntas_resp(char **a,char **b,char *preg,char **correctas) {
  char string[BUFSIZ];
  while(*a) {
   strcat(preg,*a);
   strcat(preg,"\nRespuesta: ");
   strcat(preg,*b);
   *string='\0';
   corregir(*b,*correctas,string);
   strcat(preg,string);
   strcat(preg,"\n\n");
   a++; b++; correctas++;
  }
}
void corregir(char *b,char *c,char *s) {
// las respuesta (en b) viene en un string:
//   se trata de separar estos campos. FS=":"
// para ello usamos strtok
    char *p,*pu,st[BUFSIZ];

     strcpy(st,c);
     p=strtok(st,":");
     if(p==NULL) {  // es tipo test
       if(strcmp(c,b)==0) {
          strcpy(s,"|[b]CORRECTA[b]|");
       }
       else {
           strcpy(s,"|[r]INCORRECTA[r]|");
           strcat(s,"\nLa Respuesta es: ");
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
                break;
           }
	   else {
                p=strtok(NULL,":");
           }
         }
         if(p==NULL) {
		strcpy(s,"|[r]INCORRECTA[r]|");
                strcat(s,"\nLa Respuesta es: ");
                pu=strchr(c,':');
                if(pu) {
                   strncat(s,c,pu-c);
                   puts("ssaallggoo ddee incorecta....");
                }
                else
                   strcat(s,c);
          }
     }
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
        if(*s2=='\0')
           exit(1);
}
