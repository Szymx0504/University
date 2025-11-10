#include <bits/stdc++.h>
using namespace std;
using ll = long long;
const double TOTAL_LIMIT = 179.0;

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

ll evaluate_path_cost(const vector<int>& path, const vector<vector<Edge>>& g) {
    if (path.size() <= 1) return 0;
    ll total_cost = 0;
    for (size_t i = 1; i < path.size(); ++i) {
        int u = path[i-1], v = path[i];
        for (const auto& e : g[u]) {
            if (e.to == v) {
                if (is_prime[i]) {
                    total_cost += 3LL * e.w2;
                } else {
                    total_cost += e.w1;
                }
                break;
            }
        }
    }
    return total_cost;
}

vector<int> dijkstra_prime_aware(int N, const vector<vector<Edge>>& g, int src, int tgt) {
    const ll INF = 1LL << 60;
    vector<ll> dist(N, INF);
    vector<int> par(N, -1);
    vector<int> path_len(N, 0);
    priority_queue<pair<ll, int>, vector<pair<ll, int>>, greater<>> pq;
    dist[src] = 0;
    pq.push({0, src});
    
    while (!pq.empty()) {
        auto [d, u] = pq.top(); pq.pop();
        if (d != dist[u]) continue;
        if (u == tgt) break;
        
        for (const auto& e : g[u]) {
            int new_len = path_len[u] + 1;
            ll cost = is_prime[new_len] ? 3LL * e.w2 : e.w1;
            
            if (dist[e.to] > d + cost) {
                dist[e.to] = d + cost;
                par[e.to] = u;
                path_len[e.to] = new_len;
                pq.push({dist[e.to], e.to});
            }
        }
    }
    
    if (par[tgt] == -1 && src != tgt) return {};
    
    vector<int> path;
    for (int cur = tgt; cur != -1; cur = par[cur]) path.push_back(cur);
    reverse(path.begin(), path.end());
    return path;
}

vector<int> beam_search_with_heuristic(const vector<vector<Edge>>& g, int A, int B, const TimeManager& tm) {
    int N = g.size();
    
    vector<ll> heuristic_dist(N, 1LL << 60);
    priority_queue<pair<ll, int>, vector<pair<ll, int>>, greater<>> pq;
    heuristic_dist[B] = 0;
    pq.push({0, B});
    while (!pq.empty()) {
        auto [d, u] = pq.top(); pq.pop();
        if (d != heuristic_dist[u]) continue;
        for (const auto& e : g[u]) {
            if (heuristic_dist[e.to] > d + e.w1) {
                heuristic_dist[e.to] = d + e.w1;
                pq.push({heuristic_dist[e.to], e.to});
            }
        }
    }
    
    struct State {
        vector<int> path;
        ll cost;
        ll heuristic_score;
        bool operator<(const State& other) const { return heuristic_score < other.heuristic_score; }
    };
    
    vector<State> beam;
    beam.push_back({{A}, 0, heuristic_dist[A]});
    vector<int> best_path;
    ll best_cost = 1LL << 60;
    
    for (int depth = 0; depth < 800 && !beam.empty() && tm.should_continue(0.02); depth++) {
        vector<State> next_beam;
        unordered_map<int, ll> best_seen;
        
        for (const auto& state : beam) {
            int u = state.path.back();
            if (u == B) {
                if (state.cost < best_cost) {
                    best_cost = state.cost;
                    best_path = state.path;
                }
                continue;
            }
            
            for (const auto& e : g[u]) {
                bool visited = false;
                for (int node : state.path) {
                    if (node == e.to) {
                        visited = true;
                        break;
                    }
                }
                if (visited) continue;
                
                int step = state.path.size();
                ll edge_cost = is_prime[step] ? 3LL * e.w2 : e.w1;
                ll new_cost = state.cost + edge_cost;
                
                if (best_seen.count(e.to) && new_cost >= best_seen[e.to] + 1000) continue;
                if (!best_seen.count(e.to) || new_cost < best_seen[e.to]) {
                    best_seen[e.to] = new_cost;
                }
                
                ll heuristic_score = new_cost + heuristic_dist[e.to];
                
                State new_state;
                new_state.path = state.path;
                new_state.path.push_back(e.to);
                new_state.cost = new_cost;
                new_state.heuristic_score = heuristic_score;
                next_beam.push_back(new_state);
            }
        }
        
        if (next_beam.empty()) break;
        
        if (next_beam.size() > 500) {
            nth_element(next_beam.begin(), next_beam.begin() + 500, next_beam.end());
            next_beam.resize(500);
        }
        beam = move(next_beam);
    }
    
    for (const auto& state : beam) {
        if (state.path.back() == B && state.cost < best_cost) {
            best_cost = state.cost;
            best_path = state.path;
        }
    }
    return best_path;
}

