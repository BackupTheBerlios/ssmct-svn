#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <gtk/gtk.h>

#include "callbacks.h"
#include "interface.h"
#include "support.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
void cargar_file(char *name1, char *name2, char *name3,char *titulo,char ***ent);

GdkFont *fixed_font;

extern gchar tit4[];
extern gchar tit5[];

gboolean gb_sirve=FALSE;
gboolean gb_ejer=TRUE;
char **ar[3];
char *f[500], *s1[500], *e[500];
int resp[500];
char *ar_resp[500];
int cont=0;
int de_ejer=0;
int editable=0;
int k=0;
int n1=0;
char string[BUFSIZ];

/*---------------------------------------*/

void cargar_file(char *name1, char *name2, char *name3, char *titulo,char ***ent) {
  char *p, s[BUFSIZ];
  char texto[15000];
  FILE *ar_file[3]; 
  FILE *fp1, *fp2, *fp3;
  int i,j,hay_resp; 
  fixed_font = gdk_font_load ("-misc-fixed-medium-r-*-*-*-140-*-*-*-*-*-*");
  ar[0]=f;  ar[1]=s1;  ar[2]=e;
  for(i=0;i<500;i++) {
       resp[i]=2;
       ar_resp[i]=NULL;
  }
  fp1=fopen(name1,"r");
  fp2=fopen(name2,"r");
  fp3=fopen(name3,"r");

  ar_file[0]=fp1;
  ar_file[1]=fp2;
  ar_file[2]=fp3;
  
  if(!(fp1&&fp2&&fp3)) {
     puts("problema al abrir ficheros");
     exit(1);
  }
    // obtener los títulos es el primer registro del primer file
  *titulo=*s='\0';
  while(p=fgets(s,BUFSIZ,ar_file[0])) {
       if(*s!='\n') {
         strcat(titulo,s);
       }
       else {
         titulo[strlen(titulo)-1]='\0';
         break;
       }
  } 
  // el siguiente registro son todas las entradas del trabajo van a
  // un array -luego se verá si hay que utilizar combo-
  //
  cont=i=0;
  hay_resp=0;
  for(j=0;j<3;j++) {
     *texto=*s='\0';
     while(p=fgets(s,BUFSIZ,ar_file[j])) {
       if(*s!='\n') {
         strcat(texto,s);
       }
       else {
         texto[strlen(texto)-1]='\0';
         if(j==0) {
            if(*texto=='*' && *(texto+1)==' ')
               hay_resp=1;
         }
         ar[j][i]=(char *)malloc(strlen(texto)+1);
         if(j==0 && hay_resp) {
             resp[i]=1; 
             strcpy(ar[j][i],texto+2);
         }
         if(j==0 && !hay_resp) {
           strcpy(ar[j][i],texto);
           resp[i]=0; 
         }
         if(j!=0) {
           strcpy(ar[j][i],texto);
         }
         i++;
         cont++;
         *texto=*s='\0';
         hay_resp=0;
       }
     }
     if(p==NULL) {
       ar[j][i]=NULL;
       i=0;
       *texto=*s='\0';
     }
  }
  fclose(ar_file[0]);
  fclose(ar_file[1]);
  fclose(ar_file[2]);
  fflush(stdout);
  *string='\0';
  *ent=ar[0];
}

void
on_radiobutton1_toggled                (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
gb_sirve=FALSE; 
gb_ejer=TRUE; 

}

void
on_radiobutton2_toggled                (GtkToggleButton *togglebutton,
                                        gpointer         user_data)
{
gb_sirve=TRUE; 
gb_ejer=FALSE; 

}


