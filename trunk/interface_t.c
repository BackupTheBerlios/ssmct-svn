/*
 * NO EDITE ESTE ARCHIVO - esta generado por Glade.
 */

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>

#include <gdk/gdkkeysyms.h>
#include <gtk/gtk.h>

#include "callbacks_t.h"
#include "interface_t.h"
#include "support_t.h"
void  cargar_file(char *,char **);

GtkWidget*
create_Test (char **a)
{
  GtkWidget *Test;
  GtkWidget *vbox1;
  GtkWidget *frame1;
  GtkWidget *vseparator1;
  GtkWidget *vseparator2;
  GtkWidget *vbox2;
  GtkWidget *label5, *label6;
  GtkWidget *info_label;
  GtkWidget *scrolledwindow1;
  GtkWidget *text1;
  GtkWidget *separator;
  GtkWidget *hbox1;
  GtkWidget *hbox2;
  GSList *hbox1_group = NULL;
  GtkWidget *esta;
  GtkWidget *otra;
  GtkWidget *Corrija;
  GtkWidget *button3;
  GtkWidget *arrow1;
  GtkWidget *box2;
  gchar *titulo[15], *p1, *p2;
  gint i=0;
  p1=p2=NULL;
  cargar_file(a[1],titulo);
  p1=titulo[0];
  for(i=1;titulo[i]!=NULL;i++)
    ;
  p2=titulo[i-1];
   
  Test = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  gtk_object_set_data (GTK_OBJECT (Test), "Test", Test);

    gtk_container_set_border_width (GTK_CONTAINER (Test), 10);
    if( (p1!=NULL) && (p1[0] != '\0') ) // entonces hay título
        gtk_window_set_title (GTK_WINDOW (Test), _(titulo[0]));
    gtk_widget_set_usize (GTK_WIDGET(Test), 600,500);
    gtk_window_set_policy (GTK_WINDOW(Test), TRUE,TRUE,FALSE);


  vbox1 = gtk_vbox_new (FALSE, 0);
  gtk_widget_ref (vbox1);
  gtk_object_set_data_full (GTK_OBJECT (Test), "vbox1", vbox1,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (vbox1);
  gtk_container_add (GTK_CONTAINER (Test), vbox1);

  frame1 = gtk_frame_new (NULL);
  gtk_widget_ref (frame1);
  gtk_object_set_data_full (GTK_OBJECT (Test), "frame1", frame1,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (frame1);
  gtk_box_pack_start (GTK_BOX (vbox1), frame1, TRUE, TRUE, 0);
  gtk_frame_set_label_align (GTK_FRAME (frame1), 0.08, 0.5);
  gtk_frame_set_shadow_type (GTK_FRAME (frame1), GTK_SHADOW_ETCHED_OUT);

  vbox2 = gtk_vbox_new (FALSE, 0);
  gtk_widget_ref (vbox2);
  gtk_object_set_data_full (GTK_OBJECT (Test), "vbox2", vbox2,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (vbox2);
  gtk_container_add (GTK_CONTAINER (frame1), vbox2);
//
  hbox2 = gtk_hbox_new (FALSE, 0);
  gtk_widget_ref (hbox2);
  gtk_object_set_data_full (GTK_OBJECT (Test), "hbox2", hbox2,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (hbox2);
  gtk_box_pack_start (GTK_BOX (vbox2), hbox2, FALSE, TRUE, 0);
  gtk_widget_set_usize (hbox2, -2, 24);
  gtk_container_set_border_width(GTK_CONTAINER(hbox2),2);

  info_label = gtk_label_new (_("Tiempo por Respuesta"));
  gtk_widget_ref (info_label);
  gtk_object_set_data_full (GTK_OBJECT (Test), "info_label", info_label,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (info_label);
  gtk_box_pack_start (GTK_BOX (hbox2), info_label, FALSE, FALSE, 0);
  gtk_label_set_justify (GTK_LABEL (info_label), GTK_JUSTIFY_LEFT);

  vseparator1 = gtk_vseparator_new ();
  gtk_widget_ref (vseparator1);
  gtk_object_set_data_full (GTK_OBJECT (Test), "vseparator1", vseparator1,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (vseparator1);
  gtk_box_pack_start (GTK_BOX (hbox2), vseparator1, TRUE, FALSE, 0);

  label5 = gtk_label_new (_("Puntuación última Respuesta"));
  gtk_widget_ref (label5);
  gtk_object_set_data_full (GTK_OBJECT (Test), "label5", label5,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (label5);
  gtk_box_pack_start (GTK_BOX (hbox2), label5, FALSE, FALSE, 0);
  gtk_label_set_justify (GTK_LABEL (label5), GTK_JUSTIFY_LEFT);

  vseparator2 = gtk_vseparator_new ();
  gtk_widget_ref (vseparator2);
  gtk_object_set_data_full (GTK_OBJECT (Test), "vseparator2", vseparator2,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (vseparator2);
  gtk_box_pack_start (GTK_BOX (hbox2), vseparator2, TRUE, FALSE, 0);

  label6 = gtk_label_new (_("Puntuación Parcial:"));
  gtk_widget_ref (label6);
  gtk_object_set_data_full (GTK_OBJECT (Test), "label6", label6,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (label6);
  gtk_box_pack_start (GTK_BOX (hbox2), label6, FALSE, FALSE, 0);
  gtk_label_set_justify (GTK_LABEL (label6), GTK_JUSTIFY_LEFT);

/*
  info_label = gtk_label_new ("");
  gtk_widget_ref (info_label);
  gtk_object_set_data_full (GTK_OBJECT (Test), "info_label", info_label,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (info_label);
  gtk_box_pack_start (GTK_BOX (vbox2), info_label, FALSE, FALSE, 0);
  gtk_widget_set_usize (info_label, -2, 25);
*/
  scrolledwindow1 = gtk_scrolled_window_new (NULL, NULL);
  gtk_widget_ref (scrolledwindow1);
  gtk_object_set_data_full (GTK_OBJECT (Test), "scrolledwindow1", scrolledwindow1,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (scrolledwindow1);
  gtk_box_pack_start (GTK_BOX (vbox2), scrolledwindow1, TRUE, TRUE, 0);

  text1 = gtk_text_new (NULL, NULL);
  gtk_widget_ref (text1);
  gtk_object_set_data_full (GTK_OBJECT (Test), "text1", text1,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_text_insert(GTK_TEXT(text1),NULL,NULL,NULL,p2,strlen(p2));
  gtk_text_insert(GTK_TEXT(text1),NULL,NULL,NULL,"\n\nPara comenzar y continuar pulse botón |>",42);
  gtk_widget_show (text1);
  gtk_container_add (GTK_CONTAINER (scrolledwindow1), text1);
  gtk_widget_set_usize (text1, -2, 139);
  gtk_text_set_editable (GTK_TEXT (text1), FALSE);

  hbox1 = gtk_hbox_new (FALSE, 0);
  gtk_widget_ref (hbox1);
  gtk_object_set_data_full (GTK_OBJECT (Test), "hbox1", hbox1,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (hbox1);
//
  gtk_box_pack_start (GTK_BOX (vbox1), hbox1, FALSE, FALSE, 0);
  //gtk_box_pack_start (GTK_BOX (vbox1), hbox1, TRUE, TRUE, 0);
  gtk_container_set_border_width(GTK_CONTAINER(hbox1),6);
  esta = gtk_radio_button_new_with_label (hbox1_group, _("ESTA"));
  hbox1_group = gtk_radio_button_group (GTK_RADIO_BUTTON (esta));
  gtk_widget_ref (esta);
  gtk_object_set_data_full (GTK_OBJECT (Test), "esta", esta,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (esta);
  gtk_box_pack_start (GTK_BOX (hbox1), esta, TRUE, FALSE, 0);
  gtk_container_set_border_width (GTK_CONTAINER (esta), 5);
  gtk_toggle_button_set_active (GTK_TOGGLE_BUTTON (esta), TRUE);

  otra = gtk_radio_button_new_with_label (hbox1_group, _("LA OTRA"));
  hbox1_group = gtk_radio_button_group (GTK_RADIO_BUTTON (otra));
  gtk_widget_ref (otra);
  gtk_object_set_data_full (GTK_OBJECT (Test), "otra", otra,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (otra);
  gtk_box_pack_start (GTK_BOX (hbox1), otra, TRUE, FALSE, 0);
  gtk_container_set_border_width (GTK_CONTAINER (otra), 3);

  Corrija = gtk_check_button_new_with_label (_("CORRIJA"));
  gtk_widget_ref (Corrija);
  gtk_object_set_data_full (GTK_OBJECT (Test), "Corrija", Corrija,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (Corrija);
  gtk_box_pack_start (GTK_BOX (hbox1), Corrija, TRUE, FALSE, 0);
  gtk_container_set_border_width (GTK_CONTAINER (Corrija), 3);
//
  separator = gtk_hseparator_new ();
  gtk_box_pack_start (GTK_BOX (vbox1), separator, FALSE, TRUE, 0);
  gtk_widget_show (separator);
//

  box2 = gtk_vbox_new (FALSE, 10);
  gtk_container_set_border_width (GTK_CONTAINER (box2), 6);
  gtk_box_pack_start (GTK_BOX (vbox1), box2, FALSE, TRUE, 0);
  gtk_widget_show (box2);
  

//
  button3 = gtk_button_new ();
  gtk_widget_ref (button3);
  gtk_object_set_data_full (GTK_OBJECT (Test), "button3", button3,
                            (GtkDestroyNotify) gtk_widget_unref);
  //gtk_widget_show (button3);
  //gtk_box_pack_start (GTK_BOX (box2), button3, FALSE, FALSE, 0);
  gtk_box_pack_start (GTK_BOX (box2), button3, TRUE, TRUE, 0);
  gtk_widget_set_usize(button3,-2,24);
  gtk_widget_show (button3);

  arrow1 = gtk_arrow_new (GTK_ARROW_RIGHT, GTK_SHADOW_OUT);
  gtk_widget_ref (arrow1);
  gtk_object_set_data_full (GTK_OBJECT (Test), "arrow1", arrow1,
                            (GtkDestroyNotify) gtk_widget_unref);
  gtk_widget_show (arrow1);
  gtk_container_add (GTK_CONTAINER (button3), arrow1);

  gtk_signal_connect (GTK_OBJECT (Test), "delete_event",
                      GTK_SIGNAL_FUNC (on_Test_destroy),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (esta), "clicked",
                      GTK_SIGNAL_FUNC (on_esta_clicked),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (otra), "clicked",
                      GTK_SIGNAL_FUNC (on_otra_clicked),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (Corrija), "clicked",
                      GTK_SIGNAL_FUNC (on_Corrija_clicked),
                      NULL);
  gtk_signal_connect (GTK_OBJECT (button3), "clicked",
                      GTK_SIGNAL_FUNC (on_button3_clicked),
                      NULL);

  return Test;
}

