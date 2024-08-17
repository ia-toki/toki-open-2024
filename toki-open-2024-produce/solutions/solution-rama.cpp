#include "purwokerto.h"

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

constexpr int mod = 1e9 + 7;

std::pair<long long, int> purwokerto(int N, int M, std::vector<int> T) {
  auto Compress =
      [&](vector<pair<int, int>> segments) -> vector<pair<int, int>> {
    vector<pair<int, int>> newSegments;
    for (auto [t, cnt] : segments) {
      if (cnt == 0) continue;
      if (!newSegments.empty() && newSegments.back().first == t) {
        newSegments.back().second += cnt;
      } else {
        newSegments.push_back({t, cnt});
      }
    }
    if (!newSegments.empty() && newSegments.front().first == 1) {
      newSegments.erase(begin(newSegments));
    }
    if (!newSegments.empty() && newSegments.back().first == 0) {
      newSegments.pop_back();
    }
    return newSegments;
  };
  vector<pair<int, int>> segments;
  rep(i, 0, M) segments.push_back({T[i], 1});
  long long ans = 0, ways = 1;
  down(t, N) {
    segments = Compress(segments);
    assert(sz(segments) % 2 == 0);
    rep(i, 0, sz(segments) / 2) {
      int cnt0 = segments[i * 2].second--;
      int cnt1 = segments[i * 2 + 1].second--;
      assert(cnt0 > 0 && cnt1 > 0);
      ans += t + 1;
      ways = ways * (1ll * cnt0 * cnt1 % mod) % mod;
    }
  }
  return {ans, static_cast<int>(ways % mod)};
}
