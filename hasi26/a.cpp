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

    // ???????????????????????????????????????
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
struct Data2 {
	const int i0, i1;
	const double cutCost;
	map<int,pair<double, int>> memo;
	map<pair<int,int>, pair<uint32_t, double>> cache;
	Data2(int i0, int i1) : i0(i0), i1(i1), cutCost(round(7.0*400/i0)) {}
	pair<uint32_t, double> fn(int j0, int j1) {
		if (cache.count({j0,j1})) return cache[{j0,j1}];
		return cache[{j0,j1}] = f(i0,i1,j0,j1);
	}
	pair<double, int> calc(int j0) {
		if (memo.count(j0)) return memo[j0];
		double colorCost = round(5.0*400*400/((400-i0)*(400-j0)));
		pair<double, int> res = {colorCost + fn(j0,400).second*0.005, 400};
		const int Delta1 = 5;
		const int Delta2 = 1;
		for (int j = j0+Delta1; j <= 400-Delta1; j += Delta2) {
			double cost = colorCost + calc(j).first;
			cost += fn(j0,j).second*0.005;
			cost += cutCost;
			cost += round(400.0/(i0*max(j, 400-j))); // mergeCost;
			res = min(res, {cost, j});
		}
		return memo[j0] = res;
	}
};
void solve() {
	vector<int> ipos {135,139,145,155,162,168,175,181,188,194,201,205,210,217,223,230,236,243,250,255,259,269,279,289};
	double sumCost = 0;
	cout << "cut [0] [y] ["<<ipos[0]<<"]" << endl;
	string blockId = "0.1";
	int globalCounter = 0;
	for (int i = 1; i < (int)ipos.size(); ++ i) {
		Data2 d(ipos[i-1], ipos[i]);
		auto c = d.calc(0).first;
		cerr << c << endl;
		for (int j = 0; j < 400; ) {
			int j1 = d.calc(j).second;
			auto cc = d.fn(j, j1).first;
			if (j == 0) {
				col(blockId, cc);
			} else {
				cout << "cut ["<<blockId<<"] [x] ["<<j<<"]" << endl;
				col(blockId+".1", cc);
				cout << "merge ["<<blockId<<".0] ["<<blockId<<".1]" << endl;
				blockId = to_string(++globalCounter);
			}
			j = j1;
		}
		sumCost += c;
		cout << "cut ["<<blockId<<"] [y] ["<<ipos[i]<<"]" << endl;
		blockId += ".1";
	}
	col(blockId+".1", 0xffffffff);
	// cerr << "sumCost = " << sumCost << endl;
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
