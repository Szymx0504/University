#include <bits/stdc++.h>
using namespace std;
using ll = long long;
const ll INF = (1LL<<60);
const double TOTAL_LIMIT = 19.0;

struct Edge{ int to,w1,w2; };

struct TimeManager{
    chrono::steady_clock::time_point t0;
    double total;
    TimeManager(double t):t0(chrono::steady_clock::now()),total(t){}
    inline bool ok(double s=0.02) const {
        double el = chrono::duration<double>(chrono::steady_clock::now() - t0).count();
        return el < total - s;
    }
};

vector<char> isPrime;
void sieve(int n){
    isPrime.assign(n+1,1);
    isPrime[0]=isPrime[1]=0;
    for(int i=2;i*i<=n;i++)
        if(isPrime[i])
            for(int j=i*i;j<=n;j+=i)
                isPrime[j]=0;
}

ll evalPath(const vector<int>&p,const vector<vector<Edge>>&g){
    ll sc=0;
    for(int i=1;i<(int)p.size();i++){
        int u=p[i-1], v=p[i];
        for(auto&e:g[u])
            if(e.to==v){
                sc += isPrime[i] ? 3LL*e.w2 : e.w1;
                break;
            }
    }
    return sc;
}

template<typename F>
vector<ll> dij(int N,const vector<vector<Edge>>&g,int src,F cost){
    vector<ll>d(N,INF);
    priority_queue<pair<ll,int>,vector<pair<ll,int>>,greater<>>pq;
    d[src]=0; pq.push({0,src});
    while(!pq.empty()){
        auto [cd,u]=pq.top(); pq.pop();
        if(cd!=d[u]) continue;
        for(auto&e:g[u]){
            ll w = cost(e);
            if(d[e.to] > cd+w){
                d[e.to]=cd+w;
                pq.push({d[e.to], e.to});
            }
        }
    }
    return d;
}

vector<int> dijkstraPrime(int N,const vector<vector<Edge>>&g,int A,int B){
    vector<ll>d(N,INF);
    vector<int>par(N,-1), len(N,0);
    priority_queue<pair<ll,int>,vector<pair<ll,int>>,greater<>>pq;
    d[A]=0; pq.push({0,A});

    while(!pq.empty()){
        auto [cd,u]=pq.top(); pq.pop();
        if(cd!=d[u]) continue;
        if(u==B) break;
        for(auto &e:g[u]){
            int nl = len[u]+1;
            ll w = isPrime[nl] ? 3LL*e.w2 : e.w1;
            if(d[e.to] > cd+w){
                d[e.to]=cd+w;
                par[e.to]=u;
                len[e.to]=nl;
                pq.push({d[e.to], e.to});
            }
        }
    }
    if(par[B]==-1 && A!=B) return {};
    vector<int>p;
    for(int cur=B;cur!=-1;cur=par[cur]) p.push_back(cur);
    reverse(p.begin(),p.end());
    return p;
}

vector<int> beamHQ(const vector<vector<Edge>>&g,
                   int A,int B,
                   const vector<ll>&h1,
                   const vector<ll>&h3,
                   const TimeManager&tm)
{
    int N = g.size();

    struct State{
        vector<int> path;
        vector<char> used;
        ll cost;
        double est;
    };

    State s;
    s.path={A};
    s.used.assign(N,0);
    s.used[A]=1;
    s.cost=0;
    s.est=min(h1[A],h3[A]);
    vector<State> beam={s};

    vector<int> bestP;
    ll bestSc = INF;

    const int BW0 = 250;
    const int BWmax=600;
    const int MAX_DEPTH = 1200;

    for(int depth=0; depth<MAX_DEPTH && tm.ok(); depth++){
        vector<State> cand;
        cand.reserve(beam.size()*6);
        unordered_map<int,ll> bestSeen;

        int BW = min(BWmax, BW0 + depth/40);

        for(auto &st:beam){
            int u = st.path.back();
            int pos = st.path.size();

            if(u==B){
                ll sc = st.cost;
                if(sc < bestSc){
                    bestSc=sc;
                    bestP = st.path;
                }
                continue;
            }

            for(auto&e:g[u]){
                if(st.used[e.to]) continue;

                bool pr = (pos < (int)isPrime.size() && isPrime[pos]);
                ll add = pr ? 3LL*e.w2 : e.w1;
                ll nc = st.cost + add;
                if(nc >= bestSc) continue;

                if(bestSeen.count(e.to) && nc >= bestSeen[e.to]) continue;
                bestSeen[e.to] = nc;

                double heur = min((double)h1[e.to], (double)h3[e.to]);
                double est = nc + 0.9999 * heur;

                State nx;
                nx.path=st.path;
                nx.path.push_back(e.to);
                nx.used = st.used;
                nx.used[e.to]=1;
                nx.cost = nc;
                nx.est = est;

                cand.push_back(move(nx));
            }
        }

        if(cand.empty()) break;

        if((int)cand.size() > BW){
            nth_element(cand.begin(), cand.begin()+BW, cand.end(),
                [](const State&a,const State&b){
                    return a.est < b.est;
                });
            cand.resize(BW);
        }

        sort(cand.begin(), cand.end(),
             [](const State&a,const State&b){
                 return a.est < b.est;
             });

        beam.swap(cand);
    }

    return bestP;
}
int main(){
    ios::sync_with_stdio(false);
    cin.tie(nullptr);

    int N,M,A,B;
    cin>>N>>M;
    cin>>A>>B;

    vector<vector<Edge>> g(N);
    for(int i=0;i<M;i++){
        int u,v,w1,w2;
        cin>>u>>v>>w1>>w2;
        g[u].push_back({v,w1,w2});
        g[v].push_back({u,w1,w2});
    }

    sieve(N+5);
    TimeManager tm(TOTAL_LIMIT);
    vector<ll> h1 = dij(N,g,B,[](const Edge&e){return (ll)e.w1;});
    vector<ll> h3 = dij(N,g,B,[](const Edge&e){return 3LL*(ll)e.w2;});
    vector<int> bestP;
    ll bestSc = INF;
    {
        auto p = dijkstraPrime(N,g,A,B);
        if(!p.empty()){
            ll sc = evalPath(p,g);
            if(sc < bestSc){
                bestSc=sc;
                bestP=p;
            }
        }
    }

    if(tm.ok(6.0)){
        auto p = beamHQ(g,A,B,h1,h3,tm);
        if(!p.empty()){
            ll sc = evalPath(p,g);
            if(sc < bestSc){
                bestSc=sc;
                bestP=p;
            }
        }
    }

    if(bestP.empty())
        bestP = dijkstraPrime(N,g,A,B);

    cout<<bestP.size()<<"\n";
    for(int i=0;i<(int)bestP.size();i++){
        if(i) cout<<" ";
        cout<<bestP[i];
    }
    cout<<"\n";
}