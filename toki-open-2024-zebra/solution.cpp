#include "zebra.h"

#include <vector>
#include <utility>

int find_root(int N, std::vector<int> U, std::vector<int> V) {
  std::vector<std::vector<std::pair<int, int>>> adj(N);
  for (int i = 0; i < N - 1; ++i) {
    adj[U[i]].emplace_back(V[i], i);
    adj[V[i]].emplace_back(U[i], i);
  }
  int root = 0;
  for (int i = 0; i < N; ++i) {
    if (adj[i].size() > 1) {
      root = i;
      break;
    }
  }

  std::vector<int> order(N), edges(N), depth(N, -1), queue = {root};
  depth[root] = 0;
  for (int i = 0; i < N; ++i) {
    int u = queue[i];
    order[u] = i;
    for (auto [v, idx] : adj[u]) {
      if (depth[v] != -1) {
        continue;
      }
      edges[v] = idx;
      depth[v] = depth[u] + 1;
      queue.push_back(v);
    }
  }

  std::vector<int> colours[2];
  for (int i = 1; i < N; ++i) {
    colours[depth[queue[i]] & 1].push_back(queue[i]);
  }

  auto isRoot = [&](int x) {
    std::vector<int> queue = {x};
    std::vector<int> col(N, -1), asks[2];
    col[x] = 0;
    for (int i = 0; i < N; ++i) {
      int u = queue[i];
      for (auto [v, idx] : adj[u]) {
        if (col[v] != -1) {
          continue;
        }
        col[v] = col[u] ^ 1;
        asks[col[v]].push_back(idx);
        queue.push_back(v);
      }
    }
    return (asks[0].empty() || is_zebra(asks[0]) == 0) && (asks[1].empty() || is_zebra(asks[1]) == 0);
  };

  if (colours[0].empty()) {
    if (isRoot(root)) return root;
    if (isRoot(colours[1].back())) return colours[1].back();
  } else {
    if (isRoot(colours[1].back())) return colours[1].back();
    if (isRoot(colours[0].back())) return colours[0].back();
  }

  auto colourFind = [&](std::vector<int> &colours) {
    int ret = root;
    int l = 0, r = colours.size() - 2;
    while (l <= r) {
      int mid = (l + r) / 2;
      
      std::vector<int> ask;
      for (int i = mid; i < colours.size(); ++i) {
        ask.push_back(edges[colours[i]]);
      }

      if (is_zebra(ask)) {
        ret = colours[mid];
        l = mid + 1;
      } else {
        r = mid - 1;
      }
    }
    return ret;
  };

  int ans0 = colourFind(colours[0]), ans1 = colourFind(colours[1]);
  if (order[ans0] < order[ans1]) {
    return ans1;
  }
  return ans0;
}
