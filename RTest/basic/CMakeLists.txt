cmake_minimum_required(VERSION 2.8)
project(recognizer)

find_package(OpenCV REQUIRED)
aux_source_directory(./ SOURCE_DIR)
add_executable(recognizer ${SOURCE_DIR})
target_link_libraries(recognizer ${OpenCV_LIBS})
include_directories (/usr/include/tesseract)
target_link_libraries (recognizer tesseract)