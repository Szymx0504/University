#include <bits/stdc++.h>
using namespace std;
using ll = long long;
const double TOTAL_LIMIT = 19.0;

struct Edge { 
    int to; 
    int w1; 
    int w2; 
};

struct TimeManager {
    chrono::steady_clock::time_point start;
    double total_budget;
    
    TimeManager(double budget) : start(chrono::steady_clock::now()), total_budget(budget) {}
    
    bool should_continue(double min_time = 0.01) const {
        auto now = chrono::steady_clock::now();
        double elapsed = chrono::duration<double>(now - start).count();
        return elapsed < total_budget - min_time;
    }
    
    double elapsed() const {
        auto now = chrono::steady_clock::now();
        return chrono::duration<double>(now - start).count();
    }
    
    double remaining() const {
        return max(0.0, total_budget - elapsed());
    }
};

vector<bool> is_prime;

void sieve_primes(int limit) {
    is_prime.assign(limit+1, true);
    if (limit >= 0) is_prime[0] = false;
    if (limit >= 1) is_prime[1] = false;
    for (int p = 2; p * p <= limit; ++p) {
        if (is_prime[p]) {
            for (int q = p * p; q <= limit; q += p) {
                is_prime[q] = false;
            }
        }
    }
}

vector<int> compute_prime_prefix(int max_len) {
    vector<int> prefix(max_len + 1, 0);
    for (int i = 1; i <= max_len; ++i) {
        prefix[i] = prefix[i-1] + (is_prime[i] ? 1 : 0);
    }
    return prefix;
}

vector<int> bfs_path(int N, const vector<vector<Edge>>& g, int src, int tgt) {
    vector<int> par(N, -1);
    vector<bool> vis(N, false);
    queue<int> q;
    q.push(src);
    vis[src] = true;
    par[src] = -1;
    
    while (!q.empty()) {
        int u = q.front(); q.pop();
        if (u == tgt) break;
        
        for (const auto& e : g[u]) {
            if (!vis[e.to]) {
                vis[e.to] = true;
                par[e.to] = u;
                q.push(e.to);
            }
        }
    }
    
    if (!vis[tgt]) return {};
    
    vector<int> path;
    for (int cur = tgt; cur != -1; cur = par[cur]) {
        path.push_back(cur);
    }
    reverse(path.begin(), path.end());
    return path;
}

ll evaluate_path_cost(const vector<int>& path, const vector<vector<Edge>>& g) {
    if (path.size() <= 1) return 0;
    
    ll total_cost = 0;
    for (size_t i = 1; i < path.size(); ++i) {
        int u = path[i-1], v = path[i];
        bool found = false;
        
        for (const auto& e : g[u]) {
            if (e.to == v) {
                if (is_prime[i]) {
                    total_cost += 3LL * e.w2;
                } else {
                    total_cost += e.w1;
                }
                found = true;
                break;
            }
        }
        
        if (!found) {
            for (const auto& e : g[v]) {
                if (e.to == u) {
                    if (is_prime[i]) {
                        total_cost += 3LL * e.w2;
                    } else {
                        total_cost += e.w1;
                    }
                    found = true;
                    break;
                }
            }
        }
        
        if (!found) return 1LL << 60;
    }
    return total_cost;
}

vector<int> dijkstra_path_w1(int N, const vector<vector<Edge>>& g, int src, int tgt) {
    const ll INF = 1LL << 60;
    vector<ll> dist(N, INF);
    vector<int> par(N, -1);
    
    using QueueItem = pair<ll, int>;
    priority_queue<QueueItem, vector<QueueItem>, greater<QueueItem>> pq;
    
    dist[src] = 0;
    pq.push(make_pair(0, src));
    
    while (!pq.empty()) {
        QueueItem item = pq.top(); pq.pop();
        ll d = item.first;
        int u = item.second;
        
        if (d != dist[u]) continue;
        if (u == tgt) break;
        
        for (const auto& e : g[u]) {
            ll cost = e.w1;
            if (dist[e.to] > d + cost) {
                dist[e.to] = d + cost;
                par[e.to] = u;
                pq.push(make_pair(dist[e.to], e.to));
            }
        }
    }
    
    if (par[tgt] == -1 && src != tgt) return {};
    
    vector<int> path;
    for (int cur = tgt; cur != -1; cur = par[cur]) {
        path.push_back(cur);
    }
    reverse(path.begin(), path.end());
    return path;
}

