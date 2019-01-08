cmake_minimum_required (VERSION 2.6)

# Creatng a project
project (ldws)

# Versioning
set (ldws_VERSION_MAJOR 1)
set (ldws_VERSION_MINOR 0)

# Link to OpenCV
find_package(OpenCV REQUIRED COMPONENTS core imgproc highgui)

# Add source files
set(SOURCES src/Main.cpp src/PreProcessing.cpp src/Processing.cpp)

# Create an executable
add_executable(ldws_exec ${SOURCES})

# Link to OpenCV libararies
target_link_libraries(ldws_exec ${OpenCV_LIBS})

# Copy images
add_custom_command(
        TARGET ldws_exec POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
                ${CMAKE_SOURCE_DIR}/images
                ${CMAKE_CURRENT_BINARY_DIR})
