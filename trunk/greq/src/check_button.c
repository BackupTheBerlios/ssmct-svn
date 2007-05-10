/* This file is part of greq
 * see greq.c for copyright information
 */
#include <stdio.h>
#include <gtk/gtk.h>
#include <glib.h>
#include "greq.h"

void create_check_button(gchar *label, gboolean toggled)
{
   GtkWidget *w;

#if GTK_MAJOR_VERSION < 2
   w = gtk_check_button_new_with_label (label);
#else
   w = gtk_check_button_new_with_mnemonic (label);
#endif
   if (!w) quit (RETURN_ERROR_NOMEM);

   gtk_toggle_button_set_active((GtkToggleButton *)w, toggled);
   add_to_vbox(w);  
   widgets = g_slist_append (widgets, w);
}

inline void output_check_button(GtkWidget *w)
{
   char *out;
   
   if (gtk_toggle_button_get_active((GtkToggleButton*)w)) {
      out="TRUE";
   } else {
      out="FALSE";
   }
   fputs(out, stdout);
}