vector<int> dijkstra_path_w2(int N, const vector<vector<Edge>>& g, int src, int tgt) {
    const ll INF = 1LL << 60;
    vector<ll> dist(N, INF);
    vector<int> par(N, -1);
    
    using QueueItem = pair<ll, int>;
    priority_queue<QueueItem, vector<QueueItem>, greater<QueueItem>> pq;
    
    dist[src] = 0;
    pq.push(make_pair(0, src));
    
    while (!pq.empty()) {
        QueueItem item = pq.top(); pq.pop();
        ll d = item.first;
        int u = item.second;
        
        if (d != dist[u]) continue;
        if (u == tgt) break;
        
        for (const auto& e : g[u]) {
            ll cost = 3LL * e.w2;
            if (dist[e.to] > d + cost) {
                dist[e.to] = d + cost;
                par[e.to] = u;
                pq.push(make_pair(dist[e.to], e.to));
            }
        }
    }
    
    if (par[tgt] == -1 && src != tgt) return {};
    
    vector<int> path;
    for (int cur = tgt; cur != -1; cur = par[cur]) {
        path.push_back(cur);
    }
    reverse(path.begin(), path.end());
    return path;
}

vector<ll> compute_distances(int N, const vector<vector<Edge>>& g, int target, 
                            function<ll(const Edge&)> cost_fn) {
    const ll INF = 1LL << 60;
    vector<ll> dist(N, INF);
    
    using QueueItem = pair<ll, int>;
    priority_queue<QueueItem, vector<QueueItem>, greater<QueueItem>> pq;
    
    dist[target] = 0;
    pq.push(make_pair(0, target));
    
    while (!pq.empty()) {
        QueueItem item = pq.top(); pq.pop();
        ll d = item.first;
        int u = item.second;
        
        if (d != dist[u]) continue;
        
        for (const auto& e : g[u]) {
            ll cost = cost_fn(e);
            if (dist[e.to] > d + cost) {
                dist[e.to] = d + cost;
                pq.push(make_pair(dist[e.to], e.to));
            }
        }
    }
    return dist;
}

// Improved heuristic that considers prime distribution
ll compute_heuristic(int current_len, int remaining_dist_w1, int remaining_dist_w2, 
                    const vector<int>& prime_prefix, int total_estimate_len) {
    if (remaining_dist_w1 == 0) return 0;
    
    // Estimate how many prime steps remain
    int estimated_total_len = current_len + total_estimate_len;
    estimated_total_len = min(estimated_total_len, (int)prime_prefix.size() - 1);
    
    int current_prime_count = prime_prefix[current_len];
    int estimated_total_primes = prime_prefix[estimated_total_len];
    int remaining_primes = max(0, estimated_total_primes - current_prime_count);
    
    // Weighted combination based on prime ratio
    double prime_ratio = (remaining_primes > 0) ? 
        min(1.0, (double)remaining_primes / total_estimate_len) : 0.0;
    
    return (ll)((1.0 - prime_ratio) * remaining_dist_w1 + prime_ratio * remaining_dist_w2);
}

vector<int> find_path_astar_improved(int N, const vector<vector<Edge>>& g, int A, int B, 
                                   const vector<ll>& dist_w1, const vector<ll>& dist_w2,
                                   const vector<int>& prime_prefix) {
    const ll INF = 1LL << 60;
    
    vector<ll> g_score(N, INF);
    vector<ll> f_score(N, INF);
    vector<int> parent(N, -1);
    vector<int> path_len_to(N, 0);
    
    using QueueItem = pair<ll, int>;
    priority_queue<QueueItem, vector<QueueItem>, greater<QueueItem>> open_set;
    
    g_score[A] = 0;
    path_len_to[A] = 0;
    
    // Better initial heuristic
    int est_total_len = dist_w1[A] / 10; // rough estimate
    ll h = compute_heuristic(0, dist_w1[A], dist_w2[A], prime_prefix, est_total_len);
    f_score[A] = h;
    open_set.push(make_pair(f_score[A], A));
    
    while (!open_set.empty()) {
        QueueItem item = open_set.top(); open_set.pop();
        ll f_val = item.first;
        int u = item.second;
        
        if (f_val != f_score[u]) continue;
        if (u == B) break;
        
        int current_path_len = path_len_to[u];
        
        for (const auto& e : g[u]) {
            int new_path_len = current_path_len + 1;
            ll edge_cost = is_prime[new_path_len] ? 3LL * e.w2 : e.w1;
            ll tentative_g = g_score[u] + edge_cost;
            
            if (tentative_g < g_score[e.to]) {
                parent[e.to] = u;
                g_score[e.to] = tentative_g;
                path_len_to[e.to] = new_path_len;
                
                // Improved heuristic
                int est_remaining_len = max(1, (int)(dist_w1[e.to] / 10));
                ll heuristic = compute_heuristic(new_path_len, dist_w1[e.to], dist_w2[e.to], 
                                               prime_prefix, est_remaining_len);
                f_score[e.to] = tentative_g + heuristic;
                open_set.push(make_pair(f_score[e.to], e.to));
            }
        }
    }
    
    if (parent[B] == -1 && A != B) return {};
    
    vector<int> path;
    for (int cur = B; cur != -1; cur = parent[cur]) {
        path.push_back(cur);
    }
    reverse(path.begin(), path.end());
    return path;
}

