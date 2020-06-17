import os, time, sys
import tensorflow as tf



#deleg = tf.lite.experimental.load_delegate("/Users/will/projects/cpprtx/libs/tf_gl/bazel-bin/tensorflow/lite/delegates/gpu/tensorflow_lite_gpu_dylib.dylib")

interpreter = tf.lite.Interpreter("/Users/will/projects/cpprtx/libs/tf_models/test_tfconv.tflite")#, experimental_delegates=[deleg])
interpreter.allocate_tensors()




#inp = interpreter.tensor(interpreter.get_input_details()[0]["index"])
#print(inp)
#output = interpreter.tensor(interpreter.get_output_details()[0]["index"])

for i in range(100):
	
#	inp().fill(i)
#	interpreter.invoke()
#	print("Inference %s" % output())


