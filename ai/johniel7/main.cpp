#include <bits/stdc++.h>

#define each(i, c) for (auto& i : c)
#define unless(cond) if (!(cond))

using namespace std;

template<typename P, typename Q> ostream& operator << (ostream& os, pair<P, Q> p) { os << "(" << p.first << "," << p.second << ")"; return os; }
template<typename P, typename Q> istream& operator >> (istream& is, pair<P, Q>& p) { is >> p.first >> p.second; return is; }
template<typename T> ostream& operator << (ostream& os, vector<T> v) { os << "("; for (auto& i: v) os << i << ","; os << ")"; return os; }
template<typename T> istream& operator >> (istream& is, vector<T>& v) { for (auto& i: v) is >> i; return is; }
template<typename T> ostream& operator << (ostream& os, set<T> s) { os << "#{"; for (auto& i: s) os << i << ","; os << "}"; return os; }
template<typename K, typename V> ostream& operator << (ostream& os, map<K, V> m) { os << "{"; for (auto& i: m) os << i << ","; os << "}"; return os; }
template<typename E, size_t N> istream& operator >> (istream& is, array<E, N>& a) { for (auto& i: a) is >> i; return is; }
template<typename E, size_t N> ostream& operator << (ostream& os, array<E, N>& a) { os << "[]{"; for (auto& i: a) os << i << ","; os << "}"; return os; }

template<typename T> inline T setmax(T& a, T b) { return a = std::max(a, b); }
template<typename T> inline T setmin(T& a, T b) { return a = std::min(a, b); }

using lli = long long int;
using ull = unsigned long long;
using str = string;
template<typename T> using vec = vector<T>;

constexpr array<int, 8> di({0, 1, -1, 0, 1, -1, 1, -1});
constexpr array<int, 8> dj({1, 0, 0, -1, 1, -1, -1, 1});

constexpr char LINE_CUT_TYPE_I = 'Y';
constexpr char LINE_CUT_TYPE_J = 'X';

constexpr lli BASECOST_LINE_CUT = 7;
constexpr lli BASECOST_POINT_CUT = 10;
constexpr lli BASECOST_COLOR = 5;
constexpr lli BASECOST_SWAP = 3;
constexpr lli BASECOST_MERGE = 1;

constexpr int TYPE_LINE_CUT = 0;
constexpr int TYPE_POINT_CUT = 1;
constexpr int TYPE_COLOR = 2;
constexpr int TYPE_SWAP = 3;
constexpr int TYPE_MERGE = 4;

const int N = 400;

struct Color {
  lli R, G, B, A;
  Color() { R = G = B = A = 0; }
  Color(lli r, lli g, lli b, lli a) : R(r), G(b), B(b), A(a) {}
  bool valid(void) const { return (0 <= R && R < 256) && (0 <= G && G < 256) && (0 <= B && B < 256) && (0 <= A && A < 256); }
};

Color& operator += (Color& x, const Color& y) {
  x.R += y.R;
  x.G += y.G;
  x.B += y.B;
  x.A += y.A;
  return x;
}

Color& operator -= (Color& x, const Color& y) {
  x.R -= y.R;
  x.G -= y.G;
  x.B -= y.B;
  x.A -= y.A;
  return x;
}

Color& operator *= (Color& x, lli n) {
  x.R *= n;
  x.G *= n;
  x.B *= n;
  x.A *= n;
  return x;
}

Color& operator /= (Color& x, lli n) {
  x.R /= n;
  x.G /= n;
  x.B /= n;
  x.A /= n;
  return x;
}

Color operator + (const Color& x, const Color& y) {
  Color z;
  z += x;
  z += y;
  return z;
}

Color operator - (const Color& x, const Color& y) {
  Color z;
  z -= x;
  z -= y;
  return z;
}

Color operator * (Color x, lli n) {
  x.R *= n;
  x.G *= n;
  x.B *= n;
  x.A *= n;
  return x;
}

Color operator / (Color x, lli n) {
  x.R /= n;
  x.G /= n;
  x.B /= n;
  x.A /= n;
  return x;
}

ostream& operator << (ostream& os, const Color& c)
{
  os << "Color{" << (int)c.R << "," << (int)c.G << "," << (int)c.B << "," << (int)c.A << "}";
  return os;
}

