from __future__ import absolute_import, division, print_function, unicode_literals

import sys

sys.path.append("/home/will/projects/cpprtx/build")
from GameContext import * #This must come before TF import.  No clue why.


try:
  # %tensorflow_version only exists in Colab.
  sys.path.insert(0, '/usr/local/lib/python3.6/dist-packages') #Specify tf version

  #import tensorflow.compat.v2 as tf
  import tensorflow as tf

except Exception:
  pass
#tf.enable_v2_behavior()

init_window()

import numpy as np
import time
import functools

content_path = tf.keras.utils.get_file('belfry.jpg','https://storage.googleapis.com/khanhlvg-public.appspot.com/arbitrary-style-transfer/belfry-2611573_1280.jpg')
style_path = tf.keras.utils.get_file('style23.jpg','https://storage.googleapis.com/khanhlvg-public.appspot.com/arbitrary-style-transfer/style23.jpg')

# Function to load an image from a file, and add a batch dimension.
def load_img(path_to_img):
  img = tf.io.read_file(path_to_img)
  img = tf.image.decode_image(img, channels=3)
  img = tf.image.convert_image_dtype(img, tf.float32)
  img = img[tf.newaxis, :]

  return img

# Function to pre-process style image input.
def preprocess_style_image(style_image):
  # Resize the image so that the shorter dimension becomes 256px.
  target_dim = 256
  shape = tf.cast(tf.shape(style_image)[1:-1], tf.float32)
  short_dim = min(shape)
  scale = target_dim / short_dim
  new_shape = tf.cast(shape * scale, tf.int32)
  style_image = tf.image.resize(style_image, new_shape)

  # Central crop the image.
  style_image = tf.image.resize_with_crop_or_pad(style_image, target_dim, target_dim)

  return style_image

# Function to pre-process content image input.
def preprocess_content_image(content_image):
  # Central crop the image.
  shape = tf.shape(content_image)[1:-1]
  short_dim = min(shape)
  content_image = tf.image.resize_with_crop_or_pad(content_image, short_dim, short_dim)

  return content_image

# Load the input images.
content_image = load_img(content_path)
style_image = load_img(style_path)

# Preprocess the input images.
preprocessed_content_image = preprocess_content_image(content_image)
preprocessed_style_image = preprocess_style_image(style_image)

print('Style Image Shape:', preprocessed_style_image.shape)
print('Content Image Shape:', preprocessed_content_image.shape)


# Function to run style prediction on preprocessed style image.
def run_style_predict(preprocessed_style_image):
  # Load the model.
  magenta_predict_path = "/home/will/projects/cpprtx/libs/tf_models/magenta_models/arb_style_predict.tflite"
  interpreter = tf.lite.Interpreter(model_path=magenta_predict_path)

  # Set model input.
  interpreter.allocate_tensors()
  input_details = interpreter.get_input_details()
  interpreter.set_tensor(input_details[0]["index"], preprocessed_style_image)

  # Calculate style bottleneck.
  interpreter.invoke()
  style_bottleneck = interpreter.tensor(
      interpreter.get_output_details()[0]["index"]
      )()

  return style_bottleneck

# Calculate style bottleneck for the preprocessed style image.
style_bottleneck = run_style_predict(preprocessed_style_image)
print('Style Bottleneck Shape:', style_bottleneck.shape)



# Run style transform on preprocessed style image
def run_style_transform(style_bottleneck, preprocessed_content_image):
  # Load the model.
  #deleg = tf.lite.experimental.load_delegate("/home/will/projects/cpprtx/libs/tf_gl/bazel-bin/tensorflow/lite/delegates/gpu/libtensorflowlite_gpu_delegate.so")
  meganta_transform_path = "/home/will/projects/cpprtx/libs/tf_models/magenta_models/arb_style_transform.tflite"
  interpreter = tf.lite.Interpreter(model_path=meganta_transform_path)#, experimental_delegates=[deleg])

  # Set model input.
  input_details = interpreter.get_input_details()
  interpreter.resize_tensor_input(input_details[0]["index"],
                                  preprocessed_content_image.shape)

  interpreter.allocate_tensors()


  # Set model inputs.
  interpreter.set_tensor(input_details[0]["index"], preprocessed_content_image)
  interpreter.set_tensor(input_details[1]["index"], style_bottleneck)

  interpreter.invoke()

  # Transform content image.
  stylized_image = interpreter.tensor(
      interpreter.get_output_details()[0]["index"]
      )()

  return stylized_image

# Stylize the content image using the style bottleneck.
stylized_image = run_style_transform(style_bottleneck, preprocessed_content_image)


