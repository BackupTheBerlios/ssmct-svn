/* this file is part of ssmct 
 * see ssmct.c for copyright information
 */
#include <stdio.h>
#include <string.h>
void prepara_elecc(char *trabajo,char *smct,char *ps, char c);
char *obtener_respuestas(char *resp1,FILE* fp);
long dame_trabajo(FILE *fp,char *un_trabajo,long pos);
int trabajo_array(char **ar_resp,char *un_trabajo);
void mostrar(char **);
void devolver1(char **);
void devolver(char **);
void mostrar_corregir(char **ar_resp,int,char *,char);
void mostrar_corregir_nom(char **,char *);
void mostrar_corregir_tra(char **);
int preguntar_como(void);
int pedir_que_preg(char *);
char *pedir_que_nom(char *,char *,char);
char *obtener_nombre(char *,char *,char *);
void mostrar_pregunta_num(char **,int num_preg,char *tit2);

int main() {
     int pi[2],pid, termina,hay_resp, por_tra,por_nom, por_preg, como_ver,
         hay_file, npsal,impre,accion;
     FILE *fp3;
     long n;
     char s2[BUFSIZ],tit2[500], resul_ls[BUFSIZ], string[BUFSIZ],
          salida[BUFSIZ],salida1[BUFSIZ], file[250],file_awk[250],
                         file_back[250],st[250],ps[250],smct[250];
          char s3[BUFSIZ], *punt_trabajo, todos[200000], lpt[100], new[100];
     char trabajo[100], comando[250],*p,*p1,resp1[BUFSIZ],resp2[BUFSIZ],
          trabajo_corr[60],trabajo_corr1[60],psal[BUFSIZ],argumento[250],
          trabajo1[100], back_trabajo[100],*ar_resp[250],un_trabajo[50000];
    char nom_alu[250], borrar[60], el_comando[250],back[100];

     FILE *fp, *fp1;
     int num_preg,nl,i,ret,repasar,examen,registro;
     long pos,pos_final;
     hay_file=por_tra=por_preg=por_nom=repasar=examen=impre=0;
     registro=0;
     pos=0;
     *borrar='\0'; 
     //chdir("./bin");
    
     if(pipe(pi)==-1) {puts("error en pipe"); exit(1); }
     pid=fork();
     if(pid==-1) {puts("error en fork"); exit(1); }
     if(pid==0) {
          close(pi[0]);
          dup2(pi[1],1);
	  execlp("/usr/local/bin/greq","/usr/local/bin/greq","-t",
            "Utilidades ",
            "-m", "Deseas..","Quienes lo Realizaron","Corregir todos",
	         "Mostrar un corregido",
            "Generar Respuestas", "Repasar Respuestas","Ver/Corregir exámenes",
	    "Imprimir/Ver", NULL);
            
          printf("Error al ejecutar greq\n");
      }
      close(pi[1]);
      s2[0]='\0';
      n=read(pi[0],s2,BUFSIZ);
      s2[n]='\0';
      wait(NULL);
      if(*s2=='\0') {
         exit(1);
      }
      if(*s2=='Q') {
        prepara_elecc(trabajo,smct,ps,'Q');
        pedir_que_nom(nom_alu,trabajo,'Q');
	exit(1);
      }
      if(*s2=='I'){
         impre=1;
         prepara_elecc(trabajo,smct,ps,'V');
	 accion=1;
	 *el_comando=*argumento='\0';
         strcat(el_comando,"awk -f /usr/local/bin/impre_ex.awk ");       
         strcat(argumento,"e=0");       
	 *lpt='\0';
	 *new='\0';
	 *back='\0';
	 strcat(lpt,trabajo);
	 strcat(lpt,".lpt");
	 strcat(new,trabajo);
	 strcat(new,".new");
	 strcat(back,trabajo);
	 strcat(back,".bck");
	 while(accion) {
           if(pipe(pi)==-1) {puts("error en pipe"); exit(1); }
           pid=fork();
           if(pid==-1) {puts("error en fork"); exit(1); }
           if(pid==0) {
             close(pi[0]);
             dup2(pi[1],1);
	     sprintf(comando,"%s %s %s",el_comando,argumento,trabajo);
           //  execl("/bin/sh","sh","-c",comando,NULL);       
	    execlp("awk","awk","-f","/usr/local/bin/impre_ex.awk","-v",argumento,trabajo,NULL);
             fprintf(stderr,"E-E-E-E-E: %s EE--EE: %s EE--EE: %s\n",el_comando,argumento,trabajo);
           }
           close(pi[1]);
           s2[0]='\0';
           n=read(pi[0],todos,200000);
           todos[n]='\0';
           wait(NULL);
           if(*todos=='\0') {
             exit(1);
           }
         if(pipe(pi)==-1) {puts("error en pipe"); exit(1); }
         pid=fork();
         if(pid==-1) {puts("error en fork"); exit(1); }
         if(pid==0) {
           close(pi[0]);
           dup2(pi[1],1);
           //
	   //execlp("/usr/local/bin/greq","/usr/local/bin/greq","-t","Todos los trabajos", "-XPara imprimir","-Z",todos,"-bAcción","-r","ninguna\veliminar\v","-eEspecificar",NULL);
	   execlp("/usr/local/bin/greq","/usr/local/bin/greq","-t","Todos los trabajos", "-NPara imprimir\v\0","-Z",todos,"-bAcción","-r","ninguna\veliminar\vimprimir\v","-eEspecificar",NULL);
	   
           printf("Error al ejecutar greq\n");
         }
         close(pi[1]);
         s2[0]='\0';
         n=read(pi[0],s2,BUFSIZ);
         s2[n]='\0';
         wait(&termina);
         if(*s2=='\0') {
           exit(1); //Sale aquí, cuando es impre      
       }
       if(impre) {
	 p=strchr(s2,'\v');
	 *p='\0';
	 if(strcmp("ninguna",s2)==0) {
	   unlink(lpt);
	   unlink(new);
	   accion=0;
	 }
	 if(strcmp("imprimir",s2)==0) {
	   rename(new,trabajo);
	   accion=0;
	 }
	 if(strcmp("eliminar",s2)==0) {
	   sprintf(argumento,"%s%s","e=",p+1);
	   rename(trabajo,back);
	   rename(new,trabajo);
	   unlink(lpt);
	 }
       }
      }
      }
      if(*s2=='V'){
        examen=1;
        como_ver=preguntar_como();
        if(como_ver=='P')
          por_preg=1;
        if(como_ver=='U')
          por_nom=1;
        if(como_ver=='T')
          por_tra=1;
      }
      if(*s2=='R'){
        repasar=1;
      }
      if(*s2=='M'){
	 execlp("muestra","muestra", NULL);
	 puts("Error al ejecutar: muestra\n");
      }
      if(*s2=='C') {
        // aqui interesa corregir con puntuacion y generar un fichero
        // que luego se pueda imprimir. Como listado con notas
        //
        //  Ejecutar script awk
        //  El resultado de este script va luego como string a greq
        //  en texto.
//
          prepara_elecc(trabajo,smct,ps,*s2);
	  if(*smct=='F'&&*ps=='F') {
            sprintf(comando,"%s %s","awk -f /usr/local/bin/cor_una_opcion.awk",trabajo); 
          }
	  else {
            sprintf(comando,"%s %s","awk -f /usr/local/bin/corregir_notest.awk",trabajo); 
	  }
            *salida='\0';
            fp=popen(comando,"r");
            if(fp==NULL) {
              puts("problema"); exit(1);
            }
            while(fgets(string,BUFSIZ,fp)) {
                 strcat(salida,string);
            }
            pclose(fp);
//
           
           sprintf(tit2,"%s%70s%s%60s","-X","Corrección del Trabajo: ",
            trabajo+5," ");

          if(pipe(pi)==-1) {puts("error en pipe"); exit(1); }
          pid=fork();
          if(pid==-1) {puts("error en fork"); exit(1); }
          if(pid==0) {
             close(pi[0]);
             dup2(pi[1],1);
	     execlp("/usr/local/bin/greq","/usr/local/bin/greq","-t",
               "Corregir ",tit2,"-Z",salida,NULL);
             printf("EYTYTYTYTYTYTE\n");
          }
          close(pi[1]);
          s2[0]='\0';
          n=read(pi[0],s2,BUFSIZ);
          s2[n]='\0';
          wait(NULL);
          if(*s2=='\0') {
             exit(1);
          }
      } // cierro 'C'

      if(*s2=='G') {
          if(pipe(pi)==-1) {puts("error en pipe"); exit(1); }
          pid=fork();
          if(pid==-1) {puts("error en fork"); exit(1); }
          if(pid==0) {
             close(pi[0]);
             dup2(pi[1],1);
	     execlp("/usr/local/bin/greq","/usr/local/bin/greq","-t",
               "Generación de Respuestas","-eDe que Trabajo",NULL);
                printf("Error, no ejecuta greq\n");
           }
          close(pi[1]);
          s2[0]='\0';
          n=read(pi[0],s2,BUFSIZ);
          s2[n]='\0';
          wait(NULL);
          if(*s2=='\0') {
             exit(1);
          }
          strcpy(trabajo,"resp_");
          strcat(trabajo,s2);
       //Un comando awk que retoca el fich. de resp.
          pid=fork();
          if(pid==-1) {puts("error en fork"); exit(1); }
          if(pid==0) {
             execlp("/usr/bin/awk","/usr/bin/awk", "-f", "/usr/local/bin/resp.awk",
                      trabajo, NULL);
	     puts("TRTRTRTRTRTR");
          }
          ret=wait(NULL);
          if(ret!=pid)  { puts("debo continuar en el wait\n");
          printf("%d - %d\n",ret,pid);  exit(1);}
          strcpy(back_trabajo,"back.");
          strcat(back_trabajo,trabajo);
          ret=rename(back_trabajo,trabajo);
          if(ret==-1)
          { 
            perror("Error al Renombrar: \n¿Las Respuestas existen?");
            exit(1);
          } 
          if(ret==0)
            puts("Renombró con éxito");
            exit(0);
      } // el if de Generar Respuestas
      
       if(*s2=='R') {
         prepara_elecc(trabajo,smct,ps,*s2);
         sprintf(file,"%s%s","./back_",trabajo);
         fp1=fopen(file,"w");
         if(!fp1) {
           puts("Error al abrir file temporal");
           exit(1);
          }
          num_preg=1;
          *salida='\0';
          while(1) {
            sprintf(comando,"%s %s %s%d","awk -f /usr/local/bin/repasar_respuestas1.awk",
              trabajo,"a=",num_preg); 
            fp=popen(comando,"r");
            if(fp==NULL) {
              puts("problema"); exit(1);
            }
            if(obtener_respuestas(resp1,fp)==NULL)
                break;
            while((p=fgets(string,BUFSIZ,fp))!=NULL) {
                 nl=strlen(string);
                 string[nl-1]='\v';
                 strcat(salida,string);
            }
            if(salida[strlen(salida)-1]!='\v'){
               salida[strlen(salida)+1]='\0';
               salida[strlen(salida)]='\v';
            }
            pclose(fp);
            sprintf(tit2,"%s%80s%d%45s","-X","Repasamos posibles respuestas correctas de la preg....",num_preg," ");
            while(1) {
                if(pipe(pi)==-1) {puts("error en pipe"); exit(1); }
                pid=fork();
                if(pid==-1) {puts("error en fork"); exit(1); }
                if(pid==0) {
                  close(pi[0]);
                  dup2(pi[1],1);
	          execlp("/usr/local/bin/greq","/usr/local/bin/greq","-t",
                  "Agregar Respuestas",tit2,"-z",resp1,"-pDeseas Agregar","-b",
                  "-r",salida,"-c","Terminar","-oAgregar","-aSiguiente",NULL);
                  printf("EMMEEMMEEMMEEMM\n");
                }
                close(pi[1]);
                s2[0]='\0';
                n=read(pi[0],s2,BUFSIZ);
                s2[n]='\0';
                wait(&termina);
                if(*s2=='\0') {
                  break;
                }
		printf("---%s---\n",s2);
                p=strchr(s2,'\v');
                *p='\0';
                if(*(p+1)=='T') { // es como cancelar
                  exit(1);
                }
                strcat(resp1,"\n");
                strcat(resp1,s2);
                nl=strlen(s2);
               // quitar de salida
             psal[0]='\v';
             psal[1]='\0';
             strcat(psal,s2);
             npsal=strlen(psal);
             psal[npsal]='\v';
             psal[npsal+1]='\0';
                //p=strstr(salida,s2+1);
                p=strstr(salida,psal);
                if(p==NULL)
                   p=strstr(salida,psal+1);
                *p='\0';
                strcpy(salida1,salida);
                strcat(salida1,p+nl+1);
                strcpy(salida,salida1);
                if(*salida=='\0')
                  break;
           }
           for(i=0;resp1[i]!='\0';i++) {
            if(resp1[i]=='\n')
              resp1[i]=':';
           }
           sprintf(resp2,"%d:%s\n",num_preg,resp1);
           fprintf(fp1,"%s",resp2);
           num_preg++;
           *salida=*salida1=*resp1=*resp2='\0';
    }
    fclose(fp1);
    pid=fork();
    if(pid==-1) {puts("error en fork"); exit(1); }
    if(pid==0) {
       strcpy(file_awk,"/usr/local/bin/nuevas_resp.awk");
       execlp("awk","awk","-f", file_awk,
                file, trabajo,NULL);
       printf(".......OOOOOOOOOOO..........");
    }
     ret=wait(NULL);
     if(ret!=pid)  { puts("debo continuar en el wait\n");
     printf("%d - %d\n",ret,pid);  exit(1);};
     puts("Renombrando...");
     strcpy(file_back,"./file.back");
     sprintf(st,"%s%s","./",trabajo);
     ret=rename(file_back,st);
     if(ret==-1)
     { 
       perror("Error al Renombrar: ");
       exit(1);
     } 
     if(ret==0)
      puts("Renombró con éxito");
  } // el if de 'R'
  if(*s2=='V') {
         prepara_elecc(trabajo,smct,ps,'V');
          num_preg=1;
          *salida='\0';
          strcpy(trabajo_corr,"corr_");
          strcat(trabajo_corr,trabajo);
          if((fp3=fopen(trabajo_corr,"r"))==NULL) {
                           // quiere decir que no se
                           // comenzó a escribir
              fp=fopen(trabajo,"r");
              if(!fp) {
                 puts("....Error al abrir file trabajo....");
                 exit(1);
              }
              fp3=fopen(trabajo_corr,"w");
              strcpy(borrar,trabajo_corr);
              if(!fp3) {
                 puts("..====..Error al abrir file trabajo....");
                 exit(1);
              }
          }
          else {
              hay_file=1;
              fclose(fp3);
              strcpy(trabajo_corr1,"corr1_");
              strcat(trabajo_corr1,trabajo);
              printf("**--%s--**\n",trabajo_corr1);
              printf("**--%s--**\n",trabajo_corr);
              fp=fopen(trabajo_corr,"r");
              if(!fp) {
                 puts("..====..Error al abrir file corregido para leer..");
                 exit(1);
              }
              fp3=fopen(trabajo_corr1,"w");
              strcpy(borrar,trabajo_corr1);
              if(!fp3) {
                 puts("..====..Error al abrir file correg. para grabar..");
                 exit(1);
              }
          }
          pos=0;
          pos_final=fseek(fp,0L,SEEK_END);
          pos_final=ftell(fp);
          *nom_alu='\0';
          if(por_nom) {
               pedir_que_nom(nom_alu,trabajo,' ');
          }
          if(por_preg) {
               punt_trabajo=strchr(trabajo,'_');
               num_preg=pedir_que_preg(punt_trabajo+1);
               if(num_preg==-1) {
                 printf(" **  No está Activa la opción 'por Preguntas'\n     elija cualquiera de las otras **\n");
                 fflush(stdout);
                 unlink(borrar);
                 exit(1);
               }
          }
          while(pos < pos_final - 1) { // quedan registros por leer
            pos=dame_trabajo(fp,un_trabajo,pos);
            hay_resp=trabajo_array(ar_resp,un_trabajo); // tengo un_trabajo en
                                     // memoria- ar_resp[0] es datos,
                                    //  ar_resp[1] primera respuesta
            if(por_preg) {
               mostrar_corregir(ar_resp,num_preg,NULL,'p');
            }
            if(por_nom) {
               mostrar_corregir(ar_resp,0,nom_alu,'n');
            }
            if(por_tra) {
               fflush(stdout);
               mostrar_corregir(ar_resp,0,NULL,'t');
            }
            // aqui grabamos el registro en en corr_
            for(i=0;ar_resp[i]!=NULL;i++) {
               if(ar_resp[i+1]==NULL)
                 fprintf(fp3,"%s",ar_resp[i]);
               else
                 fprintf(fp3,"%s\v",ar_resp[i]);
            }
            fprintf(fp3,"%c",'\f');
            fflush(fp3);
            devolver(ar_resp);
          }
          fclose(fp3);
          fclose(fp);
          if(hay_file) {
	     printf("--%s--%s--\n",trabajo_corr1,trabajo_corr);
             ret=rename(trabajo_corr1,trabajo_corr);
             if(ret==-1) {
	         perror("aqui el problema:");
                puts("No ha renombrado");
	     }
          }
   }
}
void mostrar_corregir_tra(char **ar_resp) {

}
char *pedir_que_nom(char *nom_alu,char *trabajo,char c){
    char resul_nombres[BUFSIZ],string[BUFSIZ],tit2[250],s2[BUFSIZ];
    char trabajo1[250], comando[250];
    FILE *fp;
    int n,pi[2],pid;
            *resul_nombres='\0';
            sprintf(comando,"awk -f /usr/local/bin/obtener_nombres1.awk %s",trabajo);
            fp=popen(comando,"r");
            if(fp==NULL) {
              puts("problema"); exit(1);
            }
            while(fgets(string,BUFSIZ,fp)) {
                  string[strlen(string)-1]='\v';
                  strcat(resul_nombres,string);
            }
            pclose(fp);

           sprintf(tit2,"%s%40s%35s","-p","Trabajos Realizados y Respondidos"," ");

          if(pipe(pi)==-1) {puts("error en pipe"); exit(1); }
          pid=fork();
          if(pid==-1) {puts("error en fork"); exit(1); }
          if(pid==0) {
             close(pi[0]);
             dup2(pi[1],1);
	     if (c!='Q') {
                  execlp("/usr/local/bin/greq","/usr/local/bin/greq","-t",
                  "Corregir ",tit2,"-bCual deseas corregir","-r",resul_nombres,
                  NULL);
                  printf("EYTYTYxxxxxTYTYTYTE\n");
             }	
	     if(c=='Q') {
                  execlp("/usr/local/bin/greq","/usr/local/bin/greq","-t",
                  "Corregir ",tit2,"-bTrabajo realizado por","-r",resul_nombres,
                  NULL);
                  printf("EXXXXXE\n");
             }	
          }
          close(pi[1]);
          s2[0]='\0';
          n=read(pi[0],s2,BUFSIZ);
          s2[n]='\0';
          wait(NULL);
          if(*s2=='\0') {
             puts("NADA");
             exit(1);
          }
  strcpy(nom_alu,s2);
  return nom_alu;
}
void mostrar_corregir_nom(char **ar_resp,char *nom_alu){
    int i;
    char *p,el_nom[250];
    p=obtener_nombre(ar_resp[0],nom_alu,el_nom);
    if(p==NULL) {
       return; // no es este registro el del nombre buscado
    }
    i=1;
    printf("Respuestas de %s\n",el_nom);
    while(ar_resp[i]) {  // ciclar hasta acabar el trabajo -NULL en ar_resp[?]-
         // mostrando y corrigiendo cada pregunta del alumno el_nom
       printf("··_··_··_··_");
       printf("%s\n",ar_resp[i]);
       // Aqui corrijo .. y luego pasará a la siguiente
       i++;
       printf("··_··_··_··_");
    }
// ........  Acabé de corregir el trabajo de ese nombre
}

