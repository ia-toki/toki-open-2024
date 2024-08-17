#include "secret.h"

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

int M;
vector<int> coords;
vector<pair<int, int>> paths;
set<int> is_blocked;

int toCoord(int x) {
  int z = int(upper_bound(all(coords), x) - coords.begin()) - 1;
  if (z < 0) z += sz(coords);
  return z;
}

struct RangeQuery {  // O(sqrt(N)) point update, O(1) range query
  static const int blockSize = 320;

  int N;
  vector<int> blockPrefixSum;
  vector<array<int, blockSize>> inBlockPrefixSum;

  RangeQuery() = default;

  RangeQuery(int N) : N(N) {
    blockPrefixSum.assign((N + blockSize - 1) / blockSize, 0);
    inBlockPrefixSum.assign((N + blockSize - 1) / blockSize, {});
  }

  void update(int p, int v) {
    int block = p / blockSize;
    int inBlock = p % blockSize;
    rep(i, block + 1, sz(blockPrefixSum)) { blockPrefixSum[i] += v; }
    rep(i, inBlock, blockSize) { inBlockPrefixSum[block][i] += v; }
  }

  int query(int p) {
    if (p < 0) return 0;
    int block = p / blockSize;
    int inBlock = p % blockSize;
    return blockPrefixSum[block] + inBlockPrefixSum[block][inBlock];
  }

  int query(int l, int r) { return query(r) - query(l - 1); }
} rangeQuery;

struct Blocks {  // O(sqrt(N log N)) per update and query
  struct OneBlock {
    int L, R;
    vector<int> regionCoords;

    int cntRegionBlocked;
    int whichRegionIsBlocked;
    lint costIfAllUnblocked;
    vector<int> isRegionBlocked;
    vector<lint> costIfRegionIsBlocked;

    OneBlock(int L, int R) {
      this->L = L;
      this->R = R;
      rep(i, L, R + 1) {
        auto [u, v] = paths[i];
        regionCoords.push_back(u);
        regionCoords.push_back(v);
      }
      sort(all(regionCoords));
      regionCoords.erase(unique(all(regionCoords)), regionCoords.end());
      cntRegionBlocked = 0;
      whichRegionIsBlocked = 0;
      costIfAllUnblocked = 0;
      isRegionBlocked.assign(sz(regionCoords), 0);
      costIfRegionIsBlocked.assign(sz(regionCoords), 0);
      rep(i, L, R + 1) {
        auto [u, v] = paths[i];
        u = toRegionIndex(u);
        v = toRegionIndex(v);
        if (u > v) swap(u, v);
        costIfAllUnblocked +=
            min(abs(coords[regionCoords[u]] - coords[regionCoords[v]]),
                M - abs(coords[regionCoords[u]] - coords[regionCoords[v]]));
        // add to u, u+1, ..., v-1
        costIfRegionIsBlocked[u] +=
            M - abs(coords[regionCoords[u]] - coords[regionCoords[v]]);
        costIfRegionIsBlocked[v] -=
            M - abs(coords[regionCoords[u]] - coords[regionCoords[v]]);
        // add to v, v+1, ..., u-1
        costIfRegionIsBlocked[0] +=
            abs(coords[regionCoords[u]] - coords[regionCoords[v]]);
        costIfRegionIsBlocked[u] -=
            abs(coords[regionCoords[u]] - coords[regionCoords[v]]);
        costIfRegionIsBlocked[v] +=
            abs(coords[regionCoords[u]] - coords[regionCoords[v]]);
      }
      rep(i, 1, sz(regionCoords)) {
        costIfRegionIsBlocked[i] += costIfRegionIsBlocked[i - 1];
      }
    }

    void toggle(int R, int sgn) {
      int regionIndex = toRegionIndex(R);
      isRegionBlocked[regionIndex] += sgn;
      if (sgn == 1 && isRegionBlocked[regionIndex] == 1) {
        ++cntRegionBlocked;
        whichRegionIsBlocked ^= regionIndex;
      }
      if (sgn == -1 && isRegionBlocked[regionIndex] == 0) {
        --cntRegionBlocked;
        whichRegionIsBlocked ^= regionIndex;
      }
    }

    lint query() {
      if (cntRegionBlocked > 1) return -1;
      if (cntRegionBlocked == 0) return costIfAllUnblocked;
      assert(0 <= whichRegionIsBlocked &&
             whichRegionIsBlocked < sz(regionCoords));
      return costIfRegionIsBlocked[whichRegionIsBlocked];
    }

    int toRegionIndex(int x) {
      int z = int(upper_bound(all(regionCoords), x) - regionCoords.begin()) - 1;
      if (z < 0) z += sz(regionCoords);
      return z;
    }
  };

  static const int blockSize = 600;

  vector<OneBlock> blocks;

  void init() {
    int L = 0;
    while (L < sz(paths)) {
      int R = min(L + blockSize, sz(paths) - 1);
      blocks.emplace_back(L, R);
      L = R + 1;
    }
  }

  void toggle(int R, int sgn) {
    for (auto& block : blocks) {
      block.toggle(R, sgn);
    }
  }

  lint query(int S, int T) {
    lint res = 0;
    for (auto& block : blocks) {
      if (block.R < S) continue;
      if (block.L > T) break;
      if (S <= block.L && block.R <= T) {
        auto block_res = block.query();
        if (block_res == -1) return -1;
        res += block_res;
      } else {
        rep(i, max(S, block.L), min(T, block.R) + 1) {
          auto [u, v] = paths[i];
          if (u > v) swap(u, v);
          bool can_uv = rangeQuery.query(u, v - 1) == 0;
          bool can_vu = rangeQuery.query(0, u - 1) == 0 &&
                        rangeQuery.query(v, sz(coords) - 1) == 0;
          if (!can_uv && !can_vu) {
            return -1;
          } else if (can_uv && can_vu) {
            res +=
                min(abs(coords[u] - coords[v]), M - abs(coords[u] - coords[v]));
          } else if (can_uv) {
            res += abs(coords[u] - coords[v]);
          } else {
            res += M - abs(coords[u] - coords[v]);
          }
        }
      }
    }
    return res;
  }
} blocks;

void init(int N, int M, std::vector<int> A) {
  ::M = M;
  coords = A;
  sort(all(coords));
  coords.erase(unique(all(coords)), coords.end());
  rangeQuery = RangeQuery(sz(coords));
  rep(i, 0, N - 1) {
    int u = A[i], v = A[i + 1];
    paths.emplace_back(toCoord(u), toCoord(v));
  }
  blocks.init();
  return;
}

void toggle(int R) {
  bool is_toggled_R = is_blocked.count(R);
  if (is_toggled_R) {
    is_blocked.erase(R);
  } else {
    is_blocked.insert(R);
  }
  rangeQuery.update(toCoord(R), is_toggled_R ? -1 : 1);
  blocks.toggle(toCoord(R), is_toggled_R ? -1 : 1);
  return;
}

long long query(int S, int T) { return blocks.query(S, T - 1); }
