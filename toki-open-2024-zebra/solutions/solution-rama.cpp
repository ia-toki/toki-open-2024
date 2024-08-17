#include "zebra.h"

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

int find_root(int N, std::vector<int> U, std::vector<int> V) {
  vector<vector<pair<int, int>>> adj(N);
  rep(i, 0, N - 1) {
    adj[U[i]].push_back({V[i], i});
    adj[V[i]].push_back({U[i], i});
  }
  int numQueries = 0;
  auto IsMonochrome = [&](vector<int> edges) {
    sort(all(edges));
    edges.erase(unique(all(edges)), end(edges));
    if (sz(edges) <= 1) return true;
    assert(++numQueries <= 19);
    return !is_zebra(edges);
  };
  auto IsRoot = [&](int u) {  // Check if u is a root in 3 queries
    vector<int> que = {u};
    vector<int> depth(N, -1);
    vector<int> parentEdge(N, -1);
    depth[u] = 0;
    rep(q, 0, sz(que)) {
      int u = que[q];
      for (auto [v, i] : adj[u])
        if (depth[v] == -1) {
          depth[v] = depth[u] + 1;
          parentEdge[v] = i;
          que.push_back(v);
        }
    }
    vector<int> odd, even;
    for (int i = 1; i < sz(que); i++) {
      (depth[que[i]] & 1 ? odd : even).push_back(parentEdge[que[i]]);
    }
    assert(!odd.empty());
    if (odd.empty() || even.empty()) {
      return IsMonochrome(odd) && IsMonochrome(even);
    } else {
      return IsMonochrome(odd) && IsMonochrome(even) &&
             !IsMonochrome({odd[0], even[0]});
    }
  };
  auto FindFurthest = YComb([&](auto self, int u, int p) -> pii {
    pair<int, int> res = {0, u};
    for (auto [v, i] : adj[u])
      if (v != p) {
        auto [d, x] = self(v, u);
        res = max(res, {d + 1, x});
      }
    return res;
  });
  auto GetOtherEnd = [&](int u, int e) { return U[e] ^ V[e] ^ u; };
  auto diamUp = FindFurthest(FindFurthest(0, -1).second, -1).second;
  auto [diamLength, diamDn] = FindFurthest(diamUp, -1);
  assert(diamLength >= 2);
  vector<int> depth(N, -1);
  vector<int> parentEdge(N, -1);
  YComb([&](auto self, int u, int p) -> void {
    depth[u] = p != -1 ? depth[p] + 1 : 0;
    for (auto [v, i] : adj[u])
      if (v != p) {
        parentEdge[v] = i;
        self(v, u);
      }
  })(diamUp, -1);
  // Special Case: Star Graph (13 queries)
  if (diamLength == 2) {
    int center = GetOtherEnd(diamDn, parentEdge[diamDn]);
    if (IsRoot(center)) return center;  // 3 queries
    // There is a single edge with a different color (10 queries)
    int lo = 0, hi = sz(adj[center]);
    while (lo < hi) {
      int mid = (lo + hi) / 2;
      vector<int> edges;
      rep(i, 0, mid + 1) edges.push_back(adj[center][i].second);
      if (IsMonochrome(edges)) {
        lo = mid + 1;
      } else {
        hi = mid;
      }
    }
    assert(lo < sz(adj[center]));
    if (lo == 1 &&
        IsMonochrome({adj[center][lo].second, adj[center].back().second})) {
      return GetOtherEnd(center, adj[center][0].second);
    } else {
      return GetOtherEnd(center, adj[center][lo].second);
    }
  }
  assert(diamLength >= 3);
  // Phase 1: Check if the line diamUp..diamDn has alternating colors (2 queries)
  {
    vector<int> diamEdges;
    for (int u = diamDn; u != diamUp; u = GetOtherEnd(u, parentEdge[u])) {
      diamEdges.push_back(parentEdge[u]);
    }
    vector<int> diamEdgesOdd, diamEdgesEven;
    rep(i, 0, sz(diamEdges)) {
      (i & 1 ? diamEdgesOdd : diamEdgesEven).push_back(diamEdges[i]);
    }
    if (IsMonochrome(diamEdgesOdd) && IsMonochrome(diamEdgesEven)) {
      // The line is alternating, so the root is either diamUp or diamDn (6 queries)
      if (IsRoot(diamUp)) return diamUp;
      if (IsRoot(diamDn)) return diamDn;
      return -1;
    }
  }
  // Phase 2: Binary Search for the first non-monochrome edge
  // with same depth parity as diamDn (9 queries)
  auto badVertex = [&]() {
    vector<pair<int, int>> edgesToSearch;
    rep(i, 0, N) {
      if (i != diamUp && depth[i] % 2 == depth[diamDn] % 2) {
        edgesToSearch.push_back({parentEdge[i], i});
      }
    }
    sort(all(edgesToSearch), [&](auto a, auto b) {
      return pair(depth[a.second], a.second == diamDn) <
             pair(depth[b.second], b.second == diamDn);
    });
    assert(edgesToSearch.back().second == diamDn);
    int lo = -1, hi = sz(edgesToSearch);
    while (lo < hi) {
      int mid = (lo + hi + 1) >> 1;
      vector<int> edges;
      rep(i, mid, sz(edgesToSearch)) edges.push_back(edgesToSearch[i].first);
      if (IsMonochrome(edges)) {
        hi = mid - 1;
      } else {
        lo = mid;
      }
    }
    return lo < 0 ? -1 : edgesToSearch[lo].second;
  }();
  if (badVertex == -1) {
    return adj[diamUp][0].first;
  }
  // Phase 3: The root is either badVertex or one of its children (9 queries)
  {
    vector<int> badChildren;
    for (auto [v, i] : adj[badVertex]) {
      if (depth[v] > depth[badVertex]) {
        badChildren.push_back(v);
      }
    }
    int lo = 0, hi = sz(badChildren);
    while (lo < hi) {
      int mid = (lo + hi) / 2;
      vector<int> edges = {parentEdge[badVertex]};
      rep(i, 0, mid + 1) edges.push_back(parentEdge[badChildren[i]]);
      if (IsMonochrome(edges)) {
        lo = mid + 1;
      } else {
        hi = mid;
      }
    }
    if (lo == sz(badChildren)) {
      return badVertex;
    }
    return badChildren[lo];
  }
  // Total Queries: 2 + 9 + 9 = 20
  // Actually, Phase 2 and Phase 3 combined use at most 17 queries
  // So worst case is actually 19 queries
  return -1;
}
