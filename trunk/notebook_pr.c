/*
 example-start notebook notebook.c */

#include<stdlib.h>
#include<fcntl.h>
#include<sys/socket.h>
#include <stdio.h>
#include <gtk/gtk.h>
#include <string.h>
#include <stdio.h>
#include <gdk/gdkx.h>
#include <X11/Xlib.h>
#include <locale.h>
#include <signal.h>
#include<netdb.h>
#include<netinet/in.h>

void memory_etiq(int,char *);
void memory_label(int,char *);
void abrir(char *);
void acero(void);
int comunica(void);
void en_array(int,int );
GtkWidget* lookup_widget(GtkWidget  *, const gchar     *);

char *aetiq[100];
char *alabel[100];
char tit[250];
int A[100][40];
    GtkWidget *text[100];
    GtkWidget *entry[100];

int tpag;
int pids;
//
GtkWidget*
lookup_widget                          (GtkWidget       *widget,
                                        const gchar     *widget_name)
{
  GtkWidget *parent, *found_widget;

  for (;;)
    {
      if (GTK_IS_MENU (widget))
        parent = gtk_menu_get_attach_widget (GTK_MENU (widget));
      else
        parent = widget->parent;
      if (parent == NULL)
        break;
      widget = parent;
    }

  found_widget = (GtkWidget*) gtk_object_get_data (GTK_OBJECT (widget),
                                                   widget_name);
  if (!found_widget)
    g_warning ("Widget not found: %s", widget_name);
  return found_widget;
}
//


int comunica(void) {
    int id_ssmct;
    FILE *fp;
    char s[100];
    fp=fopen("/tmp/.ssmct_pid","r");
    fgets(s,100,fp);
    sscanf(s,"%d",&id_ssmct);
    printf("El id de ssmct es: %d\n",id_ssmct);
    fclose(fp);
    unlink("/tmp/.ssmct_pid");
    return id_ssmct;
}


void acero() {
   int i,j;
   for(i=0;i<=99;i++) {
     aetiq[i]=alabel[i]=NULL;
     for(j=0;j<=39;j++)
       A[i][j]=0;
   }
}

void en_array(int x1,int v) {
  static int y=-1;
  static int x=0;
  if(x1!=x){
     x=x1;
     y=-1;
  }
  y++;
  A[x][y]=v;
}

/* This function rotates the position of the tabs */
void rotate_book( GtkButton   *button,
                  GtkNotebook *notebook )
{
    gtk_notebook_set_tab_pos (notebook, (notebook->tab_pos +1) %4);
}
void next_page( GtkButton   *button, GtkNotebook *notebook ) {
    static int i=0;
    static int j=0;
    static int paso=0;
    int n;
    if(paso) {
       gtk_notebook_next_page(notebook);
       return;
    }
    //if(i==0) {
       //i=comunica();
    //}
    gtk_notebook_next_page(notebook);
    n=gtk_notebook_current_page(notebook);
    if(n>j) {
      j=n;
      //printf("- grabar- %d\n",n); 
      kill(pids,SIGUSR1);
    }
    if(n==tpag) {
      usleep(500000);
      //kill(pids,SIGUSR2);
      paso=1;
    }
}

/* Add/Remove the page tabs and the borders */
void tabsborder_book( GtkButton   *button,
                      GtkNotebook *notebook )
{
    gint tval = FALSE;
    gint bval = FALSE;
    if (notebook->show_tabs == 0)
            tval = TRUE;
    if (notebook->show_border == 0)
            bval = TRUE;

    gtk_notebook_set_show_tabs (notebook, tval);
    gtk_notebook_set_show_border (notebook, bval);
}

/* Remove a page from the notebook */
void remove_book( GtkButton   *button,
                  GtkNotebook *notebook )
{
    gint page;

    page = gtk_notebook_get_current_page(notebook);
    gtk_notebook_remove_page (notebook, page);
    /* Need to refresh the widget --
     This forces the widget to redraw itself. */
    gtk_widget_draw(GTK_WIDGET(notebook), NULL);
}
void memory_etiq(int ietiq,char *s) {
   aetiq[ietiq]=(char *)malloc(strlen(s)+1);
   strcpy(aetiq[ietiq],s);
}
void memory_label(int ietiq,char *s) {
   static int i=-1;
   i++;
   alabel[i]=(char *)malloc(strlen(s)+1);
   strcpy(alabel[i],s);
   en_array(ietiq,1);
}

