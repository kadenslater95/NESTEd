
find_package(PkgConfig REQUIRED)
pkg_check_modules(GTK4 REQUIRED IMPORTED_TARGET gtk4)

find_package(OpenGL REQUIRED)
find_package(GLEW REQUIRED)


set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -g ${GTK4_CFLAGS}")

# Uncomment for debugging purposes
# set(CMAKE_VERBOSE_MAKEFILE ON)
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -v")


include_directories(${GTK4_INCLUDE_DIRS})


add_library(file_utils STATIC file_utils.c)
add_library(display_utils STATIC display_utils.c)


add_executable(main main.c)


target_link_libraries(display_utils PRIVATE PkgConfig::GTK4 file_utils OpenGL::GL OpenGL::GLU GLEW::glew)
target_link_libraries(main display_utils)
