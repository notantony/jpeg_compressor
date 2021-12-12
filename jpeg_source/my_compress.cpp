//
// Created by Anton on 12.12.2021.
//

#include "my_compress.h"


void writeToFile(const vector<int> &v, const string &filename) {
    ofstream file = ofstream(filename, std::ios_base::binary);

    if (!file.is_open()) {
        throw runtime_error("Cannot open create temporary file\n");
    }

    for (int val: v) {
        val += 128;
        if (val >= 0 && val < UCHAR_MAX) {
            file << (unsigned char) val;
        } else {
            val += 32000;
            if (!(val >= 0 && val <= USHRT_MAX)) {
                cerr << val;
                throw runtime_error("Unexpected value\n");
            }
            file << 255U << (unsigned short) val;
        }
    }
}

vector<unsigned char> readBytes(const string &filename) {
    ifstream inputFile(filename, std::ios_base::binary);
    vector<char> data = std::vector<char> ( std::istreambuf_iterator<char>(inputFile),
                              std::istreambuf_iterator<char>() );
    vector<unsigned char> result;
    for (char c: data) {
        result.push_back(c);
    }
    return result;
}

vector<unsigned char> getCompressedBytes(const string &filename) {

    string compressedFilename = filename;
    compressedFilename += ".compressed";

    ifstream inputFile = ifstream(filename, std::ios_base::binary);

    ofstream outputFile = ofstream(compressedFilename, std::ios_base::binary);
    if (!inputFile.is_open() || !outputFile.is_open()) {
        throw runtime_error("IO ERROR\n");
    }

    compress(inputFile, outputFile);

    inputFile.close();
    outputFile.close();

    ifstream compressedFile = ifstream(compressedFilename, std::ios_base::binary);

    return readBytes(compressedFilename);
}

int getPrevBlock(int i) {
    if (i % 6 == 0 || i % 6 == 4 || i % 6 == 5) {
        return i - 6;
    }
    return i - 1;
}

vector<unsigned char> myCompress(const vector<vector<int>> &blocks) {
    vector<int> DCs;
    vector<int> ACs;

    vector<int> DC1s;
    vector<int> DC2s;


    int prevDC, prevDC1, prevDC2;
    int blockIndex = 0;
    for (const auto &block: blocks) {
        if (blockIndex == 0 || blockIndex == 4 || blockIndex == 5) {
            prevDC = 0;
        } else {
            prevDC = blocks[getPrevBlock(blockIndex)][0];
        }
        DCs.push_back(block[0] - prevDC);

        if (blockIndex == 0 || blockIndex == 4 || blockIndex == 5) {
            prevDC1 = 0;
        } else {
            prevDC1 = blocks[getPrevBlock(blockIndex)][1];
        }
        DC1s.push_back(block[1] - prevDC1);

        if (blockIndex == 0 || blockIndex == 4 || blockIndex == 5) {
            prevDC2 = 0;
        } else {
            prevDC2 = blocks[getPrevBlock(blockIndex)][2];
        }
        DC2s.push_back(block[2] - prevDC2);

        int lastNonzeroI;
        for (lastNonzeroI = block.size() - 1; lastNonzeroI > 1; --lastNonzeroI) {
            if (block[lastNonzeroI] != 0) {
                break;
            }
        }
        for (int i = 1; i <= lastNonzeroI; ++i) {
            ACs.push_back(block[i]);
        }
        ++blockIndex;
    }
    for (int i = 0; i < 100; ++i) {
        cerr << DCs[i] << ' ';
    }
    cerr << '\n';
    for (int i = 0; i < 100; ++i) {
        cerr << DC1s[i] << ' ';
    }
    cerr << '\n';
    for (int i = 0; i < 100; ++i) {
        cerr << DC2s[i] << ' ';
    }
    cerr << '\n';

    string DCTmp = "./tempFileDC.bin.tmp";
    string DC1Tmp = "./tempFileDC1.bin.tmp";
    string DC2Tmp = "./tempFileDC2.bin.tmp";
    string ACTmp = "./tempFileAC.bin.tmp";

    writeToFile(DCs, DCTmp);
    writeToFile(DC1s, DC1Tmp);
    writeToFile(DC2s, DC2Tmp);

    writeToFile(ACs, ACTmp);

    vector<unsigned char> a = getCompressedBytes(DCTmp);
    for (unsigned char c: getCompressedBytes(ACTmp)) {
        a.push_back(c);
    }
//    for (unsigned char c: getCompressedBytes(DC1Tmp)) {
//        a.push_back(c);
//    }
//    for (unsigned char c: getCompressedBytes(DC2Tmp)) {
//        a.push_back(c);
//    }
    return a;
}


vector<vector<int>> myDecompress(const vector<unsigned char> &compressed) {
    return vector<vector<int>>();
}