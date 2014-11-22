/* @ginfo.c GUI to display midi info */

#undef GTK_DISABLE_DEPRECATED
#include <gtk/gtk.h>
static GtkBuilder *builder;

void
quit_activate (GtkAction *action)
{
    GtkWidget *window;
     window = GTK_WIDGET (gtk_builder_get_object (builder, "window1"));
    gtk_widget_destroy(window);
}

void
about_activate (GtkAction *action)
{
    GtkWidget *about_dlg;
    about_dlg = GTK_WIDGET(gtk_builder_get_object(builder, "aboutdialog1"));
    gtk_dialog_run(GTK_DIALOG(about_dlg));
    gtk_widget_hide(about_dlg);
}

void
help_activate (GtkAction *action)
{
    g_print ("Help not available\n");
}

int main(int argc, char *argv[])
{
    static GtkWidget *window = NULL;
    GError *err = NULL;
    GtkWidget *toolbar;
        
    gtk_init(&argc, &argv);                            
                                
    builder = gtk_builder_new();
    gtk_builder_add_from_file(builder, "ginfo.ui", &err);
    if (err)
    {
        g_error("ERROR: %s\n", err->message);
        return -1;
    }
    
    gtk_builder_connect_signals(builder, NULL);
    
    window = GTK_WIDGET(gtk_builder_get_object(builder, "window1"));
    
    g_signal_connect(window, "destroy",
    G_CALLBACK(gtk_widget_destroyed), &window);
    
    toolbar = GTK_WIDGET(gtk_builder_get_object(builder, "toolbar1"));
    gtk_style_context_add_class(gtk_widget_get_style_context (toolbar),
    "primary-toolbar");
    
    gtk_widget_show_all(window);
    
    gtk_main ();

    return 0;
}