int pedir_que_preg(char *trabajo) {
   char string[BUFSIZ],s2[BUFSIZ], num_preg[30], *p, npreg[300], st_num[20];
   char trabajo_a[60], trabajo_b[60];
   int i,j,pid,n,pos_final,pi[2], termina,fpreg;
   FILE *fp,*fp1,*fp2;
   strcpy(trabajo_a,trabajo);
   strcpy(trabajo_b,trabajo);
   strcat(trabajo_a,".a");
   strcat(trabajo_b,".b");
   fp1=fopen(trabajo_a,"r");
   fp2=fopen(trabajo_b,"r");
   if(fp1 && fp2)  // es un tipo Presentacion, no va por preguntas
    {
      fclose(fp1);
      fclose(fp2);
      return -1;
    }
   fp=fopen(trabajo,"r");
   if(fp==NULL) {
     perror("al abrir trabajo");
     exit(1);
   }
   pos_final=fseek(fp,0L,SEEK_END);
   pos_final=ftell(fp);
   fseek(fp,0L,SEEK_SET);
   n=fread(string,1,pos_final,fp);
   string[n]='\0';
   // Averiguo cantidad de preguntas
   j=0;
   for(i=0;i<n;i++) {
    if(string[i]=='\n' && string[i+1]=='\n')
      j++; 
   }
   *npreg='\0';
   for(i=1;i<=j;i++) {
     sprintf(st_num,"%d\v",i);
     strcat(npreg,st_num);
   }
   // fin averiguar cant. de líneas, en string npreg tengo el argumento para
   // greq
   if(pipe(pi)==-1) {puts("error en pipe"); exit(1); }
   pid=fork();
   if(pid==-1) {puts("error en fork"); exit(1); }
   if(pid==0) {
      close(pi[0]);
      dup2(pi[1],1);
      execlp("/usr/local/bin/greq","/usr/local/bin/greq","-t","Elección",
                "-xLas Preguntas a Corregir","-z",string,
                "-bEliges número de Pregunta", "-r",npreg,NULL);
      printf("Error al ejecutar greq\n");
   }
   close(pi[1]);
   s2[0]='\0';
   n=read(pi[0],s2,BUFSIZ);
   s2[n]='\0';
   wait(&termina);
   if(*s2=='\0') {
      exit(1);      
   }
   p=strchr(s2,'\v');
   if(p) {
       strcpy(num_preg,p+1);
   }
   fpreg=atoi(num_preg);
   return fpreg;
}
char *obtener_nombre(char *s,char *nombre,char *el_nom) {
   char *p,*p1;
   p=p1=NULL;
   p=strchr(s,':');   // p+1 apunta al segundo campo
   if(p==NULL) {
       puts("No hay");
       exit(0);
   }
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
void mostrar_corregir(char **ar_resp,int num_preg,char *nom,char car) {
   char el_nom[250],nombre[250],*p,*p1, tit2[500],tit1[250], s2[BUFSIZ];
   int i,n, pid, pi[2], termina;
   *nombre='\0'; 
   if(car=='p' || car=='t') { // es por pregunta ó trabajo
     obtener_nombre(ar_resp[0],nombre,el_nom);
   }
   if(car=='n') { // es por nombre entonces mostrar
     p=obtener_nombre(ar_resp[0],nom,el_nom);
     if(p==NULL)
       return;
   }
   if(car=='p') {
     sprintf(tit1,"Trabajo de: %s ... Pregunta: %d",el_nom,num_preg);
     sprintf(tit2,"%s%80s%45s","-x",tit1," ");
     mostrar_pregunta_num(ar_resp,num_preg,tit2);
     return;
   }
  
   i=0;
   while(ar_resp[++i]) {
      sprintf(tit1,"Trabajo de: %s ... Pregunta: %d",el_nom,i);
      sprintf(tit2,"%s%80s%45s","-x",tit1," ");
      mostrar_pregunta_num(ar_resp,i,tit2);
   }
}
void mostrar_pregunta_num(char **ar_resp,int num_preg,char *tit2) {
   int pid, pi[2],n,termina;
   char s2[BUFSIZ], *p, con[100], string[BUFSIZ], mostrar_nota[100];
     if(pipe(pi)==-1) {puts("error en pipe"); exit(1); }
     pid=fork();
     if(pid==-1) {puts("error en fork"); exit(1); }
     if(pid==0) {
        close(pi[0]);
        dup2(pi[1],1);
	//
	*mostrar_nota='\0';
	if(ar_resp[num_preg][0]=='n'){
	  // con n
          strcpy(mostrar_nota,"r\vr-\vr+\vb\vn^\v");
	}
	if(ar_resp[num_preg][0]=='b'){
	  // con b
          strcpy(mostrar_nota,"r\vr-\vr+\vb^\vn\v");
	}
	if(ar_resp[num_preg][0]=='r' && ar_resp[num_preg][1]==':'){
	  // con r
          strcpy(mostrar_nota,"r\vr-\vr+\vb\vn\v");
	}
	if(ar_resp[num_preg][0]=='r' && ar_resp[num_preg][1]=='+'){
	  // con r+
          strcpy(mostrar_nota,"r\vr-\vr+^\vb\vn\v");
	}
	if(ar_resp[num_preg][0]=='r' && ar_resp[num_preg][1]=='-'){
	  // con r-
          strcpy(mostrar_nota,"r\vr-^\vr+\vb\vn\v");
	}
	if(*mostrar_nota=='\0') {
	  // no había correció (es la primera vez)
          strcpy(mostrar_nota,"r\vr-\vr+\vb\vn\v");
	}
	//
        execlp("/usr/local/bin/greq","/usr/local/bin/greq","-t",
                "Podemos Corregir los Trabajos",tit2,"-z",ar_resp[num_preg],
                "-bCorregir con", "-r",mostrar_nota,NULL);
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
     p=strchr(s2,'\v');
     if(p) {
       strcpy(con,p+1);
       *p='\0';
     }
     // GRABAR en corr1_trabajo
     sprintf(string,"%s:%s",con,s2);
     ar_resp[num_preg]=(char *)malloc(strlen(string)+1);
     strcpy(ar_resp[num_preg],string);
}

void devolver(char **a) {
    if(a[0]) // hay solo un requerimiento
    {
      free(a[0]);
    }
}
void devolver1(char **a) {
    int i=0;
    while(a[i]) {
      free(a[i]);
      i++;
    }
}
void mostrar(char **a) {
    while(*a) {
       printf("· · · · · · · · · · · · ·");
       printf("%s\n",*a);
       a++;
    }
}
long dame_trabajo(FILE *fp,char *un_trabajo,long pos) {
    int pos_ini, i, car;
    i=0;
    pos_ini=fseek(fp,pos,SEEK_SET);
    while((car=fgetc(fp))!='\f') {
      if(car==EOF)
         break;
      un_trabajo[i++]=car;
    }
    un_trabajo[i]='\0';
    return (ftell(fp));
}
int trabajo_array(char **ar_resp,char *un_trabajo) {
      char *p,*p1;
      int i,n,k;
      k=i=0;
      n=strlen(un_trabajo)+1;
      ar_resp[0]=(char *)malloc(n);
      strcpy(ar_resp[0],un_trabajo);
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

void prepara_elecc(char *trabajo,char *smct,char *ps, char c) {
    char resul_ls[BUFSIZ],string[BUFSIZ],tit2[250],s2[BUFSIZ];
    char trabajo1[250];
    FILE *fp;
    int n,pi[2],pid;
            *resul_ls='\0';
            fp=popen("ls resp_*","r");
            if(fp==NULL) {
              puts("problema"); exit(1);
            }
            while(fgets(string,BUFSIZ,fp)) {
                  string[strlen(string)-1]='\v';
                  if(strncmp(string,"resp_",5)==0)
                        strcat(resul_ls,string+5);
            }
            pclose(fp);

           sprintf(tit2,"%s%40s%35s","-p","Trabajos Realizados y Respondidos"," ");

          if(pipe(pi)==-1) {puts("error en pipe"); exit(1); }
          pid=fork();
          if(pid==-1) {puts("error en fork"); exit(1); }
          if(pid==0) {
             close(pi[0]);
             dup2(pi[1],1);
             if(c=='C') {
                execlp("/usr/local/bin/greq","/usr/local/bin/greq","-t",
                  "Corregir ",tit2,"-bCual deseas corregir","-r",resul_ls,
                  "-cTipo Test", "-cRespuestas simples","-cUna Opción",NULL);
                printf("EYTYTYTYTYTYTE\n");
              }
              if(c=='V' || c=='R') {
	         execlp("/usr/local/bin/greq","/usr/local/bin/greq","-t",
                 "Corregir ",tit2,"-bCual deseas corregir","-r",resul_ls,NULL);
                  printf("-----EYTNNNAAS-------\n");
              }
              if(c=='Q') {
	         execlp("/usr/local/bin/greq","/usr/local/bin/greq","-t",
                 "¿Quienes lo hicieron? ",tit2,"-bCuál deseas ver","-r",resul_ls,NULL);
                  printf("-----AASAAIAI-------\n");
	      }
           }
          close(pi[1]);
          s2[0]='\0';
          n=read(pi[0],s2,BUFSIZ);
          s2[n]='\0';
          wait(NULL);
          if(*s2=='\0') {
             puts("NADA");
             exit(1);
          }
          strcpy(trabajo,"resp_");
          if(c=='C') {
            sscanf(s2,"%s\v%s\v%s",trabajo1,smct,ps);
            strcat(trabajo,trabajo1);
          }
          if(c=='V' || c=='R' || c=='Q') {
            strcat(trabajo,s2);
          }
}

char * obtener_respuestas(char *resp1,FILE* fp) {
   char *p;
   p=fgets(resp1,BUFSIZ,fp);
   if(p==NULL) { pclose(fp);return NULL; }
   resp1[strlen(resp1)-1]='\0';
   while((p=strchr(resp1,':'))!=NULL) {
       *p='\n';
   }
   return resp1;
}
int preguntar_como(void) {
     int n,pi[2],pid;
     char s2[500];
     if(pipe(pi)==-1) {puts("error en pipe"); exit(1); }
     pid=fork();
     if(pid==-1) {puts("error en fork"); exit(1); }
     if(pid==0) {
          close(pi[0]);
          dup2(pi[1],1);
	  execlp("/usr/local/bin/greq","/usr/local/bin/greq","-t",
            "Ver Exámenes",
            "-m", "Deseas ver","Por preguntas","Trabajo Completo","Un nombre",NULL);
          printf("EEEEEEEEEEEEEEEEEEEEEEEEEE\n");
      }
      close(pi[1]);
      s2[0]='\0';
      n=read(pi[0],s2,BUFSIZ);
      s2[n]='\0';
      wait(NULL);
      if(*s2=='\0') {
         exit(1);
      }
      return *s2;
}