gint delete( GtkWidget *widget,
             GtkWidget *event,
             gpointer   data )
{
   gchar *pi,*po;
   gchar enviar[59800];
   int i,j,k,l;
   pi=po=NULL;
   k=l=0;
   *enviar='\0';
   for(i=0;A[i][0]!=0;i++) {
     for(j=0;A[i][j]!=0;j++) {
       if(A[i][j]==2) {
         po = gtk_entry_get_text(GTK_ENTRY(entry[k++]));
         strcat(enviar,po);
         strcat(enviar,"\v");
       }
       if(A[i][j]==3) {
         pi=gtk_editable_get_chars((GtkEditable *)text[l++],0,-1);
         strcat(enviar,pi);
         strcat(enviar,"\v");
       }
     }
   }
   free(pi);
   free(po);
   if(*enviar!='\0')
      enviar[strlen(enviar)-1]='\0';
  
  //remover files de /tmp
  if(write(1,enviar,strlen(enviar))!=strlen(enviar))
      fprintf(stderr,"Error al escribir en pipe desde p. hijo\n");

  gtk_main_quit();
  return(FALSE);
}

void abrir(char *file) {
    FILE *fp;
    int n,ietiq;
    char s[10000],st[10000], *p;
    fp=fopen(file,"r");
    if(fp==NULL) {
       perror("Al abrir file argumento");
       exit(1);
    }
    *st='\0';
    ietiq=-1;
    p=fgets(tit,250,fp);
    if(p==NULL) {
      printf("** No hay contenido en el fichero **\n");
      exit(1);
    }
    tit[strlen(tit)-1]='\0';
    while(fgets(s,10000,fp)) {
       if(*s=='[') {
         n=strlen(s);
         if(s[n-2]==']') {
           s[n-2]='\0';
           if(*st!='\0'){
              memory_label(ietiq,st);
              *st='\0';
           }
           ietiq++;
           memory_etiq(ietiq,s+1);
           *s='\0';
         }
       }
       if(*s=='<' && s[1]=='>') {
           if(*st!='\0'){
              memory_label(ietiq,st);
              *st='\0';
           }
           en_array(ietiq,2);
           *s='\0';
       }
       if(*s=='<' && s[1]=='t' && s[2]=='>') {
           if(*st!='\0'){
              memory_label(ietiq,st);
              *st='\0';
           }
           en_array(ietiq,3);
           *s='\0';
       }
       strcat(st,s);
    }
    if(*st!='\0') {
       memory_label(ietiq,st);
    }
    fclose(fp);
    tpag=ietiq;
    printf("%d-%d-%d-%d-%d\n",A[0][0],A[0][1],A[0][2],A[0][3],A[0][4]);
    printf("%d-%d-%d-%d-%d\n",A[1][0],A[1][1],A[1][2],A[1][3],A[1][4]);
    printf("%d-%d-%d-%d-%d\n",A[2][0],A[2][1],A[2][2],A[2][3],A[2][4]);
   
}

