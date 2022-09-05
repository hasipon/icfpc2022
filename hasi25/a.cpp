#include <cmath>
#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <algorithm>
using namespace std;
long long getTime() {
	timespec ts;
	clock_gettime(CLOCK_REALTIME_COARSE, &ts);
	long long r = ts.tv_sec;
	r *= 1000000000;
	r += ts.tv_nsec;
	return r;
}
int width = -1, height = -1;
uint8_t* Image;
uint8_t get(int i, int j, int k) {
	return Image[((height - 1 - i) * width + j) * 4 + k];
}
uint32_t get(int i, int j) {
	return *(uint32_t*)&Image[((height - 1 - i) * width + j) * 4];
}
int dist(const vector<bool>& a, const vector<bool>& b) {
	int r = 0;
	for (int i = 0; i < 400; ++ i) if (a[i] != b[i]) ++ r;
	return r;
}
void solve() {
	vector<vector<bool>> a(400, vector<bool>(400));
	for (int i = 0; i < 400; ++ i) {
		for (int j = 0; j < 400; ++ j) {
			a[i][j] = (get(i, j, 0) < 128);
		}
		for (int j = 1; j+1 < 400; ++ j) {
			if (a[i][j-1] != a[i][j] && a[i][j] != a[i][j+1]) {
				a[i][j] = !a[i][j];
			}
		}
		for (int j = 1; j+2 < 400; ++ j) {
			if (a[i][j-1] != a[i][j] && a[i][j] == a[i][j+1] && a[i][j+1] != a[i][j+2]) {
				a[i][j] = !a[i][j];
				a[i][j+1] = !a[i][j+1];
			}
		}
		for (int j = 1; j+3 < 400; ++ j) {
			if (a[i][j-1] != a[i][j] && a[i][j] == a[i][j+1] && a[i][j+1] == a[i][j+2] && a[i][j+2] != a[i][j+3]) {
				a[i][j] = !a[i][j];
				a[i][j+1] = !a[i][j+1];
				a[i][j+2] = !a[i][j+2];
			}
		}
	}
	for (;;) {
		vector<int> b;
		b.push_back(0);
		for (int i = 1; i < 400; ++ i) {
			if (a[i-1] != a[i]) b.push_back(i);
		}
		b.push_back(400);
		for (int i = 1; i < (int)b.size(); ++ i) {
			if (b[i]-b[i-1] == 1) {
				int p = b[i-1];
				int d1 = dist(a[p], a[p-1]);
				int d2 = dist(a[p], a[p+1]);
				if (d1 <= d2) {
					a[p] = a[p-1];
					goto next;
				} else {
					a[p] = a[p+1];
					goto next;
				}
			}
		}
		break;
		next:;
	}
	for (;;) {
		vector<int> b;
		b.push_back(0);
		for (int i = 1; i < 400; ++ i) {
			if (a[i-1] != a[i]) b.push_back(i);
		}
		b.push_back(400);
		for (int i = 1; i < (int)b.size(); ++ i) {
			if (b[i]-b[i-1] == 2) {
				int p = b[i-1];
				int d1 = dist(a[p], a[p-1]);
				int d2 = dist(a[p], a[p+2]);
				if (d1 <= d2) {
					a[p] = a[p+1] = a[p-1];
					goto next2;
				} else {
					a[p] = a[p+1] = a[p+2];
					goto next2;
				}
			}
		}
		break;
		next2:;
	}
	for (;;) {
		vector<int> b;
		b.push_back(0);
		for (int i = 1; i < 400; ++ i) {
			if (a[i-1] != a[i]) b.push_back(i);
		}
		b.push_back(400);
		for (int i = 1; i < (int)b.size(); ++ i) {
			if (b[i]-b[i-1] == 3) {
				int p = b[i-1];
				int d1 = dist(a[p], a[p-1]);
				int d2 = dist(a[p], a[p+3]);
				if (d1 <= d2) {
					a[p] = a[p+1] = a[p+2] = a[p-1];
					goto next3;
				} else {
					a[p] = a[p+1] = a[p+2] = a[p+3];
					goto next3;
				}
			}
		}
		break;
		next3:;
	}
	string blockId = "0";
	int globalCounter = 0;
	for (int i = 1; i < 400; ++ i) {
		if (a[i-1] != a[i]) {
			cerr << i << endl;
			cout << "cut ["<<blockId<<"] [y] ["<<i<<"]" << endl;
			blockId += ".1";
			cout << "color ["<<blockId<<"] [255,255,255,255]" << endl;
			for (int j = 1; j < 400; ++ j) {
				if (a[i][j-1] != a[i][j]) {
					cout << "cut ["<<blockId<<"] [x] ["<<j<<"]" << endl;
					if (a[i][j]) {
						cout << "color ["<<blockId<<".1] [0,0,0,255]" << endl;
					} else {
						cout << "color ["<<blockId<<".1] [255,255,255,255]" << endl;
					}
					cout << "merge ["<<blockId<<".0] ["<<blockId<<".1]" << endl;
					blockId = to_string(++globalCounter);
				}
			}
		}
	}
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
