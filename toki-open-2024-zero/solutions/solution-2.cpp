#include "buffet.h"

#if not LOCAL
#define NDEBUG 1
#endif
#include <bits/stdc++.h>
using namespace std;

#define rep(i, a, b) for (auto i = a; i < (b); ++i)
#define down(x, a) for (auto x = a; x--;)
#define all(x) begin(x), end(x)
#define sz(x) int(size(x))
#define let auto const

using ll = long long;
using lint = ll;
using pii = pair<int, int>;
using vi = vector<int>;

template <class T>
struct YComb_ {
  T f;
  template <class... A>
  auto operator()(A&&... a) const {
    return f(ref(*this), forward<A>(a)...);
  }
};
template <class T>
YComb_<T> YComb(T f) {
  return {move(f)};
}

const int MAX = 55;
const int INF = 1e9;

long long min_leftover(int N, std::vector<int> A) {
  array<array<int, MAX>, MAX> dp = {};
  rep(i, 0, MAX) rep(j, 0, MAX) dp[i][j] = INF;
  dp[MAX - 1][0] = 0;
  for (auto a : A) {
    array<array<int, MAX>, MAX> ndp = {};
    rep(i, 0, MAX) rep(j, 0, MAX) ndp[i][j] = INF;
    down(i, MAX) rep(j, 0, MAX) {
      if (dp[i][j] == INF) continue;
      if (i) dp[i - 1][j] = min(dp[i - 1][j], dp[i][j]);
      if (j + 1 < MAX) dp[i][j + 1] = min(dp[i][j + 1], dp[i][j]);
      if (i + j > a) continue;
      ndp[i][j] = min(ndp[i][j], dp[i][j] + a - i - j);
    }
    dp = ndp;
  }
  lint ans = INF;
  rep(i, 0, MAX) rep(j, 0, MAX) ans = min(ans, lint(dp[i][j]));
  return ans;
}