template<typename T>
struct PrefixSum2D {
  vector<vector<T>> sum;
  PrefixSum2D() {}
  PrefixSum2D(vector<vector<T>> v) {
    const int H = v.size() + 1;
    const int W = v[0].size() + 1;
    sum.resize(H);
    fill(sum.begin(), sum.end(), vector<T>(W, 0));
    for (int i = 0; i < v.size(); ++i) {
      for (int j = 0; j < v[i].size(); ++j) {
        sum[i + 1][j + 1] = v[i][j];
      }
    }
    for (int i = 1; i < H; ++i) {
      for (int j = 0; j < W; ++j) {
        sum[i][j] += sum[i - 1][j];
      }
    }
    for (int i = 0; i < H; ++i) {
      for (int j = 1; j < W; ++j) {
        sum[i][j] += sum[i][j - 1];
      }
    }
  }
  T query(int ai, int aj, int bi, int bj) const {
    assert(0 <= ai && ai < sum.size());
    assert(0 <= bi && bi < sum.size());
    assert(0 <= aj && aj < sum[0].size());
    assert(0 <= bj && bj < sum[0].size());
    assert(ai <= bi && aj <= bj);
    return sum[bi][bj] + sum[ai][aj] - sum[ai][bj] - sum[bi][aj];
  }
};

int width = -1, height = -1;
uint8_t* Image;

uint8_t get(int i, int j, int k)
{
	return Image[((height - 1 - i) * width + j) * 4 + k];
}

uint32_t get(int i, int j)
{
	return *(uint32_t*)&Image[((height - 1 - i) * width + j) * 4];
}

Color getColor(int i, int j)
{
  return Color(get(i, j, 0),
               get(i, j, 1),
               get(i, j, 2),
               get(i, j, 3));
}

PrefixSum2D<lli> colorsumR;
PrefixSum2D<lli> colorsumG;
PrefixSum2D<lli> colorsumB;
PrefixSum2D<lli> colorsumA;

void buildColorPrefixSum2D(void)
{
  vec<vec<lli>> r(height, vec<lli>(width, 0));
  vec<vec<lli>> g(height, vec<lli>(width, 0));
  vec<vec<lli>> b(height, vec<lli>(width, 0));
  vec<vec<lli>> a(height, vec<lli>(width, 0));
  for (int i = 0; i < width; ++i) {
    for (int j = 0; j < height; ++j) {
      r[i][j] = get(i, j, 0);
      g[i][j] = get(i, j, 1);
      b[i][j] = get(i, j, 2);
      a[i][j] = get(i, j, 3);
    }
  }
  colorsumR = PrefixSum2D(r);
  colorsumG = PrefixSum2D(g);
  colorsumB = PrefixSum2D(b);
  colorsumA = PrefixSum2D(a);
  return ;
}

namespace y1r {
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
  
// https://github.com/hasipon/icfpc2022/blob/ce61bc5057d8405986457866382d4fc8802a5c3c/ueno-v4/a.cpp#L37-L149
  pair<uint32_t, double> f(int i0, int i1, int j0, int j1) {
    lli sum[3] = {0};
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
};

Color average(int mni, int mnj, int mxi, int mxj)
{
  assert(mni <= mxi);
  assert(mnj <= mxj);
  lli area = (mxi - mni) * (mxj - mnj);
  Color c;
  c.R = colorsumR.query(mni, mnj, mxi, mxj);
  c.G = colorsumG.query(mni, mnj, mxi, mxj);
  c.B = colorsumB.query(mni, mnj, mxi, mxj);
  c.A = colorsumA.query(mni, mnj, mxi, mxj);
  assert(area);
  c /= area;
  assert(c.valid());
  return c;
}

Color bestColor(int mni, int mnj, int mxi, int mxj)
{
  assert(mni <= mxi);
  assert(mnj <= mxj);
  uint32_t x = y1r::f(mni, mxi, mnj, mxj).first;
  int r = ((uint8_t*)&x)[0];
  int g = ((uint8_t*)&x)[1];
  int b = ((uint8_t*)&x)[2];
  int a = ((uint8_t*)&x)[3];
  return Color(r, g, b, a);
}  

double calcSimulality(int mni, int mnj, int mxi, int mxj, Color c)
{
  double z = 0;
  for (int i = mni; i < mxi; ++i) {
    for (int j = mnj; j < mxj; ++j) {
      double r = get(i, j, 0) - c.R;
      double g = get(i, j, 1) - c.G;
      double b = get(i, j, 2) - c.B;
      double a = get(i, j, 3) - c.A;
      r *= r;
      g *= g;
      b *= b;
      a *= a;
      z += sqrt(r + g + b + a);
    }
  }
  return round(z * 0.005);
}

Color findColor(int mni, int mnj, int mxi, int mxj)
{
  Color z = average(mni, mnj, mxi, mxj);
  double sim;
  int d = 200;
  for (int _ = 0; _ < 18; ++_) {
    int len = _ / 2;
    vec<int> v;
    v.push_back(z.R);
    v.push_back(z.G);
    v.push_back(z.B);
    double mn = 1e128;
    for (int i = 0; i < v.size(); ++i) {
      for (int j = -1; j <= 1; ++j) {
        unless (j) continue;
        v[i] += j * (1 << len);
        Color c(v[0], v[1], v[2], 255);
        if (c.valid()) {
          sim = calcSimulality(mni, mnj, mxi, mxj, c);
          if (mn > sim) {
            mn = sim;
            z = c;
          }
        }
        v[i] -= j * (1 << len);
      }
    }
  }
  return z;
}

struct Block {
  int mni, mnj;
  int mxi, mxj;
  str id;
  Color c;
  double _simulality;

