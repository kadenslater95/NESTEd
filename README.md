# NESTEd
This is a NES Tile Editor written in C. It uses GTK to make a desktop application, and uses modern OpenGL to implement the graphic display.


## Prerequisites
#### Note: This was developed on Ubuntu. So long as you can install gtk4, etc. you should be able to build from src for your platform. The instructions I provide will be using the `apt` package manager for examples.
#### Also, you might have to update some of the include paths in `.vscode/c_cpp_properties.json` to make some red squiggles go away if you are on a different platform.


### GTK4
On a debian linux distro you can install with apt
```
sudo apt-get install libgtk-4-1 libgtk-4-bin libgtk-4-dev
```
This provides GTK4 and the cflags via pkg-config which gets used in the Makefile.


### glu
On Ubuntu 24.04 (at time of writing) it seems that the the `glu.h` file is missing from the default OpenGL install, so I had to install this to have /usr/include/GL/glu.h present.
```
sudo apt-get install libglu1-mesa libglu1-mesa-dev
```
I found that package using the `apt-file` package to search for  `glu.h`.


### GLEW
With apt I installed 2 packages
```
sudo apt-get install libglew-dev libglew2.2
```


### Cmake
With apt I installed with
```
sudo apt-get install cmake
```


### cpplint
Note that this is written in C but most of the styling guidelines are still applicatble. Anything that follows the [Google Style Guide](https://google.github.io/styleguide/cppguide.html) will be consistent with what is in this project.
```
sudo apt-get install cpplint
```


## Build
Using CMake from the root directory, run these commands sequentially:
```
cmake -S . -B build

cmake --build build
```


## Run
```
build/src/main
```


## Test
```
build/test/nested_tests
```


## Lint
```
cpplint --recursive src include
```