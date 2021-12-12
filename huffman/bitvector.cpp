#include "bitvector.h"
#include <stdexcept>

bitvector::bitvector() : released(false), space(0), v(1 + sizeof(size_t)) {}

void bitvector::push(const std::vector<unsigned char> &one) {
	if (released) {
        throw std::runtime_error("Cannot push to released bitvector");
	}
	for (size_t i = 0; i < one.size(); i++) {
		if (space > 0) {
            space--;
		} else {
			v.push_back(0);
			space = 7;
		}
		v.back() = (v.back() << 1) | one[i];
	}
}

const std::vector<unsigned char> &bitvector::release() {
	if (!released) {
		released = true;
		v.back() <<= space;
		v[0] = static_cast<unsigned char>(space);
		size_t tmp = v.size();
        for (size_t i = 0; i < sizeof(size_t); i++) {
			v[sizeof(size_t) - i] = static_cast<unsigned char>(tmp & 255);
			tmp >>= 8;
		}
	}
	return v;
}

void bitvector::clear() {
	released = false;
	space = 0;
	v.resize(1 + sizeof(size_t), 0);
}


bitreader::bitreader(int space, const std::vector<unsigned char> &one) : pos(0), bit(8), last_space(space), v(one) {}

bool bitreader::ended() const {
	return !(pos + 1 < v.size() || (pos + 1 == v.size() && bit > last_space));
}

unsigned char bitreader::next() { //unsafe
	//if (ended()) {
	//	throw std::runtime_error("No elements left at the bitreader");
	//}
	bit--;
	unsigned char tmp = static_cast<unsigned char>((v[pos] & (1 << bit)) >> bit);
	if (bit == 0) {
		bit = 8;
		pos++;
	}
	return tmp;
}

std::vector<unsigned char> bitreader::to_vector() {
	std::vector<unsigned char> tmp;
	while (!ended()) {
		tmp.push_back(next());
	}
	return tmp;
}
