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
void cut(int i0, int i1, int j0, int j1, string blockId) {
	if (i1-i0 == 1) {
		if (j1-j0 == 1) {
			vector<int> sum(4);
			for (int i = 0; i < 40; ++ i) {
				for (int j = 0; j < 40; ++ j) {
					for (int k = 0; k < 4; ++ k) {
						sum[k] += get(i0*40 + i, j0*40 + j, k);
					}
				}
			}
			int c0 = (sum[0] + 20*40) / (40*40);
			int c1 = (sum[1] + 20*40) / (40*40);
			int c2 = (sum[2] + 20*40) / (40*40);
			int c3 = (sum[3] + 20*40) / (40*40);
			cout << "color [" << blockId << "] [" << c0 << "," << c1 << "," << c2 << "," << c3 << "]" << endl;
		} else {
			int j2 = (j0+j1)/2;
			cout << "cut ["<<blockId<<"] [X] ["<<(j2*40)<<"]" << endl;
			cut(i0,i1,j0,j2,blockId+".0");
			cut(i0,i1,j2,j1,blockId+".1");
		}
	} else {
		if (j1-j0 == 1) {
			int i2 = (i0+i1)/2;
			cout << "cut ["<<blockId<<"] [Y] ["<<(i2*40)<<"]" << endl;
			cut(i0,i2,j0,j1,blockId+".0");
			cut(i2,i1,j0,j1,blockId+".1");
		} else {
			int i2 = (i0+i1)/2;
			int j2 = (j0+j1)/2;
			cout << "cut ["<<blockId<<"] ["<<(j2*40)<<","<<(i2*40)<<"]" << endl;
			cut(i0,i2,j0,j2,blockId+".0");
			cut(i0,i2,j2,j1,blockId+".1");
			cut(i2,i1,j2,j1,blockId+".2");
			cut(i2,i1,j0,j2,blockId+".3");
		}
	}
}
void solve() {
	if (width != 400) throw 1;
	if (height != 400) throw 1;
	cut(0, 10, 0, 10, "0");
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