  str hash(void) const {
    char buff[500];
    sprintf(buff, "%d,%d,%d,%d-%d,%d,%d,%d", mni, mnj, mxi, mxj, c.R, c.G, c.B, c.A);
    // sprintf(buff, "%s-%d%d%d%d-%d%d%d%d", id.c_str(), mni, mnj, mxi, mxj, c.R, c.G, c.B, c.A);
    return str(buff);
  }

  Block(int mni_, int mnj_, int mxi_, int mxj_, str id_, Color c_)
    : mni(mni_), mnj(mnj_), mxi(mxi_), mxj(mxj_), id(id_), c(c_), _simulality(calcSimulality(mni, mnj, mxi, mxj, c)) {
    assert(0 <= mni && mni < mxi && mxi <= 400);
    assert(0 <= mnj && mnj < mxj && mxj <= 400);
  }

  pair<str, vec<Block>> lineCutI(int k) const { return lineCut(k, LINE_CUT_TYPE_I); }
  pair<str, vec<Block>> lineCutJ(int k) const { return lineCut(k, LINE_CUT_TYPE_J); }
  
  pair<str, vec<Block>> lineCut(int k, char type) const {
    assert(type == LINE_CUT_TYPE_I || type == LINE_CUT_TYPE_J);
    if (LINE_CUT_TYPE_I == type) assert(mni < k && k < mxi);
    if (LINE_CUT_TYPE_J == type) assert(mnj < k && k < mxj);

    char buff[500];
    sprintf(buff, "cut [%s] [%c] [%d]", id.c_str(), type, k);
    vec<Block> v;
    if (LINE_CUT_TYPE_I == type) {
      v.push_back(Block(mni, mnj,   k, mxj, id + ".0", c));
      v.push_back(Block(  k, mnj, mxi, mxj, id + ".1", c));
    }
    if (LINE_CUT_TYPE_J == type) {
      v.push_back(Block(mni, mnj, mxi,   k, id + ".0", c));
      v.push_back(Block(mni,   k, mxi, mxj, id + ".1", c));
    }

    // double w = simulality();
    // each (k, v) w -= k.simulality();
    // if (w < 0) {
    //   v.push_back(*this);
    //   cerr << v << endl;
    //   each (k, v) cerr << k.simulality() << endl;
    //   assert(!(w < 0));
    // }
    return make_pair(str(buff), v);
  }

  pair<str, vec<Block>> pointCut(int i, int j) const {
    assert(mni < i && i < mxi);
    assert(mnj < j && j < mxj);

    Block b0(mni, mnj,   i,   j, id + ".0", c);
    Block b1(  i, mnj, mxi,   j, id + ".1", c);
    Block b2(  i,   j, mxi, mxj, id + ".2", c);
    Block b3(mni,   j,   i, mxj, id + ".3", c);
    vec<Block> v({b0, b1, b2, b3});
    char buff[500];
    sprintf(buff, "cut [%s] [%d,%d]", id.c_str(), i, j);
    return make_pair(str(buff), v);
  }

  pair<str, Block> color(Color d) const
  {
    char buff[500];
    sprintf(buff, "color [%s] [%d,%d,%d,%d]", id.c_str(), d.R, d.G, d.B, d.A);
    Block b = Block{mni, mnj, mxi, mxj, id, d};
    return make_pair(str(buff), b);
  }

  pair<str, Block> colorAv(void) const
  {
    Color d = average(mni, mnj, mxi, mxj);
    return color(d);
  }

