/* this file is part of greq
 * see greq.c for copyright information
 */

#include <stdio.h>
#include <string.h>
#include <gtk/gtk.h>
#include <glib.h>
#include "greq.h"
int es_editable;
void create_text_field(gchar *text, gboolean hidden)
{
  GtkWidget *entry, *label, *box, *box2, *box1,*box0;
  GtkWidget *text1;
  GdkFont *fixed_font;

  GtkWidget *separator;
  GtkWidget *table;
  GtkWidget *vscrollbar;
  /* GdkColormap *cmap;
     GdkColor color;
     GSList *group;
  */

/***********************************************************/
  box1 = gtk_vbox_new (FALSE, 0);
  if (!box1)  quit (RETURN_ERROR_NOMEM);
  if(text) {
#if GTK_MAJOR_VERSION < 2
      label = gtk_label_new(text);
      if (!label) quit (RETURN_ERROR_NOMEM);
#else
      label = gtk_label_new_with_mnemonic(text);
      if (!label) quit (RETURN_ERROR_NOMEM);
     // gtk_label_set_mnemonic_widget((GtkLabel *)label, entry);
#endif
      gtk_box_pack_start((GtkBox *)box1, label, FALSE, TRUE, 0);
   }
   add_to_vbox(box1);
 //  widgets = g_slist_append(widgets, text1);
  //gtk_widget_show (box1);

/*
  box2 = gtk_vbox_new (FALSE, 10);
  gtk_container_set_border_width (GTK_CONTAINER (box2), 10);
  gtk_box_pack_start (GTK_BOX (box1), box2, TRUE, TRUE, 0);
  gtk_widget_show (box2);
*/

  table = gtk_table_new (2, 2, FALSE);
  gtk_table_set_row_spacing (GTK_TABLE (table), 0, 2);
  gtk_table_set_col_spacing (GTK_TABLE (table), 0, 2);
  gtk_box_pack_start (GTK_BOX (box1), table, TRUE, TRUE, 0);
  gtk_widget_show (table);

/***********************************************************/

  text1 = gtk_text_new (NULL, NULL);
  if (!text1) quit (RETURN_ERROR_NOMEM);
  fixed_font = gdk_font_load ("-misc-fixed-medium-r-*-*-*-140-*-*-*-*-*-*");
  if (hidden) {
    gtk_text_set_editable (GTK_TEXT( text1), FALSE);
    es_editable=0;
  }
  else {
    gtk_text_set_editable (GTK_TEXT(text1), TRUE);
    es_editable=1;
    gtk_text_insert ((GtkText*) text1,fixed_font,NULL,NULL,
                           " ",-1);
    gtk_text_backward_delete((GtkText*)text1,1);
  }
  gtk_table_attach (GTK_TABLE (table), text1, 0, 1, 0, 1,
                    GTK_EXPAND | GTK_SHRINK | GTK_FILL,
                    GTK_EXPAND | GTK_SHRINK | GTK_FILL, 0, 0);
  gtk_widget_show (text1);

  //widgets = g_slist_append(widgets, text1);

  /* Add a vertical scrollbar to the GtkText widget */
  vscrollbar = gtk_vscrollbar_new (GTK_TEXT (text1)->vadj);
  gtk_table_attach (GTK_TABLE (table), vscrollbar, 1, 2, 0, 1,
                    GTK_FILL, GTK_EXPAND | GTK_SHRINK | GTK_FILL, 0, 0);
  gtk_widget_show (vscrollbar);

  /* Get the system color map and allocate the color red */
  /*cmap = gdk_colormap_get_system();
  color.red = 0xffff;
  color.red = 0xffff;
  color.green = 0;
  color.blue = 0;
  color.blue = 0xffff;
  if (!gdk_color_alloc(cmap, &color)) {
    g_error("couldn't allocate color");
  }
  */
  /* Load a fixed font */

  /* Realizing a widget creates a window for it,
   * ready for us to insert some text */
  gtk_widget_realize (text1);

  /* Freeze the text widget, ready for multiple updates */
  gtk_text_freeze (GTK_TEXT (text1));

  /* Insert some colored text */
  /*gtk_text_insert (GTK_TEXT (text1), NULL, &text->style->black, NULL,
                   "Supports ", -1);
  gtk_text_insert (GTK_TEXT (text1), NULL, &color, NULL,
                   "colored ", -1);
  gtk_text_insert (GTK_TEXT (text1), NULL, &text->style->black, NULL,
                   "text and different ", -1);
  gtk_text_insert (GTK_TEXT (text1), fixed_font, &text->style->black, NULL,
                   "fonts\n\n", -1);
  */
   gtk_text_thaw(GTK_TEXT (text1));
   //gtk_signal_connect ((GtkObject*)text1, key_press_event,
		       //GTK_SIGNAL_FUNC (keypress_cb), NULL);
   widgets = g_slist_append(widgets, text1);
};
struct text_color {
   char cual_color;
   char *dir;
};
void last_text_default (gchar *text,gboolean palabra, gint cont)
{
  GSList *node;
  struct text_color * buscar_coloreada(gchar *text);
  struct text_color *coloreado;
  GtkWidget *text1,*text3,*obj;
  GdkFont *fixed_font;
  GtkType type;
//
//
  GdkColor color;
  GdkColormap *cmap;
  gint i,c;
  /* Get the system color map and allocate the color red */
  cmap = gdk_colormap_get_system();
  //color.red = 0xffff;
  color.red = 0;
  color.green = 0;
  color.blue = 0;
  //color.blue = 0xffff;

//
//


  fixed_font = gdk_font_load ("-misc-fixed-medium-r-*-*-*-140-*-*-*-*-*-*");
   
  //text1 = (GtkWidget *)g_slist_last(widgets)->data;
       node=widgets;
       for(c=0;node;node=node->next) {
         obj=(GtkWidget *)node->data;
         type=GTK_OBJECT_TYPE(obj);
          // printf("...---%ld---...\n",(long *)node->data);
       //  text3=(GtkWidget *)node->data;
         if(type==gtk_text_get_type())
	   c++;
	 if(cont==c) {
           text1=obj;
           break;
	 }
       }
   if (GTK_IS_TEXT(text1)) {
      if(palabra) {
       gtk_text_freeze(GTK_TEXT (text1));  // congelamos ya que puede
                                       // haber varias insersiones
       coloreado=buscar_coloreada(text);
       if(coloreado==NULL) {
           gtk_text_insert ((GtkText*) text1,fixed_font,NULL,NULL,
                           text,-1);
           return;
       }
       i=0;
       //gtk_text_freeze(GTK_TEXT (text1));  // congelamos ya que puede
                                       // haber varias insersiones
       while(coloreado[i].dir) {
         if(coloreado[i].cual_color=='\0') {
           gtk_text_insert ((GtkText*) text1,fixed_font,NULL,NULL,
                           coloreado[i].dir,-1);
         }
         else {
            if(coloreado[i].cual_color=='r') {
                color.red = 0xffff;
                color.green = 0;
                color.blue = 0;
            }
            if(coloreado[i].cual_color=='g') {
                color.green = 0xffff;
                color.blue = 0;
                color.red = 0;
            }
            if(coloreado[i].cual_color=='b') {
                color.blue = 0xffff;
                color.red = 0;
                color.green = 0;
            }

            if (!gdk_color_alloc(cmap, &color)) {
                g_error("couldn't allocate color");
            }
           gtk_text_insert ((GtkText*) text1,fixed_font,&color,NULL,
                           coloreado[i].dir,-1);
         }
         i++;
        }
      gtk_text_thaw(GTK_TEXT (text1));  // Redibuja -final de insersiones-
      }
      else {
        gtk_text_insert ((GtkText*) text1,fixed_font,NULL,NULL, text,-1);
        gtk_text_thaw(GTK_TEXT (text1));  // Redibuja -final de insersiones-
      }
   } else {
      fputs("ERROR: -z must be immediately preceeded by -x\n", stderr);
      quit(RETURN_ERROR_ARG);
   };
};

