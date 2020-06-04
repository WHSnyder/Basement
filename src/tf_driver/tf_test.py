#from https://www.tensorflow.org/lite/convert/python_api
#Working with Metal and OpenGL backends, though no interop yet.
#Using harcoded paths for now because laziness.

import os, time, sys
sys.path.append("/home/will/projects/cpprtx/build")
sys.path.insert(0, '/usr/local/lib/python3.6/dist-packages') #Specify tf version

from GameContext import * #This must come before TF import.  No clue why.
import tensorflow as tf


init_window()

deleg = tf.lite.experimental.load_delegate("/home/will/projects/cpprtx/libs/tf_gl/bazel-bin/tensorflow/lite/delegates/gpu/libtensorflowlite_gpu_delegate.so")
#deleg = tf.lite.experimental.load_delegate("/Users/will/projects/cpprtx/libs/tf_gl/bazel-bin/tensorflow/lite/delegates/gpu/tensorflow_lite_gpu_dylib.dylib")

interpreter = tf.lite.Interpreter("/home/will/projects/cpprtx/libs/tf_models/test_tfconv.tflite", experimental_delegates=[deleg])
interpreter.allocate_tensors()

inp = interpreter.tensor(interpreter.get_input_details()[0]["index"])
output = interpreter.tensor(interpreter.get_output_details()[0]["index"])

for i in range(10):
	inp().fill(i)
	interpreter.invoke()
	print("Inference %s" % output())