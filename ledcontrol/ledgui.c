#include <stdint.h>
#include <stdbool.h>
#include "ledgui.h"


#define LED_COLOR_RED       0
#define LED_COLOR_GREEN     1
#define LED_COLOR_YELLOW    2
#define LED_COLOR_OFF       3
#define LED_COLOR_COUNT     4

char *ledColor[] = {"red", "green", "yellow", "black"};
uint8_t ledColorData[] = {0,0};
bool ledReady = false;
GtkWidget *ledLight;

static void SetGuiLedColor(GtkLabel *led, char* fcolor);
static uint8_t GetColorIndex();
void SetFontColors(GtkWidget *grid);

void *LedThread (void *vargp)
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
    ledReady = true;
    g_signal_connect(G_OBJECT(window), "destroy", G_CALLBACK(gtk_main_quit), NULL);
    gtk_main();
}

void GPIO_write(int led, int color, int value)
{
    int colorIndex;
    ledColorData[color-1] = value;

    printf("led %d, color %d, value %d\n", led, color, value);
    colorIndex = GetColorIndex();
    printf("color=%d\n", colorIndex);
    SetGuiLedColor(GTK_LABEL(ledLight), ledColor[colorIndex]);
}

static uint8_t GetColorIndex()
{
    if ( (ledColorData[0] == 0) && (ledColorData[1] == 0) ) 
        return 3;
    if ( (ledColorData[0] == 0) && (ledColorData[1] == 1) ) 
        return 1;
    if ( (ledColorData[0] == 1) && (ledColorData[1] == 0) ) 
        return 0;
    if ( (ledColorData[0] == 1) && (ledColorData[1] == 1) ) 
        return 2;
}

void LED_WaitReady()
{
    while (ledReady == false)
    {
        // printf("##########################\n");
        Task_sleep_ms(2000);
    }
}

void Task_sleep_ms(int ms)
{
    // printf("sleep %d ms\n", ms);
    usleep(ms * 1000);
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

static void SetGuiLedColor(GtkLabel *led, char* fcolor)
{
    char text[256];
    // https://unix.stackexchange.com/questions/457584/gtk3-change-text-color-in-a-label-raspberry-pi
    snprintf(text, 256, "<span background=\"black\" foreground=\"%s\">O</span>", fcolor);
    gtk_label_set_markup (led, text);
}