  double simulality(void) const { return _simulality; }
  double simulality(const Color d) const { return calcSimulality(mni, mnj, mxi, mxj, d); }
  double simulalityAv(void) const { return simulality(average(mni, mnj, mxi, mxj)); }
  lli area(void) const { return (mxi - mni) * (mxj - mnj); }
};

ostream& operator << (ostream& os, const Block& b)
{
  os << "Block{" << b.id << make_pair(b.mni, b.mnj) << make_pair(b.mxi, b.mxj) << b.c << ",sim:" << b.simulality() << "}";
  return os;
}

Block initialBlock(void) { return Block(0, 0, 400, 400, "0", Color(255, 255, 255, 255)); }

lli calcCost(int move, const Block& b)
{
  double area = b.area();
  double C = height * width;
  switch (move) {
    case TYPE_LINE_CUT:
      return BASECOST_LINE_CUT * C / area;
    case TYPE_POINT_CUT:
      return BASECOST_POINT_CUT * C / area;
    case TYPE_COLOR:
      return BASECOST_COLOR * C / area;
    case TYPE_SWAP:
      return BASECOST_SWAP * C / area;
    case TYPE_MERGE:
      return BASECOST_MERGE * C / area;
  };
  return 1LL << 60;
}

struct State {
  vec<Block> bs;
  vec<str> cmd;
  double cost;
  double sim;
  State(){}
  State(Block b) {
    bs.push_back(b);
    cost = 0;
    sim = simulality();
  }
  double simulality(void) const {
    double z = 0;
    each (k, bs) z += k.simulality();
    return z;
  }
  double simulalityAv(void) const {
    double z = 0;
    each (k, bs) z += k.simulalityAv();
    return z;
  }
  void output(void) const {
    if (cmd.empty()) cout << "#" << endl;
    each (k, cmd) cout << k << endl;
  }
  bool empty() const { return bs.empty(); }

  double ifColorAv(void) const { return ifColorAvLastN(bs.size()); }

  double ifColorAvLastN(int n) const {
    double c = cost;
    double s = sim;
    for (int i = 0; i < n; ++i) {
      const Block& b = bs[bs.size() - i - 1];
      double curr = b.simulality();
      double x = calcCost(TYPE_COLOR, b);
      double y = b.simulalityAv();
      if (curr > x + y) {
        s -= curr;
        c += x;
        s += y;
      }
    }
    if (s < 0) {
      cerr << s << endl;
      cerr << cost << ' ' << sim << endl;
      for (int i = 0; i < n; ++i) {
        const Block& b = bs[bs.size() - i - 1];
        cerr << b << ", " << b.simulalityAv() << endl;
      }
      exit(1);
    }
    return c + s;
  }

  str hash(void) const {
    vec<str> v;
    each (b, bs) {
      v.push_back(b.hash());
      v.push_back("&");
    }
    sort(v.begin(), v.end());
    return accumulate(v.begin(), v.end(), str(""));
  }
  double score(void) const { return cost + sim; }

