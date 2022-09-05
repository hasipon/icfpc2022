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

uint32_t xorshift(void)
{
  static uint32_t x = 123456789;
  static uint32_t y = 362436069;
  static uint32_t z = 521288629;
  static uint32_t w = 88675123;
  uint32_t t;

  t = x ^ (x << 11);
  x = y; y = z; z = w;
  return w = (w ^ (w >> 19)) ^ (t ^ (t >> 8));
}

string now(void)
{
  time_t t = time(nullptr);
  tm* now = localtime(&t);

  char buffer[128];
  strftime(buffer, sizeof(buffer), "%m-%d-%Y %X", now);
  return str(buffer);
}

using Hash = ull;
Hash rhash(string s)
{
  const ull P = 709;
  ull h = 0;
  each (c, s) h = (h * P) + (tolower(c) - 'a' + 1);
  return h;
}

constexpr char LINE_CUT_TYPE_I = 'Y';
constexpr char LINE_CUT_TYPE_J = 'X';

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

bool operator == (const Color& x, const Color& y)
{
  return (x.R == y.R) && (x.G == y.G) && (x.B == y.B) && (x.A == y.A);
}

bool operator < (const Color& x, const Color& y)
{
  if (x.R != y.R) return x.R < y.R;
  if (x.G != y.G) return x.G < y.G;
  if (x.B != y.B) return x.B < y.B;
  if (x.A != y.A) return x.A < y.A;
  return false;
}

bool operator != (const Color& x, const Color& y)
{
  return !(x == y);
}

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
  static map<vec<lli>, double> memo;
  vec<lli> key({mni, mnj, mxi, mxj, c.R, c.G, c.B, c.A});
  if (memo.count(key)) {
    return memo[key];
  }

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
  return memo[key] = round(z * 0.005);
}

struct SimpleBlock {
  int mni, mnj;
  int mxi, mxj;
  Color c;
  double _simulality;
  double _simulalityAv;

  SimpleBlock(int mni_, int mnj_, int mxi_, int mxj_, str id_, Color c_)
    : mni(mni_), mnj(mnj_), mxi(mxi_), mxj(mxj_), id(id_), c(c_),
      _simulality(calcSimulality(mni, mnj, mxi, mxj, c)),
      _simulalityAv(calcSimulality(mni, mnj, mxi, mxj, average(mni, mnj, mxi, mxj))) {
    assert(0 <= mni && mni < mxi && mxi <= 400);
    assert(0 <= mnj && mnj < mxj && mxj <= 400);

    char buff[500];
    sprintf(buff, "%d,%d,%d,%d-%d,%d,%d,%d", mni, mnj, mxi, mxj, c.R, c.G, c.B, c.A);
    h = rhash(str(buff));
  }

  Hash hash(void) const { return h; }

  pair<str, vec<SimpleBlock>> lineCutI(int k) const { return lineCut(k, LINE_CUT_TYPE_I); }
  pair<str, vec<SimpleBlock>> lineCutJ(int k) const { return lineCut(k, LINE_CUT_TYPE_J); }

  pair<str, vec<SimpleBlock>> lineCut(int k, char type) const {
    assert(type == LINE_CUT_TYPE_I || type == LINE_CUT_TYPE_J);
    if (LINE_CUT_TYPE_I == type) assert(mni < k && k < mxi);
    if (LINE_CUT_TYPE_J == type) assert(mnj < k && k < mxj);

    char buff[500];
    sprintf(buff, "cut [%s] [%c] [%d]", id.c_str(), type, k);
    vec<SimpleBlock> v;
    if (LINE_CUT_TYPE_I == type) {
      v.push_back(SimpleBlock(mni, mnj,   k, mxj, id + ".0", c));
      v.push_back(SimpleBlock(  k, mnj, mxi, mxj, id + ".1", c));
    }
    if (LINE_CUT_TYPE_J == type) {
      v.push_back(SimpleBlock(mni, mnj, mxi,   k, id + ".0", c));
      v.push_back(SimpleBlock(mni,   k, mxi, mxj, id + ".1", c));
    }
    return make_pair(str(buff), v);
  }