vector<int> monte_carlo_prime_aware(const vector<vector<Edge>>& g, int A, int B, const TimeManager& tm) {
    int N = g.size();
    vector<int> best_path;
    ll best_cost = 1LL << 60;
    
    for (int iter = 0; iter < 15000 && tm.should_continue(0.5); iter++) {
        vector<int> path = {A};
        vector<bool> visited(N, false);
        visited[A] = true;
        int current = A;
        
        while (current != B && path.size() < 3 * N) {
            vector<tuple<int, ll, ll, int>> candidates;
            for (const auto& e : g[current]) {
                if (!visited[e.to]) {
                    int step = path.size();
                    bool next_prime = is_prime[step];
                    int penalty = next_prime ? 3 : 1;
                    candidates.push_back({e.to, e.w1, e.w2, penalty});
                }
            }
            if (candidates.empty()) break;
            
            vector<pair<int, double>> scored_candidates;
            for (const auto& [node, w1, w2, penalty] : candidates) {
                double score;
                if (penalty == 3) {
                    score = w2 * 3.0;
                } else {
                    score = w1 * 0.7 + w2 * 0.3;
                }
                scored_candidates.push_back({node, score});
            }
            
            sort(scored_candidates.begin(), scored_candidates.end(), 
                 [](auto& a, auto& b) { return a.second < b.second; });
            
            int k = min(4, (int)scored_candidates.size());
            int next_idx;
            if (rand() % 100 < 92) {
                next_idx = 0;
            } else {
                next_idx = rand() % k;
            }
            
            int next_node = scored_candidates[next_idx].first;
            path.push_back(next_node);
            visited[next_node] = true;
            current = next_node;
        }
        
        if (current == B) {
            ll cost = evaluate_path_cost(path, g);
            if (cost < best_cost) {
                best_cost = cost;
                best_path = path;
            }
        }
    }
    return best_path;
}

vector<int> bidirectional_prime_search(int N, const vector<vector<Edge>>& g, int A, int B) {
    const ll INF = 1LL << 60;
    vector<ll> dist_fwd(N, INF), dist_bwd(N, INF);
    vector<int> par_fwd(N, -1), par_bwd(N, -1);
    vector<int> len_fwd(N, 0), len_bwd(N, 0);
    priority_queue<pair<ll, int>, vector<pair<ll, int>>, greater<>> pq_fwd, pq_bwd;
    dist_fwd[A] = 0; dist_bwd[B] = 0;
    pq_fwd.push({0, A}); pq_bwd.push({0, B});
    int meet_node = -1;
    ll best_dist = INF;
    
    while ((!pq_fwd.empty() || !pq_bwd.empty()) && meet_node == -1) {
        if (!pq_fwd.empty()) {
            auto [d, u] = pq_fwd.top(); pq_fwd.pop();
            if (d == dist_fwd[u]) {
                for (const auto& e : g[u]) {
                    int new_len = len_fwd[u] + 1;
                    ll cost = is_prime[new_len] ? 3LL * e.w2 : e.w1;
                    if (dist_fwd[e.to] > d + cost) {
                        dist_fwd[e.to] = d + cost;
                        par_fwd[e.to] = u;
                        len_fwd[e.to] = new_len;
                        pq_fwd.push({dist_fwd[e.to], e.to});
                        if (dist_bwd[e.to] < INF) {
                            ll total = dist_fwd[e.to] + dist_bwd[e.to];
                            if (total < best_dist) {
                                best_dist = total;
                                meet_node = e.to;
                            }
                        }
                    }
                }
            }
        }
        if (!pq_bwd.empty()) {
            auto [d, u] = pq_bwd.top(); pq_bwd.pop();
            if (d == dist_bwd[u]) {
                for (const auto& e : g[u]) {
                    int new_len = len_bwd[u] + 1;
                    ll cost = is_prime[new_len] ? 3LL * e.w2 : e.w1;
                    if (dist_bwd[e.to] > d + cost) {
                        dist_bwd[e.to] = d + cost;
                        par_bwd[e.to] = u;
                        len_bwd[e.to] = new_len;
                        pq_bwd.push({dist_bwd[e.to], e.to});
                        if (dist_fwd[e.to] < INF) {
                            ll total = dist_fwd[e.to] + dist_bwd[e.to];
                            if (total < best_dist) {
                                best_dist = total;
                                meet_node = e.to;
                            }
                        }
                    }
                }
            }
        }
    }
    
    if (meet_node == -1) return {};
    
    vector<int> fwd_path, bwd_path;
    for (int cur = meet_node; cur != -1; cur = par_fwd[cur]) fwd_path.push_back(cur);
    reverse(fwd_path.begin(), fwd_path.end());
    for (int cur = par_bwd[meet_node]; cur != -1; cur = par_bwd[cur]) bwd_path.push_back(cur);
    
    vector<int> path = fwd_path;
    path.insert(path.end(), bwd_path.begin(), bwd_path.end());
    return path;
}

