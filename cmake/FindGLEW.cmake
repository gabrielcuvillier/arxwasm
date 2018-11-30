
# Try to find the GLEW library and include path.
# Once done this will define
#
# GLEW_FOUND
# GLEW_INCLUDE_DIR   Where to find GL/glew.h
# GLEW_LIBRARIES     The glew library
# GLEW_DEFINITIONS   Definitions to use when compiling code that uses glew
#
# Typical usage could be something like:
#   find_package(GLEW REQUIRED)
#   include_directories(SYSTEM ${GLEW_INCLUDE_DIR})
#   add_definitions(${GLEW_DEFINITIONS})
#   ...
#   target_link_libraries(myexe ${GLEW_LIBRARIES})
#
# The following additional options can be defined before the find_package() call:
# GLEW_USE_STATIC_LIBS  Statically link against glew (default: OFF)
#
# Adapted from:
#	http://nvidia-texture-tools.googlecode.com/svn-history/r96/trunk/cmake/FindGLEW.cmake


set(GLEW_INCLUDE_DIR "${EMSCRIPTEN_ROOT_PATH}/system/include")
set(GLEW_LIBRARY   "nul")
set(GLEW_DEFINITIONS -DGL_GLEXT_PROTOTYPES)
set(GLEW_VERSION_STRING "1.10.0")
include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(GLEW REQUIRED_VARS GLEW_LIBRARY GLEW_INCLUDE_DIR
        VERSION_VAR GLEW_VERSION_STRING)
if(GLEW_FOUND)
    set(GLEW_LIBRARIES ${GLEW_LIBRARY})
endif(GLEW_FOUND)