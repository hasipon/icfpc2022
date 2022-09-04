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
    int sum[3] = {0};

    for (int i = i0; i < i1; ++ i) {
        for (int j = j0; j < j1; ++ j) {
            for (int k = 0; k < 3; ++ k) {
                sum[k] += get(i,j,k);
            }
        }
    }

    double res[3];

    int area = (i1-i0)*(j1-j0);
    for (int k = 0; k < 3; ++ k) {
        res[k] = (sum[k] + area/2) / area;
    }

    // Weiszfeld's algorithm
    // https://en.wikipedia.org/wiki/Geometric_median
    for (auto it = 0; it < 20; it++) {
        // result = numer / denom
        double numer[3] = {0};
        double denom = 0;

        for (int i = i0; i < i1; ++i) {
            for (int j = j0; j < j1; ++j) {
                double norm = 0;

                for (int k = 0; k < 3; ++k) {
                    auto d = get(i, j, k) - res[k];
                    norm += d * d;
                }
                norm = sqrt(fmax(norm, 1e-6));

                denom += 1 / norm;
                for (int k = 0; k < 3; ++k) {
                    numer[k] += get(i, j, k) / norm;
                }
            }
        }

        double difference = 0;
        for (int k = 0; k < 3; ++k) {
            double previous = res[k];
            double next = numer[k] / denom;
            difference += fabs(previous - next);
            res[k] = next;
        }

        if (difference < 0.1) {
            break;
        }
    }

    uint8_t resInt[4];
    resInt[3] = 255;
    for (int k = 0; k < 3; ++ k) {
        int rounded = round(res[k]);
        if (rounded < 0) {
            resInt[k] = 0;
        } else if (255 < rounded) {
            resInt[k] = 255;
        } else {
            resInt[k] = rounded;
        }
    }

    double score = 0;
    for (int i = i0; i < i1; ++ i) {
        for (int j = j0; j < j1; ++ j) {
            score += sqrt(d(get(i,j), *(uint32_t*)resInt));
        }
    }

    // 山登りで最後まで収束させる
    for (;;) {
        for (int k = 0; k < 3; ++ k) {
            if (resInt[k] > 0) {
                -- resInt[k];
                double score2 = 0;
                for (int i = i0; i < i1; ++ i) {
                    for (int j = j0; j < j1; ++ j) {
                        score2 += sqrt(d(get(i,j), *(uint32_t*)resInt));
                    }
                }
                if (score2 < score) {
                    score = score2;
                    goto next;
                }
                ++ resInt[k];
            }
            if (resInt[k] < 255) {
                ++ resInt[k];
                double score2 = 0;
                for (int i = i0; i < i1; ++ i) {
                    for (int j = j0; j < j1; ++ j) {
                        score2 += sqrt(d(get(i,j), *(uint32_t*)resInt));
                    }
                }
                if (score2 < score) {
                    score = score2;
                    goto next;
                }
                -- resInt[k];
            }
        }
        break;
        next:;
    }

    return {*(uint32_t*)resInt, score};
}
void col(string blockId, uint32_t c) {
	uint8_t* x = (uint8_t*)&c;
	cout << "color ["<<blockId<<"] ["<<(int)x[0]<<","<<(int)x[1]<<","<<(int)x[2]<<","<<(int)x[3]<<"]"<<endl;
}
struct Block {
	int i0, j0, i1, j1;
	Block(){}
	Block(int i0, int j0, int i1, int j1) : i0(i0), j0(j0), i1(i1), j1(j1) {}
};
void solve() {
	if (width != 400) throw 1;
	if (height != 400) throw 1;
	col("0", 0xff000000);
	vector<pair<string,pair<int,int>>> data {
		{"0",{-1,221}},
		{"0.0",{-2,226}},
		{"0.0.1",{-1,148}},
		{"0.0.1.1",{200,303}},
		{"0.0.1.0",{103,252}},
		{"0.0.0",{-1,119}},
		{"0.0.0.0",{-2,103}},
		{"0.0.0.0.0",{16,76}},
		{"0.0.0.0.1",{67,143}},
		{"0.0.0.0.1.3",{17,193}},
		{"0.0.0.0.1.3.1",{41,167}},
		{"0.1",{-2,129}},
		{"0.1.1",{341,183}},
		{"0.1.0",{-1,324}},
		{"0.1.0.1",{375,26}},
		{"0.1.0.1.3",{-2,102}},
		{"0.1.0.1.3.0",{-2,51}},
		{"0.1.0.1.3.0.0",{-1,349}},
		{"0.1.0.1.3.0.1",{349,77}},
		{"0.1.0.0",{-2,25}},
		{"0.1.0.0.1",{-2,51}},
		{"0.1.0.0.1.0",{-1,298}},
		{"0.1.0.0.1.1",{272,102}},
		{"0.1.0.0.1.1.1",{298,76}},
		{"0.1.1.3",{288,234}},
		{"0.1.1.3.1",{314,208}},
		{"0.1.1.3.2",{315,284}},
		{"0.1.1.3.3",{263,259}},
		{"0.1.1.3.3.2",{-2,336}},
		{"0.1.1.3.3.3",{-2,285}},
		{"0.1.1.3.3.3.1",{238,310}},
		{"0.0.0.1",{144,103}},
		{"0.0.0.1.0",{-2,76}},
		{"0.0.0.1.2",{195,179}},
		{"0.0.0.1.1",{170,52}},
		{"0.0.0.1.1.2",{196,76}},
		{"0.0.0.1.1.1",{195,26}},
		{"0.0.0.1.1.0",{-2,26}},
		{"0.0.0.1.2.0",{170,154}},
		{"0.0.0.1.2.0.0",{-2,128}},
		{"0.0.1.0.3",{93,284}},
		{"0.0.1.0.3.3",{41,337}},
		{"0.0.1.0.3.3.2",{68,361}},
		{"0.0.1.0.3.3.1",{68,310}},
		{"0.0.0.1.3",{-2,205}},
	};
	map<string,Block> blocks;
	blocks["0"] = Block(0,0,400,400);
	for (auto p : data) {
		auto bid = p.first;
		auto [i,j] = p.second;
		auto b = blocks[bid];
		if (i == -1) {
			blocks[bid+".0"] = Block(b.i0, b.j0, b.i1, j);
			blocks[bid+".1"] = Block(b.i0, j, b.i1, b.j1);
			cout << "cut ["<<bid<<"] [x] ["<<j<<"]"<< endl;
		} else if (i == -2) {
			blocks[bid+".0"] = Block(b.i0, b.j0, j, b.j1);
			blocks[bid+".1"] = Block(j, b.j0, b.i1, b.j1);
			cout << "cut ["<<bid<<"] [y] ["<<j<<"]"<< endl;
		} else {
			cout << "cut ["<<bid<<"] ["<<i<<","<<j<<"]"<< endl;
			swap(i, j);
			blocks[bid+".0"] = Block(b.i0, b.j0, i, j);
			blocks[bid+".1"] = Block(b.i0, j, i, b.j1);
			blocks[bid+".2"] = Block(i, j, b.i1, b.j1);
			blocks[bid+".3"] = Block(i, b.j0, b.i1, j);
		}
		blocks.erase(p.first);
		
	}
	for (auto p : blocks) {
		auto b = p.second;
		auto cc = f(b.i0, b.i1, b.j0, b.j1).first;
		if (cc == 0xff000000) continue;
		// cout << p.first << " " << cc << endl;
		col(p.first, cc);
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
