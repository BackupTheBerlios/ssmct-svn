/* this file is part of greq
 * see greq.c for copyright information
 */

#include <stdio.h>
#include <gtk/gtk.h>
#include <glib.h>
#include "greq.h"

int create_option_menu(int optind, char *optarg, char *argv[])
{
   GtkWidget *button, *menu, *label, *hbox;
   
   if ((argv[optind] == NULL) || (*argv[optind]=='-')) {
      fprintf (stderr, "%s: option requires at least two arguments -- %c\n", 
	       argv[0], argv[optind - 2][1]);
      argv[--optind] = "-?";
      return(optind);
   }

   button = gtk_option_menu_new();
   if (!button) quit (RETURN_ERROR_NOMEM);

#if GTK_MAJOR_VERSION < 2
   label = gtk_label_new(optarg);
   if (!label) quit (RETURN_ERROR_NOMEM);
#else
   label = gtk_label_new_with_mnemonic(optarg);
   if (!label) quit (RETURN_ERROR_NOMEM);
   gtk_label_set_mnemonic_widget((GtkLabel *)label, button);
#endif

   menu = gtk_menu_new();
   if (!menu) quit (RETURN_ERROR_NOMEM);
   
   do {
      GtkWidget *item;
      Debug("Adding Menu Item: %s\n", argv[optind]);
      item = gtk_menu_item_new_with_label(argv[optind++]);
      if (!item) quit (RETURN_ERROR_NOMEM);
      gtk_menu_append ((GtkMenu *)menu, item);
      gtk_widget_show_all(menu);
   } while(argv[optind] && (*argv[optind] != '-'));
   
   gtk_option_menu_set_menu((GtkOptionMenu *)button, menu);
   gtk_signal_connect ((GtkObject*)button, key_press_event,
		       GTK_SIGNAL_FUNC (keypress_cb), NULL);
   
   hbox = gtk_hbox_new(FALSE, 4);
   if (!hbox) quit (RETURN_ERROR_NOMEM);
   
   gtk_box_pack_start((GtkBox *)hbox, label, FALSE, FALSE, 0);
   gtk_box_pack_end_defaults ((GtkBox *)hbox, button);
   
   add_to_vbox(hbox);
   widgets = g_slist_append (widgets, button);
   
   return(optind);
}

void output_option_menu(GtkWidget *om)
{
   GtkWidget *c;
   char *text=NULL;
   
   Debug("om %s\n", GTK_IS_OPTION_MENU(om) ? "OK" : "ERROR");
   c = ((GtkBin*)om)->child;
   Debug("c = %#x : %s\n", (unsigned int)c, GTK_IS_LABEL(c) ? "OK" : "ERROR" );
   if (c) {
      gtk_label_get((GtkLabel *)c, &text);
      fputs(text, stdout);
   }
}