struct BeamState {
    vector<int> path;
    ll cost;
    int current_len;
    ll heuristic_score;
    
    bool operator>(const BeamState& other) const {
        return heuristic_score > other.heuristic_score;
    }
};

vector<int> beam_search_improved(const vector<vector<Edge>>& g, int A, int B, 
                                const vector<ll>& dist_w1, const vector<ll>& dist_w2,
                                const vector<int>& prime_prefix, int beam_width, 
                                const TimeManager& tm) {
    int N = g.size();
    
    // Use a vector and sort instead of priority_queue with lambda
    vector<BeamState> beam;
    
    BeamState initial;
    initial.path.push_back(A);
    initial.cost = 0;
    initial.current_len = 0;
    initial.heuristic_score = compute_heuristic(0, dist_w1[A], dist_w2[A], prime_prefix, dist_w1[A]/10);
    beam.push_back(initial);
    
    vector<int> best_path;
    ll best_cost = 1LL << 60;
    
    int max_depth = min(200, N * 2); // Limit depth to avoid explosion
    
    for (int depth = 0; depth < max_depth && !beam.empty() && tm.should_continue(0.01); ++depth) {
        vector<BeamState> next_beam;
        unordered_map<int, ll> best_cost_at_node;
        
        for (const auto& state : beam) {
            int u = state.path.back();
            
            if (u == B) {
                if (state.cost < best_cost) {
                    best_cost = state.cost;
                    best_path = state.path;
                }
                continue;
            }
            
            if (state.cost > best_cost) continue;
            
            for (const auto& e : g[u]) {
                // Skip if already in path (avoid cycles)
                bool in_path = false;
                for (int node : state.path) {
                    if (node == e.to) {
                        in_path = true;
                        break;
                    }
                }
                if (in_path) continue;
                
                int new_len = state.current_len + 1;
                ll edge_cost = is_prime[new_len] ? 3LL * e.w2 : e.w1;
                ll new_cost = state.cost + edge_cost;
                
                // Prune if we've seen this node with better cost
                if (best_cost_at_node.count(e.to) && new_cost >= best_cost_at_node[e.to]) {
                    continue;
                }
                best_cost_at_node[e.to] = new_cost;
                
                int est_remaining_len = max(1, (int)(dist_w1[e.to] / 10));
                ll heuristic = compute_heuristic(new_len, dist_w1[e.to], dist_w2[e.to], 
                                               prime_prefix, est_remaining_len);
                
                BeamState new_state;
                new_state.path = state.path;
                new_state.path.push_back(e.to);
                new_state.cost = new_cost;
                new_state.current_len = new_len;
                new_state.heuristic_score = new_cost + heuristic;
                
                next_beam.push_back(new_state);
            }
        }
        
        // Keep only beam_width best states
        if (next_beam.size() > beam_width) {
            nth_element(next_beam.begin(), next_beam.begin() + beam_width, next_beam.end(),
                [](const BeamState& a, const BeamState& b) {
                    return a.heuristic_score < b.heuristic_score;
                });
            next_beam.resize(beam_width);
        }
        
        beam = move(next_beam);
    }
    
    // Check final beam for solutions
    for (const auto& state : beam) {
        if (state.path.back() == B && state.cost < best_cost) {
            best_cost = state.cost;
            best_path = state.path;
        }
    }
    
    return best_path;
}

bool try_fast_2opt(vector<int>& path, ll& best_cost, const vector<vector<Edge>>& g, 
                  const TimeManager& tm) {
    int n = path.size();
    if (n < 4) return false;
    
    bool improved = false;
    const int MAX_TRIES = n * 10; // Limit attempts
    
    for (int attempt = 0; attempt < MAX_TRIES && tm.should_continue(0.0005); ++attempt) {
        int i = 1 + rand() % (n - 3);
        int j = i + 2 + rand() % (n - i - 2);
        
        // Quick connectivity check
        bool valid1 = false, valid2 = false;
        for (const auto& e : g[path[i-1]]) {
            if (e.to == path[j]) {
                valid1 = true;
                break;
            }
        }
        if (!valid1) continue;
        
        for (const auto& e : g[path[i]]) {
            if (e.to == path[j+1]) {
                valid2 = true;
                break;
            }
        }
        if (!valid2) continue;
        
        vector<int> new_path = path;
        reverse(new_path.begin() + i, new_path.begin() + j + 1);
        
        ll new_cost = evaluate_path_cost(new_path, g);
        if (new_cost < best_cost) {
            path = new_path;
            best_cost = new_cost;
            improved = true;
        }
    }
    
    return improved;
}

