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

void solve(void)
{
	if (width != 400) throw 1;
	if (height != 400) throw 1;
  State s = komagire(initialBlock(), 2);
  for (int k = 0; k < s.bs.size(); ++k) {
    Color c = average(s.bs[k].mni, s.bs[k].mnj, s.bs[k].mxi, s.bs[k].mxj);
    auto p = s.bs[k].color(c);
    s.bs[k] = p.second;
    s.cmd.push_back(p.first);
  }
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
