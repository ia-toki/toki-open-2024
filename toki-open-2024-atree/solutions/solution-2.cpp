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
  let LOG = [](int N) {
    int ans = 0;
    while ((1 << ans) < N) ++ans;
    return ans;
  }(N);
  vector<vector<int>> adj(N);
  rep(i, 0, N - 1) {
    adj[U[i]].push_back(V[i]);
    adj[V[i]].push_back(U[i]);
  }
  vector<int> posA(N);
  rep(i, 0, M) posA[A[i]] = i;
  vector<int> depth(N);
  vector<vector<pair<int, int>>> goUp(N, vector<pair<int, int>>(LOG, {-1, 0}));
  vector<vector<pair<int, int>>> goDn(N, vector<pair<int, int>>(LOG, {-1, 0}));
  YComb([&](auto self, int u, int p) -> void {
    for (auto v : adj[u]) {
      if (v == p) continue;
      goUp[v][0] = {u, posA[u] < posA[v] ? 1 : 0};
      goDn[v][0] = {u, posA[u] < posA[v] ? 0 : 1};
      rep(i, 1, LOG) {
        if (auto [node, cost] = goUp[v][i - 1]; node != -1) {
          goUp[v][i] = {goUp[node][i - 1].first,
                        cost + goUp[node][i - 1].second};
        }
        if (auto [node, cost] = goDn[v][i - 1]; node != -1) {
          goDn[v][i] = {goDn[node][i - 1].first,
                        cost + goDn[node][i - 1].second};
        }
      }
      depth[v] = depth[u] + 1;
      self(v, u);
    }
  })(0, -1);
  auto getLCA = [&](int u, int v) {
    if (depth[u] < depth[v]) swap(u, v);
    int diff = depth[u] - depth[v];
    down(i, LOG) {
      if (diff & (1 << i)) {
        u = goUp[u][i].first;
      }
    }
    down(i, LOG) {
      if (goUp[u][i].first != goUp[v][i].first) {
        u = goUp[u][i].first;
        v = goUp[v][i].first;
      }
    }
    return u == v ? u : goUp[u][0].first;
  };
  auto getCost = [&](int u, int v) {
    int lca = getLCA(u, v);
    int diffUp = depth[u] - depth[lca];
    int diffDn = depth[v] - depth[lca];
    int ans = 0;
    down(i, LOG) {
      if (diffUp & (1 << i)) {
        ans += goUp[u][i].second;
        u = goUp[u][i].first;
      }
      if (diffDn & (1 << i)) {
        ans += goDn[v][i].second;
        v = goDn[v][i].first;
      }
    }
    return ans;
  };
  int current = 0;
  long long ans = 0;
  for (auto s : S) {
    ans += getCost(current, s);
    current = s;
  }
  return ans;
}
