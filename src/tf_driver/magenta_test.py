import tensorflow as tf

deleg = tf.lite.experimental.load_delegate("/Users/will/projects/cpprtx/libs/master_tf/bazel-bin/tensorflow/lite/delegates/gpu/tensorflow_lite_gpu_dylib.dylib")
predictor_path = "/Users/will/projects/cpprtx/libs/tf_models/magenta_models/arb_style_predict.tflite"

interpreter = tf.lite.Interpreter(model_path=predictor_path, experimental_delegates=[deleg])

# Set model input.
#interpreter.allocate_tensors()
