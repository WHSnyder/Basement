#from https://www.tensorflow.org/lite/convert/python_api

#Testing basic save-full load-lite capabilities.  Will next be tested with 
#macos metal delegates compiled from source... 


import os, time, sys
sys.path.append("/Users/will/projects/cpprtx/build")

#from GameContext import *

import tensorflow as tf


#init_window()

print("HERE\n")

# Construct a basic model.
'''
root = tf.train.Checkpoint()
root.v1 = tf.Variable(3.)
root.v2 = tf.Variable(2.)
root.f = tf.function(lambda x: root.v1 * root.v2 * x)

# Save the model.
export_dir = "/tmp/test_saved_model"
input_data = tf.constant(1., shape=[1, 1])
to_save = root.f.get_concrete_function(input_data)
tf.saved_model.save(root, export_dir, to_save)

# Convert the model.
converter = tf.lite.TFLiteConverter.from_saved_model(export_dir)
tflite_model = converter.convert()

print(tflite_model)
'''
#import tflite_runtime.interpreter as tflite


#deleg = tf.lite.experimental.load_delegate("/Users/will/projects/cpprtx/libs/tf_gl/bazel-bin/tensorflow/lite/delegates/gpu/libtensorflowlite_gpu_delegate.so")
deleg = tf.lite.experimental.load_delegate("/Users/will/projects/cpprtx/libs/tf_gl/bazel-bin/tensorflow/lite/delegates/gpu/tensorflow_lite_gpu_dylib.dylib")

print("deleg loaded\n")

interpreter = tf.lite.Interpreter("/Users/will/projects/cpprtx/libs/tf_models/test_tfconv.tflite", experimental_delegates=[deleg])
interpreter.allocate_tensors()

print("tensors alloced\n")

inp = interpreter.tensor(interpreter.get_input_details()[0]["index"])
output = interpreter.tensor(interpreter.get_output_details()[0]["index"])

for i in range(10):
	inp().fill(i)
	interpreter.invoke()
	print("Inference %s" % output())