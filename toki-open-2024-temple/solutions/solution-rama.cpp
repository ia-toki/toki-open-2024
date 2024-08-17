#include "temple.h"

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

long long min_weight(int N, int M, int K, std::vector<int> U,
                     std::vector<int> V, std::vector<int> W,
                     std::vector<int> P) {
  int num_nodes = N;
  vector<int> dsu(N, -1);
  vector<vector<pair<int, int>>> adj(N);
  auto find = YComb([&](auto self, int x) -> int {
    return dsu[x] < 0 ? x : dsu[x] = self(dsu[x]);
  });
  auto new_node = [&](int w) {
    dsu.push_back(-1);
    adj.emplace_back();
    return num_nodes++;
  };
  vector<int> order_edges(M);
  iota(all(order_edges), 0);
  sort(all(order_edges), [&](int i, int j) { return W[i] < W[j]; });
  rep(i, 0, sz(order_edges)) {
    int j = i;
    while (j + 1 < sz(order_edges) &&
           W[order_edges[j + 1]] == W[order_edges[i]]) {
      ++j;
    }
    rep(k, i, j + 1) {
      int e = order_edges[k];
      int u = find(U[e]), v = find(V[e]);
      if (u != v) {
        int node = new_node(0);
        dsu[u] = dsu[v] = node;
        adj[node].push_back({u, 0});
        adj[node].push_back({v, 0});
      }
    }
    rep(k, i, j + 1) {
      int e = order_edges[k];
      int u = U[e], v = V[e], w = W[e];
      int node = find(u);
      adj[u].push_back({node, w});
      adj[v].push_back({node, w});
    }
    i = j;
  }
  vector<lint> dist(num_nodes, -1);
  priority_queue<pair<lint, int>> pq;
  auto relax = [&](int node, lint d) {
    if (dist[node] == -1 || dist[node] > d) {
      dist[node] = d;
      pq.emplace(-d, node);
    }
  };
  for (auto p : P) {
    relax(p, 0);
  }
  while (!pq.empty()) {
    auto [d, node] = pq.top();
    d *= -1;
    pq.pop();
    if (d != dist[node]) continue;
    for (auto [to, w] : adj[node]) {
      relax(to, d + w);
    }
  }
  return dist[0];
}
