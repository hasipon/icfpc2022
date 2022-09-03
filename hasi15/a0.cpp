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
int f(const vector<vector<int>>& a, int i0, int i1, int j0, int j1) {
	map<uint32_t, int> cnt;
	for (int i = i0; i <= i1; ++ i) {
		for (int j = j0; j <= j1; ++ j) {
			++ cnt[a[i][j]];
		}
	}
	int r = -1;
	bool ok = true;
	for (auto x : cnt) {
		if (x.second >= 25) {
			if (r == -1) {
				r = x.first;
			} else {
				ok = false;
				r = -1;
				break;
			}
		} else if (x.second >= 5) {
			ok = false;
		}
	}
	if (!ok) return -1;
	return r;
}
void col(string blockId, uint32_t c) {
	uint8_t* x = (uint8_t*)&c;
	cout << "color ["<<blockId<<"] ["<<(int)x[0]<<","<<(int)x[1]<<","<<(int)x[2]<<","<<(int)x[3]<<"]"<<endl;
}
void solve() {
	if (width != 400) throw 1;
	if (height != 400) throw 1;
	vector<vector<int>> aa(400, vector<int>(400,2));
	for (int i = 0; i < 400; ++ i) {
		for (int j = 0; j < 400; ++ j) {
			auto c = get(i,j);
			vector<pair<double, int>> a;
			a.push_back({d(c, 4278190080), 0});
			a.push_back({d(c, 4294967295), 1});
			a.push_back({d(c, 4289546752), 2});
			aa[i][j] = min_element(a.begin(), a.end())->second;
		}
	}
	vector<vector<uint32_t>> bb(400, vector<uint32_t>(400,4289546752));
	col("0", 4289546752);
	string blockId = "0";
	vector<vector<int>> blocks;
	map<vector<int>, int> colmap;
	map<int,int> cntI, cntJ;
	for (int i = 60; i < 400; i += 40) {
		for (int j = 20; j < 360; j += 40) {
			int I0 = i-5;
			int I1 = i+5;
			int J0 = j-5;
			int J1 = j+5;
			int c = f(aa, I0, I1, J0, J1);
			if (c == 2) continue;
			for (;;) {
				bool cont = false;
				if (I0 > 0) {
					--I0;
					if (f(aa, I0, I1, J0, J1) != c) {
						++ I0;
					} else cont = true;
				}
				if (I1 < 399) {
					++I1;
					if (f(aa, I0, I1, J0, J1) != c) {
						-- I1;
					} else cont = true;
				}
				if (J0 > 0) {
					--J0;
					if (f(aa, I0, I1, J0, J1) != c) {
						++ J0;
					} else cont = true;
				}
				if (J1 < 399) {
					++J1;
					if (f(aa, I0, I1, J0, J1) != c) {
						-- J1;
					} else cont = true;
				}
				if (!cont) break;
			}
			uint32_t cc = c == 0 ? 4278190080 : 4294967295;
			for (int i = I0; i <= I1; ++ i) {
				for (int j = J0; j <= J1; ++ j) {
					bb[i][j] = cc;
				}
			}
			vector<int> bl {I0, I1, J0, J1};
			blocks.push_back(bl);
			colmap[bl] = c;
			++cntI[I0];
			++cntI[I1+1];
			++cntJ[J0];
			++cntJ[J1+1];
		}
	}
	for (auto x : cntI) cout << x.first << " " << x.second << endl;
	cout << endl;
	for (auto x : cntJ) cout << x.first << " " << x.second << endl;
	/*
	double score = 0;
	for (int i = 0; i < 400; ++ i) {
		for (int j = 0; j < 400; ++ j) {
			score += sqrt(d(bb[i][j], get(i, j)));
		}
	}
	cerr << "score = " << score*0.005 << endl;
	*/
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
