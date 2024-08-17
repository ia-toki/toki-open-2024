#include "copper.h"

#include <cstdio>
#include <vector>

std::vector<std::vector<int>> buttons;
struct State {
  int hand;
  long long cost;
  State(int _hand=0, long long _cost=0): hand(_hand), cost(_cost) {}

  void press(int node) {
    auto idx = lower_bound(buttons[node].begin(), buttons[node].end(), hand);
    if (idx == buttons[node].end()) {
      hand = buttons[node][0];
      ++cost;
    } else {
      hand = *idx;
    }
  }

  void advance(std::vector<std::vector<State>> &sparse, int step) {
    for (int k = (int) sparse.size() - 1; k >= 0; --k) {
      if (step >= (1 << k)) {
        step -= (1 << k);
        cost += sparse[k][hand].cost;
        hand = sparse[k][hand].hand;
      }
    }
  }
};

std::vector<std::vector<int>> adj;

std::vector<int> depth, heavy, size, par, head;
void dfs(int u, int p=-1) {
  size[u] = 1;
  for (int v : adj[u]) {
    if (v == p) {
      continue;
    }
    par[v] = u;
    depth[v] = depth[u] + 1;
    dfs(v, u);
    size[u] += size[v];
    if (heavy[u] == -1 || size[heavy[u]] < size[v]) {
      heavy[u] = v;
    }
  }
}

long long min_risk(int N, int M, int L, std::vector<int> U, std::vector<int> V,
                   std::vector<int> A, std::vector<int> S) {
  adj.resize(N);
  for (int i = 0; i < N - 1; ++i) {
    adj[U[i]].push_back(V[i]);
    adj[V[i]].push_back(U[i]);
  }

  depth.assign(N, 0); heavy.assign(N, -1); size.assign(N, 0); par.assign(N, 0);
  dfs(0);

  head.resize(N);
  for (int i = 0; i < N; ++i) {
    if (i == 0 || heavy[par[i]] != i) {
      for (int j = i; j != -1; j = heavy[j]) {
        head[j] = i;
      }
    }
  }

  buttons.resize(N);
  for (int j = 0; j < M; ++j) {
    buttons[A[j]].push_back(j);
  }

  int logM = 32 - __builtin_clz(M);
  std::vector<std::vector<State>> up(logM, std::vector<State>(M)), dn;
  for (int j = 0; j < M; ++j) {
    up[0][j] = State(j, 0);
  }
  dn = up;
  for (int i = 0; i < N; ++i) {
    if (heavy[i] == -1) {
      continue;
    }

    for (int j : buttons[i]) {
      dn[0][j].press(heavy[i]);
    }

    for (int j : buttons[heavy[i]]) {
      up[0][j].press(i);
    }
  }

  for (int k = 1; k < logM; ++k) {
    for (int j = 0; j < M; ++j) {
      up[k][j] = up[k - 1][j];
      up[k][j].advance(up, 1 << (k - 1));
      dn[k][j] = dn[k - 1][j];
      dn[k][j].advance(dn, 1 << (k - 1));
    }
  }

  State state = State(0, 0LL);
  for (int v : S) {
    std::vector<int> downs = {v};
    while (head[A[state.hand]] != head[v]) {
      int u = A[state.hand];
      if (depth[head[u]] < depth[head[v]]) {
        downs.push_back(head[v]);
        v = par[head[v]];
      } else {
        state.advance(up, depth[u] - depth[head[u]]);
        state.press(par[head[u]]);
      }
    }

    int u = A[state.hand];
    if (depth[u] < depth[v]) {
      state.advance(dn, depth[v] - depth[u]);
    } else {
      state.advance(up, depth[u] - depth[v]);
    }

    while (!downs.empty()) {
      int u = A[state.hand];
      if (head[downs.back()] == head[u]) {
        state.advance(dn, depth[downs.back()] - depth[u]);
      } else {
        state.advance(dn, depth[par[downs.back()]] - depth[u]);
        state.press(downs.back());
      }
      downs.pop_back();
    }
  }

  return state.cost;
}
