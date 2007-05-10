/*
 * NO EDITE ESTE ARCHIVO - esta generado por Glade.
 */

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

#include <gdk/gdkkeysyms.h>
#include <gtk/gtk.h>

#include "callbacks.h"
#include "interface.h"
#include "support.h"
void  cargar_file(char *,char *,char *,char *,char ***);
gchar tit4[1000];
gchar tit5[1000];

GtkWidget*
create_Inffunciones (char **a)
{
  GtkWidget *Inffunciones;
  GtkWidget *hpaned1;
  GtkWidget *vbox3;
  GtkWidget *label3;
  GtkWidget *entry2;
  GtkWidget *hbox3;
  GSList *hbox3_group = NULL;
  GtkWidget *radiobutton2;
  GtkWidget *radiobutton3;
  GtkWidget *button1;
  GtkWidget *arrow2;
  GtkWidget *frame2;
  GtkWidget *vbox4;
  GtkWidget *label4;
  GtkWidget *scrolledwindow2;
  GtkWidget *text1;
  GtkWidget *combo1;
  GList *combo1_items = NULL;
  GtkStyle *style_sel;
  GdkFont *fixed_font;
  GdkFont *ant_font;
  gchar titulo[BUFSIZ], *tit[6], **ent, *p;
  gint i,j;
  gboolean combo=FALSE;
  fixed_font = gdk_font_load ("-misc-fixed-medium-r-*-*-*-140-*-*-*-*-*-*");
  ent=NULL;
  cargar_file(a[1],a[2],a[3],titulo,&ent);
       // obtener todos los títulos
  for(j=i=0;i<5;i++) {
     p=strchr(&titulo[j],'\n');
     *p='\0';
     tit[i]=&titulo[j];
     p++;
     j=p-titulo;
  }
  tit[i]=&titulo[j];
  strcpy(tit4,tit[4]);
  strcpy(tit5,tit[5]);
  if(tit[1][0]=='*' && tit[1][1]==' ') {
       // entonces combo
       combo=TRUE;
       tit[1]=tit[1]+2;
  }
       // en array tit están los punteros a los 6 títulos
       
  Inffunciones = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  gtk_object_set_data (GTK_OBJECT (Inffunciones), "Inffunciones", Inffunciones);
  gtk_container_set_border_width (GTK_CONTAINER (Inffunciones), 5);
//
    gtk_widget_set_usize (GTK_WIDGET(Inffunciones), 700,200);
    gtk_window_set_policy (GTK_WINDOW(Inffunciones), TRUE,TRUE,FALSE);

  //
  //gtk_window_set_title (GTK_WINDOW (Inffunciones), _("Llamadas de Sistema y Funciones"));
  gtk_window_set_title (GTK_WINDOW (Inffunciones), _(tit[0]));

  hpaned1 = gtk_hpaned_new ();
  gtk_widget_ref (hpaned1);
  gtk_object_set_data_full (GTK_OBJECT (Inffunciones), "hpaned1", hpaned1,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (hpaned1);
  gtk_container_add (GTK_CONTAINER (Inffunciones), hpaned1);
  gtk_container_set_border_width (GTK_CONTAINER (hpaned1), 5);
  gtk_paned_set_handle_size (GTK_PANED (hpaned1), 13);
  gtk_paned_set_gutter_size (GTK_PANED (hpaned1), 16);
  gtk_paned_set_position (GTK_PANED (hpaned1), 181);

  vbox3 = gtk_vbox_new (FALSE, 0);
  gtk_widget_ref (vbox3);
  gtk_object_set_data_full (GTK_OBJECT (Inffunciones), "vbox3", vbox3,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (vbox3);
  gtk_paned_pack1 (GTK_PANED (hpaned1), vbox3, TRUE, TRUE);

  //label3 = gtk_label_new (_("Una función"));
  label3 = gtk_label_new (_(tit[1]));
  gtk_widget_ref (label3);
  gtk_object_set_data_full (GTK_OBJECT (Inffunciones), "label3", label3,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (label3);
  gtk_box_pack_start (GTK_BOX (vbox3), label3, TRUE, FALSE, 0);

/*
*/
 if(combo){
  combo1 = gtk_combo_new ();
  gtk_widget_ref (combo1);
  gtk_object_set_data_full (GTK_OBJECT (Inffunciones), "combo1", combo1,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (combo1);
  gtk_box_pack_start (GTK_BOX (vbox3), combo1, TRUE, FALSE, 0);
  for(i=0;ent[i]!=NULL;i++) 
    combo1_items = g_list_append (combo1_items, (gpointer) _(ent[i]));
  gtk_combo_set_popdown_strings (GTK_COMBO (combo1), combo1_items);
  g_list_free (combo1_items);

  entry2 = GTK_COMBO (combo1)->entry;
  gtk_widget_ref (entry2);
  gtk_object_set_data_full (GTK_OBJECT (Inffunciones), "entry2", entry2,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (entry2);
  gtk_entry_set_text (GTK_ENTRY (entry2), _(ent[0]));
 }
 else {
     entry2 = gtk_entry_new ();
   // style_sel=gtk_widget_get_style(entry2);
    //style_sel->font=fixed_font;
    gtk_widget_ref (entry2);
    gtk_object_set_data_full (GTK_OBJECT (Inffunciones), "entry2", entry2,
                            (GtkDestroyNotify) gtk_widget_unref);
    gtk_widget_show (entry2);
    //gtk_widget_set_style(GTK_WIDGET(entry2),style_sel);

    gtk_box_pack_start (GTK_BOX (vbox3), entry2, TRUE, FALSE, 0);
  } 
  hbox3 = gtk_hbox_new (FALSE, 0);
  gtk_widget_ref (hbox3);
  gtk_object_set_data_full (GTK_OBJECT (Inffunciones), "hbox3", hbox3,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (hbox3);
  gtk_box_pack_start (GTK_BOX (vbox3), hbox3, TRUE, TRUE, 0);

  radiobutton2 = gtk_radio_button_new_with_label (hbox3_group, _(tit[3]));
  hbox3_group = gtk_radio_button_group (GTK_RADIO_BUTTON (radiobutton2));
  gtk_widget_ref (radiobutton2);
  gtk_object_set_data_full (GTK_OBJECT (Inffunciones), "radiobutton2", radiobutton2,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (radiobutton2);
  gtk_box_pack_end (GTK_BOX (hbox3), radiobutton2, FALSE, FALSE, 0);

  //radiobutton3 = gtk_radio_button_new_with_label (hbox3_group, _("Sirve para"));
  radiobutton3 = gtk_radio_button_new_with_label (hbox3_group, _(tit[2]));
  hbox3_group = gtk_radio_button_group (GTK_RADIO_BUTTON (radiobutton3));
  gtk_widget_ref (radiobutton3);
  gtk_object_set_data_full (GTK_OBJECT (Inffunciones), "radiobutton3", radiobutton3,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (radiobutton3);
  gtk_box_pack_start (GTK_BOX (hbox3), radiobutton3, FALSE, FALSE, 0);

  button1 = gtk_button_new ();
  gtk_widget_ref (button1);
  gtk_object_set_data_full (GTK_OBJECT (Inffunciones), "button1", button1,
                            (GtkDestroyNotify) gtk_widget_unref);
// gtk_box_pack_start (GTK_BOX (vbox3), button1, FALSE, FALSE, 0);

  gtk_box_pack_start (GTK_BOX (vbox3), button1, TRUE, FALSE, 0);
  gtk_widget_set_usize(button1,-2,28);
  gtk_widget_show (button1);

  arrow2 = gtk_arrow_new (GTK_ARROW_RIGHT, GTK_SHADOW_OUT);
  gtk_widget_ref (arrow2);
  gtk_object_set_data_full (GTK_OBJECT (Inffunciones), "arrow2", arrow2,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (arrow2);
  gtk_container_add (GTK_CONTAINER (button1), arrow2);

  frame2 = gtk_frame_new (NULL);
  gtk_widget_ref (frame2);
  gtk_object_set_data_full (GTK_OBJECT (Inffunciones), "frame2", frame2,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (frame2);
  gtk_paned_pack2 (GTK_PANED (hpaned1), frame2, TRUE, TRUE);
  gtk_frame_set_label_align (GTK_FRAME (frame2), 0.08, 0.5);
  gtk_frame_set_shadow_type (GTK_FRAME (frame2), GTK_SHADOW_ETCHED_OUT);

  vbox4 = gtk_vbox_new (FALSE, 0);
  gtk_widget_ref (vbox4);
  gtk_object_set_data_full (GTK_OBJECT (Inffunciones), "vbox4", vbox4,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (vbox4);
  gtk_container_add (GTK_CONTAINER (frame2), vbox4);

  label4 = gtk_label_new ("");
  gtk_widget_ref (label4);
  gtk_object_set_data_full (GTK_OBJECT (Inffunciones), "label4", label4,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (label4);
  gtk_box_pack_start (GTK_BOX (vbox4), label4, FALSE, TRUE, 5);

  scrolledwindow2 = gtk_scrolled_window_new (NULL, NULL);
  gtk_widget_ref (scrolledwindow2);
  gtk_object_set_data_full (GTK_OBJECT (Inffunciones), "scrolledwindow2", scrolledwindow2,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (scrolledwindow2);
  gtk_box_pack_start (GTK_BOX (vbox4), scrolledwindow2, TRUE, TRUE, 0);
  gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolledwindow2), GTK_POLICY_NEVER, GTK_POLICY_ALWAYS);

  text1 = gtk_text_new (NULL, NULL);
  gtk_widget_ref (text1);
  gtk_object_set_data_full (GTK_OBJECT (Inffunciones), "text1", text1,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (text1);
  gtk_container_add (GTK_CONTAINER (scrolledwindow2), text1);
  gtk_widget_set_usize (text1, -2, 135);
//  entry2->style->font=fixed_font;
  //gtk_widget_realize(entry2);

  gtk_signal_connect (GTK_OBJECT (Inffunciones), "delete_event",
                      GTK_SIGNAL_FUNC (on_Informaci_n_de_funciones_destroy),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (radiobutton2), "toggled",
                      GTK_SIGNAL_FUNC (on_radiobutton1_toggled),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (radiobutton3), "toggled",
                      GTK_SIGNAL_FUNC (on_radiobutton2_toggled),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (button1), "clicked",
                      GTK_SIGNAL_FUNC (on_button1_clicked),
                      NULL);

  return Inffunciones;
}