  pair<str, vec<SimpleBlock>> pointCut(int i, int j) const {
    assert(mni < i && i < mxi);
    assert(mnj < j && j < mxj);
    SimpleBlock b0(mni, mnj,   i,   j, id + ".0", c);
    SimpleBlock b1(  i, mnj, mxi,   j, id + ".1", c);
    SimpleBlock b2(  i,   j, mxi, mxj, id + ".2", c);
    SimpleBlock b3(mni,   j,   i, mxj, id + ".3", c);
    vec<SimpleBlock> v({b0, b1, b2, b3});
    char buff[500];
    sprintf(buff, "cut [%s] [%d,%d]", id.c_str(), i, j);
    return make_pair(str(buff), v);
  }

  pair<str, SimpleBlock> color(Color d) const {
    char buff[500];
    sprintf(buff, "color [%s] [%d,%d,%d,%d]", id.c_str(), d.R, d.G, d.B, d.A);
    SimpleBlock b = SimpleBlock{mni, mnj, mxi, mxj, id, d};
    return make_pair(str(buff), b);
  }
  pair<str, SimpleBlock> colorAv(void) const { return color(average(mni, mnj, mxi, mxj)); }
};

ostream& operator << (ostream& os, const SimpleBlock& b)
{
  os << "SimpleBlock{" << b.id << make_pair(b.mni, b.mnj) << make_pair(b.mxi, b.mxj) << b.c << ",sim:" << b.simulality() << "}";
  return os;
}

struct Block {
  int mni, mnj;
  int mxi, mxj;
  str id;
  double _simulality;
  double _simulalityAv;

  vec<SimpleBlock> bs;
  Block(SimpleBlock b) {
    mni = mnj = +(1 << 29);
    mxi = mxj = -(1 << 29);
  }

  Block append(SimpleBlock sb) const {
    Block b = *this;
    b.bs.push_back(sb);
    b._simulality = sb._simulality;
    b._simulalityAv = sb._simulalityAv;
    return b;
  }

  double simulality(void) const { return _simulality; }
  double simulality(const Color d) const { return calcSimulality(mni, mnj, mxi, mxj, d); }
  double simulalityAv(void) const {
    auto a = simulality();
    auto b = simulality(average(mni, mnj, mxi, mxj));
    return min(a, b);
  }

  lli area(void) const { return (mxi - mni) * (mxj - mnj); }

  bool sameShape(const SimpleBlock& other) const
  {
    return (mxi - mni) == (other.mxi - other.mni) && (mxj - mnj) == (other.mxj - other.mnj);
  }

  bool mergeable(const SimpleBlock& other) const
  {
    vec<pair<int, int>> v;
    v.push_back({mni, mnj});
    v.push_back({mxi, mnj});
    v.push_back({mxi, mxj});
    v.push_back({mni, mxj});
    vec<pair<int, int>> u;
    u.push_back({other.mni, other.mnj});
    u.push_back({other.mxi, other.mnj});
    u.push_back({other.mxi, other.mxj});
    u.push_back({other.mni, other.mxj});
    for (int i = 0; i < v.size(); ++i) {
      for (int j = 0; j < u.size(); ++j) {
        if (v[i] == u[j] && v[(i + 1) % v.size()] == u[(j + 1) % u.size()]) {
          return true;
        }
      }
    }
    reverse(u.begin(), u.end());
    for (int i = 0; i < v.size(); ++i) {
      for (int j = 0; j < u.size(); ++j) {
        if (v[i] == u[j] && v[(i + 1) % v.size()] == u[(j + 1) % u.size()]) {
          return true;
        }
      }
    }
    return false;
  }

  SimpleBlock merge(const SimpleBlock& other, str id) const
  {
    return SimpleBlock(min(mni, other.mni),
                 min(mnj, other.mnj),
                 max(mxi, other.mxi),
                 max(mxj, other.mxj),
                 id,
                 c);
  }
};

