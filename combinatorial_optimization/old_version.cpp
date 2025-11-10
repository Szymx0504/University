#include <bits/stdc++.h>
using namespace std;
using ll = long long;
const double TOTAL_LIMIT = 19.0;

struct Edge { int to; int w1; int w2; };
struct PQ { int v; ll d; bool operator<(PQ const& o) const { return d > o.d; } };

class TimeManager {
    chrono::steady_clock::time_point start;
    double total_budget;
    
public:
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

vector<char> is_prime;
unordered_map<int, bool> prime_cache;

void sieve_primes(int limit) {
    is_prime.assign(limit+1, 1);
    if (limit >= 0) is_prime[0] = 0;
    if (limit >= 1) is_prime[1] = 0;
    for (int p = 2; (ll)p*p <= limit; ++p) if (is_prime[p]) {
        for (int q = p*p; q <= limit; q += p) is_prime[q] = 0;
    }
}

bool is_prime_cached(int idx) {
    if (idx < 0) return false;
    if (auto it = prime_cache.find(idx); it != prime_cache.end()) 
        return it->second;
    bool result = idx < (int)is_prime.size() && is_prime[idx];
    prime_cache[idx] = result;
    return result;
}

vector<int> compute_prime_prefix(int max_len) {
    vector<int> prefix(max_len + 1, 0);
    for (int i = 1; i <= max_len; ++i) {
        prefix[i] = prefix[i-1] + (is_prime_cached(i) ? 1 : 0);
    }
    return prefix;
}

vector<int> bfs_path(int N, const vector<vector<Edge>>& g, int src, int tgt) {
    vector<int> par(N, -1); 
    vector<bool> vis(N, false); 
    queue<int> q;
    q.push(src); 
    vis[src] = true;
    while(!q.empty()){
        int u = q.front(); q.pop();
        if(u == tgt) break;
        for(const auto &e: g[u]) if(!vis[e.to]){ 
            vis[e.to] = true; 
            par[e.to] = u; 
            q.push(e.to); 
        }
    }
    if(!vis[tgt]) return {};
    vector<int> p; 
    for(int cur = tgt; cur != -1; cur = par[cur]) p.push_back(cur);
    reverse(p.begin(), p.end()); 
    return p;
}

vector<int> dijkstra_path(int N, const vector<vector<Edge>>& g, int src, int tgt, function<ll(const Edge&)> cost) {
    const ll INF = (1LL<<60);
    vector<ll> dist(N, INF);
    vector<int> par(N, -1);
    struct Item{ int v; ll d; };
    struct Cmp{ bool operator()(Item const&a, Item const&b) const { return a.d > b.d; } };
    priority_queue<Item, vector<Item>, Cmp> pq;
    dist[src] = 0; 
    pq.push({src, 0});
    while(!pq.empty()){
        auto cur = pq.top(); pq.pop();
        if(cur.d != dist[cur.v]) continue;
        if(cur.v == tgt) break;
        for(const auto &e : g[cur.v]){
            ll w = cost(e);
            if(dist[e.to] > cur.d + w){
                dist[e.to] = cur.d + w;
                par[e.to] = cur.v;
                pq.push({e.to, dist[e.to]});
            }
        }
    }
    if(par[tgt] == -1 && src != tgt) return {};
    vector<int> path; 
    for(int cur = tgt; cur != -1; cur = par[cur]) path.push_back(cur);
    reverse(path.begin(), path.end()); 
    return path;
}

vector<ll> dijkstra_dist(int N, const vector<vector<Edge>>& g, int src, function<ll(const Edge&)> cost) {
    const ll INF = (1LL<<60);
    vector<ll> dist(N, INF);
    struct Item{ int v; ll d; };
    struct Cmp{ bool operator()(Item const&a, Item const&b) const { return a.d > b.d; } };
    priority_queue<Item, vector<Item>, Cmp> pq;
    dist[src] = 0; 
    pq.push({src, 0});
    while(!pq.empty()){
        auto cur = pq.top(); pq.pop();
        if(cur.d != dist[cur.v]) continue;
        for(const auto &e: g[cur.v]){
            ll w = cost(e);
            if(dist[e.to] > cur.d + w){
                dist[e.to] = cur.d + w;
                pq.push({e.to, dist[e.to]});
            }
        }
    }
    return dist;
}

ll score_of_path(const vector<int>& path, const vector<vector<Edge>>& g) {
    if(path.size() <= 1) return 0;
    ll A = 0, B = 0;
    for(size_t i = 1; i < path.size(); ++i){
        int u = path[i-1], v = path[i];
        bool found = false;
        for(const auto &e: g[u]) if(e.to == v){
            int idx = (int)i;
            if(is_prime_cached(idx)) B += 3LL * e.w2;
            else A += e.w1;
            found = true;
            break;
        }
        if(!found){
            for(const auto &e: g[v]) if(e.to == u){
                int idx = (int)i;
                if(is_prime_cached(idx)) B += 3LL * e.w2;
                else A += e.w1;
                found = true;
                break;
            }
            if(!found) return LLONG_MAX/4;
        }
    }
    return A + B;
}

inline int prefix_primes(const vector<int>& pref, int l, int r){ 
    if(r < l) return 0; 
    return pref[r] - (l > 0 ? pref[l-1] : 0); 
}

struct BeamState {
    vector<int> path;
    vector<bool> used;
    ll partial;
    double est;
    
