# Glob all .cpp files in this directory
file(GLOB UI_GENERATED_SOURCE *.c)

# Specify the file to exclude
set(EXCLUDED_FILE "ui_events.c")

# Remove the excluded file from the list
list(REMOVE_ITEM UI_GENERATED_SOURCE ${CMAKE_CURRENT_SOURCE_DIR}/${EXCLUDED_FILE})

# Add the source files for this module
target_sources(app PRIVATE ${UI_GENERATED_SOURCE})

# Specify include directories if needed
target_include_directories(app PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})