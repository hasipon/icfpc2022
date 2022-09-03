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

const char LINE_CUT_TYPE_I = 'Y';
const char LINE_CUT_TYPE_J = 'X';

const lli BASECOST_LINE_CUT = 7;
const lli BASECOST_POINT_CUT = 10;
const lli BASECOST_COLOR = 5;
const lli BASECOST_SWAP = 3;
const lli BASECOST_MERGE = 1;

const int TYPE_LINE_CUT = 0;
const int TYPE_POINT_CUT = 1;
const int TYPE_COLOR = 2;
const int TYPE_SWAP = 3;
const int TYPE_MERGE = 4;

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

ostream& operator << (ostream& os, const Color& c) {
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

double calcSimulality(int mni, int mnj, int mxi, int mxj, Color c)
{
  double z = 0;
  for (int i = mni; i < mxi; ++i) {
    for (int j = mnj; j < mxj; ++j) {
      double r = get(i, j, 0) - c.R;
      double g = get(i, j, 1) - c.G;
      double b = get(i, j, 2) - c.B;
      double a = get(i, j, 3) - c.A;
      z += sqrt(r * r + g * g + b * b + a * a);
    }
  }
  return z;
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

  str hash(void) const {
    char buff[500];
    sprintf(buff, "%s-%d%d%d%d-%d%d%d%d", id.c_str(), mni, mnj, mxi, mxj, c.R, c.G, c.B, c.A);
    return str(buff);
  }

  Block(int mni_, int mnj_, int mxi_, int mxj_, str id_, Color c_)
    : mni(mni_), mnj(mnj_), mxi(mxi_), mxj(mxj_), id(id_), c(c_) {
    assert(0 <= mni && mni < mxi && mxi <= 400);
    assert(0 <= mnj && mnj < mxj && mxj <= 400);
  }

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
    return make_pair(str(buff), v);
  }

  pair<str, vec<Block>> pointCut(int i, int j) const {
    assert(mni < i && i < mxi);
    assert(mnj < j && j < mxj);

    Block b0(
      mni, mnj,
      i, j,
      id + ".0",
      c);
    Block b1(
      i, mnj,
      mxi, j,
      id + ".1",
      c);
    Block b2(
      i, j,
      mxi, mxj,
      id + ".2",
      c);
    Block b3(
      mni, j,
      i, mxj,
      id + ".3",
      c);
    vec<Block> v({b0, b1, b2, b3});
    char buff[500];
    sprintf(buff, "cut [%s] [%d,%d]", id.c_str(), j, i);
    return make_pair(str(buff), v);
  }

  pair<str, Block> color(Color d)
  {
    char buff[500];
    sprintf(buff, "color [%s] [%d,%d,%d,%d]", id.c_str(), d.R, d.G, d.B, d.A);
    Block b = Block{mni, mnj, mxi, mxj, id, d};
    return make_pair(str(buff), b);
  }
  pair<str, Block> colorAv(void)
  {
    Color d = average(mni, mnj, mxi, mxj);
    return color(d);
  }
  double simulality(void) const { return calcSimulality(mni, mnj, mxi, mxj, c); }
  double simulality(Color d) const { return calcSimulality(mni, mnj, mxi, mxj, d); }
  double simulalityAv(void) const { return simulality(average(mni, mnj, mxi, mxj)); }
  lli area(void) const { return (mxi - mni) * (mxj - mnj); }

};
ostream& operator << (ostream& os, const Block& b) {
  os << "Block{" << b.id << make_pair(b.mni, b.mnj) << make_pair(b.mxi, b.mxj) << b.c << "}";
  return os;
}

