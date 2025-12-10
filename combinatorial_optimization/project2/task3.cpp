#include <iostream>
#include <vector>
#include <algorithm>
#include <random>
#include <chrono>
#include <queue>

using namespace std;

// --- OPTIMIZATION FLAGS ---
// Limits and Constants
const int MAXN = 100005;
const double TIME_LIMIT = 19.5; // Zostawiamy 0.5s marginesu

// --- GLOBAL STORAGE (Static allocation for speed) ---
// Graph Data
vector<int> adj[MAXN];
int degree[MAXN];

// Algorithm State
int neighbor_counts[MAXN]; // Ile razy sąsiad był widziany z obecnej ścieżki
bool on_path[MAXN];        // Czy węzeł jest w obecnej ścieżce
int current_path[MAXN];    // Tablica przechowująca ID węzłów w ścieżce
int path_len = 0;

// Best Solution Storage
int best_path[MAXN];
int best_path_len = 0;

// Fast Random Engine
mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());

// --- CORE LOGIC ---

// Funkcja dodająca węzeł do ścieżki i aktualizująca stan sąsiadów
// Inline, aby kompilator wkleił kod w miejsce wywołania (szybkość)
inline void push_node(int u) {
    current_path[path_len++] = u;
    on_path[u] = true;
    for (int v : adj[u]) {
        neighbor_counts[v]++;
    }
}

// Funkcja czyszcząca stan (Smart Reset)
// Czyści tylko to, co zmieniliśmy, unika O(N) przy każdym restarcie
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

// Funkcja próbująca rozszerzyć ścieżkę z danego końca (head)
// Zwraca true, jeśli udało się dodać węzeł
inline bool extend_path(int head) {
    // Static buffer to avoid allocation
    static int candidates[128];
    int cand_count = 0;
    bool forced_move = false;
    int best_forced = -1;

    // Przeglądamy sąsiadów głowy. 
    // Są oni już posortowani rosnąco po stopniu (w main).
    // Sprawdzamy max 20 pierwszych sąsiadów dla wydajności.
    int checks = 0;
    for (int v : adj[head]) {
        if (++checks > 20) break;

        // WARUNEK INDUKOWANEJ ŚCIEŻKI:
        // 1. Węzeł nie jest na ścieżce.
        // 2. Węzeł widzi DOKŁADNIE 1 węzeł ze ścieżki (czyli tylko 'head').
        if (!on_path[v] && neighbor_counts[v] == 1) {
            
            // HEURYSTYKA 1: TUNELOWANIE (Graph Reduction)
            // Jeśli węzeł ma stopień 1 lub 2, bierzemy go BEZWARUNKOWO.
            // To jest "wymuszony ruch", który eliminuje zbędne rozgałęzienia.
            if (degree[v] <= 2) {
                best_forced = v;
                forced_move = true;
                break; 
            }

            candidates[cand_count++] = v;
            // Ograniczenie szerokości wiązki (Beam Width) do 8
            if (cand_count >= 8) break;
        }
    }

    if (forced_move) {
        push_node(best_forced);
        return true;
    }

    if (cand_count == 0) return false;

    // HEURYSTYKA 2: WAŻONE LOSOWANIE (Weighted Random)
    // Preferujemy kandydatów z początku listy (najmniejszy stopień).
    // Index 0 jest najlepszy.
    int pick_idx = 0;
    if (cand_count > 1) {
        // 60% szans na najlepszego kandydata (Greedy), 40% na losowego z topki (Exploration)
        if (rng() % 10 < 6) pick_idx = 0;
        else pick_idx = rng() % cand_count;
    }

    push_node(candidates[pick_idx]);
    return true;
}

int main() {
    // 1. FAST I/O
    ios_base::sync_with_stdio(false);
    cin.tie(NULL);

    int N, M;
    if (!(cin >> N >> M)) return 0;

    // Wczytywanie grafu
    for (int i = 0; i < M; ++i) {
        int u, v;
        cin >> u >> v;
        adj[u].push_back(v);
        adj[v].push_back(u);
        degree[u]++;
        degree[v]++;
    }

    // 2. PRE-PROCESSING: SORTOWANIE LIST SĄSIEDZTWA
    // Kluczowy krok. Dzięki temu pętla 'extend_path' zawsze widzi
    // najpierw wierzchołki o niskim stopniu ("bezpieczne").
    vector<int> nodes_by_degree(N);
    for (int i = 0; i < N; ++i) {
        nodes_by_degree[i] = i;
        sort(adj[i].begin(), adj[i].end(), [&](int a, int b) {
            return degree[a] < degree[b];
        });
    }

    // Sortujemy też listę startową, aby częściej zaczynać od liści/małych stopni
    sort(nodes_by_degree.begin(), nodes_by_degree.end(), [&](int a, int b) {
        return degree[a] < degree[b];
    });

    auto start_time = chrono::steady_clock::now();
    long long iterations = 0;

    // 3. GŁÓWNA PĘTLA (Restart Strategy)
    while (true) {
        // Sprawdzanie czasu co 4096 iteracji (bitwise mask dla szybkości)
        if ((iterations & 4095) == 0) {
            auto now = chrono::steady_clock::now();
            if (chrono::duration<double>(now - start_time).count() > TIME_LIMIT) break;
        }
        iterations++;

        // WYBÓR STARTU
        // Losujemy indeks z rozkładem wykładniczym, aby preferować małe stopnie (początek tablicy)
        // ale dawać szansę też innym.
        int start_idx = 0;
        if (N > 1) {
            // Algorytm wyboru: bierzemy dwa losowe, wybieramy mniejszy indeks (turniej)
            // To przesuwa prawdopodobieństwo w stronę niskich stopni.
            int r1 = rng() % N;
            int r2 = rng() % N;
            int r3 = rng() % N;
            start_idx = min({r1, r2, r3}); // Min z 3 daje silne przesunięcie w stronę niskich stopni
        }
        int start_node = nodes_by_degree[start_idx];

        // Budujemy pierwszą część ścieżki
        push_node(start_node);

        // Rozszerzamy w prawo
        while(extend_path(current_path[path_len - 1]));

        // Rozszerzamy w lewo (Double-Ended Search)
        // Jeśli ścieżka ma > 1 element, odwracamy ją i próbujemy rosnąć z "byłego początku"
        if (path_len > 1) {
            // Szybkie odwrócenie tablicy w miejscu
            for(int i=0; i<path_len/2; ++i) {
                swap(current_path[i], current_path[path_len-1-i]);
            }
            // Teraz head to dawny początek. Próbujemy rosnąć.
            while(extend_path(current_path[path_len - 1]));
        }

        // Aktualizacja wyniku
        if (path_len > best_path_len) {
            best_path_len = path_len;
            // Kopiowanie pamięci (memcpy jest najszybsze)
            memcpy(best_path, current_path, path_len * sizeof(int));
        }

        // Backtracking / Reset
        clear_state();
    }

    // WYPISANIE WYNIKU
    cout << best_path_len << "\n";
    for (int i = 0; i < best_path_len; ++i) {
        cout << best_path[i] << (i == best_path_len - 1 ? "" : " ");
    }
    cout << "\n";

    return 0;
}