SimpleBlock initialSimpleBlock(void) { return SimpleBlock(0, 0, 400, 400, "0", Color(255, 255, 255, 255)); }

lli calcCost(int move, const SimpleBlock& b)
{
  constexpr lli BASECOST_LINE_CUT = 7;
  constexpr lli BASECOST_POINT_CUT = 10;
  constexpr lli BASECOST_COLOR = 5;
  constexpr lli BASECOST_SWAP = 3;
  constexpr lli BASECOST_MERGE = 1;

  double area = b.area();
  double C = (height * width);
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

double heuristic(const SimpleBlock& b)
{
  return b.simulality() - b.simulalityAv() + calcCost(TYPE_COLOR, b);
}

struct State {
  vec<SimpleBlock> bs;
  vec<str> cmd;
  double cost;
  // double sim;
  int blockIDCount;
  double sim;
  double simAv;

  State() : blockIDCount(0) {}
  State(SimpleBlock b) : blockIDCount(0) {
    bs.push_back(b);
    cost = 0;
    sim = simAv = 0;
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
    double s = simulality();
    for (int i = 0; i < n; ++i) {
      const SimpleBlock& b = bs[bs.size() - i - 1];
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
      cerr << cost << ' ' << simulality() << ' ' << simulalityAv() << endl;
      for (int i = 0; i < n; ++i) {
        const SimpleBlock& b = bs[bs.size() - i - 1];
        cerr << b << ", " << b.simulality() << ", " << b.simulalityAv() << endl;
      }
      cerr << cmd << endl;
      assert(false);
    }
    return c + s;
  }

  Hash hash(void) const {
    Hash h = 0;
    vec<Hash> v;
    each (b, bs) v.push_back(b.hash());
    sort(v.begin(), v.end());
    each (k, v) {
      const ull P = 709;
      h = h * P + k;
    }
    return h;
  }
  double score(void) const {
    // return cost + sim;
    return cost + simulality();
  }

  bool validate(void) const {
    // double s = sim;
    double s = simulality();
    each (b, bs) {
      s -= b.simulality();
      if (0 <= s) return false;
    }
    return true;
  }

  size_t size(void) const { return bs.size(); }

  State remove(size_t idx) const {
    State t = *this;
    t.sim -= t.bs[idx].simulality();
    t.simAv -= t.bs[idx].simulalityAv();
    t.bs.erase(t.bs.begin() + idx);
    return t;
  }

  State append(const SimpleBlock& b) const {
    State t = *this;
    t.sim += b.simulality();
    t.simAv += b.simulalityAv();
    t.bs.push_back(b);
    return t;
  }

  double heuristic(void) const { return ifColorAv(); }
  const SimpleBlock& operator [] (size_t idx) const { return bs.at(idx); }
};

bool operator < (const State& a, const State& b) { return a.heuristic() < b.heuristic(); }

State replace(State s, int nth, const vec<SimpleBlock>& bs, const int type, const str cmd)
{
  s = s.remove(nth);
  s.cmd.push_back(cmd);
  each (b, bs) s = s.append(b);
  return s;
}

State colorSimpleBlockF(State s, const int nth, const Color c)
{
  auto p = s[nth].color(c);
  s = s.remove(nth);
  s = s.append(p.second);
  s.cmd.push_back(p.first);
  return s;
}

State colorSimpleBlock(const State& s, const int nth, const Color& c)
{
  auto p = s[nth].color(c);
  if (s[nth].simulality() > calcCost(TYPE_COLOR, s[nth]) + p.second.simulalityAv()) {
    State t = s;
    t.cmd.push_back(p.first);
    t = t.append(p.second);
    t = t.remove(nth);
    t.cost += calcCost(TYPE_COLOR, s[nth]);
    return t;
  } else {
    return s;
  }
}

State colorSimpleBlockAv(const State& s, const int nth)
{
  const SimpleBlock& b = s.bs[nth];
  Color c = average(b.mni, b.mnj, b.mxi, b.mxj);
  return colorSimpleBlock(s, nth, c);
}

State colorSimpleBlockBest(const State& s, const int nth)
{
  const SimpleBlock& b = s.bs[nth];
  Color c = bestColor(b.mni, b.mnj, b.mxi, b.mxj);
  return colorSimpleBlock(s, nth, c);
}

State colorSimpleBlockAvAll(const State& s)
{
  State t = s;
  for (int i = 0; i < t.bs.size(); ++i) {
    t = colorSimpleBlockAv(t, i);
  }
  return t;
}

State colorSimpleBlockBestAll(const State& s)
{
  State t = s;
  for (int i = 0; i < t.bs.size(); ++i) {
    t = colorSimpleBlockBest(t, i);
  }
  return t;
}

State lineCutSimpleBlock(const State& s, const int nth, const int k, const char type)
{
  auto p = s[nth].lineCut(k, type);
  State t = replace(s, nth, p.second, TYPE_LINE_CUT, p.first);
  t.cost += calcCost(TYPE_LINE_CUT, s[nth]);
  return t;
}

State pointCutSimpleBlock(const State& s, const int nth, const int i, const int j)
{
  auto p = s[nth].pointCut(i, j);
  State t = replace(s, nth, p.second, TYPE_POINT_CUT, p.first);
  t.cost += calcCost(TYPE_POINT_CUT, s[nth]);
  return t;
}

double dryrunSwapSimpleBlocks(State s, const int x, const int y)
{
  SimpleBlock a(s[x].mni, s[x].mnj, s[x].mxi, s[x].mxj, s[y].id, s[y].c);
  SimpleBlock b(s[y].mni, s[y].mnj, s[y].mxi, s[y].mxj, s[x].id, s[x].c);

  return
    calcCost(TYPE_SWAP, a)
    + a.simulalityAv()
    + b.simulalityAv()
    - s[x].simulalityAv()
    - s[y].simulalityAv();
}

State swapSimpleBlocks(State s, const int x, const int y)
{
  SimpleBlock a(s[x].mni, s[x].mnj, s[x].mxi, s[x].mxj, s[y].id, s[y].c);
  SimpleBlock b(s[y].mni, s[y].mnj, s[y].mxi, s[y].mxj, s[x].id, s[x].c);

  char buff[500];
  sprintf(buff, "swap [%s] [%s]", s[x].id.c_str(), s[y].id.c_str());

  s = s.remove(max(x, y));
  s = s.remove(min(x, y));
  s = s.append(a);
  s = s.append(b);

  s.cmd.push_back(str(buff));
  s.cost += calcCost(TYPE_SWAP, a);
  return s;
}

double dryrunMergeSimpleBlocks(const State& s, const int i, const int j)
{
  const SimpleBlock& a = s[i];
  const SimpleBlock& b = s[j];
  SimpleBlock merged = a.merge(b, "dryrunMergeSimpleBlocks");
  return calcCost(TYPE_MERGE, (a.area() < b.area() ? b : a)) + merged.simulalityAv() - a.simulalityAv() - b.simulalityAv();
}

State mergeSimpleBlocks(State s, const int i, const int j)
{
  SimpleBlock a = s[i];
  SimpleBlock b = s[j];
  str id = to_string(++s.blockIDCount);
  SimpleBlock merged = a.merge(b, id);
  // indexの大きい順
  s = s.remove(max(i, j));
  s = s.remove(min(i, j));
  s = s.append(merged);

  char buff[500];
  sprintf(buff, "merge [%s] [%s]", a.id.c_str(), b.id.c_str());
  s.cmd.push_back(str(buff));
  s.cost += calcCost(TYPE_MERGE, (a.area() < b.area() ? b : a));
  return s;
}

const int THRESHOLD = 50;

namespace johniel5 {
  map<Hash, pair<int, char>> memoI;
  map<Hash, pair<int, char>> memoJ;
  vec<State> phase99(const State& s)
  {
    vec<State> v;
    for (int k = 0; k < s.bs.size(); ++k) {
      const SimpleBlock& b = s.bs[k];
      pair<int, char> bestI = {-1, 'a'};
      pair<int, char> bestJ = {-1, 'a'};
      const Hash hash = b.hash();
      if (memoI.count(hash)) {
        bestI = memoI[hash];
      } else {
        double sim0 = 1e50;
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
        memoI[hash] = bestI;
      }

      if (memoJ.count(hash)) {
        bestJ = memoJ[hash];
      } else {
        double sim0 = 1e50;
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
        memoJ[hash] = bestJ;
      }
      if (bestI.first != -1) {
        State t = lineCutSimpleBlock(s, k, bestI.first, bestI.second);
        v.push_back(t);
      }
      if (bestJ.first != -1) {
        State t = lineCutSimpleBlock(s, k, bestJ.first, bestJ.second);
        v.push_back(t);
      }
      if (bestI.first != -1 && bestJ.first != -1) {
        State t = pointCutSimpleBlock(s, k, bestI.first, bestJ.first);
        v.push_back(t);
      }
    }

    return v;
  }

  vec<State> phase1(const State& s)
  {
    map<vec<int>, vec<int>> shapeidx;
    for (int i = 0; i < s.size(); ++i) {
      shapeidx[vec<int>({s[i].mni, s[i].mnj, s[i].mxi, s[i].mxj})].push_back(i);
    }

    vec<State> v;
    random_device seed_gen;
    mt19937 engine(seed_gen());

    State t = s;
    vec<int> idx(xorshift() % s.size());
    iota(idx.begin(), idx.end(), 0);
    shuffle(idx.begin(), idx.end(), engine);
    for (int k = 0; k + 1 < idx.size(); ++k) {
      int a = idx[k];
      int b = idx[k + 1];
      if (t[a].sameShape(t[b])) {
        double score = dryrunSwapSimpleBlocks(t, a, b);
        if (score < t.simulalityAv()) {
          t = swapSimpleBlocks(t, a, b);
        }
      }
    }
    v.push_back(t);
    return v;
  }
  vec<State> phase2(const State& s)
  {
    map<Color, vec<int>> coloridx;
    for (int i = 0; i < s.size(); ++i) {
      coloridx[s[i].c].push_back(i);
    }

    vec<State> v;
    random_device seed_gen;
    mt19937 engine(seed_gen());
    vec<int> idx(s.size());
    iota(idx.begin(), idx.end(), 0);
    shuffle(idx.begin(), idx.end(), engine);

    State t = s;
    for (int a = 0; a < t.size(); ++a) {
      pair<int, int> best = {-1, -1};
      double sim = 1e50;
      for (int b = a + 1; b < t.size(); ++b) {
        if (t[a].mergeable(t[b])) {
          double score = dryrunMergeSimpleBlocks(t, a, b);
          if (score < sim) {
            sim = score;
            best = make_pair(a, b);
          }
        }
      }
      if (best.first != -1) {
        t = mergeSimpleBlocks(t, best.first, best.second);
        --a;
      }
    }
    v.push_back(t);
    return v;
  }
}

void solve(State ini)
{
  if (width != 400) throw 1;
  if (height != 400) throw 1;

  State best = ini;
  const int MULTI_SRC = 5;

  priority_queue<State> q;
  {
    const int STEP = 2000;
    for (int _ = 0; _ < STEP; ++_) {
      clog << "phase1/" << _ << "th-try(" << now() << "),queue.size:" << q.size() << "," << "best.score=" << best.simulality() <<"＋" << best.cost << ",H:" << best.ifColorAv() << ",best.size:" << best.size() << ",cmd.size:" << best.cmd.size() << endl;
      auto t = johniel5::phase1(ini);
      each (k, t) {
        if (k.heuristic() < best.heuristic()) best = k;
        Hash h = k.hash();
        q.push(k);
      }
    }
  }
  {
    const int STEP = 2000;
    for (int _ = 0; _ < STEP; ++_) {
      clog << "phase2/" << _ << "th-try(" << now() << "),queue.size:" << q.size() << "," << "best.score=" << best.simulality() <<"＋" << best.cost << ",H:" << best.ifColorAv() << ",best.size:" << best.size() << ",cmd.size:" << best.cmd.size() << endl;
      auto t = johniel5::phase2(ini);
      each (k, t) {
        if (k.heuristic() < best.heuristic()) best = k;
        Hash h = k.hash();
        if (k.size() < 100) q.push(k);
        while (q.size() && MULTI_SRC < q.size()) q.pop();
      }
    }
  }
  {
    set<Hash> vis;
    while (MULTI_SRC < q.size()) q.pop();
    const lli WIDTH = 600;
    const int STEP = 30;
    for (int _ = 0; _ < STEP && q.size(); ++_) {
      clog << "phase99/" << _ << "th-gen(" << now() << "),queue.size:" << q.size() << "," << "best.score=" << best.simulality() <<"＋" << best.cost << ",H:" << best.ifColorAv() << ",cmd.size:" << best.cmd.size() << ",cache-size:" << make_pair(johniel5::memoI.size(),johniel5::memoJ.size()) << ",visited:" << vis.size() << endl;
      priority_queue<State> nq;
      while (q.size()) {
        if (q.top().heuristic() < best.heuristic()) best = q.top();
        auto t = johniel5::phase99(q.top());
        each (k, t) {
          Hash h = k.hash();
          if (vis.count(h)) continue;
          vis.insert(h);
          nq.push(k);
        }
        while (nq.size() && WIDTH < nq.size()) nq.pop();
        q.pop();
      }
      q = nq;
    }
  }
  best = colorSimpleBlockBestAll(best);
  best.output();
  clog << best.score() << "," << best.cmd.size() << endl;
  return ;
}

State initialjson(void)
{
// N(初期ブロック数), 問題25以下は常に0
// BLOCK_ID_i
// MIN_X_i MIN_Y_i
// MAX_X_i MAX_Y_i
// R_i G_i B_i A_i
// ...(0<=i<N)
// M(PNGのピクセル数)
// R_i G_i B_i A_i
// ...(0<=i<M)
  State s;

  int n;
  cin >> n;
  clog << n << " initial blocks" << endl;
  vec<pair<str, vec<int>>> bs;
  for (int _ = 0; _ < n; ++_) {
    str id;
    int mni, mnj, mxi, mxj, r, g, b, a;
    cin >> id >> mni >> mnj >> mxi >> mxj >> r >> g >> b >> a;
    bs.push_back(make_pair(id, vec<int>({mni, mnj, mxi, mxj, r, g, b, a})));
  }

  int m;
  cin >> m;
  clog << m << " pixels" << endl;
  Image = new uint8_t[width*height*4];
  for (int i = 0; i < height; ++i) {
    for (int j = 0; j < width; ++j) {
      uint8_t r, g, b, a;
      cin >> r >> g >> b >> a;
      Image[((height - 1 - i) * width + j) * 4 + 0] = r;
      Image[((height - 1 - i) * width + j) * 4 + 1] = g;
      Image[((height - 1 - i) * width + j) * 4 + 2] = b;
      Image[((height - 1 - i) * width + j) * 4 + 3] = a;
    }
  }
  buildColorPrefixSum2D();

  each (k, bs) {
    str id  = k.first;
    int mni = k.second[0];
    int mnj = k.second[1];
    int mxi = k.second[2];
    int mxj = k.second[3];
    int r = k.second[4];
    int g = k.second[5];
    int b = k.second[6];
    int a = k.second[7];
    s = s.append(SimpleBlock(mni, mnj, mxi, mxj, id, Color(r, g, b, a)));
  }
  if (s.empty()) s = s.append(initialSimpleBlock());
  s.blockIDCount = n - 1;
  clog << "initial simulality:" << s.simulality() << ' ' << s.blockIDCount << endl;
  return s;
}

int main(int argc, char *argv[])
{
  width = height = 400;
  State s = initialjson();
  str B = now();
  solve(s);
  str E = now();
  clog << "start:" << B << endl;
  clog << "end:" << E << endl;
  return 0;
}
