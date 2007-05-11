/* this file is part of greq
 * see greq.c for copyright information
 */

#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <gtk/gtk.h>
#include <glib.h>
#include "greq.h"

void create_combo_field(gchar *text, gboolean hidden)
{
   GtkWidget *cbo_first, *label, *box;
   
   cbo_first=gtk_combo_new();
   
   //entry = gtk_entry_new();
   if (!cbo_first) quit (RETURN_ERROR_NOMEM);
  // if (hidden) gtk_entry_set_visibility ((GtkEntry*)entry, FALSE);
   gtk_signal_connect ((GtkObject*)cbo_first, key_press_event,
		       GTK_SIGNAL_FUNC (keypress_cb), NULL);
   
   if(text) {
      box = gtk_hbox_new(FALSE, 1);
      if (!box)	quit (RETURN_ERROR_NOMEM);
#if GTK_MAJOR_VERSION < 2
      label = gtk_label_new(text);
      if (!label) quit (RETURN_ERROR_NOMEM);
#else
      label = gtk_label_new_with_mnemonic(text);
      if (!label) quit (RETURN_ERROR_NOMEM);
      gtk_label_set_mnemonic_widget((GtkLabel *)label, entry);
#endif

      gtk_box_pack_start((GtkBox *)box, label, FALSE, TRUE, 0);
      gtk_box_pack_end((GtkBox *)box, cbo_first, FALSE, TRUE, 0);
      add_to_vbox(box);
   } else {
      add_to_vbox(cbo_first);
   }
   widgets = g_slist_append(widgets, cbo_first);
};

void last_combo_default (gchar *text)
{
   GtkWidget *combo;
   gint i,j;
   gchar *p,*p1, st[BUFSIZ],first[BUFSIZ];
   GList *list_of_combo=NULL;
   p1=NULL;
   *first='\0'; 
   combo = (GtkWidget *)g_slist_last(widgets)->data;
   if (GTK_IS_COMBO(combo)) {
    for(j=i=0;text[i]!='\0';j++,i++) {
       if(text[i]!='\v')
         st[j]=text[i];
       else {
         st[j]='\0';
         p=(gchar *)malloc((strlen(st))+1);
         strcpy(p,st);
	 if(p[strlen(p)-1]=='^') {
	    p[strlen(p)-1]='\0';
	    p1=p; 
	 }
         list_of_combo=g_list_append(list_of_combo,p);
         if(*first=='\0')
           strcpy(first,st);
         j=-1;
       }
     }   
   
     gtk_combo_set_popdown_strings(GTK_COMBO(combo),list_of_combo);
     if(p1){
      gtk_entry_set_text(GTK_ENTRY(GTK_COMBO(combo)->entry),p1);
     }
     else {
      gtk_entry_set_text(GTK_ENTRY(GTK_COMBO(combo)->entry),first);
     }
     gtk_entry_set_editable(GTK_ENTRY(GTK_COMBO(combo)->entry),FALSE);
   } else {
      fputs("ERROR: -r must be immediately preceeded by -b\n", stderr);
      quit(RETURN_ERROR_ARG);
   };
};

inline void output_combo (GtkWidget *cbo_first)
{
   fputs(gtk_entry_get_text(GTK_ENTRY(GTK_COMBO(cbo_first)->entry)),stdout);
}
