//
// Created by Anton on 12.12.2021.
//

#ifndef JPEG_COMPRESSOR_HUF_MAIN_H
#define JPEG_COMPRESSOR_HUF_MAIN_H


extern void compress(std::ifstream &input, std::ofstream &output);
extern void decompress(std::ifstream &input, std::ofstream &output);
extern std::vector<std::pair<unsigned short, unsigned short>> getCodes(std::istream &input);

#endif //JPEG_COMPRESSOR_HUF_MAIN_H
