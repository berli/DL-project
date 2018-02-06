'''
from __future__ import print_function
import os
import matplotlib.pyplot as plt
import tensorflow as tf
from PIL import Image
import numpy
import tensorflow as tf
'''
 
#include <tensorflow/core/platform/env.h>
#include <tensorflow/core/public/session.h>

#include <iostream>

using namespace tensorflow;

# Import MNIST data
#from tensorflow.examples.tutorials.mnist import input_data

# Parameters
float learning_rate = 0.001
int training_iters = 3000
int batch_size = 10
int display_step = 2

# Network Parameters
int n_input = 128*128 # MNIST data input (img shape: 28*28)
int n_classes = 10 # MNIST total classes (0-9 digits)
float dropout = 0.75 # Dropout, probability to keep units

# tf Graph input
auto root = tensorflow::Scope::NewRootScope();
#x = tf.placeholder(tf.float32, [None, 128, 128, 3])
TensorShape x_shape({None, 128, 128, 3});
auto x = Placeholder(root.WithOpName("input"), DataType::FLOAT, Placeholder::Shape(x_shape));

#y = tf.placeholder(tf.float32, [None, n_classes])
auto y = Placeholder(root.WithOpName("input"), DataType::FLOAT);
#keep_prob = tf.placeholder(tf.float32) #dropout (keep probability)
auto y = Placeholder(root.WithOpName("input"), DataType::FLOAT);


# Create some wrappers for simplicity
def conv2d(x, W, b, strides=1):
    # Conv2D wrapper, with bias and relu activation
    x = tf.nn.conv2d(x, W, strides=[1, strides, strides, 1], padding='SAME')
    auto x = Conv2D(root.WithOpName("output"), x, W, {1, strides, strides, 1}, "SAME");
    #x = tf.nn.bias_add(x, b)
    BiasAdd(scope,x,b);
    #return tf.nn.relu(x)
    return Relu(scope,x);


def maxpool2d(x, k=2):
    # MaxPool2D wrapper
    #return tf.nn.max_pool(x, ksize=[1, k, k, 1], strides=[1, k, k, 1],
    #                      padding='SAME')
    return MaxPool(scope, x,
    ArraySlice< int > ksize({1,k,k,1}),
    ArraySlice< int > ksize({1,k,k,1}),
  "SAME"
);


# Create model
def conv_net(x, weights, biases, dropout):
    # Reshape input picture
    #x = tf.reshape(x, shape=[-1, 128, 128, 3])
    TensorShape x_shape({-1, 128, 128, 3});
    x = Reshape(scope, x, Placeholder::Shape(x_shape));

    # Convolution Layer
    conv1 = conv2d(x, weights['wc1'], biases['bc1'])
    print(conv1.shape)
    # Max Pooling (down-sampling)
    conv1 = maxpool2d(conv1, k=2)
    print(conv1.shape)
    # Convolution Layer
    conv2 = conv2d(conv1, weights['wc2'], biases['bc2'])
    print(conv2.shape)
    # Max Pooling (down-sampling)
    conv2 = maxpool2d(conv2, k=2)
    print(conv2.shape)
    # Fully connected layer
    # Reshape conv2 output to fit fully connected layer input
    fc1 = tf.reshape(conv2, [-1, weights['wd1'].get_shape().as_list()[0]])
    #fc1 = tf.add(tf.matmul(fc1, weights['wd1']), biases['bd1'])
    auto fc1 = Add(const ::tensorflow::Scope & scope,::tensorflow::Input x,::tensorflow::Input y);

    #fc1 = tf.nn.relu(fc1)
    fc1 = Relu(const ::tensorflow::Scope & scope,::tensorflow::Input features)
    # Apply Dropout
    fc1 = tf.nn.dropout(fc1, dropout)

    # Output, class prediction
    out = tf.add(tf.matmul(fc1, weights['out']), biases['out'])
    return out

# Store layers weight & bias
weights = {
    # 5x5 conv, 1 input, 32 outputs
    'wc1': tf.Variable(tf.random_normal([5, 5, 3, 24])),
    # 5x5 conv, 32 inputs, 64 outputs
    'wc2': tf.Variable(tf.random_normal([5, 5, 24, 96])),
    # fully connected, 7*7*64 inputs, 1024 outputs
    'wd1': tf.Variable(tf.random_normal([32*32*96, 1024])),
    # 1024 inputs, 10 outputs (class prediction)
    'out': tf.Variable(tf.random_normal([1024, n_classes]))
}

biases = {
    'bc1': tf.Variable(tf.random_normal([24])),
    'bc2': tf.Variable(tf.random_normal([96])),
    'bd1': tf.Variable(tf.random_normal([1024])),
    'out': tf.Variable(tf.random_normal([n_classes]))
}

# Construct model
pred = conv_net(x, weights, biases, keep_prob)
pred_result=tf.argmax(pred, 1)
pred_result=ArgMax(const ::tensorflow::Scope & scope, ::tensorflow::Input input, ::tensorflow::Input dimension)
# Define loss and optimizer
cost = tf.reduce_mean(tf.nn.softmax_cross_entropy_with_logits(logits=pred, labels=y))
optimizer = tf.train.AdamOptimizer(learning_rate=learning_rate).minimize(cost)

# Evaluate model
correct_pred = tf.equal(tf.argmax(pred, 1), tf.argmax(y, 1))
accuracy = tf.reduce_mean(tf.cast(correct_pred, tf.float32))

# Initializing the variables
init = tf.global_variables_initializer()
saver=tf.train.Saver()

# Launch the graph
with tf.Session() as sess:
    saver.restore(sess, "./model/model.ckpt")
    step = 1
    # Keep training until reach max iterations
    list = os.listdir("./test_resize/")
    # print(list)
    print(len(list))
    input_xs = []
    for image in list:
        img = Image.open("./test_resize/"+image)
        img_ndarray = numpy.asarray(img,dtype='float32')
        img_ndarray = numpy.reshape(img_ndarray,[128,128,3])
        print(type(img_ndarray.tolist()))
        input_xs.append(img_ndarray.tolist())
        input_xs = numpy.asarray(input_xs)
        prediction_test = sess.run(pred_result,feed_dict={x:input_xs,keep_prob:1})
        print(prediction_test)

    print('Test Finished')
    '''
    for batch_id in range(0, 2):
        batch = list[batch_id * 10:batch_id * 10 + 10]
        batch_xs = []
        batch_ys = []
        for image in batch:
            id_tag = image.find("-")
            score = image[0:id_tag]
            # print(score)
            img = Image.open("./test_resize/" + image)
            img_ndarray = numpy.asarray(img, dtype='float32')
            img_ndarray = numpy.reshape(img_ndarray, [128, 128, 3])
            # print(img_ndarray.shape)
            batch_x = img_ndarray
            batch_xs.append(batch_x)
        print(batch_xs)
        # print(batch_ys)
        batch_xs = numpy.asarray(batch_xs)
        #print(batch_xs.shape)
        # Run optimization op (backprop)
        pred_result_test=sess.run(pred_result, feed_dict={x: batch_xs,keep_prob: 1.})
        print(pred_result_test)
    print("Test Finished!")
    '''
