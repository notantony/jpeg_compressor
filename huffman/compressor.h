#ifndef COMPRESSOR_H
#define COMPRESSOR_H

#include <fstream>
#include <iostream>
#include <exception>
#include <algorithm>
#include <memory>
#include <queue>
#include <list>
#include "bitvector.h"


class compressor {
private:
	struct node {
		int l, r, leaf;
		node(int l, int r) : l(l), r(r), leaf(-1) {}
		node(int x) : l(-1), r(-1), leaf(x) {};
	};

	bool mode_c = true;
	int global = 0;//variable used for walk

	std::vector<node> tree;
	std::vector<unsigned char> coded_tree;//tree 0-1 code
	std::vector<unsigned char> leaves;//chars at positions determined by walk 
	std::vector<unsigned char> codes[256];//storage while compressing

	void build_tree_c(std::vector<uint64_t> &cnt);
	void walk_c(int x, std::vector<unsigned char> &cur);

	void walk_d(size_t x, int &index);
public:
	compressor(std::vector<uint64_t> &cnt);//compressing constructor
	const std::vector<unsigned char> &get_tree_code() const;
	const std::vector<unsigned char> &get_leaves_code() const;
	const std::vector<unsigned char> &get_code(unsigned char i) const;//can be called only when compressing

	compressor(bitreader one, const std::vector<unsigned char> &leaves);//decompressing constructor
	unsigned char get_char(bitreader &br) const;//can be called only when decompressing

};


#endif