/*
 * Archivo main.c inicial generado por Glade. Edítelo como
 * lo necesite, Glade nunca sobreescribirá este archivo.
 */

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <gtk/gtk.h>
#include <locale.h>
#include "interface_t.h"
#include "support_t.h"

int
main (int argc, char *argv[])
{
  GtkWidget *Test;

#ifdef ENABLE_NLS
  bindtextdomain (PACKAGE, PACKAGE_LOCALE_DIR);
  textdomain (PACKAGE);
#endif

  //gtk_set_locale ();
  setlocale (LC_ALL,"es_ES");
  setlocale (LC_NUMERIC,"en_GB");
  setlocale (LC_TIME,"en_GB");
  gtk_init (&argc, &argv);
  Test = create_Test (argv);
  gtk_widget_show (Test);

  gtk_main ();
  return 0;
}




