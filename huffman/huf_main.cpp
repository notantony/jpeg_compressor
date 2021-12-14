#include <fstream>
#include <iostream>
#include <cstring>
#include <algorithm>
#include "bitvector.h"
#include "compressor.h"
#include <stdexcept>
#include <ctime>
#include <iomanip>

const size_t BUFFER_SIZE = 4096 * 8;
static unsigned char buffer[BUFFER_SIZE];

void test(std::ifstream &input) {
    input.get();
	if(!input.eof())input.unget();
}

void input_check(std::ifstream &input) {
	if (input.bad()) {
		throw std::runtime_error("Error while reading input file");
	} else if (input.fail()) {
		throw std::runtime_error("Error: input file is corrupted");
	}
}

void my_writev(std::ofstream &output, const std::vector<unsigned char> &tmp) {
    for (size_t i = 0; i < tmp.size();) {
        size_t j = 0;
		for (; j < BUFFER_SIZE && i < tmp.size(); j++, i++) {
			buffer[j] = tmp[i];
		}
		if (output.good()) {
			output.write(reinterpret_cast<char*>(buffer), j);
		}
		if (output.fail()) {
			throw std::runtime_error("Error while writing compressed file");
		}
	}
}

bitreader my_readv(std::ifstream &input) {
	char c;
	int first;
	input.get(c);
	input_check(input);
	first = reinterpret_cast<unsigned char&>(c);
	size_t len = 0;
    for (size_t i = 0; i < sizeof(size_t); i++) {
		input.get(c);
		input_check(input);
		len = (len << 8) | reinterpret_cast<unsigned char&>(c);
	}
	len -= sizeof(size_t) + 1;
	std::vector<unsigned char> tmp;
	while (len > 0) {
		input.read(reinterpret_cast<char*>(buffer), std::min(sizeof(buffer), len));
		input_check(input);
		len -= input.gcount();
		for (int i = 0; i < input.gcount(); i++) {
			tmp.push_back(buffer[i]);
		}
	}
	return bitreader(first, tmp);
}

extern std::vector<std::pair<unsigned short, unsigned short>> getCodes(std::istream &input) {
    std::vector<uint64_t> cnt(256 + 255, 0);
    cnt.shrink_to_fit();
    while (input.good()) {
        input.read(reinterpret_cast<char*>(buffer), sizeof(buffer));
        for (int i = 0; i < input.gcount(); i++) {
            cnt[buffer[i]]++;
        }
    }
    if (input.bad()) {
        throw std::runtime_error("Error while reading input file");
    }
    compressor compressor(cnt);


//    my_writev(output, compressor.get_leaves_code());
    bitvector bv;
    bv.push(compressor.get_tree_code());
//    my_writev(output, bv.release());
    bv.clear();

    input.clear();
    input.seekg(0, input.beg);
    std::vector<unsigned char> tmp;
//    test(input);
    std::vector<std::pair<unsigned short, unsigned short>> result;
    for (int c = 0; c <= UCHAR_MAX; ++c) {
        std::vector<unsigned char> code = compressor.get_code((unsigned char)c);
        short unsigned codeVal = 0;
        for (int i = 0; i < code.size(); ++i) {
            codeVal *= 2;
            codeVal += code[i];
        }
        result.push_back(std::make_pair(codeVal, (short unsigned)code.size()));
    }
    return result;
}


extern void compress(std::ifstream &input, std::ofstream &output) {
	std::vector<uint64_t> cnt(256 + 255, 0);
	cnt.shrink_to_fit();
	while (input.good()) {
		input.read(reinterpret_cast<char*>(buffer), sizeof(buffer));
		for (int i = 0; i < input.gcount(); i++) {
			cnt[buffer[i]]++;
		}
	}
	if (input.bad()) {
		throw std::runtime_error("Error while reading input file");
	}
	compressor compressor(cnt);

	double progress_ttl = 0, progress_size = 0, total_cnt = 0;
	for (size_t i = 0; i < 256; i++) {
		total_cnt += cnt[i];
		progress_ttl += cnt[i];
		progress_size += compressor.get_code(i).size() * cnt[i];
	}
	double entopy = 0;
	for (size_t i = 0; i < 256; i++) {
		entopy += cnt[i] / total_cnt * compressor.get_code(i).size();
	}
	std::cout << std::setprecision(4);
	std::cout << "Avg len: " << entopy << "\n";

    std::cout << "Approximate ouput file size is " << progress_size / 8 / 1024 << " kB\n";

	my_writev(output, compressor.get_leaves_code());
	bitvector bv;
	bv.push(compressor.get_tree_code());
	my_writev(output, bv.release());
	bv.clear();

	input.clear();
	input.seekg(0, input.beg);
	std::vector<unsigned char> tmp;
	test(input);
	while (input.good()) {
		input.read(reinterpret_cast<char*>(buffer), sizeof(buffer));
		for (int i = 0; i < input.gcount(); i++) {
			bv.push(compressor.get_code(buffer[i]));
		}
		my_writev(output, bv.release());
		bv.clear();
		test(input);
	}
	if (input.bad()) {
		throw std::runtime_error("Error while reading input file");
	}
}

extern void decompress(std::ifstream &input, std::ofstream &output) {
	input.read(reinterpret_cast<char*>(buffer), 256);
	input_check(input);
	char check[256];
	memset(check, 0, 256);
	std::vector<unsigned char> tmp(256);
	for (int i = 0; i < 256; i++) {
		tmp[i] = buffer[i];
		if (check[buffer[i]] != 0) {
			throw std::runtime_error("Error: input file is corrupted");
		}
		check[buffer[i]] = 1;
	}
	compressor compressor(my_readv(input), tmp);
	test(input);
	while (input.good()) {
		bitreader br = my_readv(input);
		tmp.clear();
		while (!br.ended()) {
			tmp.push_back(compressor.get_char(br));
		}
		my_writev(output, tmp);
		test(input);
	}
	if (input.bad()) {
		throw std::runtime_error("Error while reading input file");
	}
}



int main_huf(int n, char* args[]) {
#ifndef CODER
	char *mode_str = "c";
#else
	char *mode_str = "d";
#endif


	try {
		double start_time = clock();
        if (n == 2 && strcmp(args[1], "help") == 0) {
            std::cout << "Format: \"<c/d> <input filename> <output filename>\" \nUse \"c\" for compressing, \"d\" for decompressing";
        }
		if (n != 3) {
			std::cout << "Wrong number of arguments\nUsage: \"program.exe <input filename> <output filename>\" \n";
			return 1;
		}
		int mode = -1;

		if (strcmp(mode_str, "c") == 0) {
			mode = 1;
		} else {
			mode = 2;
		}

		std::ifstream input(args[1], std::ios_base::binary);
		if (!input.is_open()) {
			std::cout << "Cannot open file for reading\n";
			return 1;
		}
		std::ofstream output(args[2], std::ios_base::binary);
		if (!output.is_open()) {
			std::cout << "Cannot open file for writing\n";
			return 1;
		}	

		if (mode == 1) {
			compress(input, output);
		} else if (mode == 2) {
			decompress(input, output);
		}

		std::cout << "Done successfully, elapsed time: " << (static_cast<double>(clock()) - start_time) / static_cast<double>(CLOCKS_PER_SEC) << " seconds\n" ;
	} catch (std::runtime_error e){
		std::cout << e.what();
	}
	return 0;
}

