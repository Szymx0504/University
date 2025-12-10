#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <cstring>

using namespace std;

// --- KONFIGURACJA ---
const double TIME_LIMIT = 17.5;
const int MAXN = 100005;
// Zwiￃﾄￂﾙkszamy limit operacji, bo na siatkach DFS musi gￃﾅￂﾂￃﾄￂﾙbiej szukaￃﾄￂﾇ
const int OPS_LIMIT_BASE = 150000; 

// --- PAMIￃﾄￂﾘￃﾄￂﾆ STATYCZNA ---
vector<int> adj[MAXN];
int degree[MAXN];

// Stan ￃﾅￂﾛcieￃﾅￂﾼki
int neighbor_counts[MAXN]; 
bool in_path[MAXN];
int path_stack[MAXN];
int path_len = 0;

// Najlepszy wynik
int best_path[MAXN];
int best_len = 0;

// Tabu
int visited_token[MAXN];
int current_token = 1;
int banned_node = -1;

chrono::steady_clock::time_point start_total;

// --- SZYBKI RNG ---
struct XorShift {
    uint32_t state;
    XorShift(uint32_t seed = 123456789) : state(seed) { if(state==0) state=1; }
    inline uint32_t next() {
        uint32_t x = state; x ^= x << 13; x ^= x >> 17; x ^= x << 5; return state = x;
    }
    inline int next_int(int n) { return next() % n; }
} rng;

// --- POMOCNICZE ---

inline bool check_timeout() {
    static int cnt = 0;
    if ((++cnt & 255) == 0) {
        auto now = chrono::steady_clock::now();
        if (chrono::duration<double>(now - start_total).count() > TIME_LIMIT) return true;
    }
    return false;
}

inline void push_node(int u) {
    in_path[u] = true;
    path_stack[path_len++] = u;
    for (int v : adj[u]) neighbor_counts[v]++;
}

inline void pop_node() {
    if (path_len == 0) return;
    int u = path_stack[--path_len];
    in_path[u] = false;
    for (int v : adj[u]) neighbor_counts[v]--;
}

inline void clear_path() {
    while (path_len > 0) pop_node();
}

inline void update_best() {
    if (path_len > best_len) {
        best_len = path_len;
        memcpy(best_path, path_stack, path_len * sizeof(int));
    }
}

// --- LOGIKA SZCZEPIENIA (GRAFTING) ---
void graft_path_with_tabu(bool reverse_source, int keep_percent) {
    clear_path();
    banned_node = -1; 
    
    if (best_len == 0) return;
    
    int keep_len = max(1, (best_len * keep_percent) / 100);
    if (keep_len >= best_len) keep_len = best_len - 1; 
    if (keep_len < 1) keep_len = 1;

    for (int i = 0; i < keep_len; ++i) {
        int idx = reverse_source ? (best_len - 1 - i) : i;
        int u = best_path[idx];
        
        if (in_path[u]) break;
        if (i > 0 && neighbor_counts[u] != 1) break;
        push_node(u);
    }
    
    int next_idx = reverse_source ? (best_len - 1 - keep_len) : keep_len;
    if (next_idx >= 0 && next_idx < best_len) {
        banned_node = best_path[next_idx];
    }
}

// --- WARNSDORFF HEURISTIC (LOOK-AHEAD) ---
// Oblicza "jakoￃﾅￂﾛￃﾄￂﾇ" kandydata v.
// Im MNIEJ valid sￃﾄￂﾅsiadￃﾃￂﾳw ma v, tym lepiej (wchodzimy w ciasne miejsca).
inline int evaluate_candidate(int v) {
    int valid_neighbors = 0;
    for (int nxt : adj[v]) {
        // Sprawdzamy, czy nxt byￃﾅￂﾂby legalnym ruchem z v
        // Warunek: nxt nie w ￃﾅￂﾛcieￃﾅￂﾼce, i nxt ma obecnie count=0 (po wejￃﾅￂﾛciu do v bￃﾄￂﾙdzie miaￃﾅￂﾂ 1)
        if (!in_path[nxt] && neighbor_counts[nxt] == 0 && visited_token[nxt] != current_token) {
            valid_neighbors++;
        }
    }
    return valid_neighbors;
}

