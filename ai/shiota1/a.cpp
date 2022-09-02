#include <iostream>
#include <vector>
#include<cstdlib>
#include<cmath>

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
  if (!fgets(buf, sizeof(buf), stdin))
    throw 1;
  for (;;) {
    if (!fgets(buf, sizeof(buf), stdin))
      throw 1;
    if (string(buf) == "ENDHDR\n")
      break;
    if (string(buf) == "TUPLTYPE RGB_ALPHA\n")
      continue;
    int val;
    sscanf(buf, "%s %d", name, &val);
    auto n = string(name);
    if (n == "WIDTH") {
      width = val;
    } else if (n == "HEIGHT") {
      height = val;
    } else if (n == "DEPTH") {
      if (val != 4)
        throw 1;
    } else if (n == "MAXVAL") {
      if (val != 255)
        throw 1;
    }
  }
  Image = new uint8_t[width * height * 4];
  if ((int)fread(Image, 1, width * height * 4, stdin) != width * height * 4)
    throw 1;

  vector<int> ansV;
  for (int d = 0; d < 4; d++) {
    int ansCost = 1e9;
    int ansCol = -1;
    for (int c = 0; c < 256; c++) {
      int cost = 0;
      for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
          int target = get(y, x, d);
          int di = abs(target- c);
          cost += sqrt(di * di);
        }
      }
      if (ansCost > cost) {
        ansCost = cost;
        ansCol = c;
      }
    }
    ansV.push_back(ansCol);
  }
  cout << "color [0] [" << ansV[0] << "," << ansV[1] << "," << ansV[2] << ","
       << ansV[3] << "]" << endl;
}
