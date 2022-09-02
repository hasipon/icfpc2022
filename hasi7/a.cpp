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
uint32_t f(int i0, int i1, int j0, int j1) {
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
	return *(uint32_t*)res;
}
void col(string blockId, uint32_t c) {
	if (c == 4294967295) return;
	uint8_t* x = (uint8_t*)&c;
	cout << "color ["<<blockId<<"] ["<<(int)x[0]<<","<<(int)x[1]<<","<<(int)x[2]<<","<<(int)x[3]<<"]"<<endl;
}
void solve() {
	if (width != 400) throw 1;
	if (height != 400) throw 1;
	uint32_t cc[4][4];
	for (int i = 0; i < 4; ++ i) {
		for (int j = 0; j < 4; ++ j) {
			cc[i][j] = f(i*100, i*100+100, j*100, j*100+100);
			cerr << cc[i][j] << " ";
		}
		cerr << endl;
	}
	cout << "cut [0] [200,200]"<<endl;
	cout << "cut [0.0] [100,100]"<<endl;
	cout << "cut [0.1] [300,100]"<<endl;
	cout << "cut [0.2] [300,300]"<<endl;
	cout << "cut [0.3] [100,300]"<<endl;
	col("0.0.0", cc[0][0]);
	col("0.0.1", cc[0][1]);
	col("0.1.0", cc[0][2]);
	col("0.1.1", cc[0][3]);
	col("0.0.3", cc[1][0]);
	col("0.0.2", cc[1][1]);
	col("0.1.3", cc[1][2]);
	col("0.1.2", cc[1][3]);
	col("0.3.0", cc[2][0]);
	col("0.3.1", cc[2][1]);
	col("0.2.0", cc[2][2]);
	col("0.2.1", cc[2][3]);
	col("0.3.3", cc[3][0]);
	col("0.3.2", cc[3][1]);
	col("0.2.3", cc[3][2]);
	col("0.2.2", cc[3][3]);
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
