project (uvbook C)
cmake_minimum_required (VERSION 2.8)

set (CMAKE_C_FLAGS "${CMAKE_ENABLE_C99}") 
set_property (GLOBAL PROPERTY USE_FOLDERS ON)

if (MSVC)
    add_definitions (-DWIN32)
endif (MSVC)

include ("cmake/utils.cmake")
add_subdirectory (libuv)
add_subdirectory (code)
