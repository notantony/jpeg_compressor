cmake_minimum_required(VERSION 3.0)
project(jpeg_compressor)

set(CMAKE_CXX_STANDARD 11)

add_library(JpegSource
        jpeg_source/jpegdec.cpp jpeg_source/jpegenc.cpp)

target_link_libraries(JpegSource)

add_executable(jpeg_encoder main.cpp)
add_executable(jpec_decoder main2.cpp)
