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

uint32_t toUint32(int x[4]) {
	uint8_t r[4];
	for (int k = 0; k < 4; ++ k) r[k] = x[k];
	return *(uint32_t*)r;
}

struct Block {
	string blockId;
	int minX, minY, maxX, maxY;
	uint32_t color;
};

vector<Block> InitialBlocks;
uint32_t TargetImage[400][400];
string BlockIds[400][400];
uint32_t CurImage[400][400];
double Sim[400][400];
int BlockSize, ImageSize;

double calcSim(int i0, int j0, uint32_t c) {
	double r = 0;
	for (int ii = 0; ii < BlockSize; ++ ii) {
		for (int jj = 0; jj < BlockSize; ++ jj) {
			int i = i0 * BlockSize + ii;
			int j = j0 * BlockSize + jj;
			r += sqrt(d(TargetImage[i][j], c));
		}
	}
	return r;
}

int main() {
	int numBlock;
	cin >> numBlock;
	InitialBlocks.resize(numBlock);
	for (auto& b : InitialBlocks) {
		int color[4];
		cin >> b.blockId >> b.minX >> b.minY >> b.maxX >> b.maxY >> color[0] >> color[1] >> color[2] >> color[3];
		b.color = toUint32(color);
	}
	BlockSize = InitialBlocks[0].maxX - InitialBlocks[0].minX;
	ImageSize = 400 / BlockSize;
	double costSwap = 3*400*400/BlockSize/BlockSize;
	int x;
	cin >> x;
	if (x != 160000) throw 1;
	for (int i = 0; i < 160000; ++ i) {
		int color[4];
		cin >> color[0] >> color[1] >> color[2] >> color[3];
		TargetImage[399 - i / 400][i % 400] = toUint32(color);
	}
	{
		set<pair<int,int>> a;
		for (auto& b : InitialBlocks) {
			int dx = b.maxX-b.minX;
			int dy = b.maxY-b.minY;
			if (dx != dy) throw 1;
			if (b.minX % BlockSize != 0) throw 1;
			if (b.minY % BlockSize != 0) throw 1;
			a.insert({dx,dy});
		}
		if (a.size() != 1) throw 1;
	}
	for (auto& b : InitialBlocks) {
		int i = b.minY / BlockSize;
		int j = b.minX / BlockSize;
		CurImage[i][j]  = b.color;
		Sim[i][j] = calcSim(i, j, CurImage[i][j]);
		BlockIds[i][j] = b.blockId;
	}
	int counter = 0;
	for (;;) {
		int I0 = -1, J0 = -1, I1 = -1, J1 = -1;
		double bestDelta = costSwap;
		for (int tt = 0; tt < 100000; ++ tt) {
			int i0 = rand() % ImageSize;
			int j0 = rand() % ImageSize;
			int i1 = rand() % ImageSize;
			int j1 = rand() % ImageSize;
			auto prev = Sim[i0][j0] + Sim[i1][j1];
			auto s0 = calcSim(i0, j0, CurImage[i1][j1]);
			auto s1 = calcSim(i1, j1, CurImage[i0][j0]);
			auto delta = (prev-(s0+s1))*0.005;
			if (delta > bestDelta) {
				I0 = i0;
				J0 = j0;
				I1 = i1;
				J1 = j1;
				bestDelta = delta;
			}
		}
		if (I0 == -1) break;
		{
			int i0 = I0, j0 = J0, i1 = I1, j1 = J1;
			Sim[i0][j0] = calcSim(i0, j0, CurImage[i1][j1]);
			Sim[i1][j1] = calcSim(i1, j1, CurImage[i0][j0]);
			swap(CurImage[i0][j0], CurImage[i1][j1]);
			++ counter;
			cout << "swap ["<<BlockIds[i0][j0]<<"] ["<<BlockIds[i1][j1]<<"]" <<endl;
			swap(BlockIds[i0][j0], BlockIds[i1][j1]);
		}
	}
	double sumSim = 0;
	for (int i = 0; i < ImageSize; ++ i) for (int j = 0; j < ImageSize; ++ j) {
		sumSim += Sim[i][j];
	}
	cerr << sumSim*0.005 + counter * costSwap << endl;
}
