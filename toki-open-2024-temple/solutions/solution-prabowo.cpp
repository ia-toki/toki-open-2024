#include "temple.h"

#include <algorithm>
#include <numeric>
#include <vector>

const long long kInfL = 1e18;

long long min_weight(int N, int M, int K,
                     std::vector<int> U, std::vector<int> V, std::vector<int> W,
                     std::vector<int> P) {
  std::vector<long long> dist(N + M, kInfL);
  for (int p : P) {
    dist[p] = 0;
  }

  std::vector<int> par(N + M);
  std::iota(par.begin(), par.end(), 0);
  auto dsu = [&](int u) {
    std::vector<int> chain = {u};
    while (par[chain.back()] != chain.back()) {
      chain.push_back(par[chain.back()]);
    }
    for (int i = (int) chain.size() - 1; i >= 0; --i) {
      dist[chain[i]] = std::min(dist[chain[i]], dist[par[chain[i]]]);
      par[chain[i]] = chain.back();
    }
    return chain.back();
  };

  std::vector<int> edges(M);
  std::iota(edges.begin(), edges.end(), 0);
  std::sort(edges.begin(), edges.end(), [&](int i, int j) {
    return W[i] < W[j];
  });
  for (int i = 0; i < M; ) {
    int j = i;
    while (j < M && W[edges[j]] == W[edges[i]]) {
      int u = U[edges[j]], v = V[edges[j]];
      par[dsu(u)] = par[dsu(v)] = N + j;
      ++j;
    }

    for (int k = i; k < j; ++k) {
      int u = U[edges[k]], v = V[edges[k]], w = W[edges[k]];
      int headU = dsu(u), headV = dsu(v);
      dist[headU] = std::min(dist[headU], dist[v] + w);
      dist[headV] = std::min(dist[headV], dist[u] + w);
    }
    i = j;
  }

  dsu(0);
  return dist[0];
}
