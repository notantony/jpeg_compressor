//
// Created by Anton on 12.12.2021.
//

#ifndef JPEG_COMPRESSOR_JPEGENC_H
#define JPEG_COMPRESSOR_JPEGENC_H

#include <vector>
extern void
my_processDU(const std::vector<int> &data, FILE *fp, int &bitBuf, int &bitCnt, const unsigned short HTDC[256][2],
             const unsigned short HTAC[256][2]);

extern void my_processDU_Triangle(const std::vector<int> &data, int nDC, FILE *fp, int &bitBuf, int &bitCnt,
                                  const unsigned short HTDC[256][2], const unsigned short HTAC[256][2], int usebitEOB);

extern int my_processDU_Analyse(const std::vector<int> &data, std::ofstream &dcData, std::ofstream &acData);

extern void jo_writeBits(FILE *fp, int &bitBuf, int &bitCnt, const unsigned short *bs);

#endif //JPEG_COMPRESSOR_JPEGENC_H