void
on_button1_clicked                     (GtkButton       *button,
                                        gpointer         user_data)
{


GtkWidget *entry = lookup_widget(GTK_WIDGET(button), "entry2");
GtkWidget *info_entry = lookup_widget (GTK_WIDGET(button), "text1");
GtkWidget *rad_currency = lookup_widget(GTK_WIDGET(button), "radiobutton3");
GtkWidget *rad_capital = lookup_widget(GTK_WIDGET(button), "radiobutton2");
GtkWidget *info_label = lookup_widget(GTK_WIDGET(button), "label4");
gchar *entry_text = gtk_entry_get_text(GTK_ENTRY(entry));
GtkWidget *dialog, *dg_label, *dg_OK;
gint x,n;
gchar *pi=NULL;

for (x=0; ar[0][x]!=NULL; x++)
{

  if(!strcmp(entry_text, ar[0][x]) == 1)
  {
    gtk_widget_show (info_entry);
    gtk_widget_show (info_label);
    if (gb_sirve == TRUE)
    {
       if(editable) {
          pi=gtk_editable_get_chars((GtkEditable *)info_entry,0,-1);
           //printf("\n---%d-%s\n",k,pi);
          if(strcmp(pi,ar[2][k])!=0) {
           if(ar_resp[k]){
              free(ar_resp[k]);
           }
           ar_resp[k]=(char *)malloc(strlen(pi)+1);
           strcpy(ar_resp[k],pi);
           g_free(pi);
           if(ar[2][k]) {
              g_free(ar[2][k]);
           }
           ar[2][k]=(char *)malloc(strlen(ar_resp[k])+1);
           strcpy(ar[2][k],ar_resp[k]);
          // printf("-%d-%s\n",k,ar[2][k]);
           editable=0;
           n=gtk_text_get_length(GTK_TEXT(info_entry));
           gtk_text_set_point(GTK_TEXT(info_entry),n);
          }
          else {
           editable=0;
          }
       }
      n=gtk_text_get_point (GTK_TEXT(info_entry));
      n=gtk_text_backward_delete (GTK_TEXT(info_entry),n);
      // - congelar
      gtk_text_freeze(GTK_TEXT(info_entry));
      gtk_text_insert (GTK_TEXT(info_entry), fixed_font, NULL,NULL,ar[1][x],-1);
      gtk_text_set_editable(GTK_TEXT(info_entry),FALSE);
      editable=0;
      gtk_label_set_text(GTK_LABEL(info_label), tit4);
      // - descongelar
      gtk_text_thaw(GTK_TEXT(info_entry));
    }
    if (gb_ejer == TRUE)
    {
       if(editable) {
          pi=gtk_editable_get_chars((GtkEditable *)info_entry,0,-1);
           //printf("\n---%d-%s\n",k,pi);
          if(strcmp(pi,ar[2][k])!=0) {
           if(ar_resp[k]){
              free(ar_resp[k]);
           }
           ar_resp[k]=(char *)malloc(strlen(pi)+1);
           strcpy(ar_resp[k],pi);
           g_free(pi);
           if(ar[2][k]) {
              g_free(ar[2][k]);
           }
           ar[2][k]=(char *)malloc(strlen(ar_resp[k])+1);
           strcpy(ar[2][k],ar_resp[k]);
           //printf("-%d-%s\n",k,ar[2][k]);
           editable=0;
           n=gtk_text_get_length(GTK_TEXT(info_entry));
           gtk_text_set_point(GTK_TEXT(info_entry),n);
          }
          else {
           editable=0;
          }
       }
       n=gtk_text_get_point (GTK_TEXT(info_entry));
       n=gtk_text_backward_delete (GTK_TEXT(info_entry),n);
      // gtk_text_thaw(GTK_TEXT(info_entry));
      // - congelar
      gtk_text_freeze(GTK_TEXT(info_entry));
     gtk_text_insert (GTK_TEXT(info_entry), fixed_font, NULL,NULL,ar[2][x],-1);
      // - descongelar
      gtk_text_thaw(GTK_TEXT(info_entry));
       gtk_label_set_text(GTK_LABEL(info_label), tit5);
       if(resp[x]==1) {
          gtk_text_set_editable(GTK_TEXT(info_entry),TRUE);
          editable=1;
          k=x;
       }
       else {
          gtk_text_set_editable(GTK_TEXT(info_entry),FALSE);
          editable=0;
       }
     //  de_ejer=1;
     }
     break;
   } //if
 } //del while

   if(ar[0][x]==NULL)
   {
     dialog = gtk_dialog_new();
     dg_label = gtk_label_new ("¡Entrada no encontrada!");
     dg_OK = gtk_button_new_with_label("OK");
     gtk_signal_connect_object(GTK_OBJECT (dg_OK), "clicked", gtk_widget_destroy, GTK_OBJECT(dialog));
     gtk_container_add (GTK_CONTAINER (GTK_DIALOG(dialog)->action_area), dg_OK);
     gtk_container_add (GTK_CONTAINER (GTK_DIALOG(dialog)->vbox), dg_label);
     gtk_widget_show_all (dialog);
     gtk_widget_grab_focus(dialog);
   }
}// de la función

/*---------------------------------------*/

gint
on_Informaci_n_de_funciones_destroy    (GtkObject       *object,
                                        gpointer         user_data)
{ 
  int i;
  gchar *pi;
  gchar enviar[59800];
GtkWidget *info_entry = lookup_widget (GTK_WIDGET(object), "text1");

       if(editable) {
         pi=gtk_editable_get_chars((GtkEditable *)info_entry,0,-1);
         if(strcmp(pi,ar[2][k])!=0) {
           if(ar_resp[k]){
              free(ar_resp[k]);
           }
           ar_resp[k]=(char *)malloc(strlen(pi)+1);
           strcpy(ar_resp[k],pi);
           g_free(pi);
           if(ar[2][k]) {
              g_free(ar[2][k]);
           }
           ar[2][k]=(char *)malloc(strlen(ar_resp[k])+1);
           strcpy(ar[2][k],ar_resp[k]);
         }
       }
  *enviar='\0';
  for(i=0;resp[i]!=2;i++) {
    if(resp[i]==1) {
       strcat(enviar,ar[2][i]);
       strcat(enviar,"\v");
    }
    free(ar[2][i]);
    free(ar[1][i]);
    free(ar[0][i]);
  }
  if(*enviar!='\0')
    enviar[strlen(enviar)-1]='\0';
  
  //remover files de /tmp
  unlink("/tmp/FILE1");
  unlink("/tmp/FILE2");
  unlink("/tmp/FILE3");
  if(write(1,enviar,strlen(enviar))!=strlen(enviar))
      fprintf(stderr,"Error al escribir en pipe desde p. hijo\n");
  gtk_main_quit();
  return (FALSE);
}
