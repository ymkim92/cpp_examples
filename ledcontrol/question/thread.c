
#include <gtk/gtk.h>


typedef struct
{
  GtkWidget  *window;
  guint       progress_id;
} WorkerData;


static gboolean
worker_finish_in_idle (gpointer data)
{
  WorkerData *wd = data;
  
  /* we're done, stop updating the progress bar */
  g_source_remove (wd->progress_id);
  /* and destroy everything */
  gtk_widget_destroy (wd->window);
  g_free (wd);
  
  return FALSE; /* stop running */
}

static gpointer
worker (gpointer data)
{
  WorkerData *wd = data;
  
  /* hard work here */
  g_usleep (5000000);
  
  /* we finished working, do something back in the main thread */
  g_idle_add (worker_finish_in_idle, wd);
  
  return NULL;
}

static gboolean
update_progress_in_timeout (gpointer pbar)
{
  gtk_progress_bar_pulse (pbar);
  
  return TRUE; /* keep running */
}

static void
button_clicked_handler (GtkWidget  *button,
                        gpointer    data)
{
  WorkerData *wd;
  GThread    *thread;
  GtkWidget  *pbar;
  
  wd = g_malloc (sizeof *wd);
  
  wd->window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  
  pbar = gtk_progress_bar_new ();
  gtk_container_add (GTK_CONTAINER (wd->window), pbar);
  
  gtk_widget_show_all (wd->window);
  
  /* add a timeout that will update the progress bar every 100ms */
  wd->progress_id = g_timeout_add (100, update_progress_in_timeout, pbar);
  
  /* run the time-consuming operation in a separate thread */
  thread = g_thread_new ("worker", worker, wd);
  g_thread_unref (thread);
}

int
main (int     argc,
      char  **argv)
{
  GtkWidget  *window;
  GtkWidget  *button;
  
  gtk_init (&argc, &argv);
  
  window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  g_signal_connect (G_OBJECT (window), "destroy",
                    G_CALLBACK (gtk_main_quit), NULL);
  
  button = gtk_button_new_from_stock (GTK_STOCK_OK);
  g_signal_connect (G_OBJECT (button), "clicked",
                    G_CALLBACK (button_clicked_handler), NULL);
  gtk_container_add (GTK_CONTAINER (window), button);
  
  gtk_widget_show_all (window);
  
  gtk_main ();
  
  return 0;
}
