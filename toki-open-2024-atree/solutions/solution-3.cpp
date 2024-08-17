#include "copper.h"

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

long long min_risk(int N, int M, int L, std::vector<int> U, std::vector<int> V,
                   std::vector<int> A, std::vector<int> S) {
  vector<vector<int>> adj(N);
  rep(i, 0, N - 1) {
    adj[U[i]].push_back(V[i]);
    adj[V[i]].push_back(U[i]);
  }
  vector<vector<pair<int, int>>> cost(M, vector<pair<int, int>>(N, {-1, -1}));
  rep(i, 0, M) {
    rep(j, i, M) {
      if (cost[i][A[j]].first != -1) {
        continue;
      }
      cost[i][A[j]] = {0, j};
    }
    rep(j, 0, i) {
      if (cost[i][A[j]].first != -1) {
        continue;
      }
      cost[i][A[j]] = {1, j};
    }
  }
  int current = 0;
  long long ans = 0;
  for (auto s : S) {
    int node = A[current];
    auto path = YComb([&](auto self, int u, int p) -> vector<int> {
      if (u == s) return {u};
      for (auto v : adj[u]) {
        if (v == p) continue;
        auto path = self(v, u);
        if (!path.empty()) {
          path.push_back(u);
          return path;
        }
      }
      return {};
    })(node, -1);
    reverse(all(path));
    for (auto u : path) {
      auto [c, nxt] = cost[current][u];
      ans += c;
      current = nxt;
    }
  }
  return ans;
}
