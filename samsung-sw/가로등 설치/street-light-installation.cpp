#include <bits/stdc++.h>
using namespace std;
using ll = long long;

ll N;

set<ll> lights;
set<pair<ll,ll>> gaps;
vector<ll> posById(1);

pair<ll,ll> makeGap(ll left, ll right){
    return {-(right-left),left};
}

void addGap(ll left, ll right){
    gaps.insert(makeGap(left, right));
}

void removeGap(ll left,ll right){
    gaps.erase(makeGap(left,right));
}

void initialize(int M){
    vector<ll> initial(M);

    for(int i=0;i<M;i++){
        cin >> initial[i];
        lights.insert(initial[i]);
        posById.push_back(initial[i]);
    }

    for(int i=0;i<M-1;i++){
        addGap(initial[i],initial[i+1]);
    }
}

void addLight(){
    auto[negDist,left] = *gaps.begin();

    ll dist = -negDist;
    ll right = left + dist;

    removeGap(left,right);

    ll middle= left + (right-left+1)/2;

    lights.insert(middle);
    posById.push_back(middle);

    addGap(left,middle);
    addGap(middle,right);
}

void removeLight(int id){
    ll x = posById[id];
    auto it = lights.find(x);
    bool hasLeft = (it != lights.begin());
    auto rightIt = next(it);
    bool hasRight = (rightIt != lights.end());

    ll left = -1, right = -1;

    if(hasLeft){
        left = *prev(it);
        removeGap(left,x);
    }

    if(hasRight){
        right = *rightIt;
        removeGap(x,right);
    }

    if(hasLeft && hasRight) addGap(left,right);
    lights.erase(it);
    posById[id] = -1;
}

ll calculateAns(){
    ll leftmost = *lights.begin();
    ll rightmost = *lights.rbegin();

    ll maximumGap = -gaps.begin()->first;

    ll leftPower = 2*(leftmost - 1);
    ll rightPower = 2*(N-rightmost);

    return max({leftPower,rightPower,maximumGap});
}

int main(){
    ios::sync_with_stdio(0),cin.tie(0);

    int Q;
    cin >> Q;

    while(Q--){
        int c;
        cin >> c;

        if(c == 100) {
            int M;
            cin >> N >> M;
            initialize(M);
        }
        else if(c == 200){
            addLight();
        }
        else if(c==300){
            int d;
            cin >> d;
            removeLight(d);
        }
        else{
            cout << calculateAns()<< '\n';
        }
    }
}
