#include <stdlib.h>
#include <stdio.h>
#include <gtk/gtk.h>

#define TRAINING_IMAGES  ((const unsigned char *)"train-images.idx3-ubyte")
#define TRAINING_LABELS  ((const unsigned char *)"train-labels.idx1-ubyte")

#define TEST_IMAGES  ((const unsigned char *)"t10k-images.idx3-ubyte")
#define TEST_LABELS  ((const unsigned char *)"t10k-labels.idx1-ubyte")

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
    double power;

    /** The digit represented by this output (0-9) */
    char value;
} OutputNeuron;

/** Each synapse represents a connection between an Input- and an OutputNeuron */
struct Synapses {
    OutputNeuron *target;

    double weight;
};

typedef struct Images {
    char* pixels;

    /** The digit represented by this image (0-9) */
    char value;
} Image;

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

/** Trains the weights of the synapses for a given set of images. */
void train_network(InputNeuron *inputs, size_t inputs_size, Image *images, size_t images_size) {
    for (int i = 0; i < images_size; i++)
    {
        /** Loop through the inputs/pixels */
        for (int j = 0; j < inputs_size; j++)
        {
            /** MNIST uses greyscales, for now this function tries to see a pixel as 0 or 1 */
            if (images[i].pixels[j] > 230)
            {
                /** Loop through the outgoing synapses for the current input */
                for (int t = 0; t < 10; t++)
                {
                    /** Increase the weight of the synapse which targets the output which
                        represents the current immage */
                    if (inputs[j].synapses[t].target->value == images[i].value)
                    {
                        inputs[j].synapses[t].weight += 0.5 - (inputs[j].synapses[t].weight / 2);
                    }
                }
            }
        }
    }
}

Image *load_image_set(char* images_file, char* labels_file, int number_of_images)
{
    Image *images = malloc (number_of_images * sizeof(Image));

    /** Taken from http://www.cplusplus.com/reference/cstdio/fread/ */
    FILE *pFile;
    long lSize;
    char* buffer;
    size_t result;

    pFile = fopen ( images_file , "rb" );
    if (pFile==NULL) {
        fputs ("File error",stderr);
        exit (1);
    }

    // obtain file size:
    fseek (pFile , 0 , SEEK_END);
    lSize = ftell (pFile);
    rewind (pFile);

    // allocate memory to contain the whole file:
    buffer = (char*) malloc (sizeof(char)*lSize);
    if (buffer == NULL) {
        fputs ("Memory error",stderr);
        exit (2);
    }

    // copy the file into the buffer:
    result = fread (buffer,1,lSize,pFile);
    if (result != lSize) {fputs ("Reading error",stderr); exit (3);}

    /** Read the pixels for each image. */
    for (int i = 0; i < number_of_images; i++)
    {
        images[i].pixels = malloc(28 * 28);
        for (int j = 0; j < 28 * 28; j++)
        {
            /** (i * 28 * 28): Index of the current image multiplied by image size.
                16: Payload of the file, can be ignored.
                j: Index of the current pixel of the current image. */
            images[i].pixels[j] = buffer[(i * 28 * 28) + 16 + j];
        }
    }

    // terminate
    fclose (pFile);
    free (buffer);

    pFile = fopen ( labels_file , "rb" );
    if (pFile==NULL) {
        fputs ("File error",stderr);
        exit (1);
    }

    // obtain file size:
    fseek (pFile , 0 , SEEK_END);
    lSize = ftell (pFile);
    rewind (pFile);

    // allocate memory to contain the whole file:
    buffer = (char*) malloc (sizeof(char)*lSize);
    if (buffer == NULL) {
        fputs ("Memory error",stderr);
        exit (2);
    }

    // copy the file into the buffer:
    result = fread (buffer,1,lSize,pFile);
    if (result != lSize) {fputs ("Reading error",stderr); exit (3);}

    /** Read the labels for each image */
    for (int i = 0; i < number_of_images; i++)
    {
        images[i].value = buffer[i + 8];
    }

    // terminate
    fclose (pFile);
    free (buffer);

    return images;
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
