//
// Created by Anton on 12.12.2021.
//

#ifndef JPEG_COMPRESSOR_MY_COMPRESS_H
#define JPEG_COMPRESSOR_MY_COMPRESS_H

#include <vector>
#include <fstream>
#include "../huffman/huf_main.h"
#include <iostream>

using namespace std;

class my_compress {

};


extern vector<unsigned char> myCompress(const vector<vector<int>> &blocks);

#endif //JPEG_COMPRESSOR_MY_COMPRESS_H
