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
        val += 126;
        if (val >= 0 && val < UCHAR_MAX) {
            file << (unsigned char) val;
        } else {
            cerr << val << '\n';

//            val += 127;
//            if (!(val >= 0 && val <= UCHAR_MAX + 255)) {
//                cerr << val;
//                throw runtime_error("Unexpected value\n");
//            }
//            file << (unsigned char) 255 << (unsigned char) 255 << (unsigned char) val - 255;
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

vector<unsigned char> getCompressedBytes(const string &filename);

vector<unsigned char> compressHuffman(const vector<unsigned char> &data) {
    string tmpName = "tmp_huffman.bin";
    FILE *tmp = fopen(tmpName.c_str(), "wb"); // TODO;
    for (const auto &c: data) {
        fputc(c, tmp);
    }
    fclose(tmp);

    ifstream tmpStream = ifstream(tmpName, ios_base::binary);
    vector<pair<unsigned short , unsigned short >> codes = getCodes(tmpStream);
//    for (int i = 0; i < 256; ++i) {
//        cout << "VALUE: " << i << " CODE: " << codes[i].first << " LEN: " << codes[i].second << "\n";
//    }
    cout << "0 CODE_LEN: " << getCodes(tmpStream)[0].second << "\n";
    tmpStream.close();

    vector<unsigned char> result = getCompressedBytes(tmpName);
    remove(tmpName.c_str());
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

int ZERO_SEQ_MARKER = 256;
int ZERO_SEQ2_MARKER = 257;

vector<int> foldZeros(const vector<int> &data, int blockSize = -1) {
    vector<int> result;
//    if (blockSize != -1 && data.size() % blockSize != 0) {
//        throw exception("Data do not divide on block size");
//    }
    for (int i = 0; i < data.size(); ++i) {
        if (i + 2 < data.size() && data[i] == 0 && data[i + 1] == 0 && data[i + 2] == 0) {
            int seqLen = 0;
            while (i < data.size() && data[i] == 0) {
                ++seqLen;
                ++i;
            }
            --i;
            if (seqLen < 255) {
                result.push_back(ZERO_SEQ_MARKER);
                result.push_back(seqLen);
            } else if (seqLen >= 255) {
                result.push_back(ZERO_SEQ2_MARKER);
                result.push_back(seqLen - 255);
            }
//            cerr << "ZeroSeqLen";
//            throw exception("zeroSeqLen");
        } else {
            result.push_back(data[i]);
        }
    }
    return result;
}

int MARKERS[2] {ZERO_SEQ_MARKER, ZERO_SEQ2_MARKER};




int DC_SPECIAL_CNT = 1;
int DC_DIFF_CNT = 1;

vector<int> zerosSeq;

vector<unsigned char> codeChannel(vector<vector<int>> &blocks) {
    vector<int> DCVector;
    vector<int> ACVector;

    for (int i = 0; i < blocks.size(); ++i) {
        vector<int> &block = blocks[i];
        for (int DCIndex = 0; DCIndex < DC_SPECIAL_CNT; ++DCIndex) {
            int prevDC;
            if (DCIndex == 0) {
                prevDC = 0;
            } else {
                prevDC = blocks[DCIndex][DCIndex];
            }
            DCVector.push_back(block[DCIndex] - prevDC);
//            if (DCIndex < DC_DIFF_CNT) {
//            } else {
//                DCVector[DCIndex].push_back(block[DCIndex]);
//            }
//        }
        }
    }
    return vector<unsigned char >();
}

extern vector<unsigned char> myCompress(vector<vector<int>> &blocks) {

//    cout << "Mapping values\n";
//    for (auto &block: blocks) {
//        for (auto &val: block) {
//            for (int marker: MARKERS) {
//                if (val == marker * 10) {
//                    throw exception("256 * 10");
//                }
//                if (val == marker) {
//                    val *= 10;
//                }
//            }
//        }
//    }


    cout << "Group blocks by channel\n";
    vector<vector<int>> CrBlocks;
    vector<vector<int>> CbBlocks;
    vector<vector<int>> YBlocks;
    for (int blockI = 0; blockI < blocks.size(); ++blockI) {
        if (blockI % 6 == 4) {
            CrBlocks.push_back(blocks[blockI]);
        } else if (blockI % 6 == 5) {
            CbBlocks.push_back(blocks[blockI]);
        } else {
            YBlocks.push_back(blocks[blockI]);
        }
    }


    vector<int> ACs;
    int DCCount = 3;
    vector<vector<int>> DCVectors(DCCount);

    int blockIndex = 0;
    for (const auto &block: blocks) {
        for (int DCIndex = 0; DCIndex < DCCount; ++DCIndex) {
            int prevDC;
            if (blockIndex == 0 || blockIndex == 4 || blockIndex == 5) {
                prevDC = 0;
            } else {
                prevDC = blocks[getPrevBlock(blockIndex)][DCIndex];
            }
            if (DCIndex < 3) {
                DCVectors[DCIndex].push_back(block[DCIndex] - prevDC);
            } else {
                DCVectors[DCIndex].push_back(block[DCIndex]);
            }
        }
//        cerr << "Out\n";
//        cerr << "In -";
        for (int i = DCCount; i < block.size(); ++i) {
            ACs.push_back(block[i]);
        }
//        cerr << "Out\n";
        ++blockIndex;

//        int lastNonzeroI;
//        for (lastNonzeroI = block.size() - 1; lastNonzeroI > DCCount; --lastNonzeroI) {
//            if (block[lastNonzeroI] != 0) {
//                break;
//            }
//        }
//
//        for (int i = DCCount; i <= lastNonzeroI; ++i) {

    }

    for (int i = 0; i < DCCount; ++i) {
        DCVectors[i] = foldZeros(DCVectors[i]);

        cerr << "DC_cf_" << i << ":\n";
        for (int j = 0; j < 100; ++j) {
            cerr << DCVectors[i][j] << ' ';
        }
        cerr << '\n';
    }

    vector<unsigned char> result;
    for (int i = 0; i < DCCount; ++i) {
        string DCITmpFilename = "./tempFileDC";
        DCITmpFilename += to_string(i) + ".bin.tmp";

        writeToFile(DCVectors[i], DCITmpFilename);

        for (unsigned char c: getCompressedBytes(DCITmpFilename)) {
            result.push_back(c);
        }
    }


    cerr << "In3 - ";
    ACs = foldZeros(ACs);
    cerr << "Out\n";

    string ACTmp = "./tempFileAC.bin.tmp";
    writeToFile(ACs, ACTmp);
    for (unsigned char c: getCompressedBytes(ACTmp)) {
        result.push_back(c);
    }
    return result;
}


vector<vector<int>> myDecompress(const vector<unsigned char> &compressed) {
    return vector<vector<int>>();
}