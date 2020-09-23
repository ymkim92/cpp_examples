// gcc ledgui.c `pkg-config --cflags gtk+-3.0 pkg-config --libs gtk+-3.0`

#include <gtk/gtk.h>
#include <string.h>
#include <stdlib.h>

#define LED_COLOR_RED       0
#define LED_COLOR_GREEN     1
#define LED_COLOR_YELLOW    2
#define LED_COLOR_OFF       3
#define LED_COLOR_COUNT     4

char *color[] = {"red", "green", "yellow", "black"};
GtkWidget *ledLight;
GMutex mutex;
gchar ledText[256];

void *LedTask1 (void *vargp);
void *LedTask2 (void *vargp);
void LedGuiThread ();
void SetFontColors(GtkWidget *grid);
gboolean updateLabel(gpointer data);
void Task_sleep_ms(int ms);
void SetTextColor(char* fcolor);

int main() 
{ 
    pthread_t thread_id; 
    pthread_t thread_id2; 

    LedGuiThread();
    pthread_create(&thread_id2, NULL, LedTask1, NULL); 
    pthread_create(&thread_id, NULL, LedTask2, NULL);

    gtk_main();
    // pthread_join(thread_id2, NULL); 
    // pthread_join(thread_id, NULL); 
    exit(0); 
}

void *LedTask1 (void *vargp)
{
    while (1)
    {
        Task_sleep_ms(100);
        SetTextColor(color[LED_COLOR_RED]);
        g_idle_add(updateLabel, ledLight);
        Task_sleep_ms(100);
        SetTextColor(color[LED_COLOR_OFF]);
        g_idle_add(updateLabel, ledLight);
    }
}

void *LedTask2 (void *vargp)
{
    while (1)
    {
        Task_sleep_ms(100);
        SetTextColor(color[LED_COLOR_GREEN]);
        g_idle_add(updateLabel, ledLight);
        Task_sleep_ms(100);
        SetTextColor(color[LED_COLOR_OFF]);
        g_idle_add(updateLabel, ledLight);
    }
}

void LedGuiThread()
{
    GtkWidget *window;
    GtkWidget *grid;
    GtkWidget *ledLabel;
    int argc=0;
    char **argv=NULL;
    gtk_init (&argc,&argv);

    //Declarations
    window    = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title(GTK_WINDOW(window), "LED");
    grid      = gtk_grid_new ();

    SetFontColors(grid);

    ledLabel  = gtk_label_new ("1");
    ledLight  = gtk_label_new ("O");

    //Set Properties
    gtk_container_set_border_width (GTK_CONTAINER(window), 20);
    gtk_widget_set_size_request    (GTK_WIDGET(window), 200, 50);
    gtk_grid_set_row_spacing       (GTK_GRID(grid), 4);
    gtk_grid_set_column_spacing    (GTK_GRID(grid), 4);
    gtk_container_add              (GTK_CONTAINER(window), grid);

    //Fill the grid with shit                  (x, y, h, v)
    gtk_grid_attach (GTK_GRID(grid), ledLabel,       2, 2, 2, 1);
    gtk_grid_attach (GTK_GRID(grid), ledLight,       2, 3, 2, 1);

    gtk_widget_show_all (window);
    g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(gtk_main_quit), NULL);
}

void SetFontColors(GtkWidget *grid)
{
    GtkCssProvider *provider;
    GtkStyleContext *context;

    /* Change default font and color throughout the widget */
    provider = gtk_css_provider_new();
    gtk_css_provider_load_from_data(provider,
                                    "* {"
                                    " color: white;"
                                    " background-color: black;"
                                    " font: 20px monospace;"
                                    "}",
                                    -1,
                                    NULL);

    context = gtk_widget_get_style_context(grid);
    gtk_style_context_add_provider(context,
                                   GTK_STYLE_PROVIDER(provider),
                                   GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);

}

gboolean updateLabel(gpointer data)
{
    char text[256];
    GtkLabel *led = data;
    g_mutex_lock (&mutex);
    strncpy(text, ledText, sizeof(text));
    g_mutex_unlock (&mutex);
    gtk_label_set_markup (GTK_LABEL (led), text);

    //  If the function returns FALSE 
    // it is automatically removed from the list 
    // of event sources and will not be called again.
    return FALSE; 
}

void Task_sleep_ms(int ms)
{
    // printf("sleep %d ms\n", ms);
    usleep(ms * 1000);
}

void SetTextColor(char* fcolor)
{
    g_mutex_lock(&mutex);
    snprintf(ledText, 256, "<span background=\"black\" foreground=\"%s\">O</span>", fcolor);
    g_mutex_unlock(&mutex);
}