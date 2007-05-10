/* this file is part of greq
 * see greq.c for copyright information
 */

#include <stdio.h>
#include <string.h>
#include <gtk/gtk.h>
#include <glib.h>
#include "greq.h"
int es_editable;
void create_text_paned(gchar *text, gboolean hidden)
{
  GtkWidget *entry, *label, *label1, *box, *box2, *box1,*box0;
  GtkWidget *text1,*text2 ;
  GdkFont *fixed_font;
GtkWidget *vpaned1;
  gchar *p, lbl1_text[BUFSIZ],lbl2_text[BUFSIZ];
  GtkWidget *separator;
  GtkWidget *table, *table1;
  GtkWidget *vscrollbar, *vscrollbar1;

/***********************************************************/
  box1 = gtk_vbox_new (FALSE, 0);
  box2 = gtk_vbox_new (FALSE, 0);
  if (!box1)  quit (RETURN_ERROR_NOMEM);
  if(text) {
      p=strchr(text,'\v');
      if(p) {
         *p='\0';
         strcpy(lbl1_text,text);
         strcpy(lbl2_text,p+1);
      }
  #if GTK_MAJOR_VERSION < 2
      label = gtk_label_new(lbl1_text);
      label1 = gtk_label_new(lbl2_text);
      if (!label) quit (RETURN_ERROR_NOMEM);
      if (!label1) quit (RETURN_ERROR_NOMEM);
  #else
  
      label = gtk_label_new_with_mnemonic(text);
      label1 = gtk_label_new_with_mnemonic(text);
      if (!label) quit (RETURN_ERROR_NOMEM);
      //gtk_label_set_mnemonic_widget((GtkLabel *)label, ?);
   #endif
//
//
// NUEVO
//
   }
    vpaned1=gtk_vpaned_new();
    gtk_paned_gutter_size(GTK_PANED(vpaned1),16);
    gtk_paned_handle_size(GTK_PANED(vpaned1),16);
  
  //lbl_vpaned2_left=gtk_text_new(NULL,NULL);  
  //gtk_paned_pack1(GTK_PANED(vpaned1),lbl_vpaned2_left,TRUE,TRUE);


  //gtk_container_add(GTK_CONTAINER(frm_paned),vpaned1);


//
// FIN NUEVO
//
   //   gtk_box_pack_start((GtkBox *)box1, vpaned1, FALSE, TRUE, 0);
//      gtk_box_pack_start((GtkBox *)box1, label, FALSE, TRUE, 0);
   //}

  table = gtk_table_new (2, 2, FALSE);
  gtk_table_set_row_spacing (GTK_TABLE (table), 0, 2);
  gtk_table_set_col_spacing (GTK_TABLE (table), 0, 2);
  gtk_box_pack_start (GTK_BOX (box1), label, FALSE, TRUE, 0);
  gtk_box_pack_start (GTK_BOX (box1), table, TRUE, TRUE, 0);
  gtk_paned_pack1(GTK_PANED(vpaned1),box1,TRUE,TRUE);
  gtk_widget_show (table);
  table1 = gtk_table_new (2, 2, FALSE);
  gtk_table_set_row_spacing (GTK_TABLE (table1), 0, 2);
  gtk_table_set_col_spacing (GTK_TABLE (table1), 0, 2);
  gtk_box_pack_start (GTK_BOX (box2), label1, FALSE, TRUE, 0);
  gtk_box_pack_start (GTK_BOX (box2), table1, TRUE, TRUE, 0);
  gtk_paned_pack2(GTK_PANED(vpaned1),box2,TRUE,TRUE);
  gtk_widget_show (table1);
  
  add_to_vbox(vpaned1);

/***********************************************************/

  text1 = gtk_text_new (NULL, NULL);
  if (!text1) quit (RETURN_ERROR_NOMEM);
  fixed_font = gdk_font_load ("-misc-fixed-medium-r-*-*-*-140-*-*-*-*-*-*");
  if (hidden) {
    gtk_text_set_editable (GTK_TEXT( text1), FALSE);
    es_editable=0;
  }
  else {
    //gtk_text_set_editable (GTK_TEXT(text1), TRUE);
    //es_editable=1;
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

  /* Load a fixed font */

  /* Realizing a widget creates a window for it,
   * ready for us to insert some text */
  gtk_widget_realize (text1);

  /* Freeze the text widget, ready for multiple updates */
  gtk_text_freeze (GTK_TEXT (text1));

   gtk_text_thaw(GTK_TEXT (text1));
   //widgets = g_slist_append(widgets, text1);


  text2 = gtk_text_new (NULL, NULL);
  if (!text2) quit (RETURN_ERROR_NOMEM);
  fixed_font = gdk_font_load ("-misc-fixed-medium-r-*-*-*-140-*-*-*-*-*-*");
  if (hidden) {
    gtk_text_set_editable (GTK_TEXT( text2), FALSE);
    es_editable=0;
  }
  else {
    gtk_text_set_editable (GTK_TEXT(text2), TRUE);
    es_editable=1;
    gtk_text_insert ((GtkText*) text2,fixed_font,NULL,NULL,
                           " ",-1);
    gtk_text_backward_delete((GtkText*)text2,1);
  }
  gtk_table_attach (GTK_TABLE (table1), text2, 0, 1, 0, 1,
                    GTK_EXPAND | GTK_SHRINK | GTK_FILL,
                    GTK_EXPAND | GTK_SHRINK | GTK_FILL, 0, 0);
  gtk_widget_show (text2);

  //widgets = g_slist_append(widgets, text1);

  /* Add a vertical scrollbar to the GtkText widget */
  vscrollbar1 = gtk_vscrollbar_new (GTK_TEXT (text2)->vadj);
  gtk_table_attach (GTK_TABLE (table1), vscrollbar1, 1, 2, 0, 1,
                    GTK_FILL, GTK_EXPAND | GTK_SHRINK | GTK_FILL, 0, 0);
  gtk_widget_show (vscrollbar1);

  /* Load a fixed font */

  /* Realizing a widget creates a window for it,
   * ready for us to insert some text */
  gtk_widget_realize (text2);

  /* Freeze the text widget, ready for multiple updates */
  gtk_text_freeze (GTK_TEXT (text2));

   gtk_text_thaw(GTK_TEXT (text2));
   widgets = g_slist_append(widgets, text1);
   widgets = g_slist_append(widgets, text2);
   //printf("==%ld==%ld==\n",(long *)text1,(long *)text2);

}