vector<int> remove_cycles(const vector<int>& path) {
    if (path.empty()) return path;
    unordered_map<int, int> last_pos;
    vector<int> result;
    for (int i = 0; i < path.size(); i++) {
        int node = path[i];
        if (last_pos.count(node)) {
            result.resize(last_pos[node]);
        }
        last_pos[node] = result.size();
        result.push_back(node);
    }
    return result;
}

bool optimize_with_prime_awareness(vector<int>& path, ll& best_cost, const vector<vector<Edge>>& g, const TimeManager& tm) {
    int n = path.size();
    if (n < 4) return false;
    bool improved = false;
    
    for (int i = 1; i < n - 2 && tm.should_continue(0.001); i++) {
        for (int j = i + 2; j < n - 1 && tm.should_continue(0.001); j++) {
            bool valid1 = false, valid2 = false;
            for (const auto& e : g[path[i-1]]) if (e.to == path[j]) { valid1 = true; break; }
            for (const auto& e : g[path[i]]) if (e.to == path[j+1]) { valid2 = true; break; }
            if (!valid1 || !valid2) continue;
            
            vector<int> new_path = path;
            reverse(new_path.begin() + i, new_path.begin() + j + 1);
            
            bool might_improve = false;
            for (int k = i; k <= j; k++) {
                if (is_prime[k] && !is_prime[new_path.size() - (j - k) - 1]) {
                    might_improve = true;
                    break;
                }
            }
            
            if (might_improve) {
                ll new_cost = evaluate_path_cost(new_path, g);
                if (new_cost < best_cost) {
                    path = new_path;
                    best_cost = new_cost;
                    improved = true;
                }
            }
        }
    }
    
    for (int tries = 0; tries < 1000 && tm.should_continue(0.001); tries++) {
        int i = 1 + rand() % (n - 3);
        int j = i + 1 + rand() % (n - i - 2);
        
        bool valid1 = false, valid2 = false;
        for (const auto& e : g[path[i-1]]) if (e.to == path[j]) { valid1 = true; break; }
        for (const auto& e : g[path[i]]) if (e.to == path[j+1]) { valid2 = true; break; }
        if (!valid1 || !valid2) continue;
        
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
    
    sieve_primes(300000);
    TimeManager tm(TOTAL_LIMIT);
    
    vector<int> best_path;
    ll best_cost = 1LL << 60;
    
    if (tm.should_continue(8.0)) {
        auto path = beam_search_with_heuristic(graph, A, B, tm);
        if (!path.empty()) {
            ll cost = evaluate_path_cost(path, graph);
            if (cost < best_cost) {
                best_cost = cost;
                best_path = path;
            }
        }
    }
    
    if (tm.should_continue(3.0)) {
        auto path = bidirectional_prime_search(N, graph, A, B);
        if (!path.empty()) {
            ll cost = evaluate_path_cost(path, graph);
            if (cost < best_cost) {
                best_cost = cost;
                best_path = path;
            }
        }
    }
    
    if (tm.should_continue(4.0)) {
        auto path = monte_carlo_prime_aware(graph, A, B, tm);
        if (!path.empty()) {
            ll cost = evaluate_path_cost(path, graph);
            if (cost < best_cost) {
                best_cost = cost;
                best_path = path;
            }
        }
    }
    
    auto path4 = dijkstra_prime_aware(N, graph, A, B);
    if (!path4.empty()) {
        ll cost = evaluate_path_cost(path4, graph);
        if (cost < best_cost) {
            best_cost = cost;
            best_path = path4;
        }
    }
    
    if (!best_path.empty() && tm.should_continue(3.0)) {
        vector<int> current = best_path;
        ll current_cost = best_cost;
        for (int i = 0; i < 5 && tm.should_continue(0.2); i++) {
            if (optimize_with_prime_awareness(current, current_cost, graph, tm)) {
                if (current_cost < best_cost) {
                    best_cost = current_cost;
                    best_path = current;
                }
            }
        }
    }
    
    if (best_path.empty()) {
        best_path = dijkstra_prime_aware(N, graph, A, B);
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