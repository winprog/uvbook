project (uvbook)
cmake_minimum_required (VERSION 2.8)

set_property (GLOBAL PROPERTY USE_FOLDERS ON)

include ("cmake/utils.cmake")
add_subdirectory (libuv)
add_subdirectory (code)
