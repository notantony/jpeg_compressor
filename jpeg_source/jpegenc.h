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
                                  const unsigned short HTDC[256][2], const unsigned short HTAC[256][2], int usebitEOB,
                                  std::vector<bool> &bitSeq);

extern int my_processDU_Analyse(const std::vector<int> &data, int nDC, std::ofstream &dcData, std::ofstream &acData, int usebitEOB);

extern void jo_writeBits(FILE *fp, int &bitBuf, int &bitCnt, const unsigned short *bs);

extern void jo_calcBits(int val, unsigned short bits[2]);

#endif //JPEG_COMPRESSOR_JPEGENC_H