  bool validate(void) const {
    double s = sim;
    each (b, bs) {
      s -= b.simulality();
      if (0 <= s) return false;
    }
    return true;
  }
};
bool operator < (const State& a, const State& b) { return a.score() < b.score(); }

State replace(const State& s, int nth, const vec<Block>& bs, const int type, const str cmd)
{
  State t = s;
  t.cost += calcCost(TYPE_COLOR, t.bs[nth]);
  t.sim -= t.bs[nth].simulality();
  t.bs.erase(t.bs.begin() + nth);
  t.cmd.push_back(cmd);
  each (b, bs) {
    t.sim += b.simulality();
    t.bs.push_back(b);
  }
  // assert(t.sim == t.simulality());
  return t;
}

State colorBlock(const State& s, const int nth, const Color c)
{
  State t = s;
  t.cost += calcCost(TYPE_COLOR, t.bs[nth]);
  t.sim -= t.bs[nth].simulality();
  auto p = t.bs[nth].color(c);
  t.bs[nth] = p.second;
  t.cmd.push_back(p.first);
  t.sim += t.bs[nth].simulality();
  // assert(t.sim == t.simulality());
  return t;
}

State colorBlockAv(const State& s, const int nth)
{
  const Block& b = s.bs[nth];
  Color c = average(b.mni, b.mnj, b.mxi, b.mxj);
  return colorBlock(s, nth, c);
}

State colorBlockBest(const State& s, const int nth)
{
  const Block& b = s.bs[nth];
  Color c = bestColor(b.mni, b.mnj, b.mxi, b.mxj);
  return colorBlock(s, nth, c);
}

State colorBlockAvAll(const State& s)
{
  State t = s;
  for (int i = 0; i < t.bs.size(); ++i) {
    t = colorBlockAv(t, i);
  }
  return t;
}

State colorBlockBestAll(const State& s)
{
  State t = s;
  for (int i = 0; i < t.bs.size(); ++i) {
    t = colorBlockBest(t, i);
  }
  return t;
}

State lineCutBlock(const State& s, const int nth, const int k, const char type)
{
  auto p = s.bs[nth].lineCut(k, type);
  return replace(s, nth, p.second, TYPE_LINE_CUT, p.first);
}

State pointCutBlock(const State& s, const int nth, const int i, const int j)
{
  auto p = s.bs[nth].pointCut(i, j);
  return replace(s, nth, p.second, TYPE_POINT_CUT, p.first);
}

namespace johniel5 {
  map<str, pair<int, char>> memoI;
  map<str, pair<int, char>> memoJ;
  vec<State> cutOneBlock(const State& s)
  {
    vec<State> v;
    for (int k = 0; k < s.bs.size(); ++k) {
      double sim0 = 1e50;
      const Block& b = s.bs[k];
      pair<int, char> bestI = {-1, 'a'};
      pair<int, char> bestJ = {-1, 'a'};
      
      if (memoI.count(b.hash())) {
        bestI = memoI[b.hash()];
      } else {
        for (int i = b.mni + 1; i < b.mxi; ++i) {
          auto p = b.lineCut(i, LINE_CUT_TYPE_I);
          double sim = 0;
          sim += p.second[0].simulalityAv();
          sim += p.second[1].simulalityAv();
          sim += calcCost(TYPE_LINE_CUT, b);
          if (sim < sim0) {
            bestI = {i, LINE_CUT_TYPE_I};
            sim0 = sim;
          }
        }
        memoI[b.hash()] = bestI;
      }

      if (memoJ.count(b.hash())) {
        bestJ = memoJ[b.hash()];
      } else {
        for (int j = b.mnj + 1; j < b.mxj; ++j) {
          auto p = b.lineCut(j, LINE_CUT_TYPE_J);
          double sim = 0;
          sim += p.second[0].simulalityAv();
          sim += p.second[1].simulalityAv();
          sim += calcCost(TYPE_LINE_CUT, b);
          if (sim < sim0) {
            bestJ = {j, LINE_CUT_TYPE_J};
            sim0 = sim;
          }
        }
        memoJ[b.hash()] = bestJ;
      }
      if (bestI.first != -1) {
        State t = lineCutBlock(s, k, bestI.first, bestI.second);
        v.push_back(t);
      }
      if (bestJ.first != -1) {
        State t = lineCutBlock(s, k, bestJ.first, bestJ.second);
        v.push_back(t);
      }
      if (bestI.first != -1 && bestJ.first != -1) {
        State t = pointCutBlock(s, k, bestI.first, bestJ.first);
        v.push_back(t);
      }
    }
    return v;
  }
}

void solve(void)
{
  buildColorPrefixSum2D();
  if (width != 400) throw 1;
  if (height != 400) throw 1;

  State best(initialBlock());
  priority_queue<State> q;
  q.push(best);
  const lli WIDTH = 1000;
  const int STEP = 30;
  for (int _ = 0; _ < STEP && q.size(); ++_) {
    clog << _ << "th-gen,queue.size:" << q.size() << "," << "best.score=" << best.sim <<"＋" << best.cost << ",H:" << best.ifColorAv() << ",cmd.size:" << best.cmd.size() << ",cache-size:" << make_pair(johniel5::memoI.size(),johniel5::memoJ.size()) << endl;
    priority_queue<State> nq;
    set<str> vis;
    while (q.size()) {
      if (q.top().ifColorAv() < best.ifColorAv()) best = q.top();
      auto t = johniel5::cutOneBlock(q.top());
      q.pop();
      each (k, t) {
        str h = k.hash();
        if (vis.count(h)) continue;
        vis.insert(h);
        nq.push(k);
      }
      while (nq.size() && WIDTH < nq.size()) nq.pop();
    }
    q = nq;
  }
  best = colorBlockBestAll(best);
  best.output();
  clog << best.score() << "," << best.cmd.size() << endl;
  return ;
}

int main(int argc, char *argv[])
{
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

  return 0;
}
