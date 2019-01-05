cmake_minimum_required (VERSION 2.6)
project (ldws)
# The version number.
set (ldws_VERSION_MAJOR 1)
set (ldws_VERSION_MINOR 0)


find_package(OpenCV REQUIRED COMPONENTS core imgproc highgui)

#Bring the headers, such as Student.h into the project
# include_directories(include)
 
#Can manually add the sources using the set command as follows:
#set(SOURCES src/mainapp.cpp src/Student.cpp)
 
#However, the file(GLOB...) allows for wildcard additions:
set(SOURCES src/Main.cpp src/PreProcessing.cpp src/Processing.cpp)
 
add_executable(ldws_exec ${SOURCES})
target_link_libraries(ldws_exec ${OpenCV_LIBS})
# add_custom_target(ldws ALL DEPENDS "${CMAKE_CURRENT_SRC_DIR}/images/ProcessedFrame.PNG")

add_custom_command(
        TARGET ldws_exec POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
                ${CMAKE_SOURCE_DIR}/images
                ${CMAKE_CURRENT_BINARY_DIR})
