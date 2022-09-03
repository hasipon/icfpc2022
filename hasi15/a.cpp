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
map<pair<int,int>,uint32_t> colmap;
vector<int> Ipos = {43,83,122,162,202,241,281,320,360,400};
vector<int> Jpos = {0,40,79,119,159,198,238,277,317,357};
uint32_t Cols[] = {4278190080, 4294967295};
void cut0(string blockId, int i0) {
	auto c1 = colmap[{i0,40}];
	cout << "cut ["<<blockId<<"] [x] [277]" << endl;
	col(blockId+".1", c1);
	cout << "cut ["<<blockId<<".0] [x] [238]" << endl;
	cout << "cut ["<<blockId<<".0.0] [x] [119]" << endl;
	col(blockId+".0.0.1", c1);
	cout << "cut ["<<blockId<<".0.0.0] [x] [40]" << endl;
	cout << "cut ["<<blockId<<".0.0.0.1] [x] [79]" << endl;
	cout << "cut ["<<blockId<<".0.0.1] [x] [159]" << endl;
	cout << "cut ["<<blockId<<".0.0.1.1] [x] [198]" << endl;
	cout << "swap ["<<blockId<<".0.0.0.1.0] ["<<blockId<<".0.0.1.1.0]" << endl;
}
void cut1(string blockId, int i0, int i1) {
	vector<int> ii;
	for (int i : Ipos) if (i0 <= i && i <= i1) ii.push_back(i);
	for (int p = 1; p < (int)ii.size(); ++ p) {
		col(blockId, colmap[{ii[p-1],0}]);
		string blockId2 = blockId;
		if (p+1<(int)ii.size()) {
			cout << "cut ["<<blockId<<"] [y] ["<<ii[p]<<"]" << endl;
			blockId2 += ".0";
		}
		cut0(blockId2, ii[p-1]);
		blockId += ".1";
	}
}
void cut2(string blockId, int i0, int i1) {
	vector<int> ii;
	for (int i : Ipos) if (i0 <= i && i <= i1) ii.push_back(i);
	for (int p = 1; p < (int)ii.size(); ++ p) {
		col(blockId, colmap[{ii[p-1],317}]);
		if (p+1<(int)ii.size()) {
			cout << "cut ["<<blockId<<"] [y] ["<<ii[p]<<"]" << endl;
		}
		blockId += ".1";
	}
}
void solve() {
	if (width != 400) throw 1;
	if (height != 400) throw 1;
	col("0", 4289546752);
	cout << "cut [0] [357,83]" << endl;
	cout << "cut [0.0] [317,43]" << endl;
	cout << "cut [0.3] [x] [317]" << endl;
	for (int i = 0; i < (int)Ipos.size(); ++ i) {
		for (int j = 0; j < (int)Jpos.size(); ++ j) {
			colmap[{Ipos[i], Jpos[j]}] = Cols[(i+j+1)%2];
		}
	}
	cut1("0.0.3", 43,83);
	cut1("0.3.0", 83,400);
	cut2("0.3.1", 83,400);
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
