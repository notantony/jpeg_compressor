//
// Created by Anton on 12.12.2021.
//

#ifndef JPEG_COMPRESSOR_JPEGENC_H
#define JPEG_COMPRESSOR_JPEGENC_H

#include <vector>
extern int my_processDU(const std::vector<int> &data, FILE *fp, int &bitBuf, int &bitCnt, int DC, const unsigned short HTDC[256][2], const unsigned short HTAC[256][2]);


extern void jo_writeBits(FILE *fp, int &bitBuf, int &bitCnt, const unsigned short *bs);

#endif //JPEG_COMPRESSOR_JPEGENC_H
