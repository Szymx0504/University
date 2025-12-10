#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <chrono>
#include <queue>

using namespace std;

const int MAXN = 100005;
const double TIME_LIMIT = 19.5;
vector<int> adj[MAXN];
int degree[MAXN];

int neighbor_counts[MAXN];
bool on_path[MAXN];
int current_path[MAXN];
int path_len = 0;

int best_path[MAXN];
int best_path_len = 0;

mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());

inline void push_node(int u) {
    current_path[path_len++] = u;
    on_path[u] = true;
    for (int v : adj[u]) {
        neighbor_counts[v]++;
    }
}

inline void clear_state() {
    for (int i = 0; i < path_len; ++i) {
        int u = current_path[i];
        on_path[u] = false;
        for (int v : adj[u]) {
            neighbor_counts[v]--;
        }
    }
    path_len = 0;
}

inline bool extend_path(int head) {
    static int candidates[128];
    int cand_count = 0;
    bool forced_move = false;
    int best_forced = -1;

    int checks = 0;
    for (int v : adj[head]) {
        if (++checks > 20) break;

        if (!on_path[v] && neighbor_counts[v] == 1) {
            
            if (degree[v] <= 2) {
                best_forced = v;
                forced_move = true;
                break; 
            }

            candidates[cand_count++] = v;
            if (cand_count >= 8) break;
        }
    }

    if (forced_move) {
        push_node(best_forced);
        return true;
    }

    if (cand_count == 0) return false;

    int pick_idx = 0;
    if (cand_count > 1) {
        if (rng() % 10 < 6) pick_idx = 0;
        else pick_idx = rng() % cand_count;
    }

    push_node(candidates[pick_idx]);
    return true;
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int N, M;
    if (!(cin >> N >> M)) return 0;

    for (int i = 0; i < M; ++i) {
        int u, v;
        cin >> u >> v;
        adj[u].push_back(v);
        adj[v].push_back(u);
        degree[u]++;
        degree[v]++;
    }

    vector<int> nodes_by_degree(N);
    for (int i = 0; i < N; ++i) {
        nodes_by_degree[i] = i;
        sort(adj[i].begin(), adj[i].end(), [&](int a, int b) {
            return degree[a] < degree[b];
        });
    }

    sort(nodes_by_degree.begin(), nodes_by_degree.end(), [&](int a, int b) {
        return degree[a] < degree[b];
    });

    auto start_time = chrono::steady_clock::now();
    long long iterations = 0;

    while (true) {
        if ((iterations & 4095) == 0) {
            auto now = chrono::steady_clock::now();
            if (chrono::duration<double>(now - start_time).count() > TIME_LIMIT) break;
        }
        iterations++;
        
        int start_idx = 0;
        if (N > 1) {
            int r1 = rng() % N;
            int r2 = rng() % N;
            int r3 = rng() % N;
            start_idx = min({r1, r2, r3});
        }
        int start_node = nodes_by_degree[start_idx];

        push_node(start_node);

        while(extend_path(current_path[path_len - 1]));
        
        if (path_len > 1) {
            for(int i=0; i<path_len/2; ++i) {
                swap(current_path[i], current_path[path_len-1-i]);
            }
            while(extend_path(current_path[path_len - 1]));
        }

        if (path_len > best_path_len) {
            best_path_len = path_len;
            memcpy(best_path, current_path, path_len * sizeof(int));
        }

        clear_state();
    }

    cout << best_path_len << "\n";
    for (int i = 0; i < best_path_len; ++i) {
        cout << best_path[i] << (i == best_path_len - 1 ? "" : " ");
    }
    cout << "\n";

    return 0;
}