# SPDX-License-Identifier: Apache-2.0

# Glob all .cpp files in this directory
file(GLOB UI_SOURCE *.cpp)

# Add the source files for this module
target_sources(app PRIVATE ${UI_SOURCE})

# Specify include directories if needed
target_include_directories(app PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

add_subdirectory(generated)
