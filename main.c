#include <stdlib.h>
#include <gtk/gtk.h>

typedef struct Synapses Synapse;

/** Each InputNeuron represents one pixel in a 28 * 28 image */
typedef struct {
    /** Array of outgoing synapses from this input one to each of the outputs.
        If the pixel represented by this neuron is not white, the target of each
        synapse is accessed and its power increased by the weight of the synapse. */
    Synapse *synapses;
} InputNeuron;

/** Each OutputNeuron represents one of ten digits (0-9) */
typedef struct {
    /** Represents the likelihood this value matches the input image. Highest output wins */
    int power;

    /** The digit represented by this output (0-9) */
    char value;
} OutputNeuron;

/** Each synapse represents a connection between an Input- and an OutputNeuron */
struct Synapses {
    /** The target of this synapse */
    OutputNeuron *target;

    /** Weight of this synapse. */
    int weight;
};

/** Create the network and return an array of the input neurons */
InputNeuron *create_network() {
    OutputNeuron *outputs;
    InputNeuron *inputs;

    /** Create the outputs and assign values*/
    outputs = malloc(10 * sizeof(OutputNeuron));
    for (int i = 0; i < 10; i++)
    {
        outputs[i].value = i;
        outputs[i].power = 0;
    }

    /** Create the inputs and the synapses.
        Outer loop iterates through the inputs and
        creates the synapses for each input, inner loop iterates through
        the synapses and connects the outputs with the synapses. */
    inputs = malloc(28 * 28 * sizeof(InputNeuron));
    for (int i = 0; i < 28 * 28; i++)
    {
        inputs[i].synapses = malloc(10 * sizeof(Synapse));
        for (int j = 0; j < 10; j++)
        {
            inputs[i].synapses[j].weight = 0;
            inputs[i].synapses[j].target = &(outputs[j]);
        }
    }

    return inputs;
}

static void helloWorld (GtkWidget *wid, GtkWidget *win)
{
  GtkWidget *dialog = NULL;
  dialog = gtk_message_dialog_new (GTK_WINDOW (win), GTK_DIALOG_MODAL, GTK_MESSAGE_INFO, GTK_BUTTONS_CLOSE, "Hello World!");
  gtk_window_set_position (GTK_WINDOW (dialog), GTK_WIN_POS_CENTER);
  gtk_dialog_run (GTK_DIALOG (dialog));
  gtk_widget_destroy (dialog);
}

int main (int argc, char *argv[])
{


  GtkWidget *button = NULL;
  GtkWidget *win = NULL;
  GtkWidget *vbox = NULL;

  /* Initialize GTK+ */
  g_log_set_handler ("Gtk", G_LOG_LEVEL_WARNING, (GLogFunc) gtk_false, NULL);
  gtk_init (&argc, &argv);
  g_log_set_handler ("Gtk", G_LOG_LEVEL_WARNING, g_log_default_handler, NULL);

  /* Create the main window */
  win = gtk_window_new (GTK_WINDOW_TOPLEVEL);
  gtk_container_set_border_width (GTK_CONTAINER (win), 8);
  gtk_window_set_title (GTK_WINDOW (win), "Hello World");
  gtk_window_set_position (GTK_WINDOW (win), GTK_WIN_POS_CENTER);
  gtk_widget_realize (win);
  g_signal_connect (win, "destroy", gtk_main_quit, NULL);

  /* Create a vertical box with buttons */
  vbox = gtk_vbox_new (TRUE, 6);
  gtk_container_add (GTK_CONTAINER (win), vbox);

  button = gtk_button_new_from_stock (GTK_STOCK_DIALOG_INFO);
  g_signal_connect (G_OBJECT (button), "clicked", G_CALLBACK (helloWorld), (gpointer) win);
  gtk_box_pack_start (GTK_BOX (vbox), button, TRUE, TRUE, 0);

  button = gtk_button_new_from_stock (GTK_STOCK_CLOSE);
  g_signal_connect (button, "clicked", gtk_main_quit, NULL);
  gtk_box_pack_start (GTK_BOX (vbox), button, TRUE, TRUE, 0);

  /* Enter the main loop */
  gtk_widget_show_all (win);
  gtk_main ();
  return 0;
}
