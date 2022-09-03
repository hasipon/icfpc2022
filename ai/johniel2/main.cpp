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

struct Color {
  uint8_t R, G, B, A;
  Color(int r, int g, int b, int a) : R(r), G(b), B(b), A(a) {}
  bool valid(void) const {
    return (0 <= R && R < 256) && (0 <= G && G < 256) && (0 <= B && B < 256) && (0 <= A && A < 256);
  }
};
ostream& operator << (ostream& os, const Color& c) {
  os << "Color{" << (int)c.R << "," << (int)c.G << "," << (int)c.B << "," << (int)c.A << "}";
  return os;
}

Color average(int mni, int mnj, int mxi, int mxj)
{
  lli r, g, b, a;
  r = g = b = a = 0;
  lli cnt = 0;
  for (int i = mni; i < mxi; ++i) {
    for (int j = mnj; j < mxj; ++j) {
      ++cnt;
      r += get(i, j, 0);
      g += get(i, j, 1);
      b += get(i, j, 2);
      a += get(i, j, 3);
    }
  }
  if (cnt) {
    r /= cnt;
    g /= cnt;
    b /= cnt;
    a /= cnt;
  }
  return Color{r, g, b, a};
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

  Block(int mni_, int mnj_, int mxi_, int mxj_, str id_, Color c_)
    : mni(mni_), mnj(mnj_), mxi(mxi_), mxj(mxj_), id(id_), c(c_) {
    // cout << make_pair(mni, mnj) << ' ' << make_pair(mxi, mxj) << ' ' << make_pair(i, j) << endl;
    assert(0 <= mni && mni <= mxi && mxi <= 400);
    assert(0 <= mnj && mnj <= mxj && mxj <= 400);
  }

  pair<str, vec<Block>> pointCut(int i, int j) const {
    // cout << make_pair(mni, mnj) << ' ' << make_pair(mxi, mxj) << ' ' << make_pair(i, j) << endl;
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
    sprintf(buff, "cut [%s] [%d,%d]", id.c_str(), i, j);
    return make_pair(str(buff), v);
  }

  pair<str, Block> color(Color d)
  {
    char buff[500];
    sprintf(buff, "color [%s] [%d,%d,%d,%d]", id.c_str(), d.R, d.G, d.B, d.A);
    Block b = Block{mni, mnj, mxi, mxj, id, d};
    return make_pair(str(buff), b);
  }

  double dist(void) const {
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
};
ostream& operator << (ostream& os, const Block& b) {
  os << "Block{" << b.id << make_pair(b.mni, b.mnj) << make_pair(b.mxi, b.mxj) << b.c << "}";
  return os;
}

struct State {
  vec<Block> bs;
  vec<str> cmd;
  lli cost;
};

Block initialBlock(void)
{
  return Block{0, 0, 400, 400, "0", Color{0, 0, 0, 0}};
}

Color getColor(int i, int j)
{
  return Color{
    get(i, j, 0),
    get(i, j, 1),
    get(i, j, 2),
    get(i, j, 3),
  };
}

State komagire(Block root, int depth)
{
  State s;
  s.bs = vec<Block>({root});
  std::random_device seed_gen;
  std::mt19937 engine(seed_gen());
  normal_distribution<> nd(1.0, 0.3);
  for (int _ = 0; _ < depth; ++_) {
    vec<Block> v;
    each (k, s.bs) {
      double a = abs(nd(engine));
      double b = abs(nd(engine));
      int i = k.mni + (k.mxi - k.mni) / 2 * a;
      int j = k.mnj + (k.mxj - k.mnj) / 2 * b;
      setmin(i, k.mxi - 1);
      setmin(j, k.mxj - 1);
      auto p = k.pointCut(i, j);
      s.cmd.push_back(p.first);
      each (j, p.second) v.push_back(j);
    }
    s.bs = v;
  }
  return s;
}

State onePoint(void)
{
  State s;
  int i = 200;
  int j = 200;
  for (int _ = 0; _ <= 16; ++_) {
    clog << make_pair(i, j) << ' ' << _ << endl;
    for (int dir = 0; dir < 8; ++dir) {
      int ni = i + di[dir] * (1 << (8 - (_ / 2)));
      int nj = j + dj[dir] * (1 << (8 - (_ / 2)));
      double mn = 1e50;
      unless (0 <= ni && ni < 400) continue;
      unless (0 <= nj && nj < 400) continue;

      Block b = initialBlock();
      auto p = b.pointCut(i, j);
      State t;
      t.cmd.push_back(p.first);
      double z = 0;
      each (k, p.second) {
        Color d = findColor(k.mni, k.mnj, k.mxi, k.mxj);
        auto p = k.color(d);
        k = p.second;
        t.cmd.push_back(p.first);
        z += k.dist();
      }
      clog << make_pair(i, j)<< ' ' << make_pair(ni, nj) << ' ' << (1 << (8 - (_ / 2))) << ' ' << z << endl;
      // clog << z << " < " << mn << ' ' << ((z < mn) ? "true" : "fals") << endl;
      if (z < mn) {
        mn = z;
        s = t;
        i = ni;
        j = nj;
      }
    }
  }
  return s;
}

void solve(void)
{
  if (width != 400) throw 1;
  if (height != 400) throw 1;
  State s = onePoint();
  each (k, s.cmd) cout << k << endl;
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
