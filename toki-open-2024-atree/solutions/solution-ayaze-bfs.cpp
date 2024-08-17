#include "copper.h"

#include <bits/stdc++.h>
using namespace std;

const int kMaxL = 4005;
const int kMaxN = 4005;
const int kMaxM = 4005;

int dist[kMaxL][kMaxM];
vector<int> buttons[kMaxN];
vector<int> adj[kMaxN];
vector<int> adjButton[kMaxN];

long long min_risk(int N, int M, int L, std::vector<int> U, std::vector<int> V,
                   std::vector<int> A, std::vector<int> S) {
    for (int i = 0 ; i+1 < N ; i++) {
        adj[U[i]].push_back(V[i]);
        adj[V[i]].push_back(U[i]);
    }

    for (int i = M-1 ; i >= 0 ; i--) {
        buttons[A[i]].push_back(i);
        for (int nxt : adj[A[i]]) {
            if (!buttons[nxt].empty()) {
                // smallest that is larger
                adjButton[i].push_back(buttons[nxt].back());
            }
        }
    }

    for (int i = 0 ; i < M ; i++) {
        for (int nxt : adj[A[i]]) {
            // smallest button
            adjButton[i].push_back(buttons[nxt].back());
        }
    }

    for (int i = 0 ; i <= L ; i++) {
        for (int j = 0 ; j < M ; j++) {
            dist[i][j] = 1e9;
        }
    }

    deque<tuple<int, int, int>> dq;
    for (int b : buttons[0]) {
        dist[0][b] = 0;
        dq.push_back({0, b, 0});
    }

    while (!dq.empty()) {
        auto [idx, button, d] = dq.front();
        dq.pop_front();

        if (dist[idx][button] != d) continue;
        if (idx == L) continue;

        int cur = A[button];
        if (cur == S[idx] && dist[idx+1][button] > dist[idx][button]) {
            dist[idx+1][button] = dist[idx][button];
            dq.push_front({idx+1, button, dist[idx][button]});
        }

        for (int nextButton : adjButton[button]) {
            int nextDist = dist[idx][button] + (button > nextButton);
            if (nextDist < dist[idx][nextButton]) {
                dist[idx][nextButton] = nextDist;

                if (button > nextButton) {
                    dq.push_back({idx, nextButton, nextDist});
                } else {
                    dq.push_front({idx, nextButton, nextDist});
                }
            }
        }
    }

    int ret = 1e9;
    for (int i = 0 ; i < M ; i++) {
        ret = min(ret, dist[L][i]);
    }

    return ret;
}