lli calcCost(int move, const Block& b)
{
  switch (move) {
    case TYPE_LINE_CUT:
      return BASECOST_LINE_CUT * (height * width) / b.area();
    case TYPE_POINT_CUT:
      return BASECOST_POINT_CUT * (height * width) / b.area();
    case TYPE_COLOR:
      return BASECOST_COLOR * (height * width) / b.area();
    case TYPE_SWAP:
      return BASECOST_SWAP * (height * width) / b.area();
    case TYPE_MERGE:
      return BASECOST_MERGE * (height * width) / b.area();
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
    cost = sim = 0;
    simulality();
  }
  double simulality(void) {
    double z = 0;
    each (k, bs) z += k.simulality();
    return cost = z;
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

  void colorAv(void)
  {
    for (int k = 0; k < bs.size(); ++k) {
      auto p = bs[k].colorAv();
      cmd.push_back(p.first);
      bs[k] = p.second;
      cost += calcCost(TYPE_COLOR, bs[k]);
    }
    return;
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
  void move(int type, const Block& b)
  {
    cost += calcCost(type, b);
  }
};
bool operator < (const State& a, const State& b) {
  return a.cost + a.sim < b.cost + b.sim;
}

// map<str, vec<pair<str, vec<Block>>>> memo1;
// map<str, pair<str, vec<Block>>> memo2;
map<str, pair<str, vec<Block>>> memo;
vec<State> cutOneBlock(const State& s)
{
  // if (memo1.count(s.hash())) return memo1[s.hash()];
  vec<State> v;
  for (int k = 0; k < s.bs.size(); ++k) {
    vec<pair<str, vec<Block>>> bests;
    pair<str, vec<Block>> best;
    double sim0 = 1e50;
    Block b = s.bs[k];
    if (memo.count(b.hash())) {
      best = memo[b.hash()];
    } else {
      for (int i = b.mni + 1; i < b.mxi; ++i) {
        auto p = b.lineCut(i, LINE_CUT_TYPE_I);
        double sim = 0;
        sim += p.second[0].simulalityAv();
        sim += p.second[1].simulalityAv();
        sim += calcCost(TYPE_LINE_CUT, b);
        if (sim < sim0) {
          sim0 = sim;
          best = p;
        }
      }
      for (int j = b.mnj + 1; j < b.mxj; ++j) {
        auto p = b.lineCut(j, LINE_CUT_TYPE_J);
        double sim = 0;
        sim += p.second[0].simulalityAv();
        sim += p.second[1].simulalityAv();
        sim += calcCost(TYPE_LINE_CUT, b);
        if (sim < sim0) {
          sim0 = sim;
          best = p;
        }
      }
    }
    State t = s;
    t.bs.erase(t.bs.begin() + k);
    t.bs.push_back(best.second[0]);
    t.bs.push_back(best.second[1]);
    t.cmd.push_back(best.first);
    t.cost = t.simulalityAv();
    t.move(TYPE_LINE_CUT, b);
    v.push_back(t);
    memo[b.hash()] = best;
  }
  return v;
}

Block initialBlock(void)
{
  Block b(0, 0, 400, 400, "0", Color());
  b.colorAv();
  return b;
}

void solve(void)
{
  buildColorPrefixSum2D();
  if (width != 400) throw 1;
  if (height != 400) throw 1;

  State best(initialBlock());
  best.simulality();
  priority_queue<State> q;
  q.push(best);
  const lli WIDTH = 20;
  for (int _ = 0; _ < 50 && q.size(); ++_) {
    clog << _ << "th gen queue size: " << q.size() << ", " << "best sim: " << best.cost << endl;
    priority_queue<State> nq;
    while (q.size()) {
      {
        State s = q.top();
        s.colorAv();
        if (s < best) best = s;
      }
      auto t = cutOneBlock(q.top());
      q.pop();
      if (t.empty()) continue;
      each (k, t) nq.push(k);
      while (nq.size() && WIDTH < nq.size()) nq.pop();
    }
    q = nq;
  }
  best.colorAv();
  best.output();
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
// https://shindannin.hatenadiary.com/entry/2021/03/06/115415
// https://atcoder.jp/contests/intro-heuristics/submissions/15983533
