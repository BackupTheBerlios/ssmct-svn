#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif


#include "callbacks_t.h"
#include "interface_t.h"
#include "support_t.h"
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <stdlib.h>
void cargar_file(char *name1,char **titulo);
void UpdateSeconds(int);

gboolean gb_resol=FALSE;
gboolean gb_sirve=TRUE;
gboolean gb_ejer=FALSE;
gboolean es_esta=TRUE;
gboolean es_otra=FALSE;
gboolean final=FALSE;
gboolean prime=TRUE;
GdkColor rojo={0,0xffff,0x0000,0x0000};
GdkColor verde={0,0x0000,0xffff,0x0000};
GdkColor azul={0,0x0000,0x0000,0xffff};
GdkFont *font;
GdkFont *font1;
char **ar[3];
  char *f[500], *s1[500], *e[500];
gdouble  parcial=0;
gchar buf_env[BUFSIZ];
gint tiempo,cont=0;
gint conti=0;
gint nl=0;
  gdouble num[5];
static int nSeconds=0;
static gint timer=0;
static int bTimerRunning=FALSE;
GtkWidget *label1;
GtkWidget *label2;
GtkWidget *label3;

/*---------------------------------------*/

void cargar_file(char *name1,char **titulo) {
  char *p, s[BUFSIZ], tit[BUFSIZ];
  char texto[15000];
  FILE *fp1;
  int i,j,k; 
  ar[0]=f; 
  ar[1]=s1; 
  ar[2]=e; 
  fp1=fopen(name1,"r");
  if(!fp1) {
     puts("problema al abrir ficheros");
     exit(1);
  }
//
//el primer registro es de título
//
  i=0;
  p=fgets(s,BUFSIZ,fp1);
  if(*s=='\n') {
     // entonces no hay título ni tiempo ni penalización, ni puntuación
  }

  else {
     strcpy(tit,s);
     p=strtok(tit,":");
     while(p!=NULL) {
        titulo[i]=(char *)malloc(strlen(p)+1);
        strcpy(titulo[i],p);
        //puts(titulo[i]);
        p=strtok(NULL,":");
        i++;
     }
  }
  // ahora cargo presentación o segundo título
  *texto=*s='\0';
  while(p=fgets(s,BUFSIZ,fp1)) {
       if(*s!='\n') {
         strcat(texto,s);
       }
       else {
         titulo[i]=(char *)malloc(strlen(texto)+1);
         strcpy(titulo[i],texto);
         *texto=*s='\0';
         break;
       }
  }
//
  titulo[++i]=NULL;
  i=0;
  while(1) {
   for(cont=0;cont<3;cont++) {
     *texto=*s='\0';
     while(p=fgets(s,BUFSIZ,fp1)) {
       if(*s!='\n') {
         strcat(texto,s);
       }
       else {
         ar[cont][i]=(char *)malloc(strlen(texto)+1);
         strcpy(ar[cont][i],texto);
         *texto=*s='\0';
         break;
       }
     }
     if(p==NULL) {
       ar[0][i]=ar[1][i]=ar[2][i]=NULL;
       i=0;
       *texto=*s='\0';
       break;
     }
   }
   if(p==NULL) break;
   i++;
  }
  fclose(fp1);
  // Cargar_fuentes
  font=gdk_font_load("-misc-fixed-bold-r-*-*-*-140-*-*-*-*-*-*");
font1=gdk_font_load("-adobe-times-medium-i-normal--12-120-75-75-p-63-iso8859-1");
  for(i=1;i<=4;i++) {
    num[i]=strtod(titulo[i],NULL);
  }
  //printf("%.2f - %.2f - %.2f - %.2f\n",num[1],num[2],num[3],num[4]);
  *buf_env='\n';
  buf_env[1]='\0';
}
gint TimerCallback(gpointer data)
{
  nSeconds++;
  UpdateSeconds(nSeconds);
}
void StartTimer()
{
  if(!bTimerRunning) {
    nSeconds=0;
    timer=gtk_timeout_add(1000,TimerCallback,NULL);
    bTimerRunning=TRUE;
  }
}
void StopTimer()
{
  if(bTimerRunning) {
   gtk_timeout_remove(timer);
   bTimerRunning=FALSE;
  }
}
void UpdateSeconds(int nSeconds)
{
  char buffer[44];
  sprintf(buffer,"Time: %d", nSeconds);
  gtk_label_set(GTK_LABEL (label1),buffer);
}
void Etiquetas(double pultima,int cont) {
  char buffer[150];
  sprintf(buffer,"Puntuación última Respuesta: %.2g/%.2g",pultima,num[1]);
  gtk_label_set(GTK_LABEL (label2),buffer);
  parcial+=pultima;
  sprintf(buffer,"Puntuación Parcial: %.2g/%.2g",parcial,cont*num[1]);
  gtk_label_set(GTK_LABEL (label3),buffer);
}
gint
on_Test_destroy                        (GtkObject       *object,
                                        gpointer         user_data)
{
 if(final) {
   unlink("/tmp/FILE1");
   if(write(1,buf_env,strlen(buf_env))!=strlen(buf_env))
      fprintf(stderr,"Error al escribir en pipe desde p. hijo\n");
   gtk_main_quit();
   return (FALSE);
 }
 return (TRUE);
}


void
on_esta_clicked                        (GtkButton       *button,
                                        gpointer         user_data)
{

  es_esta=TRUE;
  es_otra=FALSE;

}


void
on_otra_clicked                        (GtkButton       *button,
                                        gpointer         user_data)
{
  es_esta=FALSE;
  es_otra=TRUE;
}


void
on_Corrija_clicked                     (GtkButton       *button,
                                        gpointer         user_data)
{
}

