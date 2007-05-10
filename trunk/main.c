/*
 * Archivo main.c inicial generado por Glade. Edítelo como
 * lo necesite, Glade nunca sobreescribirá este archivo.
 */

#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <gtk/gtk.h>
#include <locale.h>
#include "interface.h"
#include "support.h"
//void  cargar_file(char *,char *, char*);

int
main (int argc, char *argv[])
{
  GtkWidget *Informaci_n_de_funciones;
#ifdef ENABLE_NLS
  bindtextdomain (GETTEXT_PACKAGE, PACKAGE_LOCALE_DIR);
  bind_textdomain_codeset (GETTEXT_PACKAGE, "UTF-8");
  textdomain (GETTEXT_PACKAGE);
#endif

  setlocale (LC_ALL,"es_ES");
  //gtk_set_locale ();

  gtk_init (&argc, &argv);
/*
  add_pixmap_directory (PACKAGE_DATA_DIR "/pixmaps");
  add_pixmap_directory (PACKAGE_SOURCE_DIR "/pixmaps");
*/

  /*
   * The following code was added by Glade to create one of each component
   * (except popup menus), just so that you see something after building
   * the project. Delete any components that you don't want shown initially.
   */
  Informaci_n_de_funciones = create_Inffunciones (argv);
  gtk_widget_show (Informaci_n_de_funciones);

  gtk_main ();
  return 0;
}

