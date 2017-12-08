# MNIST_simple
A simple neural network in C to identify handwritten digits

This uses the MNIST database. Read more about it here http://yann.lecun.com/exdb/mnist/

It is a simple neural network with no hidden layers. Each input neuron represents one pixel in an image
which can be either white (0) or black (1) and each output neuron represents one digit (from 0 - 9).
Both are connected through synapses which carry a weight. The weights are increased when training the network
and determine the power each output neuron will have after evaluating an image. Highest power wins!

Weights are increased with the following formula

```
weight += 0.5 - weight / 2
```
which converges against 1. Since it does happen fairly quickly, too much training will reduce the prediction rate.

The network can correctly identify ~40% of the test images after training with 60k images, but this can be improved to ~70%
when the training size is reduced to 2k.

![alt text](https://i.imgur.com/tI9dUo4.png)