/*----------------------------------------------------------------------------*/
void
on_button3_clicked                     (GtkButton       *button,
                                        gpointer         user_data)
{
GtkWidget *text1 = lookup_widget(GTK_WIDGET(button), "text1");
GtkWidget *esta = lookup_widget(GTK_WIDGET(button), "esta");
GtkWidget *otra = lookup_widget(GTK_WIDGET(button), "otra");
GtkWidget *Corrija = lookup_widget(GTK_WIDGET(button), "Corrija");
GtkWidget *button3 = lookup_widget(GTK_WIDGET(button), "button3");
GtkWidget *info_label = lookup_widget(GTK_WIDGET(button), "info_label");
GtkWidget *label5 = lookup_widget(GTK_WIDGET(button), "label5");
GtkWidget *label6 = lookup_widget(GTK_WIDGET(button), "label6");
GtkWidget *dialog, *dg_label, *dg_OK;
gint x,n,i;

gdouble pultima=0;
gchar c, s1[50], *buffer, *p1, buf[1024];
label1=info_label;
label2=label5;
label3=label6;

   if(ar[0][conti]==NULL) {
      return;
   }
   if(prime) {
      prime=FALSE;
      n=gtk_text_get_point (GTK_TEXT(text1));
      n=gtk_text_backward_delete (GTK_TEXT(text1),n);
      gtk_text_insert (GTK_TEXT(text1),font,NULL,NULL,ar[0][conti],-1);
      gtk_text_insert (GTK_TEXT(text1), font, NULL,NULL,"\n",-1);
      if(ar[1][conti][0]=='*')
        p1=(ar[1][conti])+1;
      else
        p1=ar[1][conti];
      gtk_text_insert (GTK_TEXT(text1),font, &text1->style->black,NULL,p1,-1);
      nl=strlen(p1);
      StartTimer();
      gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(Corrija),FALSE);
   }
   buffer=gtk_editable_get_chars(GTK_EDITABLE(text1),0,-1);
   if(TRUE==gtk_toggle_button_get_active(GTK_TOGGLE_BUTTON(Corrija))) { 
     StopTimer();
     n=gtk_text_backward_delete (GTK_TEXT(text1),nl);
     if(es_otra) {
       if(ar[2][conti][0]=='*') {
          //printf("--%d--BIEN\n",conti);
	  if(nSeconds<=num[3]){
            pultima=num[1];
            c='b';
	  }
	  else {
            pultima=num[4];
            c='t';
	  }
          //en roja 1 y azul 2
          // insertar ambas
         gtk_text_insert (GTK_TEXT(text1), font, &rojo,NULL,ar[1][conti],-1);
         gtk_text_insert (GTK_TEXT(text1), font,&azul,NULL,ar[2][conti]+1,-1);
         gtk_text_insert (GTK_TEXT(text1), NULL,NULL,NULL,"\n\n",-1);
       }
       else {
         pultima=num[2];
         c='n';
         gtk_text_insert (GTK_TEXT(text1), font, &azul,NULL,ar[1][conti]+1,-1);
         gtk_text_insert (GTK_TEXT(text1), font, &rojo,NULL,ar[2][conti],-1);
         gtk_text_insert (GTK_TEXT(text1), NULL,NULL,NULL,"\n\n",-1);
       }
     }
     if(es_esta) {
       if(ar[1][conti][0]=='*'){
	 if(nSeconds<=num[3]){
           pultima=num[1];
           c='b';
         }
	 else {
            pultima=num[4];
	    c='t';
	 }
        //printf("--%d--BIEN\n",conti);
          // en azul 1 y roja 2
          // insertar ambas
        gtk_text_insert (GTK_TEXT(text1), font, &azul,NULL,ar[1][conti]+1,-1);
        gtk_text_insert (GTK_TEXT(text1), font, &rojo,NULL,ar[2][conti],-1);
        gtk_text_insert (GTK_TEXT(text1), NULL,NULL,NULL,"\n\n",-1);
       }
       else {
         pultima=num[2];
         c='n';
         gtk_text_insert (GTK_TEXT(text1), font, &rojo,NULL,ar[1][conti],-1);
         gtk_text_insert (GTK_TEXT(text1), font,&azul,NULL,ar[2][conti]+1,-1);
         gtk_text_insert (GTK_TEXT(text1), NULL,NULL,NULL,"\n\n",-1);
       }
     }
     conti++;
     Etiquetas(pultima,conti);
     sprintf(buf,"%d:%c:%.2g\n",conti,c,pultima);
     strcat(buf_env,buf);
     if(ar[0][conti]==NULL) {
          gtk_text_insert (GTK_TEXT(text1), font, NULL,NULL,"\n\n\n  --FIN--",-1);
	  final=TRUE;
          return;
     }
     gtk_text_insert (GTK_TEXT(text1), font, &text1->style->black,NULL,ar[0][conti],-1);
     gtk_text_insert (GTK_TEXT(text1), font, NULL,NULL,"\n",-1);
     if(ar[1][conti][0]=='*')
        p1=(ar[1][conti])+1;
     else
        p1=ar[1][conti];
     nl=strlen(p1);
     gtk_text_insert (GTK_TEXT(text1), font, &text1->style->black,NULL,p1,-1);
     StartTimer();
     g_free(buffer);
     gtk_toggle_button_set_active(GTK_TOGGLE_BUTTON(Corrija),FALSE);
   }
   else {
     g_free(buffer);
   }
 // n=gtk_text_backward_delete (GTK_TEXT(text1),n);
//  gtk_text_insert (GTK_TEXT(text1), NULL, NULL,NULL,buffer,strlen(buffer));
}
/*---------------------------------------*/

