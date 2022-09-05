#include <bits/stdc++.h>

#define each(i, c) for (auto& i : c)
#define unless(cond) if (!(cond))
// #define endl "\n"

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
constexpr lli mod = 1e9 + 7;
// constexpr lli mod = 998244353;

struct S {

};

int main(int argc, char *argv[])
{
  ios_base::sync_with_stdio(false);
  cin.tie(nullptr);
  cout.setf(ios_base::fixed);
  cout.precision(15);

  const int N = 20;
  deque<int> q[N];
  int cnt = 0;
  for (int i = 0; i < N; ++i) {
    for (int j = 0; j < N; ++j) {
      q[i].push_back(cnt++);
    }
  }
  const int M = 10;
  const int K = 5;
  for (int i = 0; i < M; ++i) {
    while (K < q[i].size()) {
      int x = q[i].front(); q[i].pop_front();
      int y = q[i].front(); q[i].pop_front();
      cout << "merge " << "[" << x << "]" << ' ' << "[" << y << "]" << endl;
      q[i].push_front(cnt++);
    }
  }
  int x = q[0].front();
  for (int i = 1; i < M; ++i) {
    int y = q[i].front();
    cout << "merge " << "[" << x << "]" << ' ' << "[" << y << "]" << endl;
    x = cnt++;
  }

  cout << "cut [558] [Y] [20]" << endl;
  cout << "cut [558.1] [Y] [40]" << endl;
  cout << "cut [558.1.1] [Y] [60]" << endl;
  cout << "cut [558.1.1.1] [Y] [80]" << endl;
  cout << "cut [558.1.1.1.1] [Y] [100]" << endl;
  cout << "cut [558.1.1.1.1.1] [Y] [120]" << endl;
  cout << "cut [558.1.1.1.1.1.1] [Y] [140]" << endl;
  cout << "cut [558.1.1.1.1.1.1.1] [Y] [160]" << endl;
  cout << "cut [558.1.1.1.1.1.1.1.1] [Y] [180]" << endl;
  cout << "cut [558.1.1.1.1.1.1.1.1.1] [Y] [200]" << endl;
  cout << "cut [558.1.1.1.1.1.1.1.1.1.1] [Y] [220]" << endl;
  cout << "cut [558.1.1.1.1.1.1.1.1.1.1.1] [Y] [240]" << endl;
  cout << "cut [558.1.1.1.1.1.1.1.1.1.1.1.1] [Y] [260]" << endl;
  cout << "cut [558.1.1.1.1.1.1.1.1.1.1.1.1.1] [Y] [280]" << endl;
  cout << "cut [558.1.1.1.1.1.1.1.1.1.1.1.1.1.1] [Y] [300]" << endl;
  // cout << "cut [558.1.1.1.1.1.1.1.1.1.1.1.1.1.1.1] [X] [320]" << endl;


  {
    int cnt = 559;
    {
      int x = 200;
      cout << "merge [558.0] [" << x << "]" << endl; x+=20;
      cout << "merge [" << cnt++ << "] [" << x << "]" << endl; x += 20;
      cout << "merge [" << cnt++ << "] [" << x << "]" << endl; x += 20;
      cout << "merge [" << cnt++ << "] [" << x << "]" << endl; x += 20;
      cout << "merge [" << cnt++ << "] [" << x << "]" << endl; x += 20;
      cout << "merge [" << cnt++ << "] [" << x << "]" << endl; x += 20;
      cout << "merge [" << cnt++ << "] [" << x << "]" << endl; x += 20;
      cout << "merge [" << cnt++ << "] [" << x << "]" << endl; x += 20;
      cout << "merge [" << cnt++ << "] [" << x << "]" << endl; x += 20;
      cout << "merge [" << cnt++ << "] [" << x << "]" << endl; x += 20;
    }
    {
      cnt++;      int x = 201;
      cout << "merge [558.1.0] [" << x << "]" << endl; x+=20;
      cout << "merge [" << cnt++ << "] [" << x << "]" << endl; x += 20;
      cout << "merge [" << cnt++ << "] [" << x << "]" << endl; x += 20;
      cout << "merge [" << cnt++ << "] [" << x << "]" << endl; x += 20;
      cout << "merge [" << cnt++ << "] [" << x << "]" << endl; x += 20;
      cout << "merge [" << cnt++ << "] [" << x << "]" << endl; x += 20;
      cout << "merge [" << cnt++ << "] [" << x << "]" << endl; x += 20;
      cout << "merge [" << cnt++ << "] [" << x << "]" << endl; x += 20;
      cout << "merge [" << cnt++ << "] [" << x << "]" << endl; x += 20;
      cout << "merge [" << cnt++ << "] [" << x << "]" << endl; x += 20;
    }
    {
      cnt++;      int x = 202;
      cout << "merge [558.1.1.0] [" << x << "]" << endl; x+=20;
      cout << "merge [" << cnt++ << "] [" << x << "]" << endl; x += 20;
      cout << "merge [" << cnt++ << "] [" << x << "]" << endl; x += 20;
      cout << "merge [" << cnt++ << "] [" << x << "]" << endl; x += 20;
      cout << "merge [" << cnt++ << "] [" << x << "]" << endl; x += 20;
      cout << "merge [" << cnt++ << "] [" << x << "]" << endl; x += 20;
      cout << "merge [" << cnt++ << "] [" << x << "]" << endl; x += 20;
      cout << "merge [" << cnt++ << "] [" << x << "]" << endl; x += 20;
      cout << "merge [" << cnt++ << "] [" << x << "]" << endl; x += 20;
      cout << "merge [" << cnt++ << "] [" << x << "]" << endl; x += 20;
    }
    {
      cnt++;      int x = 203;
      cout << "merge [558.1.1.1.0] [" << x << "]" << endl; x+=20;
      cout << "merge [" << cnt++ << "] [" << x << "]" << endl; x += 20;
      cout << "merge [" << cnt++ << "] [" << x << "]" << endl; x += 20;
      cout << "merge [" << cnt++ << "] [" << x << "]" << endl; x += 20;
      cout << "merge [" << cnt++ << "] [" << x << "]" << endl; x += 20;
      cout << "merge [" << cnt++ << "] [" << x << "]" << endl; x += 20;
      cout << "merge [" << cnt++ << "] [" << x << "]" << endl; x += 20;
      cout << "merge [" << cnt++ << "] [" << x << "]" << endl; x += 20;
      cout << "merge [" << cnt++ << "] [" << x << "]" << endl; x += 20;
      cout << "merge [" << cnt++ << "] [" << x << "]" << endl; x += 20;
    }
    {
      cnt++;      int x = 204;
      cout << "merge [558.1.1.1.1.0] [" << x << "]" << endl; x+=20;
      cout << "merge [" << cnt++ << "] [" << x << "]" << endl; x += 20;
      cout << "merge [" << cnt++ << "] [" << x << "]" << endl; x += 20;
      cout << "merge [" << cnt++ << "] [" << x << "]" << endl; x += 20;
      cout << "merge [" << cnt++ << "] [" << x << "]" << endl; x += 20;
      cout << "merge [" << cnt++ << "] [" << x << "]" << endl; x += 20;
      cout << "merge [" << cnt++ << "] [" << x << "]" << endl; x += 20;
      cout << "merge [" << cnt++ << "] [" << x << "]" << endl; x += 20;
      cout << "merge [" << cnt++ << "] [" << x << "]" << endl; x += 20;
      cout << "merge [" << cnt++ << "] [" << x << "]" << endl; x += 20;
    }
    {
      cnt++;      int x = 205;
      cout << "merge [558.1.1.1.1.1.0] [" << x << "]" << endl; x+=20;
      cout << "merge [" << cnt++ << "] [" << x << "]" << endl; x += 20;
      cout << "merge [" << cnt++ << "] [" << x << "]" << endl; x += 20;
      cout << "merge [" << cnt++ << "] [" << x << "]" << endl; x += 20;
      cout << "merge [" << cnt++ << "] [" << x << "]" << endl; x += 20;
      cout << "merge [" << cnt++ << "] [" << x << "]" << endl; x += 20;
      cout << "merge [" << cnt++ << "] [" << x << "]" << endl; x += 20;
      cout << "merge [" << cnt++ << "] [" << x << "]" << endl; x += 20;
      cout << "merge [" << cnt++ << "] [" << x << "]" << endl; x += 20;
      cout << "merge [" << cnt++ << "] [" << x << "]" << endl; x += 20;
    }
    {
      cnt++;      int x = 206;
      cout << "merge [558.1.1.1.1.1.1.0] [" << x << "]" << endl; x+=20;
      cout << "merge [" << cnt++ << "] [" << x << "]" << endl; x += 20;
      cout << "merge [" << cnt++ << "] [" << x << "]" << endl; x += 20;
      cout << "merge [" << cnt++ << "] [" << x << "]" << endl; x += 20;
      cout << "merge [" << cnt++ << "] [" << x << "]" << endl; x += 20;
      cout << "merge [" << cnt++ << "] [" << x << "]" << endl; x += 20;
      cout << "merge [" << cnt++ << "] [" << x << "]" << endl; x += 20;
      cout << "merge [" << cnt++ << "] [" << x << "]" << endl; x += 20;
      cout << "merge [" << cnt++ << "] [" << x << "]" << endl; x += 20;
      cout << "merge [" << cnt++ << "] [" << x << "]" << endl; x += 20;
    }
    {
      cnt++;      int x = 207;
      cout << "merge [558.1.1.1.1.1.1.1.0] [" << x << "]" << endl; x+=20;
      cout << "merge [" << cnt++ << "] [" << x << "]" << endl; x += 20;
      cout << "merge [" << cnt++ << "] [" << x << "]" << endl; x += 20;
      cout << "merge [" << cnt++ << "] [" << x << "]" << endl; x += 20;
      cout << "merge [" << cnt++ << "] [" << x << "]" << endl; x += 20;
      cout << "merge [" << cnt++ << "] [" << x << "]" << endl; x += 20;
      cout << "merge [" << cnt++ << "] [" << x << "]" << endl; x += 20;
      cout << "merge [" << cnt++ << "] [" << x << "]" << endl; x += 20;
      cout << "merge [" << cnt++ << "] [" << x << "]" << endl; x += 20;
      cout << "merge [" << cnt++ << "] [" << x << "]" << endl; x += 20;
    }
    {
      cnt++;      int x = 208;
      cout << "merge [558.1.1.1.1.1.1.1.1.0] [" << x << "]" << endl; x+=20;
      cout << "merge [" << cnt++ << "] [" << x << "]" << endl; x += 20;
      cout << "merge [" << cnt++ << "] [" << x << "]" << endl; x += 20;
      cout << "merge [" << cnt++ << "] [" << x << "]" << endl; x += 20;
      cout << "merge [" << cnt++ << "] [" << x << "]" << endl; x += 20;
      cout << "merge [" << cnt++ << "] [" << x << "]" << endl; x += 20;
      cout << "merge [" << cnt++ << "] [" << x << "]" << endl; x += 20;
      cout << "merge [" << cnt++ << "] [" << x << "]" << endl; x += 20;
      cout << "merge [" << cnt++ << "] [" << x << "]" << endl; x += 20;
      cout << "merge [" << cnt++ << "] [" << x << "]" << endl; x += 20;
    }
    {
      cnt++;      int x = 209;
      cout << "merge [558.1.1.1.1.1.1.1.1.1.0] [" << x << "]" << endl; x+=20;
      cout << "merge [" << cnt++ << "] [" << x << "]" << endl; x += 20;
      cout << "merge [" << cnt++ << "] [" << x << "]" << endl; x += 20;
      cout << "merge [" << cnt++ << "] [" << x << "]" << endl; x += 20;
      cout << "merge [" << cnt++ << "] [" << x << "]" << endl; x += 20;
      cout << "merge [" << cnt++ << "] [" << x << "]" << endl; x += 20;
      cout << "merge [" << cnt++ << "] [" << x << "]" << endl; x += 20;
      cout << "merge [" << cnt++ << "] [" << x << "]" << endl; x += 20;
      cout << "merge [" << cnt++ << "] [" << x << "]" << endl; x += 20;
      cout << "merge [" << cnt++ << "] [" << x << "]" << endl; x += 20;
    }
    {
      cnt++;      int x = 210;
      cout << "merge [558.1.1.1.1.1.1.1.1.1.1.0] [" << x << "]" << endl; x+=20;
      cout << "merge [" << cnt++ << "] [" << x << "]" << endl; x += 20;
      cout << "merge [" << cnt++ << "] [" << x << "]" << endl; x += 20;
      cout << "merge [" << cnt++ << "] [" << x << "]" << endl; x += 20;
      cout << "merge [" << cnt++ << "] [" << x << "]" << endl; x += 20;
      cout << "merge [" << cnt++ << "] [" << x << "]" << endl; x += 20;
      cout << "merge [" << cnt++ << "] [" << x << "]" << endl; x += 20;
      cout << "merge [" << cnt++ << "] [" << x << "]" << endl; x += 20;
      cout << "merge [" << cnt++ << "] [" << x << "]" << endl; x += 20;
      cout << "merge [" << cnt++ << "] [" << x << "]" << endl; x += 20;
    }
    {
      cnt++;      int x = 211;
      cout << "merge [558.1.1.1.1.1.1.1.1.1.1.1.0] [" << x << "]" << endl; x+=20;
      cout << "merge [" << cnt++ << "] [" << x << "]" << endl; x += 20;
      cout << "merge [" << cnt++ << "] [" << x << "]" << endl; x += 20;
      cout << "merge [" << cnt++ << "] [" << x << "]" << endl; x += 20;
      cout << "merge [" << cnt++ << "] [" << x << "]" << endl; x += 20;
      cout << "merge [" << cnt++ << "] [" << x << "]" << endl; x += 20;
      cout << "merge [" << cnt++ << "] [" << x << "]" << endl; x += 20;
      cout << "merge [" << cnt++ << "] [" << x << "]" << endl; x += 20;
      cout << "merge [" << cnt++ << "] [" << x << "]" << endl; x += 20;
      cout << "merge [" << cnt++ << "] [" << x << "]" << endl; x += 20;
    }
    {
      cnt++;      int x = 212;
      cout << "merge [558.1.1.1.1.1.1.1.1.1.1.1.1.0] [" << x << "]" << endl; x+=20;
      cout << "merge [" << cnt++ << "] [" << x << "]" << endl; x += 20;
      cout << "merge [" << cnt++ << "] [" << x << "]" << endl; x += 20;
      cout << "merge [" << cnt++ << "] [" << x << "]" << endl; x += 20;
      cout << "merge [" << cnt++ << "] [" << x << "]" << endl; x += 20;
      cout << "merge [" << cnt++ << "] [" << x << "]" << endl; x += 20;
      cout << "merge [" << cnt++ << "] [" << x << "]" << endl; x += 20;
      cout << "merge [" << cnt++ << "] [" << x << "]" << endl; x += 20;
      cout << "merge [" << cnt++ << "] [" << x << "]" << endl; x += 20;
      cout << "merge [" << cnt++ << "] [" << x << "]" << endl; x += 20;
    }
    {
      cnt++;      int x = 213;
      cout << "merge [558.1.1.1.1.1.1.1.1.1.1.1.1.1.0] [" << x << "]" << endl; x+=20;
      cout << "merge [" << cnt++ << "] [" << x << "]" << endl; x += 20;
      cout << "merge [" << cnt++ << "] [" << x << "]" << endl; x += 20;
      cout << "merge [" << cnt++ << "] [" << x << "]" << endl; x += 20;
      cout << "merge [" << cnt++ << "] [" << x << "]" << endl; x += 20;
      cout << "merge [" << cnt++ << "] [" << x << "]" << endl; x += 20;
      cout << "merge [" << cnt++ << "] [" << x << "]" << endl; x += 20;
      cout << "merge [" << cnt++ << "] [" << x << "]" << endl; x += 20;
      cout << "merge [" << cnt++ << "] [" << x << "]" << endl; x += 20;
      cout << "merge [" << cnt++ << "] [" << x << "]" << endl; x += 20;
    }
    {
      cnt++;      int x = 214;
      cout << "merge [558.1.1.1.1.1.1.1.1.1.1.1.1.1.1.0] [" << x << "]" << endl; x+=20;
      cout << "merge [" << cnt++ << "] [" << x << "]" << endl; x += 20;
      cout << "merge [" << cnt++ << "] [" << x << "]" << endl; x += 20;
      cout << "merge [" << cnt++ << "] [" << x << "]" << endl; x += 20;
      cout << "merge [" << cnt++ << "] [" << x << "]" << endl; x += 20;
      cout << "merge [" << cnt++ << "] [" << x << "]" << endl; x += 20;
      cout << "merge [" << cnt++ << "] [" << x << "]" << endl; x += 20;
      cout << "merge [" << cnt++ << "] [" << x << "]" << endl; x += 20;
      cout << "merge [" << cnt++ << "] [" << x << "]" << endl; x += 20;
      cout << "merge [" << cnt++ << "] [" << x << "]" << endl; x += 20;
    }
    {
      cnt++;      int x = 215;
      cout << "merge [558.1.1.1.1.1.1.1.1.1.1.1.1.1.1.1] [" << x << "]" << endl; x+=20;
      cout << "merge [" << cnt++ << "] [" << x << "]" << endl; x += 20;
      cout << "merge [" << cnt++ << "] [" << x << "]" << endl; x += 20;
      cout << "merge [" << cnt++ << "] [" << x << "]" << endl; x += 20;
      cout << "merge [" << cnt++ << "] [" << x << "]" << endl; x += 20;
      cout << "merge [" << cnt++ << "] [" << x << "]" << endl; x += 20;
      cout << "merge [" << cnt++ << "] [" << x << "]" << endl; x += 20;
      cout << "merge [" << cnt++ << "] [" << x << "]" << endl; x += 20;
      cout << "merge [" << cnt++ << "] [" << x << "]" << endl; x += 20;
      cout << "merge [" << cnt++ << "] [" << x << "]" << endl; x += 20;
    }

    ++cnt;
    cout << "merge [568] [578]" << endl;
    cout << "merge [" << cnt++ << "] [588]" << endl;
    cout << "merge [" << cnt++ << "] [598]" << endl;
    cout << "merge [" << cnt++ << "] [608]" << endl;
    cout << "merge [" << cnt++ << "] [618]" << endl;
    cout << "merge [" << cnt++ << "] [628]" << endl;
    cout << "merge [" << cnt++ << "] [638]" << endl;
    cout << "merge [" << cnt++ << "] [648]" << endl;
    cout << "merge [" << cnt++ << "] [658]" << endl;
    cout << "merge [" << cnt++ << "] [668]" << endl;
    cout << "merge [" << cnt++ << "] [678]" << endl;
    cout << "merge [" << cnt++ << "] [688]" << endl;
    cout << "merge [" << cnt++ << "] [698]" << endl;
    cout << "merge [" << cnt++ << "] [708]" << endl;
    cout << "merge [" << cnt++ << "] [718]" << endl;

    cout << "cut [733] [X] [20]" << endl;
    cout << "cut [733.1] [X] [40]" << endl;
    cout << "cut [733.1.1] [X] [60]" << endl;
    cout << "cut [733.1.1.1] [X] [80]" << endl;
    cout << "cut [733.1.1.1.1] [X] [100]" << endl;
    cout << "cut [733.1.1.1.1.1] [X] [120]" << endl;
    cout << "cut [733.1.1.1.1.1.1] [X] [140]" << endl;
    cout << "cut [733.1.1.1.1.1.1.1] [X] [160]" << endl;
    cout << "cut [733.1.1.1.1.1.1.1.1] [X] [180]" << endl;
    cout << "cut [733.1.1.1.1.1.1.1.1.1] [X] [200]" << endl;
    cout << "cut [733.1.1.1.1.1.1.1.1.1.1] [X] [220]" << endl;
    cout << "cut [733.1.1.1.1.1.1.1.1.1.1.1] [X] [240]" << endl;
    cout << "cut [733.1.1.1.1.1.1.1.1.1.1.1.1] [X] [260]" << endl;
    cout << "cut [733.1.1.1.1.1.1.1.1.1.1.1.1.1] [X] [280]" << endl;
    cout << "cut [733.1.1.1.1.1.1.1.1.1.1.1.1.1.1] [X] [300]" << endl;
    cout << "cut [733.1.1.1.1.1.1.1.1.1.1.1.1.1.1.1] [X] [320]" << endl;
    cout << "cut [733.1.1.1.1.1.1.1.1.1.1.1.1.1.1.1.1] [X] [340]" << endl;
    cout << "cut [733.1.1.1.1.1.1.1.1.1.1.1.1.1.1.1.1.1] [X] [360]" << endl;
    cout << "cut [733.1.1.1.1.1.1.1.1.1.1.1.1.1.1.1.1.1.1] [X] [380]" << endl;

    str s = "733";
    cnt = 732;
    for (int i = 0; i < 20; ++i) {
      ++cnt;
      int y = 16 + 20 * i;
      for (int j = 0; j < 4; ++j) {
        if (j) {
          cout << "merge [" << ++cnt << "] [" << y+j << "]" << endl;
        } else {
          if (i == 19) {
            s.pop_back();
            s.pop_back();
            cout << "merge [" << s << ".1] [" << y << "]" << endl;
          } else {
            cout << "merge [" << s << ".0] [" << y << "]" << endl;
          }
        }
      }
      s += ".1";
    }

    int p = 814;
    int q = 745 - 4;
    for (int i = 0; i < 19; ++i) {
      if (i) {
        cout << "merge [" << p++ << "] [" << (q += 4) << "]" << endl;
      } else {
        cout << "merge [737] [741]" << endl;
      }
    }
  }

  return 0;
}
