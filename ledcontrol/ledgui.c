#include "ledgui.h"

static GtkTextBuffer *buffer;

static void activate (GtkApplication* app, gpointer user_data)
{
    GtkWidget *window;

    window = gtk_application_window_new(app);
    gtk_window_set_title(GTK_WINDOW(window), "LED");
    gtk_window_set_default_size(GTK_WINDOW(window), 200, 50);

    GtkWidget *view;
    GtkCssProvider *provider;
    GtkStyleContext *context;

    view = gtk_text_view_new();
    gtk_container_add (GTK_CONTAINER (window), view);
    gtk_widget_set_name (view, "led1");

    buffer = gtk_text_view_get_buffer(GTK_TEXT_VIEW(view));
    gtk_text_buffer_set_text(buffer, "1 2 3 4\no o o o", -1);

    /* Change default font and color throughout the widget */
    provider = gtk_css_provider_new();
    gtk_css_provider_load_from_data(provider,
                                    "* {"
                                    " color: white;"
                                    " background-color: black;"
                                    " font: 20 monospace;"
                                    "}",
                                    -1,
                                    NULL);

    context = gtk_widget_get_style_context(view);
    gtk_style_context_add_provider(context,
                                   GTK_STYLE_PROVIDER(provider),
                                   GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
#if 1
    /* Change left margin throughout the widget */
    gtk_text_view_set_left_margin (GTK_TEXT_VIEW (view), 40);
    gtk_text_view_set_top_margin (GTK_TEXT_VIEW (view), 2);

    GtkTextTag *tag;
    char color[] = "black";
    /* Use a tag to change the color for just one part of the widget */
    tag = gtk_text_buffer_create_tag (buffer, "blue_foreground",
                            "foreground", color, NULL);  

    SetGuiLedColor(0, tag);
    SetGuiLedColor(1, tag);
    SetGuiLedColor(2, tag);
    SetGuiLedColor(3, tag);
    // g_print ("Hello World\n");
#endif
    gtk_widget_show_all(window);
}

void SetGuiLedColor(int id, GtkTextTag *tag)
{
    GtkTextIter start, end;
    int idStart, idEnd;
    idStart = id*2+8;
    idEnd = idStart+1;
    g_print ("LED color %d %d\n", idStart, idEnd);
    gtk_text_buffer_get_iter_at_offset (buffer, &start, idStart);
    gtk_text_buffer_get_iter_at_offset (buffer, &end, idEnd);
    gtk_text_buffer_apply_tag (buffer, tag, &start, &end);
}

void *LedThread (void *vargp)
{
    GtkApplication *app;
    int status;

    app = gtk_application_new("org.gtk.example", G_APPLICATION_FLAGS_NONE);
    g_signal_connect(app, "activate", G_CALLBACK(activate), NULL);
    status = g_application_run(G_APPLICATION(app), 0, NULL);
    g_object_unref(app);
}