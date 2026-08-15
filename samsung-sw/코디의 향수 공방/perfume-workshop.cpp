#include <bits/stdc++.h>
using namespace std;
using ll = long long;

vector<int> scent(1);
vector<bool> alive(1,false);

vector<int> getActiveScent(){
    vector<int> active;

    for(int i = 1; i<scent.size(); i++){
        if(alive[i]) active.push_back(scent[i]);
    }

    return active;
}

int getMinCount(int k){
    vector<int> active = getActiveScent();

    if(active.empty()) return -1;

    sort(active.begin(),active.end());
    active.erase(unique(active.begin(),active.end()),active.end());

    vector<int> dist(k+1,-1);
    queue<int> q;
    dist[0]=0;
    q.push(0);

    while(!q.empty()){
        int currentSum = q.front(); q.pop();

        for(int value : active){
            if(currentSum + value >k) break;

            int nextSum = currentSum + value;

            if(dist[nextSum]!=-1) continue;

            dist[nextSum] = dist[currentSum] + 1;

            if(nextSum==k) return dist[nextSum];
            q.push(nextSum);
        }
    }
    return -1;
}

ll getOrderCount(ll k){
    vector<int> active = getActiveScent();
    sort(active.begin(),active.end());
    ll ans = 0;

    for(int top : active) {
        for(int middle : active) {
            ll minBase = k - top - middle;
            auto it = lower_bound(active.begin(),active.end(),minBase);
            ans += (active.end()-it);
        }
    }

    return ans;
}

int main() {
    ios::sync_with_stdio(0), cin.tie(0);
    int q;
    cin >> q;

    while(q--){
        int c;
        cin >> c;

        if(c == 1){
            int n;
            cin >> n;

            scent.assign(n+1,0);
            alive.assign(n+1,true);

            alive[0] = false;

            for(int i=1;i<=n;i++) cin >> scent[i];
        }
        else if(c==2){
            int v;
            cin >> v;
            scent.push_back(v);
            alive.push_back(true);
        }
        else if(c==3){
            int i;
            cin >> i;
            if(i >= scent.size() || !alive[i]) cout << -1 << '\n';
            else{
                cout << scent[i] << '\n';
                alive[i] = false;
            }
        }
        else if(c==4){
            int k;
            cin >> k;
            cout << getMinCount(k) << '\n';
        }
        else{
            ll k;
            cin >> k;
            cout << getOrderCount(k) << '\n';
        }
    }
}