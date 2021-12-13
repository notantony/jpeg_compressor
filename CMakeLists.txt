cmake_minimum_required(VERSION 3.0)
project(jpeg_compressor)

set(CMAKE_CXX_STANDARD 11)

add_library(JpegSource
        jpeg_source/jpegdec.cpp jpeg_source/jpegenc.cpp jpeg_source/jpegenc.h jpeg_source/my_compress.cpp jpeg_source/my_compress.h)

add_library(huffman
        huffman/bitvector.h huffman/bitvector.cpp huffman/compressor.h huffman/compressor.cpp huffman/huf_main.cpp huffman/huf_main.h)

add_executable(jpeg_encoder main.cpp)
add_executable(jpeg_decoder main2.cpp)

target_link_libraries(jpeg_decoder JpegSource huffman)
target_link_libraries(jpeg_encoder JpegSource huffman)