#include <cmath>
#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <algorithm>
using namespace std;
long long sumsst;
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
uint8_t get(int j, int i, int k) {
	return Image[((height - 1 - i) * width + j) * 4 + k];
}
uint32_t get(int j, int i) {
	return *(uint32_t*)&Image[((height - 1 - i) * width + j) * 4];
}
double d(uint32_t x, uint32_t y) {
	int x0 = ((uint8_t*)&x)[0] - ((uint8_t*)&y)[0];
	int x1 = ((uint8_t*)&x)[1] - ((uint8_t*)&y)[1];
	int x2 = ((uint8_t*)&x)[2] - ((uint8_t*)&y)[2];
	//int x3 = ((uint8_t*)&x)[3] - ((uint8_t*)&y)[3];
	double r = 0;
	r += x0*x0;
	r += x1*x1;
	r += x2*x2;
	//r += x3*x3;
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

enum CutType {
    VCut, HCut, PCut, InvalidCut
};

struct Cut {
    CutType type;
    int x;
    int y;

    Cut() {}

    int i0, i1, j0, j1, areaLimit;
    Cut(int i0, int i1, int j0, int j1, int areaLimit) : i0(i0), i1(i1), j0(j0), j1(j1), areaLimit(areaLimit), type(PCut) {}

    bool operator<(const Cut rhs) const {
        if (type != rhs.type) {
            return type < rhs.type;
        }

        if (x != rhs.x) {
            return x < rhs.x;
        }

        return y < rhs.y;
    }
};

// この長方形の中で，areaLimitを満たしつつ一番色が違う領域をみつける．
Cut calcVHCut(int i0, int i1, int j0, int j1, int areaLimit) {
    Cut bestCut;
    bestCut.type = InvalidCut;

	if ((i1-i0)*(j1-j0) < areaLimit) return bestCut;
	double score = 100;

	for (int i = i0+10; i <= i1-10; ++ i) {
		int area0 = (i-i0)*(j1-j0);
		int area1 = (i1-i)*(j1-j0);
		if (area0 >= areaLimit || area1 >= areaLimit) {
			double s = vecDist(calcAve(i0, i, j0, j1), calcAve(i, i1, j0, j1));
			if (s > score) {
                bestCut.type = VCut;
                bestCut.x = i;
                bestCut.y = 0;
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
                bestCut.type = HCut;
                bestCut.x = 0;
                bestCut.y = j;
				score = s;
			}
		}
	}

	return bestCut;
}

bool checkPCut(int i0, int i1, int j0, int j1, int areaLimit) {
    if ((i1-i0)*(j1-j0) < areaLimit) return false;

    const auto areaAve = calcAve(i0, i1, j0, j1);
    constexpr double score = 100;

    for (int i = i0+10; i <= i1-10; ++ i) {
        for (int j = j0 + 10; j <= j1 - 10; ++j) {
            int area0 = (i - i0) * (j - j0);
            int area1 = (i1 - i) * (j - j0);
            int area2 = (i1 - i) * (j1 - j);
            int area3 = (i - i0) * (j1 - j);

            if (area0 < areaLimit && area1 < areaLimit && area2 < areaLimit && area3 < areaLimit) {
                continue;
            }

            if (score < vecDist(calcAve(i0, i, j0, j), areaAve)) {
                return true;
            }

            if (score < vecDist(calcAve(i, i1, j0, j), areaAve)) {
                return true;
            }

            if (score < vecDist(calcAve(i, i1, j, j1), areaAve)) {
                return true;
            }

            if (score < vecDist(calcAve(i0, i, j, j1), areaAve)) {
                return true;
            }
        }
    }

    return false;
}

void calcPCut(Cut &bestCut) {
    int i0 = bestCut.i0, i1 = bestCut.i1, j0 = bestCut.j0, j1 = bestCut.j1;

    const auto areaAve = calcAve(i0, i1, j0, j1);
    double score = 0;
    for (int i = i0+10; i <= i1-10; ++ i) {
        for (int j = j0 + 10; j <= j1 - 10; ++j) {
            int area0 = (i - i0) * (j - j0);
            int area1 = (i1 - i) * (j - j0);
            int area2 = (i1 - i) * (j1 - j);
            int area3 = (i - i0) * (j1 - j);

            auto area0Diff = vecDist(calcAve(i0, i, j0, j), areaAve);
            auto area1Diff = vecDist(calcAve(i, i1, j0, j), areaAve);
            auto area2Diff = vecDist(calcAve(i, i1, j, j1), areaAve);
            auto area3Diff = vecDist(calcAve(i0, i, j, j1), areaAve);

            double s = sqrt(area0Diff) + sqrt(area1Diff) + sqrt(area2Diff) + sqrt(area3Diff);
            if (score < s) {
                bestCut.type = PCut;
                bestCut.x = i;
                bestCut.y = j;
                score = s;
            }
        }
    }
}

struct Hoge {
	int i0, i1, j0, j1;
    Cut c;
	vector<Hoge> a;
	pair<uint32_t, double> fval;
	Hoge(int i0, int i1, int j0, int j1) : i0(i0), i1(i1), j0(j0), j1(j1), fval(0, -1) {
        c.type = InvalidCut;
    }
	Hoge(Hoge x, Hoge y, int i0, int i1, int j0, int j1, Cut c)  : i0(i0), i1(i1), j0(j0), j1(j1), c(c), fval(0, -1) {
		a.push_back(x);
		a.push_back(y);
	}
    Hoge(Hoge xa, Hoge xb, Hoge xc, Hoge xd, int i0, int i1, int j0, int j1, Cut c)  : i0(i0), i1(i1), j0(j0), j1(j1), c(c), fval(0, -1) {
        a.push_back(xa);
        a.push_back(xb);
        a.push_back(xc);
        a.push_back(xd);
    }

	double calcScore() {
		double r = 0;
		if ((i1-i0)*(j1-j0) == 400*400) {
			r += 5;
		}
		if (c.type == InvalidCut) {
			return r + calcSim();
		}
        if (c.type == VCut || c.type == HCut) {
            r += round(7.0 * 400 * 400 / ((i1 - i0) * (j1 - j0)));
            if (c.type == VCut) {
                int area0 = (c.x - i0) * (j1 - j0);
                int area1 = (i1 - c.x) * (j1 - j0);
                r += round(5.0 * 400 * 400 / max(area0, area1));
            } else {
                int area0 = (i1 - i0) * (c.y - j0);
                int area1 = (i1 - i0) * (j1 - c.y);
                r += round(5.0 * 400 * 400 / max(area0, area1));
            }
            r += a[0].calcScore();
            r += a[1].calcScore();
            return r;
        }
        if (c.type == PCut) {
            r += round(10.0 * 400 * 400 / ((i1 - i0) * (j1 - j0)));

            int argmin = -1;
            int areaMin = 10000000;
            int areas[4] = {
                    (c.x - i0) * (c.y - j0),
                    (i1 - c.x) * (c.y - j0),
                    (i1 - c.x) * (j1 - c.y),
                    (c.x - i0) * (j1 - c.y)
            };
            for (int i = 0; i < 4; ++i) {
                if (areas[i] < areaMin) {
                    areaMin = areas[i];
                    argmin = i;
                }
            }

            for (int i = 0; i < 4; i++) {
                if (i != argmin) {
                    r += round(5.0 * 400 * 400 / areas[i]);
                }
            }

            /*
            int areaM = 0;
            int area0 = (c.x - i0) * (c.y - j0);
            int area1 = (i1 - c.x) * (c.y - j0);
            int area2 = (i1 - c.x) * (j1 - c.y);
            int area3 = (c.x - i0) * (j1 - c.y);

            areaMax = max(max(area0, area1), max(area2, area3));

            */

            r += a[0].calcScore();
            r += a[1].calcScore();
            r += a[2].calcScore();
            r += a[3].calcScore();
            return r;
        }
        return r;
	}
	uint32_t getColor() {
		if (fval.second < 0) fval = f(i0,i1,j0,j1);
		return fval.first;
	}
	double calcSim() {
		if (fval.second < 0) fval = f(i0,i1,j0,j1);
		return fval.second * 0.005;
	}
	uint32_t getRootColor() {
		if (c.type == InvalidCut) {
			return getColor();
		} else if (c.type == VCut) {
			int area0 = (c.x-i0)*(j1-j0);
			int area1 = (i1-c.x)*(j1-j0);
			if (area0 <= area1) {
				return a[0].getRootColor();
			} else {
				return a[1].getRootColor();
			}
		} else if (c.type == HCut) {
			int area0 = (i1-i0)*(c.y-j0);
			int area1 = (i1-i0)*(j1-c.y);
			if (area0 <= area1) {
				return a[0].getRootColor();
			} else {
				return a[1].getRootColor();
			}
		} else {
            int argmin = -1;
            int areaMin = 10000000;
            int areas[4] = {
                    (c.x - i0) * (c.y - j0),
                    (i1 - c.x) * (c.y - j0),
                    (i1 - c.x) * (j1 - c.y),
                    (c.x - i0) * (j1 - c.y)
            };
            for (int i = 0; i < 4; ++i) {
                if (areas[i] < areaMin) {
                    areaMin = areas[i];
                    argmin = i;
                }
            }
            return a[argmin].getRootColor();
        }
	}
	void output(string blockId, bool isRoot=false) {
		if (isRoot) {
			col(blockId, getRootColor());
		}
		if (c.type == InvalidCut) {
			// do nothing
		} else if (c.type == VCut) {
			int area0 = (c.x-i0)*(j1-j0);
			int area1 = (i1-c.x)*(j1-j0);
			cout << "cut ["<<blockId<<"] [X] ["<<c.x<<"]"<<endl;
			if (area0 <= area1) {
				col(blockId+".1", a[1].getRootColor());
			} else {
				col(blockId+".0", a[0].getRootColor());
			}
			a[0].output(blockId+".0");
			a[1].output(blockId+".1");
		} else if (c.type == HCut) {
			int area0 = (i1-i0)*(c.y-j0);
			int area1 = (i1-i0)*(j1-c.y);
			cout << "cut ["<<blockId<<"] [Y] ["<<c.y<<"]"<<endl;
			if (area0 <= area1) {
				col(blockId+".1", a[1].getRootColor());
			} else {
				col(blockId+".0", a[0].getRootColor());
			}
			a[0].output(blockId+".0");
			a[1].output(blockId+".1");
		} else {
            cout << "cut ["<<blockId<<"] ["<<c.x<<","<<c.y<<"]"<<endl;

            int argmin = -1;
            int areaMin = -1;
            int areas[4] = {
                    (c.x - i0) * (c.y - j0),
                    (i1 - c.x) * (c.y - j0),
                    (i1 - c.x) * (j1 - c.y),
                    (c.x - i0) * (j1 - c.y)
            };
            for (int i = 0; i < 4; ++i) {
                if (areaMin <= areas[i]) {
                    areaMin = areas[i];
                    argmin = i;
                }
            }
            for (int i = 0; i < 4; i++) {
                if (i != argmin) {
                    col(blockId+"." + std::to_string(i), a[i].getRootColor());
                }
            }

            for (int i = 0; i < 4; i++) {
                a[i].output(blockId+"." + std::to_string(i));
            }
        }
	}
};
Hoge walk(int i0, int i1, int j0, int j1) {
	auto res = Hoge(i0, i1, j0, j1);
	double score = res.calcScore();

    {
        set <Cut> cuts;

        for (int i = 40; i <= 100; ++i) {
            auto p = calcVHCut(i0, i1, j0, j1, i * i);
            if (p.type != InvalidCut) {
                cuts.insert(p);
            }
        }

        vector <Cut> cuts2(cuts.begin(), cuts.end());
        random_shuffle(cuts2.begin(), cuts2.end());
        int walkArea = (i1 - i0) * (j1 - j0);
        int n = 2;
        if (walkArea <= 50 * 50) n = 5;
        else if (walkArea <= 80 * 80) n = 3;
        for (int i = 0; i < n && i < (int) cuts2.size(); ++i) {
            auto p = cuts2[i];
            if (p.type == VCut) {
                int i = p.x;
                auto h0 = walk(i0, i, j0, j1);
                auto h1 = walk(i, i1, j0, j1);
                auto h = Hoge(h0, h1, i0, i1, j0, j1, p);
                auto s = h.calcScore();
                if (s < score) {
                    res = h;
                    score = s;
                }
            } else if (p.type == HCut) {
                int j = p.y;
                auto h0 = walk(i0, i1, j0, j);
                auto h1 = walk(i0, i1, j, j1);
                auto h = Hoge(h0, h1, i0, i1, j0, j1, p);
                auto s = h.calcScore();
                if (s < score) {
                    res = h;
                    score = s;
                }
            }
        }
    }
    {
        set <Cut> cuts;

        for (int i = 40; i <= 100; ++i) {
            if (checkPCut(i0, i1, j0, j1, i * i)) {
                cuts.emplace(i0, i1, j0, j1, i * i);
            }
        }

        vector <Cut> cuts2(cuts.begin(), cuts.end());
        random_shuffle(cuts2.begin(), cuts2.end());
        int walkArea = (i1 - i0) * (j1 - j0);
        int n = 1;
        for (int it = 0; it < n && it < (int) cuts2.size(); ++it) {
            auto p = cuts2[it];
            calcPCut(p);
            //printf("pcut (%d,%d,%d,%d)\n", i0, i1, j0, j1);
            int i = p.x;
            int j = p.y;

            auto h0 = walk(i0, i, j0, j);
            auto h1 = walk(i, i1, j0, j);
            auto h2 = walk(i, i1, j, j1);
            auto h3 = walk(i0, i, j, j1);

            auto h = Hoge(h0, h1, h2, h3, i0, i1, j0, j1, p);
            auto s = h.calcScore();
            if (s < score) {
                //printf("pcut (%d,%d,%d,%d) selected\n", i0, i1, j0, j1);
                res = h;
                score = s;
            }
        }
    }


	return res;
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
	Hoge hoge = walk(0, 400, 0, 400);
	cerr << hoge.calcScore() << endl;
	hoge.output("0", true);
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
	cerr << "sumsst="<<sumsst << endl;
}
