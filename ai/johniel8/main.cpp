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

ull rhash(string s)
{
  const ull P = 709;
  ull h = 0;
  each (c, s) h = (h * P) + (tolower(c) - 'a' + 1);
  return h;
}

constexpr char LINE_CUT_TYPE_I = 'Y';
constexpr char LINE_CUT_TYPE_J = 'X';

using CommandType = int;

constexpr CommandType TYPE_LINE_CUT = 0;
constexpr CommandType TYPE_POINT_CUT = 1;
constexpr CommandType TYPE_COLOR = 2;
constexpr CommandType TYPE_SWAP = 3;
constexpr CommandType TYPE_MERGE = 4;

const int N = 400;

struct Color {
  lli R, G, B, A;
  Color() { R = G = B = A = 0; }
  Color(lli r, lli g, lli b, lli a) : R(r), G(b), B(b), A(a) {}
  bool valid(void) const { return (0 <= R && R < 256) && (0 <= G && G < 256) && (0 <= B && B < 256) && (0 <= A && A < 256); }
  double distance(const Color& other) const
  {
    double r = R - other.R;
    double g = G - other.G;
    double b = B - other.B;
    double a = A - other.A;
    return sqrt(r * r + g * g + b * b + a * a);
  }
  double simulality(const Color& other) const {
    return round(distance(other) * 0.005);
  }
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

const int WIDTH = 400;
const int HEIGHT = 400;
const int CANVAS_SIZE = WIDTH * HEIGHT;

uint8_t* Image;
lli canvas[HEIGHT][WIDTH][4];

lli get(int i, int j, int k)
{
	return canvas[i][j][k];
}

lli get(int i, int j)
{
  const lli B = (1 << 8);
  return ((((get(i, j, 0) * B) + (get(i, j, 1))) * B) + get(i, j, 2)) * B + get(i, j, 3);
}

Color getColor(int i, int j)
{
  return Color(get(i, j, 0),
               get(i, j, 1),
               get(i, j, 2),
               get(i, j, 3));
}

Color initialColor[HEIGHT][WIDTH];
double initialSimulality;

PrefixSum2D<lli> colorsumR;
PrefixSum2D<lli> colorsumG;
PrefixSum2D<lli> colorsumB;
PrefixSum2D<lli> colorsumA;

void buildColorPrefixSum2D(void)
{
  vec<vec<lli>> r(HEIGHT, vec<lli>(WIDTH, 0));
  vec<vec<lli>> g(HEIGHT, vec<lli>(WIDTH, 0));
  vec<vec<lli>> b(HEIGHT, vec<lli>(WIDTH, 0));
  vec<vec<lli>> a(HEIGHT, vec<lli>(WIDTH, 0));
  for (int i = 0; i < WIDTH; ++i) {
    for (int j = 0; j < HEIGHT; ++j) {
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

map<lli, Color> _bestColor;
Color bestColor(int mni, int mnj, int mxi, int mxj)
{
  assert(mni <= mxi);
  assert(mnj <= mxj);
  lli key = 0;
  key = key * 401 + mni;
  key = key * 401 + mnj;
  key = key * 401 + mxi;
  key = key * 401 + mxj;
  if (_bestColor.count(key)) return _bestColor[key];

  uint32_t x = y1r::f(mni, mxi, mnj, mxj).first;
  int r = ((uint8_t*)&x)[0];
  int g = ((uint8_t*)&x)[1];
  int b = ((uint8_t*)&x)[2];
  int a = ((uint8_t*)&x)[3];
  return _bestColor[key] = Color(r, g, b, a);
}

map<lli, double> _calcSimulality;
double calcSimulality(int mni, int mnj, int mxi, int mxj, Color c)
{
  lli key = 0;
  key = key * 401 + mni;
  key = key * 401 + mnj;
  key = key * 401 + mxi;
  key = key * 401 + mxj;
  key = key * 401 + c.R;
  key = key * 401 + c.G;
  key = key * 401 + c.B;
  if (_calcSimulality.count(key)) return _calcSimulality[key];

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
  return _calcSimulality[key] = round(z * 0.005);
}

lli calcCost(int move, double area)
{
  constexpr lli BASECOST_LINE_CUT = 7;
  constexpr lli BASECOST_POINT_CUT = 10;
  constexpr lli BASECOST_COLOR = 5;
  constexpr lli BASECOST_SWAP = 3;
  constexpr lli BASECOST_MERGE = 1;

  const double C = CANVAS_SIZE;
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

using Hash = str;

struct State {
  vec<int> fixed;
  Color color;
  double sim;
  double cost;
  int blockid;
  vec<str> cmd;
  State() :
    sim(0.0),
    cost(0.0),
    blockid(0),
    fixed(vec<int>(WIDTH, 0)),
    color(Color(0, 0, 0, 0)) {
    sim = calcSimulality(0, 0, HEIGHT, WIDTH, color);
  }

  void validate(void) const {
    for (int i = 0; i + 1 < fixed.size(); ++i) {
      assert(fixed[i] >= fixed[i + 1]);
    }
    return ;
  }

  bool finished(void) const {
    return count(fixed.begin(), fixed.end(), HEIGHT) == fixed.size();
  }

  double heuristic(void) const {
    return accumulate(fixed.begin(), fixed.end(), 0.0) * (cost + sim);
    // return (cost + sim) / cost + (cost + sim) / accumulate(fixed.begin(), fixed.end(), 0.0);
  }

  double score(double atmost = 1e50) const {
    double s = cost + sim;
    for (int j = 0; j < WIDTH; ++j) {
      for (int i = 0; i < fixed[j]; ++i) {
        if (color.A == 0) initialColor[i][j];
        s += getColor(i, j).simulality(color);
        if (atmost < s) return s;
      }
    }
    return s;
  }
};

bool operator < (const State& a, const State& b)
{
  return a.heuristic() < b.heuristic();
}

ostream& operator << (ostream& os, const State& s)
{
  os << "State{"<< s.fixed << ',' << s.color << ",sim:" << s.sim << ",cost:" << s.cost << "," << s.cmd << '}';
  return os;
}

double simRemaining(int i, int j, Color c)
{
  double sim = 0;
  sim += calcSimulality(i, 0, HEIGHT, WIDTH, c);
  sim += calcSimulality(0, j, HEIGHT, WIDTH, c);
  sim -= calcSimulality(i, j, HEIGHT, WIDTH, c);
  return sim;
}

State run(const State& s, const int loop)
{
  static char buff[500];
  assert(!s.finished());

  State z = s;
  z.sim = 1e50;

  const int AREA_FIXED = accumulate(s.fixed.begin(), s.fixed.end(), 0);
  const int AREA_FREE = CANVAS_SIZE - AREA_FIXED;

  for (int _ = 0; _ < 100; ++_) {
    const int j = xorshift() % WIDTH + 1;
    if (HEIGHT == s.fixed[j - 1]) continue;
    const int i = s.fixed[j - 1] + xorshift() % (HEIGHT - s.fixed[j - 1]) + 1;

    unless ((i + j) < 80 * (loop + 1)) continue;

    State t = s;
    Color c;
    double dsim = 0;
    int area0 = 0;
    int area3 = 0;
    {
      for (int b = 0; b < j; ++b) {
        for (int a = s.fixed[b]; a < i; ++a) {
          Color d = getColor(a, b);
          c += d;
          ++area0;
        }
        setmax(t.fixed[b], i);
        area3 += HEIGHT - s.fixed[b];
      }
      unless (area0) continue;
      c /= area0;
      for (int b = 0; b < j; ++b) {
        for (int a = s.fixed[b]; a < i; ++a) {
          Color d = getColor(a, b);
          dsim -= s.color.simulality(d);
          dsim += c.simulality(d);
        }
      }
    }

    const int area2 = (HEIGHT - i) * (WIDTH - j);
    const int area1 = AREA_FREE - area0 - area2 - area3;

    if (s.fixed[j]) {

      sprintf(buff, "cut [%d] [%d,%d]", s.blockid, s.fixed[j], j);
      t.cmd.push_back(str(buff));
      t.cost += calcCost(TYPE_POINT_CUT, AREA_FREE);

      sprintf(buff, "color [%d.2] [%d,%d,%d,%d]", t.blockid, c.R, c.G, c.B, c.A);
      t.cmd.push_back(str(buff));
      t.cost += calcCost(TYPE_COLOR, AREA_FREE);
      t.color = c;

      sprintf(buff, "merge [%d.0] [%d.1]", t.blockid, t.blockid);
      t.cmd.push_back(str(buff));
      t.cost += calcCost(TYPE_MERGE, max(area0, area1));

      sprintf(buff, "merge [%d.2] [%d.3]", t.blockid, t.blockid);
      t.cmd.push_back(str(buff));
      t.cost += calcCost(TYPE_MERGE, max(area2, area3));

      sprintf(buff, "merge [%d] [%d]", t.blockid + 1, t.blockid + 2);
      t.cmd.push_back(str(buff));
      t.cost += calcCost(TYPE_MERGE, max(area0 + area1, area2 + area3));

      t.blockid += 3;
      t.sim += dsim;
    } else {

      sprintf(buff, "cut [%d] [X] [%d]", s.blockid, j);
      t.cmd.push_back(str(buff));
      t.cost += calcCost(TYPE_POINT_CUT, AREA_FREE);

      sprintf(buff, "color [%d.0] [%d,%d,%d,%d]", t.blockid, c.R, c.G, c.B, c.A);
      t.cmd.push_back(str(buff));
      t.cost += calcCost(TYPE_COLOR, AREA_FREE);
      t.color = c;

      sprintf(buff, "merge [%d.0] [%d.1]", t.blockid, t.blockid);
      t.cmd.push_back(str(buff));
      t.cost += calcCost(TYPE_MERGE, max(area0, area1));

      t.blockid += 1;
      t.sim += dsim;
    }
    return t;
  }

  return z;
}

State solve(State ini)
{
  State best = ini;
  priority_queue<State> q;
  q.push(ini);
  set<vec<int>> vis;
  for (int loop = 0; loop < 100; ++loop) {
    clog << loop << "th-gen(" << now() << "):" << "q.size:" << q.size() << ",best.score:" << best.score() << ",best.sim:" << best.sim << ",best.heuristic:" << best.heuristic() << ",cache.size:" << _bestColor.size() << ",best.cmd.size:" << best.cmd.size() << endl;

    {
      vec<State> v;
      while (q.size()) {
        v.push_back(q.top());
        q.pop();
      }
      reverse(v.begin(), v.end());
      each (i, v) {
        if (!vis.count(i.fixed)) {
          vis.insert(i.fixed);
          q.push(i);
        }
      }
    }

    priority_queue<State> nq;
    while (q.size()) {
      if (q.top().score() < best.score()) best = q.top();
      unless (q.top().finished()) {
        for (int __ = 0; __ < 100; ++__) {
          nq.push(run(q.top(), loop));
        }
      }
      const int QWIDTH = 500;
      while (QWIDTH < nq.size()) nq.pop();
      q.pop();
    }
    q = nq;
  }

  if (q.size() && q.top().cmd.size()) each (i, q.top().cmd) cout << i << endl;

  if (best.cmd.size()) {
    best.cmd.pop_back();
    best.cmd.pop_back();
    best.cmd.pop_back();
  }
  return best;
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

  int n;
  cin >> n;
  clog << n << " initial blocks" << endl;
  vec<pair<str, vec<int>>> bs;
  for (int i = 0; i < HEIGHT; ++i) {
    for (int j = 0; j < WIDTH; ++j) {
      initialColor[i][j] = Color(255, 255, 255, 255);
    }
  }
  for (int _ = 0; _ < n; ++_) {
    str id;
    int mni, mnj, mxi, mxj, r, g, b, a;
    cin >> id >> mni >> mnj >> mxi >> mxj >> r >> g >> b >> a;
    for (int i = mni; i < mxi; ++i) {
      for (int j = mnj; j < mxj; ++j) {
        initialColor[i][j] = Color(r, g, b, a);
      }
    }
  }

  int m;
  cin >> m;
  clog << m << " pixels" << endl;
  assert(m == WIDTH*HEIGHT);
  Image = new uint8_t[WIDTH*HEIGHT*4];
  for (int i = 0; i < HEIGHT; ++i) {
    for (int j = 0; j < WIDTH; ++j) {
      cin >> canvas[i][j][0] >> canvas[i][j][1] >> canvas[i][j][2] >> canvas[i][j][3];
    }
  }
  buildColorPrefixSum2D();

  initialSimulality = 0;
  for (int i = 0; i < HEIGHT; ++i) {
    for (int j = 0; j < WIDTH; ++j) {
      initialSimulality += initialColor[i][j].simulality(getColor(i, j));
    }
  }

  State s;
  s.blockid = n;
  s.color = Color(255, 255, 255, 255);
  s.sim = calcSimulality(0, 0, 400, 400, s.color);
  clog << "initialSimulality:" << initialSimulality << endl;
  return s;
}

int main(int argc, char *argv[])
{
  State ini = initialjson();
  str B = now();
  State best = solve(ini);
  each (i, best.cmd) cout << i << endl;
  str E = now();
  clog << "start:" << B << endl;
  clog << "end:" << E << endl;
  return 0;
}
