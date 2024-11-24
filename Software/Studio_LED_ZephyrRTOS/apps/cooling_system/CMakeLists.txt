# Glob all .cpp files in this directory
file(GLOB COOLING_SYSTEM_SOURCE *.cpp)

# Add the source files for this module
target_sources(app PRIVATE ${COOLING_SYSTEM_SOURCE})

# Specify include directories if needed
target_include_directories(app PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})