// --- MAIN SOLVER ---
void run_solver(int N) {
    static int starts[MAXN];
    for(int i=0; i<N; ++i) starts[i] = i;
    static int candidates[200];
    static int scores[200]; // Wyniki heurystyki Warnsdorffa

    int stagnation_counter = 0;
    int last_best_len = 0;

    while(true) {
        if(check_timeout()) break;

        // 1. SETUP & GRAFTING
        bool grafted = false;
        
        if (best_len > last_best_len) {
            last_best_len = best_len;
            stagnation_counter = 0;
        } else {
            stagnation_counter++;
        }
        
        // Grafting logic (agresywny restart przy stagnacji)
        if (best_len > 10 && (rng.next() % 100 < 80)) { 
            bool reverse_mode = (rng.next() & 1);
            int keep_pct;
            
            if (stagnation_counter > 15) {
                keep_pct = 1 + rng.next_int(10); // Atomic cut (Hard Reset)
                stagnation_counter = 0; 
            } else if (stagnation_counter > 5) {
                keep_pct = 30 + rng.next_int(30);
            } else {
                keep_pct = 70 + rng.next_int(25);
            }
            
            graft_path_with_tabu(reverse_mode, keep_pct);
            if (path_len > 0) grafted = true;
        }

        if (!grafted) {
            clear_path();
            current_token++; 
            banned_node = -1;
            // Losowy start
            push_node(starts[rng.next_int(N)]);
        } else {
            if (rng.next() % 5 == 0) current_token++;
        }

        // 2. DFS LOOP
        long long ops = 0;
        
        while(ops < OPS_LIMIT_BASE) {
            if(check_timeout()) return;
            ops++;

            int head = path_stack[path_len-1];
            int cand_cnt = 0;

            // Zbieranie kandydatￃﾃￂﾳw
            // Limit skanowania nie jest potrzebny, bo max degree to ~15
            for(int v : adj[head]) {
                if(!in_path[v] && neighbor_counts[v] == 1 
                   && visited_token[v] != current_token
                   && v != banned_node) {
                    candidates[cand_cnt] = v;
                    cand_cnt++;
                    // Max degree jest maￃﾅￂﾂe, wiￃﾄￂﾙc nie musimy breakowaￃﾄￂﾇ wczeￃﾅￂﾛnie
                }
            }

            if(cand_cnt > 0) {
                if (banned_node != -1) banned_node = -1; 
                
                int best_pick = 0;
                
                // --- WARNSDORFF RULE ---
                if (cand_cnt > 1) {
                    // Obliczamy "dostￃﾄￂﾙpnoￃﾅￂﾛￃﾄￂﾇ" kaￃﾅￂﾼdego kandydata
                    int min_score = 100000;
                    // Tablica indeksￃﾃￂﾳw z minimalnym wynikiem (dla losowoￃﾅￂﾛci)
                    static int best_indices[200];
                    int best_count = 0;

                    for(int i=0; i<cand_cnt; ++i) {
                        int score = evaluate_candidate(candidates[i]);
                        scores[i] = score;
                        if (score < min_score) {
                            min_score = score;
                            best_count = 0;
                            best_indices[best_count++] = i;
                        } else if (score == min_score) {
                            best_indices[best_count++] = i;
                        }
                    }
                    
                    // Wybieramy jednego z najlepszych (najmniejsza liczba swobody)
                    // "Hugging the wall"
                    
                    // Czasem (10%) zignoruj heurystykￃﾄￂﾙ dla losowoￃﾅￂﾛci
                    if (rng.next() % 10 == 0) {
                        best_pick = rng.next_int(cand_cnt);
                    } else {
                        best_pick = best_indices[rng.next_int(best_count)];
                    }
                }
                
                push_node(candidates[best_pick]);
                
            } else {
                // STUCK
                if(path_len > best_len) {
                    update_best();
                    stagnation_counter = 0;
                    ops -= 5000; 
                }

                visited_token[head] = current_token;
                pop_node();

                if(path_len == 0) break;
                if (grafted && path_len < best_len / 10) break; // Szybki restart jeￃﾅￂﾛli graft siￃﾄￂﾙ nie udaￃﾅￂﾂ
            }
        }
    }
}

int main() {
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int N, M;
    if (!(cin >> N >> M)) return 0;

    for(int i=0; i<M; ++i) {
        int u, v;
        if (!(cin >> u >> v)) return 1;
        adj[u].push_back(v);
        adj[v].push_back(u);
        degree[u]++;
        degree[v]++;
    }

    // Sortowanie nie jest juￃﾅￂﾼ tak krytyczne przy Warnsdorffie, 
    // ale nadal warto sortowaￃﾄￂﾇ rosnￃﾄￂﾅco po stopniu.
    for(int i=0; i<N; ++i) {
        sort(adj[i].begin(), adj[i].end(), [&](int a, int b){
            return degree[a] < degree[b];
        });
    }

    start_total = chrono::steady_clock::now();
    rng.state = start_total.time_since_epoch().count(); 
    if(rng.state == 0) rng.state = 999;
    
    run_solver(N);

    cout << best_len << "\n";
    for(int i=0; i<best_len; ++i) {
        cout << best_path[i] << (i==best_len-1?"":" ");
    }
    cout << "\n";

    return 0;
}