/*
 * greq.c
 *
 * Copyright (C) 2006, Paulino Huerta Sanchez
 * Extended and modified version of the original greq by Felix Braun, Berlin
 * 
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#include <locale.h>
#include <stdio.h> // for printf
#include <unistd.h>// for getopt
#include <gtk/gtk.h>
#include <gdk/gdkkeysyms.h> //for GDK_Return
#include <glib.h> // for g_slist_*

#define MAIN_PROGRAM
#include "greq.h"
#include "version.h"

inline void create_text_prompt(gchar *text)
{
   GtkWidget *label;
   
   label = gtk_label_new(text);
   if(!label)
     quit(RETURN_ERROR_NOMEM);
   gtk_label_set_line_wrap ((GtkLabel *)label, TRUE);
   add_to_vbox(label);
}

int main (int argc, char *argv[])
{
   gchar *ok_label = NULL;
   gchar *cancel_label = NULL;
   gint c,cont;
/*-------------*/
#ifdef ENABLE_NLS
  bindtextdomain (GETTEXT_PACKAGE, PACKAGE_LOCALE_DIR);
  bind_textdomain_codeset (GETTEXT_PACKAGE, "UTF-8");
  textdomain (GETTEXT_PACKAGE);
#endif
  setlocale(LC_ALL,"es_ES");
/*-------------*/
   gtk_init (&argc, &argv);
   
   win = gtk_dialog_new ();
   if (!win) quit (RETURN_ERROR_NOMEM);
   gtk_signal_connect ((GtkObject *)win, "delete_event", // quit if window
		       GTK_SIGNAL_FUNC (cancel_cb), NULL);// gets closed
   msgbox = gtk_vbox_new (FALSE, 0);
   if (!msgbox) quit (RETURN_ERROR_NOMEM);
   gtk_container_set_border_width((GtkContainer *)msgbox, 5);
   //gtk_box_pack_start_defaults ((GtkBox*)(((GtkDialog*)win)->vbox), msgbox);
 
    // para que no haya límite al minimizar 
    gtk_window_set_policy (GTK_WINDOW(win), TRUE,TRUE,FALSE);
    //
   gtk_box_pack_start ((GtkBox*)(((GtkDialog*)win)->vbox), msgbox,TRUE,TRUE,0);
   cont=0; 
   while ((c = getopt (argc, argv, "h?m:t:p:E::e::d:B::b::r:X::x::z:Z:c:C:a::o:N::n::")) != -1)
     {
	extern char *optarg;
	extern int optind;
	switch (c)
	  {
	   case 't':  
	     Debug("set window title: %s\n", optarg);
	     gtk_window_set_title ((GtkWindow *)win, (gchar *)optarg);
	     break;
	   case 'c':
	   case 'C':
	     create_check_button(optarg, c=='C');
	     Debug("checkbox: %s\n",optarg);
	     break;
	   case 'p':
	     create_text_prompt(optarg);
	     break;
	   case 'e':
	   case 'E':  
	     create_entry_field(optarg, c=='E');
	     Debug("text entry field: %s\n", optarg);
	     break;
	   case 'd':
	     last_entry_default(optarg);
	     Debug("defaults to: %s\n", optarg);
	     break;
	   case 'b':
	   case 'B':
	     create_combo_field(optarg,c=='B');
	     Debug("defaults to: %s\n", optarg);
	     break;
	   case 'r':
	     last_combo_default(optarg);
	     Debug("defaults to: %s\n", optarg);
	     break;
	   case 'x':
	   case 'X':
	     create_text_field(optarg, c=='X');
	     Debug("text field: %s\n", optarg);
	     break;
	   case 'n':
	   case 'N':
	     create_text_paned(optarg, c=='N');
	     Debug("text paned: %s\n", optarg);
	     break;
	   case 'z':
	   case 'Z':
	     last_text_default(optarg,c=='Z',++cont);
	     Debug("defaults to: %s\n", optarg);
	     break;
	   case 'm':
	     optind = create_option_menu(optind, optarg, argv);
	     break;
	   case 'o':
	     ok_label = (gchar *)optarg;
	     break;
	   case 'a':
	     if(optarg) {
		cancel_label = (gchar *)optarg;
	     } else {
		/* set to an impossible value, so that we know later on, that
		 * we don't want to create the cancel button.
		 */
		cancel_label = (gchar *)win;
	     } 
	     break;
	   case '?':
	   case 'h':
	     fputs(GREQ_VERSION, stderr);
	     fprintf(stderr,"Usage: %s [-t<window-title>] [-p<text-prompt>] [[-(e | E)[<entry-label>] [-d<default-value>]] [-(c | C)<checkbox-label>] [-m<menu-label> <menu-item> ...] [-o<ok-label>] [-a[<abort-label>]]\n", argv[0]);
	     quit(RETURN_ERROR_ARG);
	  }
     }
   
   if ( optind < argc )
     {
	fprintf(stderr, "%s: Unknown argument", argv[0]);
	if (optind < argc - 1) fputc ('s', stderr);
	fputc(':',stderr);
	do {
	   fputc(' ', stderr);
	   fputs(argv[optind++], stderr);
	} while (optind < argc);
	fputc('\n', stderr);
	quit(RETURN_ERROR_ARG);
     }
   
   create_ok_button(ok_label);
   if(cancel_label != (gchar *)win)
     create_cancel_button(cancel_label);

#if GTK_MAJOR_VERSION < 2
   if(widgets) {
      Debug ("Setting focus\n");
      gtk_widget_grab_focus((GtkWidget *) widgets->data);
   };
#endif
   
   gtk_widget_show_all (win);
   gtk_main();
   
   // we should never get here, but just to make gcc happy
   return (RETURN_OK);
}

void quit (int code)
{
   if (widgets)
     g_slist_free(widgets);
   gtk_exit(code);
};

gint keypress_cb (GtkWidget *w, GdkEvent *e, void *v)
{
   switch (e->key.keyval)
     {
      case GDK_Return:
	ok_cb (NULL, NULL);
	return (TRUE);
      case GDK_Escape:
	cancel_cb(NULL, NULL);
	return (TRUE);
      default:
	return (FALSE);
     }
};

inline void add_to_vbox(GtkWidget *child)
{
   //gtk_box_pack_start((GtkBox *)msgbox, child,FALSE, FALSE, 0);
gtk_box_pack_start((GtkBox *)msgbox, child,TRUE,TRUE, 0);
}
