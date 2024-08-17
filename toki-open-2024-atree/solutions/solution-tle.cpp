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
  vector<int> depth(N);
  vector<vector<int>> lift(N, vector<int>(LOG, -1));
  YComb([&](auto self, int u, int p) -> void {
    for (auto v : adj[u]) {
      if (v == p) continue;
      lift[v][0] = u;
      rep(i, 1, LOG) {
        if (lift[v][i - 1] != -1) {
          lift[v][i] = lift[lift[v][i - 1]][i - 1];
        }
      }
      depth[v] = depth[u] + 1;
      self(v, u);
    }
  })(0, -1);
  let getLCA = [&](int u, int v) {
    if (depth[u] < depth[v]) swap(u, v);
    let diff = depth[u] - depth[v];
    down(i, LOG) {
      if ((diff >> i) & 1) {
        u = lift[u][i];
      }
    }
    down(i, LOG) {
      if (lift[u][i] != lift[v][i]) {
        u = lift[u][i];
        v = lift[v][i];
      }
    }
    return u == v ? u : lift[u][0];
  };
  vector<vector<int>> posA(N);
  rep(i, 0, M) posA[A[i]].push_back(i);
  vector<vector<pair<int, int>>> goUp(M, vector<pair<int, int>>(LOG, {-1, 0}));
  vector<vector<pair<int, int>>> goDn(M, vector<pair<int, int>>(LOG, {-1, 0}));
  rep(i, 0, M) {
    let node = A[i];
    let parent = lift[node][0];
    if (parent == -1) {
      continue;
    }
    {
      let it = lower_bound(all(posA[parent]), i);
      if (it != posA[parent].end()) {
        goUp[i][0].first = *it;
        goUp[i][0].second = 0;
      } else {
        goUp[i][0].first = *posA[parent].begin();
        goUp[i][0].second = 1;
      }
    }
    {
      let it = upper_bound(all(posA[parent]), i);
      if (it != posA[parent].begin()) {
        goDn[i][0].first = *prev(it);
        goDn[i][0].second = 0;
      } else {
        goDn[i][0].first = *posA[parent].rbegin();
        goDn[i][0].second = 1;
      }
    }
  }
  rep(j, 1, LOG) {
    rep(i, 0, M) {
      if (let[u, d] = goUp[i][j - 1]; u != -1) {
        goUp[i][j].first = goUp[u][j - 1].first;
        goUp[i][j].second = goUp[u][j - 1].second + d;
      }
      if (let[u, d] = goDn[i][j - 1]; u != -1) {
        goDn[i][j].first = goDn[u][j - 1].first;
        goDn[i][j].second = goDn[u][j - 1].second + d;
      }
    }
  }
  int current = 0;
  long long ans = 0;
  for (auto s : S) {
    let lca = getLCA(A[current], s);
    let diffUp = depth[A[current]] - depth[lca];
    let diffDn = depth[s] - depth[lca];
    rep(i, 0, diffUp) {
      ans += goUp[current][0].second;
      current = goUp[current][0].first;
    }
    assert(A[current] == lca);
    let tryFrom = [&](int from) {
      int cost = 0;
      down(i, LOG) {
        if ((diffDn >> i) & 1) {
          cost += goDn[from][i].second;
          from = goDn[from][i].first;
        }
      }
      assert(A[from] == lca);
      if (from < current) {
        cost += 1;
      }
      return cost;
    };
    int lo = 0, hi = sz(posA[s]) - 1;
    let bestAns = tryFrom(posA[s][hi]);
    ans += bestAns;
    while (lo < hi) {
      let mid = (lo + hi) / 2;
      let midAns = tryFrom(posA[s][mid]);
      if (midAns == bestAns) {
        hi = mid;
      } else {
        lo = mid + 1;
      }
    }
    current = posA[s][lo];
  }
  return ans;
}