struct text_color *buscar_coloreada(gchar *text) {
  struct text_color *ar;
  char *p,*p1,*inicio, *final,st[50];
  int cont,i;
  cont = 0;
  p=p1=text;
  while(p=strstr(p,"|[")) {
    p=p+2;
    cont++;
  }
  if(cont==0)
    return NULL;
  p=inicio=text;
  ar=(struct text_color *)malloc(cont*5*sizeof(struct text_color));
  for(i=0;i<(cont*5);i++) {
    ar[i].dir=NULL;
    ar[i].cual_color='\0';
  }
    i=0;  
    while(p=strstr(p,"|[")) {
      if(p[2]=='r'||p[2]=='g'||p[2]=='b') {
         if(p[3]==']') {
           //es el inicio de un color veamos si hay final
           if(p1=strstr(p+4,"]|")) { //AQUI HABRÍA QUE DEPURAR//
               //hay final
               *p='\0';
               ar[i].dir=(char *)malloc(strlen(inicio)+1);
               strcpy(ar[i].dir,inicio);
               inicio=p+4;
               final=p1-2;
               *final='\0';
               //el texto coloreado
               i++;
               ar[i].dir=(char *)malloc(strlen(inicio)+1);
               strcpy(ar[i].dir,inicio);
               ar[i].cual_color=*(inicio-2);
               i++;
               p=inicio=final+4;
           }
           else{ p=p+2; }
         }
         else{ p=p+2; }
      }
      else{ p=p+2; }
    }
    // inicio
               ar[i].dir=(char *)malloc(strlen(inicio)+1);
               strcpy(ar[i].dir,inicio);
  return ar;
}

inline void output_text (GtkWidget *text1,gboolean *edit)
{
   guint n;
  // n=gtk_text_get_length(text1);
   if(GTK_EDITABLE(text1)->editable)
     fputs(gtk_editable_get_chars(GTK_EDITABLE (text1),0,-1), stdout);
   else
     *edit=FALSE;
}
