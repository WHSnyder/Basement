Basic OpenGL game engine written in Python and C++.  Engine is rather low effort and current functionality is mostly hardcoded, the main point in it's creation was learning about OpenGL and exposing an interface to Tensorflow.  The Python scripting layer is bound to the C++ rendering logic via pybind11.  I chose Python to make integration with Tensorflow Lite easy.  This project is paired with my [fork of tensorflow](https://github.com/WHSnyder/tensorflow) that (eventually) allows for integration of GPU delegates with game engines.  Once that fork is operational, this engine will be used with it to test integration of basic style transfer models operating on GL framebuffer textures.

Feature overview:

- Nvidia PhysX 3.4 for physics and scene queries
- Basic shadow mapping
- Perlin noise terrain generator
- Arbitrary 3D models filetypes
- Node editor for creating OpenGL pipelines (very incomplete, based on [this ImGUI demo](https://github.com/thedmd/imgui-node-editor))
- Python bindings for important game control functions
