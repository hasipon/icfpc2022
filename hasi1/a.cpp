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

	vector<int> cut_i;
	cut_i.push_back(0);
	for (int i = 1; i < height; ++ i) {
		for (int j = 0; j < width; ++ j) {
			if (get(i-1, j) != get(i, j)) {
				cut_i.push_back(i);
				break;
			}
		}
	}
	cut_i.push_back(height);
	string blockId = "0";
	for (int ii = 1; ii < (int)cut_i.size(); ++ ii) {
		int i = cut_i[ii-1];
		int i1 = cut_i[ii];
		string blockId2 = blockId;
		if (i1 < height) {
			cout << "cut [" << blockId << "] [Y] [" << i1 << "]" << endl;
			blockId2 += ".0";
		}
		for (int j = 1; j < width; ++ j) {
			if (get(i, j-1) != get(i, j)) {
				cout << "cut [" << blockId2 << "] [X] [" << j << "]" << endl;
				cout << "color [" << blockId2 << ".0] [" << (int)get(i,j-1,0) << "," << (int)get(i,j-1,1) << "," << (int)get(i,j-1,2) << "," << (int)get(i,j-1,3) << "]" << endl;
				blockId2 += ".1";
			}
		}
		cout << "color [" << blockId2 << "] [" << (int)get(i,width-1,0) << "," << (int)get(i,width-1,1) << "," << (int)get(i,width-1,2) << "," << (int)get(i,width-1,3) << "]" << endl;
		blockId += ".1";
	}
}
