#ifndef BITVECTOR_H
#define BITVECTOR_H

#include <vector>
#include <cstddef>


class bitvector {
private:
	bool released = false;
	int space;
	std::vector<unsigned char> v;
public:
	void push(const std::vector<unsigned char> &one);
	const std::vector<unsigned char> &release();
	void clear();
    bitvector();
};

class bitreader {
private:
    size_t pos;
    int bit, last_space;
	std::vector<unsigned char> v;
public:
	bitreader(int space, const std::vector<unsigned char> &one);
	unsigned char next();
	bool ended() const;
	std::vector<unsigned char> to_vector();
};


#endif
