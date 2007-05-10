/* this file is part of greq
 * see greq.c for copyright information
 */

#include <gtk/gtk.h>
#include <stdio.h> /* for putchar() */
#include "greq.h"

void ok_cb (GtkWidget *w, void *v)
{  
   gboolean edit=TRUE;
   GtkWidget *widget;
   GtkType type;
   GSList *current = widgets;
   
   while (current) {
      edit=TRUE;
      widget = (GtkWidget *)current->data;
      current = current->next;
      
      type = GTK_OBJECT_TYPE(widget);
      if (type == gtk_entry_get_type())
	output_entry(widget);
      if (type == gtk_text_get_type())
	output_text(widget,&edit);
      if (type == gtk_combo_get_type())
	output_combo(widget);
      if (type == gtk_check_button_get_type())
	output_check_button(widget);
      if (type == gtk_option_menu_get_type())
	output_option_menu(widget);
      
      /* if there are more widgets with output values, separate them */
      if (current && edit) {
	putchar ('\v');
      }
   }
   quit(RETURN_OK);
}

void create_ok_button(gchar *label)
{
   GtkWidget *w;
   
   w = gtk_button_new_with_label (label ? label : " OK ");
   if (!w) quit (RETURN_ERROR_NOMEM);
   GTK_WIDGET_SET_FLAGS(w, GTK_CAN_DEFAULT);
   gtk_container_add (GTK_CONTAINER(GTK_DIALOG(win)->action_area), w);
   gtk_signal_connect ((GtkObject*)w, "clicked", GTK_SIGNAL_FUNC(ok_cb), NULL);
   gtk_widget_grab_default(w);
   return;
};

void create_cancel_button(gchar *label)
{
   GtkWidget *w;
   w = gtk_button_new_with_label (label ? label : " Cancel ");
   if (!w) quit (RETURN_ERROR_NOMEM);
   GTK_WIDGET_SET_FLAGS(w, GTK_CAN_DEFAULT);
   gtk_container_add ((GtkContainer *)(((GtkDialog *)win)->action_area), w);
   gtk_signal_connect ((GtkObject*)w, "clicked", 
		       GTK_SIGNAL_FUNC (cancel_cb), NULL);
   return;
};

void cancel_cb (GtkWidget *w, void *v)
{
   quit (RETURN_ERROR_CANCEL);
};
