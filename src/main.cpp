#include <iostream>
#include "network.h"
#include "fully_connected_layer.h"
#include "convolutional_layer.h"
#include "pooling_layer.h"
#include "mnist_parser.h"

using namespace nn;

int main(void) {
    network<cross_entropy, gradient_descent> nn;
    convolutional_layer<sigmoid_activation> C1(32, 32, 5, 1, 6);
    average_pooling_layer<sigmoid_activation> S2(28, 28, 6, 2);
    convolutional_layer<sigmoid_activation> C3(14, 14, 5, 6, 16);
    average_pooling_layer<sigmoid_activation> S4(10, 10, 16, 2);
    convolutional_layer<sigmoid_activation> C5(5, 5, 5, 16, 120);
    fully_connected_layer<sigmoid_activation> F6(120, 84);
    fully_connected_layer<sigmoid_activation> F7(84, 10);

    nn.add(&C1);
    nn.add(&S2);
    nn.add(&C3);
    nn.add(&S4);
    nn.add(&C5);
    nn.add(&F6);
    nn.add(&F7);

    std::vector<label_t> train_labels, test_labels;
    std::vector<vec_t> train_images, test_images;

    parse_labels("train-labels.idx1-ubyte", &train_labels);
    parse_images("train-images.idx3-ubyte", &train_images);
    parse_labels("t10k-labels.idx1-ubyte", &test_labels);
    parse_images("t10k-images.idx3-ubyte", &test_images);

    for (int epoch = 0; epoch < 3; epoch++) {
        nn.train(train_images, train_labels);
        nn.learner().alpha *= 0.8;
    }

    int success = 0;

    for (int i = 0; i < test_labels.size(); i++) {
        vec_t out;
        nn.predict(test_images[i], &out);

        const label_t predicted = max_index(out);
        const label_t actual = test_labels[i];

        if (predicted == actual) success++;
    }
    std::cout << "result:" << success << "/" << test_labels.size();
}