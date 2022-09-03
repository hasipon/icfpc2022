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
pair<uint32_t, double> f(int i0, int i1, int j0, int j1) {
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
	return {*(uint32_t*)res, score};
}
void col(string blockId, uint32_t c) {
	uint8_t* x = (uint8_t*)&c;
	cout << "color ["<<blockId<<"] ["<<(int)x[0]<<","<<(int)x[1]<<","<<(int)x[2]<<","<<(int)x[3]<<"]"<<endl;
}
int cumu[512][512][4];
vector<double> calcAve(int i0, int i1, int j0, int j1) {
	vector<double> r(4);
	for (int k = 0; k < 4; ++ k) {
		r[k] = cumu[i1][j1][k] - cumu[i1][j0][k] - cumu[i0][j1][k] + cumu[i0][j0][k];
		r[k] /= (i1-i0) * (j1-j0);
	}
	return r;
}
double vecDist(const vector<double>& v0, const vector<double>& v1) {
	double r = 0;
	for (int k = 0; k < 4; ++ k) {
		double d = v0[k]-v1[k];
		r += d*d;
	}
	return r;
}
int areaLimit = 3600;
pair<int,int> calcCut(int i0, int i1, int j0, int j1) {
	pair<int,int> r {-1,-1};
	if ((i1-i0)*(j1-j0) < areaLimit) return r;
	double score = 100;
	for (int i = i0+10; i <= i1-10; ++ i) {
		int area0 = (i-i0)*(j1-j0);
		int area1 = (i1-i)*(j1-j0);
		if (area0 >= areaLimit || area1 >= areaLimit) {
			double s = vecDist(calcAve(i0, i, j0, j1), calcAve(i, i1, j0, j1));
			if (s > score) {
				r = {0, i};
				score = s;
			}
		}
	}
	for (int j = j0+10; j <= j1-10; ++ j) {
		int area0 = (i1-i0)*(j-j0);
		int area1 = (i1-i0)*(j1-j);
		if (area0 >= areaLimit || area1 >= areaLimit) {
			double s = vecDist(calcAve(i0, i1, j0, j), calcAve(i0, i1, j, j1));
			if (s > score) {
				r = {1, j};
				score = s;
			}
		}
	}
	return r;
}
struct Hoge {
	int i0, i1, j0, j1, t, v;
	vector<Hoge> a;
	Hoge(int i0, int i1, int j0, int j1) : i0(i0), i1(i1), j0(j0), j1(j1), t(-1) {}
	Hoge(Hoge x, Hoge y, int i0, int i1, int j0, int j1, int t, int v)  : i0(i0), i1(i1), j0(j0), j1(j1), t(t), v(v) {
		a.push_back(x);
		a.push_back(y);
	}
	double calcScore() {
		if (t == -1) {
			return calcSim() + round(5.0*400*400/((i1-i0)*(j1-j0)));
		} else {
			double r = round(7.0*400*400/((i1-i0)*(j1-j0)));
			if (a[0].t == -1) {
				r += round(5.0*400*400/((i1-i0)*(j1-j0)));
				r += a[0].calcSim();
				r += a[1].calcScore();
			} else if (a[1].t == -1) {
				r += round(5.0*400*400/((i1-i0)*(j1-j0)));
				r += a[1].calcSim();
				r += a[0].calcScore();
			} else {
				r += a[0].calcScore();
				r += a[1].calcScore();
			}
			return r;
		}
	}
	uint32_t getColor() {
		return f(i0,i1,j0,j1).first;
	}
	double calcSim() {
		return f(i0,i1,j0,j1).second * 0.005;
	}
	void output(string blockId) {
		if (t == -1) {
			auto p = f(i0,i1,j0,j1);
			col(blockId, p.first);
		} else {
			if (a[0].t == -1) {
				col(blockId, a[0].getColor());
				if (t == 0) {
					cout << "cut ["<<blockId<<"] [Y] ["<<v<<"]"<<endl;
				} else {
					cout << "cut ["<<blockId<<"] [X] ["<<v<<"]"<<endl;
				}
				a[1].output(blockId+".1");
			} else if (a[1].t == -1) {
				col(blockId, a[1].getColor());
				if (t == 0) {
					cout << "cut ["<<blockId<<"] [Y] ["<<v<<"]"<<endl;
				} else {
					cout << "cut ["<<blockId<<"] [X] ["<<v<<"]"<<endl;
				}
				a[0].output(blockId+".0");
			} else {
				if (t == 0) {
					cout << "cut ["<<blockId<<"] [Y] ["<<v<<"]"<<endl;
				} else {
					cout << "cut ["<<blockId<<"] [X] ["<<v<<"]"<<endl;
				}
				a[0].output(blockId+".0");
				a[1].output(blockId+".1");
			}
		}
	}
};
Hoge walk(int i0, int i1, int j0, int j1) {
	auto p = calcCut(i0,i1,j0,j1);
	if (p.first == -1) {
		return Hoge(i0, i1, j0, j1);
	} else if (p.first == 0) {
		int i = p.second;
		return Hoge(walk(i0, i, j0, j1), walk(i, i1, j0, j1), i0, i1, j0, j1, 0, i);
	} else {
		int j = p.second;
		return Hoge(walk(i0, i1, j0, j), walk(i0, i1, j, j1), i0, i1, j0, j1, 1, j);
	}
}
void solve() {
	if (width != 400) throw 1;
	if (height != 400) throw 1;
	for (int i = 0; i < 400; ++ i) {
		for (int j = 0; j < 400; ++ j) {
			for (int k = 0; k < 4; ++ k) {
				cumu[i+1][j+1][k] = cumu[i+1][j][k] + cumu[i][j+1][k] - cumu[i][j][k] + get(i,j,k);
			}
		}
	}
	Hoge hoge(0,0,0,0);
	double score = 1e100;
	int al = -1;
	for (int i = 40; i <= 100; ++ i) {
		areaLimit = i*i;
		auto h = walk(0, 400, 0, 400);
		auto s = h.calcScore();
		if (s < score) {
			hoge = h;
			score = s;
			al = areaLimit;
		}
	}
	cerr << al << " " << score << endl;
	hoge.output("0");
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
