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
pair<double, uint32_t> score0() {
	vector<int> sum(4);
	for (int i = 0; i < 400; ++ i) for (int j = 0; j < 400; ++ j) for (int k = 0; k < 4; ++ k) {
		sum[k] += get(i,j,k);
	}
	uint8_t res[4];
	for (int k = 0; k < 4; ++ k) {
		res[k] = (sum[k] + 200*400) / (400*400);
	}
	double score = 0;
	for (int i = 0; i < 400; ++ i) for (int j = 0; j < 400; ++ j) {
		score += sqrt(d(get(i,j), *(uint32_t*)res));
	}
	for (;;) {
		for (int k = 0; k < 4; ++ k) {
			if (res[k] > 0) {
				-- res[k];
				double score2 = 0;
				for (int i = 0; i < 400; ++ i) for (int j = 0; j < 400; ++ j) {
					score2 += d(get(i,j), *(uint32_t*)res);
				}
				if (score2 < score) {
					//for (int k = 0; k < 4; ++ k) cerr << (int)res[k] << " "; cerr << endl;
					score = score2;
					goto next;
				}
				++ res[k];
			}
			if (res[k] < 255) {
				++ res[k];
				double score2 = 0;
				for (int i = 0; i < 400; ++ i) for (int j = 0; j < 400; ++ j) {
					score2 += sqrt(d(get(i,j), *(uint32_t*)res));
				}
				if (score2 < score) {
					//for (int k = 0; k < 4; ++ k) cerr << (int)res[k] << " "; cerr << endl;
					score = score2;
					goto next;
				}
				-- res[k];
			}
		}
		break;
		next:;
	}
	return {score, *(uint32_t*)res};
}
pair<double, uint32_t> score1(int i0) {
	vector<int> sum(4);
	for (int i = 0; i < 200; ++ i) for (int j = 0; j < 400; ++ j) for (int k = 0; k < 4; ++ k) {
		sum[k] += get(i0+i,j,k);
	}
	uint8_t res[4];
	for (int k = 0; k < 4; ++ k) {
		res[k] = (sum[k] + 200*400) / (400*400);
	}
	double score = 0;
	for (int i = 0; i < 200; ++ i) for (int j = 0; j < 400; ++ j) {
		score += sqrt(d(get(i0+i,j), *(uint32_t*)res));
	}
	for (;;) {
		for (int k = 0; k < 4; ++ k) {
			if (res[k] > 0) {
				-- res[k];
				double score2 = 0;
				for (int i = 0; i < 200; ++ i) for (int j = 0; j < 400; ++ j) {
					score2 += d(get(i0+i,j), *(uint32_t*)res);
				}
				if (score2 < score) {
					//for (int k = 0; k < 4; ++ k) cerr << (int)res[k] << " "; cerr << endl;
					score = score2;
					goto next;
				}
				++ res[k];
			}
			if (res[k] < 255) {
				++ res[k];
				double score2 = 0;
				for (int i = 0; i < 200; ++ i) for (int j = 0; j < 400; ++ j) {
					score2 += sqrt(d(get(i0+i,j), *(uint32_t*)res));
				}
				if (score2 < score) {
					//for (int k = 0; k < 4; ++ k) cerr << (int)res[k] << " "; cerr << endl;
					score = score2;
					goto next;
				}
				-- res[k];
			}
		}
		break;
		next:;
	}
	return {score, *(uint32_t*)res};
}
pair<double, pair<uint32_t, uint32_t>> score1() {
	auto r0 = score1(0);
	auto r1 = score1(200);
	return {r0.first + r1.first, {r0.second, r1.second}};
}
void solve() {
	if (width != 400) throw 1;
	if (height != 400) throw 1;
	auto p0 = score0();
	auto p1 = score1();
	cerr << p0.first*0.005 << endl;
	cerr << p1.first*0.005 << endl;
	auto res = (uint8_t*)&p1.second.first;
	cout << "color [0] ["<<(int)res[0]<<","<<(int)res[1]<<","<<(int)res[2]<<","<<(int)res[3]<<"]"<<endl;
	cout << "cut [0] [Y] [200]"<<endl;
	res = (uint8_t*)&p1.second.second;
	cout << "color [0.1] ["<<(int)res[0]<<","<<(int)res[1]<<","<<(int)res[2]<<","<<(int)res[3]<<"]"<<endl;
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