int main( int argc,
          char *argv[] )
{
    GtkWidget *window;
    GtkWidget *button;
    GtkWidget *table;
    GtkWidget *notebook;
    GtkWidget *frame;
    GtkWidget *label;
    GtkWidget *label1;
    GtkWidget *vbox;
    GtkWidget *checkbutton1;
    GtkWidget *checkbutton;
    GtkWidget *text1;
    GtkWidget *entry2;
    GtkWidget *entry3;
    GtkWidget *scrolledwindow2;
    GtkWidget *scrolled[100];
    GdkFont *font;
    GdkFont *font1;
    GtkStyle *style;
    GtkStyle *style1;
    GtkStyle *defstyle;
    int i,j,i_l,i_e,i_t;
    char buf[1000],buf1[1000],buf2[1000],buf3[1000],buf4[1000];
    char s[10000];
    setlocale(LC_ALL,"es_ES");
    pids=comunica();
    abrir(argv[1]);
    gtk_init (&argc, &argv);
    
    font = gdk_font_load ("-misc-fixed-bold-r-*-*-*-140-*-*-*-*-*-*");
    font1 = gdk_font_load ("-misc-fixed-medium-r-*-*-*-140-*-*-*-*-*-*");
    //font=gdk_font_load("-adobe-helvetica-bold-r-normal--18-180-75-75-p-103-iso8859-1");
    defstyle=gtk_widget_get_default_style();
    style=gtk_style_copy(defstyle);
    style1=gtk_style_copy(defstyle);
    style->font=font;
    style1->font=font1;
    window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    
    gtk_window_set_title(GTK_WINDOW(window),tit);

    gtk_signal_connect (GTK_OBJECT (window), "delete_event",
                        GTK_SIGNAL_FUNC (delete), NULL);

    gtk_container_set_border_width (GTK_CONTAINER (window), 10);
//
    gtk_widget_set_usize (GTK_WIDGET(window), 600,500);
    gtk_window_set_policy (GTK_WINDOW(window), TRUE,TRUE,FALSE);
//

    table = gtk_table_new(5,5,FALSE);
    gtk_container_add (GTK_CONTAINER (window), table);

    /* Create a new notebook, place the position of the tabs */
    notebook = gtk_notebook_new ();
    gtk_notebook_set_tab_pos (GTK_NOTEBOOK (notebook), GTK_POS_TOP);
    gtk_table_attach_defaults(GTK_TABLE(table), notebook, 0,5,0,4);
    gtk_widget_show(notebook);
    gtk_notebook_set_show_tabs (GTK_NOTEBOOK(notebook),FALSE);

    /* Let's append a bunch of pages to the notebook */
    i_l=i_e=i_t=0;
    for (i=0;aetiq[i]!=NULL;i++) {
        strcpy(buf,aetiq[i]);
        frame = gtk_frame_new (aetiq[i]);
        gtk_container_set_border_width (GTK_CONTAINER (frame), 10);
        gtk_widget_set_usize (frame, 100, 75);
        gtk_widget_show (frame);
        vbox=gtk_vbox_new(FALSE,0);
        gtk_widget_show(vbox);
        gtk_container_add(GTK_CONTAINER (frame), vbox);
        for(j=0;A[i][j]!=0;j++) {
          if(A[i][j]==1) {
             label = gtk_label_new (alabel[i_l]);
             gtk_widget_set_style(label,style);
             gtk_label_set_justify (GTK_LABEL (label), GTK_JUSTIFY_LEFT);
             //gtk_label_set_line_wrap (GTK_LABEL (label), TRUE);
             gtk_widget_show (label);
             gtk_box_pack_start(GTK_BOX (vbox), label,TRUE,FALSE,0);
             i_l++;
          }
          if(A[i][j]==2) {
            entry[i_e] = gtk_entry_new ();
            gtk_widget_set_style(entry[i_e],style1);
            gtk_widget_ref (entry[i_e]);
            sprintf(buf1,"%s%d","entry",i_e);
            gtk_object_set_data_full (GTK_OBJECT (window), buf1, entry[i_e],
                            (GtkDestroyNotify) gtk_widget_unref);
             gtk_widget_show (entry[i_e]);
             gtk_box_pack_start (GTK_BOX (vbox), entry[i_e], TRUE, FALSE, 0);
             i_e++;
           }
          if(A[i][j]==3) {
            scrolled[i_t] = gtk_scrolled_window_new (NULL, NULL);
            gtk_widget_ref (scrolled[i_t]);
            sprintf(buf2,"%s%d","scrolled",i_t);
            gtk_object_set_data_full (GTK_OBJECT (window),
                         buf2, scrolled[i_t],
                         (GtkDestroyNotify) gtk_widget_unref);
            gtk_widget_show (scrolled[i_t]);
            gtk_box_pack_start (GTK_BOX (vbox), scrolled[i_t], TRUE, TRUE, 0);
            gtk_scrolled_window_set_policy (GTK_SCROLLED_WINDOW (scrolled[i_t]),
                                           GTK_POLICY_NEVER, GTK_POLICY_ALWAYS);
            text[i_t] = gtk_text_new (NULL, NULL);
            gtk_widget_set_style(text[i_t],style1);
            gtk_widget_ref (text[i_t]);
            sprintf(buf3,"%s%d","text",i_t);
            gtk_object_set_data_full (GTK_OBJECT (window), buf3, text[i_t],
                            (GtkDestroyNotify) gtk_widget_unref);
            gtk_widget_show (text[i_t]);
            gtk_container_add (GTK_CONTAINER (scrolled[i_t]), text[i_t]);
            gtk_widget_set_usize (text[i_t], -2, 135);
            gtk_text_set_editable(GTK_TEXT(text[i_t]),TRUE);
            i_t++;
          }
        }

//  gtk_label_set_justify (GTK_LABEL (label), GTK_JUSTIFY_FILL);
 // gtk_label_set_line_wrap (GTK_LABEL (label), TRUE);

        label = gtk_label_new (buf);
        gtk_notebook_append_page (GTK_NOTEBOOK (notebook), frame, label);
    }


    /* Set what page to start at (page 1) */
    gtk_notebook_set_page (GTK_NOTEBOOK(notebook), 0);


/* Create a bunch of buttons */
    button = gtk_button_new_with_label ("close/enviar");
    gtk_signal_connect_object (GTK_OBJECT (button), "clicked",
                               GTK_SIGNAL_FUNC (delete), NULL);
    //gtk_table_attach_defaults(GTK_TABLE(table), button, 0,1,4,5);
    gtk_table_attach(GTK_TABLE(table), button, 0,1,4,5,GTK_EXPAND,GTK_SHRINK,6,10);
    gtk_widget_show(button);

    button = gtk_button_new_with_label ("next page");
    gtk_signal_connect (GTK_OBJECT (button), "clicked",
                               (GtkSignalFunc) (next_page),
                               GTK_OBJECT (notebook));
    //gtk_table_attach_defaults(GTK_TABLE(table), button, 1,2,4,5);
    gtk_table_attach(GTK_TABLE(table), button, 1,2,4,5,GTK_EXPAND,GTK_SHRINK,0,0);
    gtk_widget_show(button);

    button = gtk_button_new_with_label ("prev page");
    gtk_signal_connect_object (GTK_OBJECT (button), "clicked",
                               (GtkSignalFunc) gtk_notebook_prev_page,
                               GTK_OBJECT (notebook));
    //gtk_table_attach_defaults(GTK_TABLE(table), button, 2,3,4,5);
    gtk_table_attach(GTK_TABLE(table), button, 2,3,4,5,GTK_EXPAND,GTK_SHRINK,0,0);
    gtk_widget_show(button);

    button = gtk_button_new_with_label ("tab position");
    gtk_signal_connect (GTK_OBJECT (button), "clicked",
                        (GtkSignalFunc) rotate_book,
                        GTK_OBJECT(notebook));
    //gtk_table_attach_defaults(GTK_TABLE(table), button, 3,4,4,5);
    gtk_table_attach(GTK_TABLE(table), button, 3,4,4,5,GTK_EXPAND,GTK_SHRINK,0,0);
    gtk_widget_show(button);

    button = gtk_button_new_with_label ("tabs/border on/off");
    gtk_signal_connect (GTK_OBJECT (button), "clicked",
                        (GtkSignalFunc) tabsborder_book,
                        GTK_OBJECT (notebook));
    //gtk_table_attach_defaults(GTK_TABLE(table), button, 4,5,4,5);
    gtk_table_attach(GTK_TABLE(table), button, 4,5,4,5,GTK_EXPAND,GTK_SHRINK,0,0);
    gtk_widget_show(button);

 /* button = gtk_button_new_with_label ("remove page");
    gtk_signal_connect (GTK_OBJECT (button), "clicked",
                        (GtkSignalFunc) remove_book,
                        GTK_OBJECT(notebook));
    gtk_table_attach_defaults(GTK_TABLE(table), button, 5,6,3,4);
    gtk_widget_show(button);
 */

    gtk_widget_show(table);
    gtk_widget_show(window);

    gtk_main ();

    return(0);
}
/* example-end */

