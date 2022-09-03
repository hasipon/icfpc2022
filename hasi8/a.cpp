#include <cmath>
#include <iostream>
#include <vector>
#include <map>
#include <algorithm>
using namespace std;
int width = -1, height = -1;
uint8_t* Image;
uint8_t get(int i, int j, int k) {
	return Image[((height - 1 - i) * width + j) * 4 + k];
}
uint32_t get(int i, int j) {
	return *(uint32_t*)&Image[((height - 1 - i) * width + j) * 4];
}
double d(uint32_t x, uint32_t y) {
	int x0 = ((uint8_t*)&x)[0] - ((uint8_t*)&y)[0];
	int x1 = ((uint8_t*)&x)[1] - ((uint8_t*)&y)[1];
	int x2 = ((uint8_t*)&x)[2] - ((uint8_t*)&y)[2];
	int x3 = ((uint8_t*)&x)[3] - ((uint8_t*)&y)[3];
	double r = 0;
	r += x0*x0;
	r += x1*x1;
	r += x2*x2;
	r += x3*x3;
	return r;
}
pair<uint32_t, double> f(int i0, int i1, int j0, int j1) {
	vector<int> sum(4);
	for (int i = i0; i < i1; ++ i) for (int j = j0; j < j1; ++ j) for (int k = 0; k < 4; ++ k) {
		sum[k] += get(i,j,k);
	}
	uint8_t res[4];
	int area = (i1-i0)*(j1-j0);
	for (int k = 0; k < 4; ++ k) {
		res[k] = (sum[k] + area/2) / area;
	}
	double score = 0;
	for (int i = i0; i < i1; ++ i) for (int j = j0; j < j1; ++ j) score += sqrt(d(get(i,j), *(uint32_t*)res));
	for (;;) {
		for (int k = 0; k < 4; ++ k) {
			if (res[k] > 0) {
				-- res[k];
				double score2 = 0;
				for (int i = i0; i < i1; ++ i) for (int j = j0; j < j1; ++ j) score2 += sqrt(d(get(i,j), *(uint32_t*)res));
				if (score2 < score) {
					score = score2;
					goto next;
				}
				++ res[k];
			}
			if (res[k] < 255) {
				++ res[k];
				double score2 = 0;
				for (int i = i0; i < i1; ++ i) for (int j = j0; j < j1; ++ j) score2 += sqrt(d(get(i,j), *(uint32_t*)res));
				if (score2 < score) {
					score = score2;
					goto next;
				}
				-- res[k];
			}
		}
		break;
		next:;
	}
	return {*(uint32_t*)res, score};
}
void col(string blockId, uint32_t c) {
	if (c == 4294967295) return;
	uint8_t* x = (uint8_t*)&c;
	cout << "color ["<<blockId<<"] ["<<(int)x[0]<<","<<(int)x[1]<<","<<(int)x[2]<<","<<(int)x[3]<<"]"<<endl;
}
struct Hoge{
	int typ;
	int I0, I1, I2, J0, J1, J2;
	string blockId;
	uint32_t color;
	double score;
	vector<Hoge> child;
	Hoge(){}
	Hoge(string bid, int i0, int i1, int j0, int j1){
		typ = 1;
		I0 = i0;
		I1 = i1;
		J0 = j0;
		J1 = j1;
		blockId = bid;
		auto p = f(i0,i1,j0,j1);
		color = p.first;
		score = p.second*0.005 + 5.0*400*400/((i1-i0)*(j1-j0));
	}
	void cut(int i, int j) {
		if (typ != 1) throw 1;
		typ = 2;
		I2 = i;
		J2 = j;
		score = 10.0*400*400/((I1-I0)*(J1-J0));
		child.push_back(Hoge(blockId+".0", I0, i, J0, j));
		child.push_back(Hoge(blockId+".1", I0, i, j, J1));
		child.push_back(Hoge(blockId+".2", i, I1, j, J1));
		child.push_back(Hoge(blockId+".3", i, I1, J0, j));
		for (auto& x : child) score += x.score;
	}
	void output() {
		if (typ == 1) {
			col(blockId, color);
		} else if (typ == 2) {
			cout << "cut ["<<blockId<<"] ["<<J2<<","<<I2<<"]"<<endl;
			for (auto& x : child) x.output();
		}
	}
};
const int delta = 80;
Hoge walk(string bid, int i0, int i1, int j0, int j1, int depth) {
	Hoge root(bid, i0, i1, j0, j1);
	Hoge best = root;
	for (int i = i0+delta; i < i1; i += delta) {
		for (int j = j0+delta; j < j1; j += delta) {
			Hoge h = root;
			h.cut(i, j);
			if (h.score < best.score) {
				best = h;
			}
			if (depth == 0) {
				h.score = 10.0*400*400/((h.I1-h.I0)*(h.J1-h.J0));
				for (auto& x : h.child) {
					x = walk(x.blockId, x.I0, x.I1, x.J0, x.J1, depth+1);
					h.score += x.score;
				}
				if (h.score < best.score) {
					best = h;
				}
			}
		}
	}
	return best;
}
void solve() {
	if (width != 400) throw 1;
	if (height != 400) throw 1;
	auto best = walk("0", 0, 400, 0, 400, 0);
	cerr << best.score << endl;
	best.output();
}
int main() {
	char buf[1000], name[1000];
	if (!fgets(buf, sizeof(buf), stdin)) throw 1;
	for (;;) {
		if (!fgets(buf, sizeof(buf), stdin)) throw 1;
		if (string(buf) == "ENDHDR\n") break;
		if (string(buf) == "TUPLTYPE RGB_ALPHA\n") continue;
		int val;
		sscanf(buf, "%s %d", name, &val);
		auto n = string(name);
		if (n == "WIDTH") {
			width = val;
		} else if (n == "HEIGHT") {
			height = val;
		} else if (n == "DEPTH") {
			if (val != 4) throw 1;
		} else if (n == "MAXVAL") {
			if (val != 255) throw 1;
		}
	}
	Image = new uint8_t[width*height*4];
	if ((int)fread(Image, 1, width*height*4, stdin) != width*height*4) throw 1;
	solve();
}
