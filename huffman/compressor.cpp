#include "compressor.h"

class cmp {
	std::vector<uint64_t> *cnt;
	int pos = 256;
public:
	cmp(std::vector<uint64_t> *cnt) : cnt(cnt) {}
	bool operator() (int a, int b) {
		return (*cnt)[a] > (*cnt)[b];
	}
};



compressor::compressor(std::vector<uint64_t> &cnt) {
	int nz = 0;
	for (int i = 0; i < cnt.size(); ++i) {
		if (cnt[i] != 0) {
			++nz;
		}
	}
	leaves = std::vector<unsigned char> (nz);

	build_tree_c(cnt);
    std::vector<unsigned char> tmp;
    walk_c(leaves.size() + leaves.size() - 1 - 1, tmp);
}

const std::vector<unsigned char> &compressor::get_tree_code() const {
	return coded_tree;
}
const std::vector<unsigned char> &compressor::get_leaves_code() const {
	return leaves;
}

void compressor::build_tree_c(std::vector<uint64_t> &cnt) {
	std::priority_queue<int, std::vector<int>, cmp> q(&cnt);
	for (int i = 0; i < leaves.size(); i++) {
		q.push(i);
		tree.push_back(node(i));
	}
	for (int i = 0; i + 1 < leaves.size(); i++) {
		int a = q.top();
		q.pop();
		int b = q.top();
		q.pop();
//		std::cerr << "Node,l, r: " << leaves.size() + i << " " << a << " " << b << std::endl;
		cnt[leaves.size() + i] = cnt[a] + cnt[b];
		q.push(leaves.size() + i);
		tree.push_back(node(a, b));
	}
}
void compressor::walk_c(int x, std::vector<unsigned char> &cur) {
//	std::cerr << x << std::endl;
	if (x < leaves.size()) {
		leaves[global] = x;
		tree[x].leaf = global++;
		coded_tree.push_back(0);//up
		codes[x] = cur;
		return;
	}
	coded_tree.push_back(1);//down
	cur.push_back(0);
	walk_c(tree[x].l, cur);
	cur.back() = 1;
	walk_c(tree[x].r, cur);
	cur.pop_back();
}

const std::vector<unsigned char> &compressor::get_code(unsigned char i) const {
	if (!mode_c) {
		throw std::runtime_error("Cannot get leaf code in decompresor mode");
	}
	return codes[i];
}

compressor::compressor(bitreader one, const std::vector<unsigned char> &leaves) : mode_c(false), tree(1, node(-1)), coded_tree(one.to_vector()), leaves(leaves) {
	int tmp = 0;
	walk_d(0, tmp);
}

void compressor::walk_d(size_t x, int &index) {
	if (coded_tree[index] == 0) {
		tree[index].leaf = global++;
		index++;
		return;
	}
	tree[x].l = static_cast<int>(tree.size());
	tree.push_back(node(-1));
	index++;
	walk_d(tree.size() - 1, index);

	tree[x].r = static_cast<int>(tree.size());
	tree.push_back(node(-1));
	walk_d(tree.size() - 1, index);
}

unsigned char compressor::get_char(bitreader &br) const {
	int cur = 0;
	while (tree[cur].l != -1) {
		if (!br.ended()) {
			if (!br.next()) {
				cur = tree[cur].l;
			} else {
				cur = tree[cur].r;
			}
		} else {
			throw std::runtime_error("Error: input file is corrupted");
		}
	}
	return leaves[tree[cur].leaf];
}
