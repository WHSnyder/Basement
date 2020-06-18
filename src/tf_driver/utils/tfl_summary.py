import os, time, sys
import tensorflow as tf



interpreter = tf.lite.Interpreter("/home/will/projects/cpprtx/libs/tf_models/magenta_models/arb_style_transform.tflite")#, experimental_delegates=[deleg])
#interpreter.allocate_tensors()

tensors = interpreter.get_tensor_details()

for i,tensor in enumerate(tensors):
	print(str(i) + "\n" + str(tensor) + "\n")
