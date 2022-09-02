#include <iostream>
#include <vector>
using namespace std;
int width = -1, height = -1;
uint8_t* Image;
uint8_t get(int i, int j, int k) {
	return Image[((height - 1 - i) * width + j) * 4 + k];
}
uint32_t get(int i, int j) {
	return *(uint32_t*)&Image[((height - 1 - i) * width + j) * 4];
}
vector<int> a40;
void cutX(vector<int> a, string blockId, int x, int y) {
	if (a.empty()) {
		vector<int> sum(4);
		for (int i = 0; i < 40; ++ i) {
			for (int j = 0; j < 40; ++ j) {
				for (int k = 0; k < 4; ++ k) {
					sum[k] += get(y + i, x + j, k);
				}
			}
		}
		int c0 = (sum[0] + 20*40) / (40*40);
		int c1 = (sum[1] + 20*40) / (40*40);
		int c2 = (sum[2] + 20*40) / (40*40);
		int c3 = (sum[3] + 20*40) / (40*40);
		cout << "color [" << blockId << "] [" << c0 << "," << c1 << "," << c2 << "," << c3 << "]" << endl;
		return;
	}
	int m = a.size()/2;
	cout << "cut [" << blockId << "] [X] [" << a[m] << "]" << endl;
	vector<int> a0, a1;
	for (int x : a) {
		if (x < a[m]) a0.push_back(x);
		if (x > a[m]) a1.push_back(x);
	}
	cutX(a0, blockId+".0", x, y);
	cutX(a1, blockId+".1", a[m], y);
}
void cutY(vector<int> a, string blockId, int y) {
	if (a.empty()) {
		cutX(a40, blockId, 0, y);
		return;
	}
	int m = a.size()/2;
	cout << "cut [" << blockId << "] [Y] [" << a[m] << "]" << endl;
	vector<int> a0, a1;
	for (int x : a) {
		if (x < a[m]) a0.push_back(x);
		if (x > a[m]) a1.push_back(x);
	}
	cutY(a0, blockId+".0", y);
	cutY(a1, blockId+".1", a[m]);
}
void solve() {
	if (width != 400) throw 1;
	if (height != 400) throw 1;
	for (int i = 40; i < 400; i += 40) a40.push_back(i);
	cutY(a40, "0", 0);
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