vector<int> remove_cycles(const vector<int>& path) {
    if (path.empty()) return path;
    
    unordered_map<int, int> last_occurrence;
    vector<int> result;
    result.reserve(path.size());
    
    for (int node : path) {
        if (last_occurrence.find(node) != last_occurrence.end()) {
            result.resize(last_occurrence[node] + 1);
        }
        last_occurrence[node] = result.size();
        result.push_back(node);
    }
    
    return result;
}

void try_initial_paths(const vector<vector<Edge>>& g, int N, int A, int B, 
                      vector<int>& best_path, ll& best_cost,
                      const vector<ll>& dist_w1, const vector<ll>& dist_w2,
                      const vector<int>& prime_prefix) {
    // Try standard Dijkstra variants
    auto path1 = dijkstra_path_w1(N, g, A, B);
    if (!path1.empty()) {
        ll cost1 = evaluate_path_cost(path1, g);
        if (cost1 < best_cost) {
            best_cost = cost1;
            best_path = path1;
        }
    }
    
    auto path2 = dijkstra_path_w2(N, g, A, B);
    if (!path2.empty()) {
        ll cost2 = evaluate_path_cost(path2, g);
        if (cost2 < best_cost) {
            best_cost = cost2;
            best_path = path2;
        }
    }
    
    // Try improved A*
    auto path3 = find_path_astar_improved(N, g, A, B, dist_w1, dist_w2, prime_prefix);
    if (!path3.empty()) {
        ll cost3 = evaluate_path_cost(path3, g);
        if (cost3 < best_cost) {
            best_cost = cost3;
            best_path = path3;
        }
    }
}

int main() {
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    srand(time(0));
    
    int N, M, A, B;
    cin >> N >> M;
    cin >> A >> B;
    
    vector<vector<Edge>> graph(N);
    
    for (int i = 0; i < M; ++i) {
        int u, v, w1, w2;
        cin >> u >> v >> w1 >> w2;
        graph[u].push_back({v, w1, w2});
        graph[v].push_back({u, w1, w2});
    }
    
    sieve_primes(N + 1000);
    auto prime_prefix = compute_prime_prefix(N + 1000);
    
    TimeManager tm(TOTAL_LIMIT);
    
    auto dist_w1 = compute_distances(N, graph, B, [](const Edge& e) { return (ll)e.w1; });
    auto dist_w2 = compute_distances(N, graph, B, [](const Edge& e) { return 3LL * e.w2; });
    
    vector<int> best_path;
    ll best_cost = 1LL << 60;
    
    // Phase 1: Try initial path finding algorithms
    try_initial_paths(graph, N, A, B, best_path, best_cost, dist_w1, dist_w2, prime_prefix);
    
    // Phase 2: Beam search with different widths
    vector<int> beam_widths = {30, 60, 100};
    for (int width : beam_widths) {
        if (!tm.should_continue(2.0)) break;
        auto path = beam_search_improved(graph, A, B, dist_w1, dist_w2, prime_prefix, width, tm);
        if (!path.empty()) {
            ll cost = evaluate_path_cost(path, graph);
            if (cost < best_cost) {
                best_cost = cost;
                best_path = path;
            }
        }
    }
    
    // Phase 3: Quick local optimization
    if (!best_path.empty() && tm.should_continue(0.5)) {
        vector<int> current_path = best_path;
        ll current_cost = best_cost;
        
        // Try a few 2-opt moves
        for (int i = 0; i < 3 && tm.should_continue(0.1); ++i) {
            try_fast_2opt(current_path, current_cost, graph, tm);
        }
        
        if (current_cost < best_cost) {
            best_cost = current_cost;
            best_path = current_path;
        }
    }
    
    // Fallback to BFS if no path found
    if (best_path.empty()) {
        best_path = bfs_path(N, graph, A, B);
        if (!best_path.empty()) {
            best_cost = evaluate_path_cost(best_path, graph);
        } else {
            best_path.push_back(A);
        }
    }
    
    best_path = remove_cycles(best_path);
    
    cout << best_path.size() << "\n";
    for (size_t i = 0; i < best_path.size(); ++i) {
        if (i > 0) cout << " ";
        cout << best_path[i];
    }
    cout << "\n";
    
    return 0;
}