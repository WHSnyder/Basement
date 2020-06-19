import os, time, sys
import tensorflow as tf

interpreter = tf.lite.Interpreter("/home/will/projects/cpprtx/libs/tf_models/magenta_models/arb_style_transform.tflite")
input_details = interpreter.get_input_details()

interpreter.resize_tensor_input(input_details[0]['index'], (1, 600, 600, 3))

tensors = interpreter.get_tensor_details()

for i,tensor in enumerate(tensors):
	print(str(i) + ": name = " + tensor["name"] + " shape = "  + str(tensor["shape"]))