    BeamState(int N) : used(N, false), partial(0), est(0.0) {}
};

struct Candidate {
    double est;
    vector<int> path;
    vector<bool> used;
    ll partial;
};

void run_beam_improved(const vector<vector<Edge>>& g, int A, int B, const vector<ll>& dh1, const vector<ll>& dh2,
                      const vector<int>& dist_to_B, const vector<int>& pref_primes, double avg_extra,
                      int beam_width, double budget, bool use_mix, double alpha,
                      vector<int>& best_path, ll& best_score, mt19937 &rng, const TimeManager& tm) {
    int N = (int)g.size();
    vector<BeamState> beam;
    beam.reserve(beam_width + 10);
    
    BeamState start(N);
    start.path.push_back(A);
    start.used[A] = true;
    start.partial = 0;
    start.est = min((double)dh1[A], (double)dh2[A]);
    beam.push_back(move(start));

    vector<Candidate> candidates;
    candidates.reserve(beam_width * 25);

    while(tm.should_continue(0.008) && !beam.empty()) {
        candidates.clear();

        for(auto &st : beam){
            int u = st.path.back();
            int pathlen = (int)st.path.size();
            
            for(const auto &e : g[u]){
                int v = e.to;
                if(st.used[v]) continue;
                
                int edgeIndex = pathlen;
                bool prime = is_prime_cached(edgeIndex);
                ll add = prime ? 3LL * e.w2 : (ll)e.w1;
                ll np = st.partial + add;
                
                if(np >= best_score * 1.08) continue;
                
                double h1 = (dh1[v] >= (1LL<<60)) ? 1e18 : (double)dh1[v];
                double h2 = (dh2[v] >= (1LL<<60)) ? 1e18 : (double)dh2[v];
                double heur;
                if(use_mix) 
                    heur = (1.0 - alpha) * h1 + alpha * h2;
                else 
                    heur = min(h1, h2);
                    
                int r = dist_to_B[v];
                if(r >= (int)1e8) continue;
                
                int expect_pr = prefix_primes(pref_primes, edgeIndex + 1, edgeIndex + r);
                heur += expect_pr * avg_extra * 0.65;
                
                double random_tiebreak = (rng() % 1000) / 600000.0;
                double est = (double)np + heur + random_tiebreak;
                
                Candidate c;
                c.est = est;
                c.path = st.path;
                c.path.push_back(v);
                c.used = st.used;
                c.used[v] = true;
                c.partial = np;
                candidates.push_back(move(c));
            }
        }

        if(candidates.empty()) break;
        
        size_t keep = min((size_t)beam_width, candidates.size());
        if(candidates.size() > keep) {
            nth_element(candidates.begin(), candidates.begin() + keep, candidates.end(),
                       [](const Candidate &a, const Candidate &b){ return a.est < b.est; });
            sort(candidates.begin(), candidates.begin() + keep,
                 [](const Candidate &a, const Candidate &b){ return a.est < b.est; });
        }

        vector<BeamState> next;
        next.reserve(keep);
        
        for(size_t i = 0; i < keep; i++){
            const auto &c = candidates[i];
            BeamState ns(N);
            ns.path = c.path;
            ns.used = c.used;
            ns.partial = c.partial;
            ns.est = c.est;
            
            if(!ns.path.empty() && ns.path.back() == B){
                if(ns.partial < best_score){
                    best_score = ns.partial;
                    best_path = ns.path;
                }
                continue;
            }
            next.push_back(move(ns));
        }
        beam.swap(next);
    }

    for(auto &st : beam){
        if(!st.path.empty() && st.path.back() == B){ 
            if(st.partial < best_score){
                best_score = st.partial;
                best_path = st.path;
            }
            continue; 
        }
        
        vector<int> path = st.path;
        vector<bool> used = st.used;
        int cur = path.back();
        int steps = 0;
        bool fail = false;
        
        while(cur != B && steps < 5000 && tm.should_continue(0.001)){
            ++steps;
            int bestv = -1;
            double bestvscore = 1e300;
            int edgeIndex = (int)path.size();
            
            for(const auto &e: g[cur]){
                int v = e.to; 
                if(used[v]) continue;
                bool prime = is_prime_cached(edgeIndex);
                ll add = prime ? 3LL * e.w2 : (ll)e.w1;
                double heur = min((double)dh1[v], (double)dh2[v]) * 0.6;
                double val = (double)add + heur;
                if(val < bestvscore){ 
                    bestvscore = val; 
                    bestv = v; 
                }
            }
            
            if(bestv == -1){ 
                fail = true; 
                break; 
            }
            
            used[bestv] = true;
            path.push_back(bestv);
            cur = bestv;
        }
        
        if(!fail && path.back() == B){
            ll sc = score_of_path(path, g);
            if(sc < best_score){
                best_score = sc;
                best_path = path;
            }
        }
    }
}

vector<int> remove_cycles_fast(const vector<int>& path) {
    if(path.empty()) return path;
    
    int max_node = 0;
    for(int node : path) max_node = max(max_node, node);
    
    vector<int> last_seen(max_node + 1, -1);
    vector<int> result;
    result.reserve(path.size());
    
    for(int i = 0; i < path.size(); ++i) {
        int node = path[i];
        if(last_seen[node] != -1) {
            result.resize(last_seen[node] + 1);
        }
        last_seen[node] = result.size();
        result.push_back(node);
    }
    
    return result;
}

bool advanced_local_improve(vector<vector<Edge>>& g, vector<int>& path, ll& score, const TimeManager& tm, mt19937 &rng){
    int L = (int)path.size();
    if(L <= 2) return false;
    
    bool improved = false;
    const int MAX_ITERS = 600;
    
    for(int iter = 0; iter < MAX_ITERS && tm.should_continue(0.006); ++iter){
        bool local_improved = false;
        
        if(L >= 4){
            for(int attempt = 0; attempt < 4 && !local_improved; ++attempt){
                int i = uniform_int_distribution<int>(0, L-3)(rng);
                int j = uniform_int_distribution<int>(i+2, min(L-1, i + min(20, L-1-i)))(rng);
                int s = path[i], t = path[j];
                
                const ll INF = (1LL<<60);
                int N = (int)g.size();
                vector<ll> dist(N, INF);
                vector<int> par(N, -1);
                struct Item{ int v; ll d; };
                struct Cmp{ bool operator()(Item const&a, Item const&b) const { return a.d > b.d; } };
                priority_queue<Item, vector<Item>, Cmp> pq;
                
                dist[s] = 0;
                pq.push({s,0});
                bool found = false;
                
                while(!pq.empty() && tm.should_continue(0.0003)){
                    auto cur = pq.top(); pq.pop();
                    if(cur.d != dist[cur.v]) continue;
                    if(cur.v == t){ found = true; break; }
                    for(const auto &e: g[cur.v]){
                        ll w = (ll)e.w1;
                        if(dist[e.to] > cur.d + w){
                            dist[e.to] = cur.d + w;
                            par[e.to] = cur.v;
                            pq.push({e.to, dist[e.to]});
                        }
                    }
                }
                
                if(found){
                    vector<int> rep;
                    for(int cur = t; cur != -1; cur = par[cur]) rep.push_back(cur);
                    reverse(rep.begin(), rep.end());
                    
                    if(rep.size() > 1){
                        vector<int> new_path;
                        new_path.reserve(i + rep.size() + (L - j - 1));
                        for(int k = 0; k <= i; ++k) new_path.push_back(path[k]);
                        for(size_t k = 1; k < rep.size(); ++k) new_path.push_back(rep[k]);
                        for(int k = j + 1; k < L; ++k) new_path.push_back(path[k]);
                        
                        unordered_set<int> seen;
                        bool ok = true;
                        for(int nd : new_path){
                            if(seen.count(nd)){ ok = false; break; }
                            seen.insert(nd);
                        }
                        
                        if(ok){
                            bool edges_ok = true;
                            for(size_t k = 1; k < new_path.size(); ++k){
                                int u = new_path[k-1], v = new_path[k];
                                bool edge_found = false;
                                for(const auto &e: g[u]) if(e.to == v){ edge_found = true; break; }
                                if(!edge_found){
                                    for(const auto &e: g[v]) if(e.to == u){ edge_found = true; break; }
                                    if(!edge_found){
                                        edges_ok = false;
                                        break;
                                    }
                                }
                            }
                            if(edges_ok){
                                ll new_score = score_of_path(new_path, g);
                                if(new_score < score * 0.995){
                                    path = move(new_path);
                                    score = new_score;
                                    improved = true;
                                    local_improved = true;
                                    L = path.size();
                                    break;
                                }
                            }
                        }
                    }
                }
            }
        }
        
        if(!local_improved && L >= 5){
            for(int attempt = 0; attempt < 6 && !local_improved; ++attempt){
                int a = uniform_int_distribution<int>(1, L-3)(rng);
                int b = uniform_int_distribution<int>(a+1, min(L-2, a+10))(rng);
                vector<int> test = path;
                reverse(test.begin() + a, test.begin() + b + 1);
                
                bool edges_ok = true;
                for(int k = max(1, a); k <= min(b, L-2); k++){
                    int u = test[k-1], v = test[k];
                    bool edge_found = false;
                    for(const auto &e: g[u]) if(e.to == v){ edge_found = true; break; }
                    if(!edge_found){
                        for(const auto &e: g[v]) if(e.to == u){ edge_found = true; break; }
                        if(!edge_found){
                            edges_ok = false;
                            break;
                        }
                    }
                }
                
                if(edges_ok){
                    ll sc2 = score_of_path(test, g);
                    if(sc2 < score * 0.995){
                        path = move(test);
                        score = sc2;
                        improved = true;
                        local_improved = true;
                        L = path.size();
                        break;
                    }
                }
            }
        }
        
        if(!local_improved && L >= 4 && (iter % 3 == 0)){
            for(int attempt = 0; attempt < 3 && !local_improved; ++attempt){
                int pos = uniform_int_distribution<int>(1, L-2)(rng);
                int u = path[pos-1], v = path[pos+1];
                bool found = false;
                for(const auto &e: g[u]){
                    if(e.to == v){
                        vector<int> test = path;
                        test.erase(test.begin() + pos);
                        ll sc2 = score_of_path(test, g);
                        if(sc2 < score * 0.995){
                            path = move(test);
                            score = sc2;
                            improved = true;
                            local_improved = true;
                            L = path.size();
                        }
                        found = true;
                        break;
                    }
                }
                if(found) break;
            }
        }
        
        if(!local_improved && iter > 80) break;
    }
    
    return improved;
}

void try_dijkstra_variants(const vector<vector<Edge>>& g, int N, int A, int B, vector<int>& best_path, ll& best_score) {
    auto try_and_update = [&](function<ll(const Edge&)> cost){
        auto p = dijkstra_path(N, g, A, B, cost);
        if(!p.empty()){
            ll sc = score_of_path(p, g);
            if(sc < best_score){
                best_score = sc;
                best_path = p;
            }
        }
    };

    try_and_update([&](const Edge& e)->ll{ return (ll)e.w1; });
    try_and_update([&](const Edge& e)->ll{ return 3LL*(ll)e.w2; });
    try_and_update([&](const Edge& e)->ll{ return (ll)e.w1 + 3LL*(ll)e.w2; });
    try_and_update([&](const Edge& e)->ll{ return (ll)e.w1 + (ll)e.w2; });
    try_and_update([&](const Edge& e)->ll{ return (ll)e.w1 + 2LL*(ll)e.w2; });
    try_and_update([&](const Edge& e)->ll{ return 2LL*(ll)e.w1 + (ll)e.w2; });
    
    for(int i = 1; i < 10; i += 2){
        double ratio = i * 0.1;
        try_and_update([ratio](const Edge& e)->ll{ 
            return (ll)((1.0 - ratio) * e.w1 + ratio * 3 * e.w2);
        });
    }
}

void multi_start_local_search(vector<vector<Edge>>& g, int A, int B, vector<int>& best_path, ll& best_score, const TimeManager& tm, mt19937 &rng) {
    int N = (int)g.size();
    
    for(int i = 0; i < 6 && tm.should_continue(0.12); ++i){
        vector<int> path;
        if(i == 0) {
            path = dijkstra_path(N, g, A, B, [](const Edge& e)->ll{ return (ll)e.w1; });
        } else if(i == 1) {
            path = dijkstra_path(N, g, A, B, [](const Edge& e)->ll{ return 3LL*(ll)e.w2; });
        } else if(i == 2) {
            path = dijkstra_path(N, g, A, B, [](const Edge& e)->ll{ return (ll)e.w1 + (ll)e.w2; });
        } else {
            double alpha = (i-3) * 0.25;
            path = dijkstra_path(N, g, A, B, [alpha](const Edge& e)->ll{ 
                return (1.0 - alpha) * (ll)e.w1 + alpha * 3LL * (ll)e.w2; 
            });
        }
        
        if(path.empty()) continue;
        
        ll current_score = score_of_path(path, g);
        vector<int> current_path = path;
        
        TimeManager local_tm(min(tm.remaining() * 0.15, 2.0));
        if(advanced_local_improve(g, current_path, current_score, local_tm, rng)) {
            if(current_score < best_score){
                best_score = current_score;
                best_path = current_path;
            }
        }
    }
}

void iterative_refinement(vector<vector<Edge>>& g, vector<int>& best_path, ll& best_score, const TimeManager& tm, mt19937 &rng) {
    if(best_path.empty() || !tm.should_continue(0.15)) return;
    
    vector<int> current_best = best_path;
    ll current_score = best_score;
    
    for(int round = 0; round < 4 && tm.should_continue(0.03); round++){
        TimeManager round_tm(tm.remaining() * 0.25);
        if(advanced_local_improve(g, current_best, current_score, round_tm, rng)){
            if(current_score < best_score){
                best_score = current_score;
                best_path = current_best;
            }
        }
        
        if(current_best.size() > 8 && tm.should_continue(0.02)){
            vector<int> perturbed = current_best;
            int changes = 0;
            const int MAX_CHANGES = min(2, (int)perturbed.size() / 15);
            
            for(int c = 0; c < MAX_CHANGES; c++){
                int i = uniform_int_distribution<int>(1, perturbed.size()-2)(rng);
                int j = uniform_int_distribution<int>(1, perturbed.size()-2)(rng);
                if(i != j) {
                    bool valid_swap = true;
                    if(i > 0) {
                        int u = perturbed[i-1], v = perturbed[j];
                        bool found = false;
                        for(const auto &e: g[u]) if(e.to == v){ found = true; break; }
                        if(!found) for(const auto &e: g[v]) if(e.to == u){ found = true; break; }
                        if(!found) valid_swap = false;
                    }
                    if(valid_swap && i < perturbed.size()-1) {
                        int u = perturbed[j], v = perturbed[i+1];
                        bool found = false;
                        for(const auto &e: g[u]) if(e.to == v){ found = true; break; }
                        if(!found) for(const auto &e: g[v]) if(e.to == u){ found = true; break; }
                        if(!found) valid_swap = false;
                    }
                    if(valid_swap && j > 0) {
                        int u = perturbed[j-1], v = perturbed[i];
                        bool found = false;
                        for(const auto &e: g[u]) if(e.to == v){ found = true; break; }
                        if(!found) for(const auto &e: g[v]) if(e.to == u){ found = true; break; }
                        if(!found) valid_swap = false;
                    }
                    if(valid_swap && j < perturbed.size()-1) {
                        int u = perturbed[i], v = perturbed[j+1];
                        bool found = false;
                        for(const auto &e: g[u]) if(e.to == v){ found = true; break; }
                        if(!found) for(const auto &e: g[v]) if(e.to == u){ found = true; break; }
                        if(!found) valid_swap = false;
                    }
                    
                    if(valid_swap) {
                        swap(perturbed[i], perturbed[j]);
                        changes++;
                    }
                }
            }
            
            if(changes > 0){
                ll perturbed_score = score_of_path(perturbed, g);
                TimeManager perturb_tm(tm.remaining() * 0.2);
                if(advanced_local_improve(g, perturbed, perturbed_score, perturb_tm, rng)){
                    if(perturbed_score < best_score){
                        best_score = perturbed_score;
                        best_path = perturbed;
                        current_best = perturbed;
                        current_score = perturbed_score;
                    }
                }
            }
        }
    }
}

int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);
    
    auto prog_start = chrono::steady_clock::now();
    
    int N, M, A, B;
    if(!(cin >> N >> M)) return 0;
    cin >> A >> B;
    
    vector<vector<Edge>> g(N);
    ll tot_extra = 0;
    ll cnt_extra = 0;
    
    for(int i = 0; i < M; ++i){
        int u, v, w1, w2;
        cin >> u >> v >> w1 >> w2;
        if(u < 0 || u >= N || v < 0 || v >= N) continue;
        g[u].push_back({v, w1, w2});
        g[v].push_back({u, w1, w2});
        tot_extra += max(0LL, 3LL * (ll)w2 - (ll)w1);
        ++cnt_extra;
    }

    sieve_primes(max(2, N + 200));
    auto pref_primes = compute_prime_prefix(N + 1500);
    
    double avg_extra = cnt_extra > 0 ? (double)tot_extra / (double)cnt_extra : 1.0;

    vector<int> best_path;
    ll best_score = LLONG_MAX / 4;

    try_dijkstra_variants(g, N, A, B, best_path, best_score);
    
    if(best_path.empty()){
        auto p = bfs_path(N, g, A, B);
        if(!p.empty()){
            best_score = score_of_path(p, g);
            best_path = p;
        }
    }

    auto now = chrono::steady_clock::now();
    double used = chrono::duration<double>(now - prog_start).count();
    double remaining = max(0.0, TOTAL_LIMIT - used);
    TimeManager tm(remaining);

    vector<ll> dh_w1 = dijkstra_dist(N, g, B, [](const Edge& e) -> ll { return (ll)e.w1; });
    vector<ll> dh_w3w2 = dijkstra_dist(N, g, B, [](const Edge& e) -> ll { return 3LL * (ll)e.w2; });
    
    vector<int> dist_to_B(N, 1e9);
    {
        queue<int> q;
        dist_to_B[B] = 0;
        q.push(B);
        while(!q.empty()){
            int u = q.front(); q.pop();
            for(const auto &e: g[u]){
                if(dist_to_B[e.to] > dist_to_B[u] + 1){
                    dist_to_B[e.to] = dist_to_B[u] + 1;
                    q.push(e.to);
                }
            }
        }
    }

    mt19937 rng((unsigned)chrono::high_resolution_clock::now().time_since_epoch().count());

    if(tm.remaining() > 0.25){
        multi_start_local_search(g, A, B, best_path, best_score, tm, rng);
    }

    if(tm.remaining() > 0.2){
        double beam_total = min(tm.remaining() * 0.5, 7.0);
        vector<tuple<bool, double, int>> beam_configs = {
            {false, 0.0, 220},
            {true, 0.15, 180},
            {true, 0.35, 180},
            {true, 0.55, 180},
            {true, 0.75, 180},
            {true, 0.95, 180}
        };
        
        for(const auto& [use_mix, alpha, width] : beam_configs){
            if(!tm.should_continue(0.08)) break;
            double allot = min(beam_total / beam_configs.size(), tm.remaining() * 0.9);
            run_beam_improved(g, A, B, dh_w1, dh_w3w2, dist_to_B, pref_primes, avg_extra,
                            width, allot, use_mix, alpha, best_path, best_score, rng, tm);
        }
    }

    if(!best_path.empty() && tm.should_continue(0.1)){
        iterative_refinement(g, best_path, best_score, tm, rng);
    }

    if(best_path.empty()){
        auto p = bfs_path(N, g, A, B);
        if(!p.empty()) best_path = p;
        else best_path = {A};
    }

    best_path = remove_cycles_fast(best_path);

    bool valid = true;
    for(size_t i = 1; i < best_path.size(); ++i){
        int u = best_path[i-1], v = best_path[i];
        bool edge_found = false;
        for(const auto &e: g[u]) if(e.to == v){ edge_found = true; break; }
        if(!edge_found){
            for(const auto &e: g[v]) if(e.to == u){ edge_found = true; break; }
            if(!edge_found){
                valid = false;
                break;
            }
        }
    }
    
    if(!valid){
        best_path = bfs_path(N, g, A, B);
        if(best_path.empty()) best_path = {A};
    }

    cout << best_path.size() << "\n";
    for(size_t i = 0; i < best_path.size(); ++i){
        if(i) cout << " ";
        cout << best_path[i];
    }
    cout << "\n";
    
    return 0;
}

// 92.08