#include <bits/stdc++.h>
using namespace std;
using ll = long long;

enum TurtleState{
    ACTIVE,
    ARRIVED,
    FOSSIL
};

struct Turtle {
    int r,c;
    TurtleState state = ACTIVE;
    int arriveTurn = -1;
};

struct Volcano{
    int r,c;
    int threshold;
    int pressure;
};

int N,M,K;

vector<vector<int>> coral;
vector<vector<int>> occupied;
vector<vector<int>> volcanoIdx;

vector<Turtle> turtles;
vector<Volcano> volcanoes;

int dr[4] = {0,1,0,-1};
int dc[4] = {1,0,-1,0};

bool inRange(int r, int c){
    return 0<=r && r<N && 0<=c && c<N;
}

void moveTurtle(int id, int turn){
    Turtle &turtle = turtles[id];

    if(turtle.state != ACTIVE) return;

    vector<vector<int>> dist(N, vector<int>(N,-1));
    queue<pair<int,int>> q;

    int destR = N-1;
    int destC = N-1;

    dist[destR][destC] = 0;
    q.push({destR,destC});

    while(!q.empty()){
        auto [r,c] = q.front();q.pop();

        for(int dir = 0; dir<4; dir++){
            int nr = r + dr[dir];
            int nc = c + dc[dir];

            if(!inRange(nr,nc)) continue;
            if(dist[nr][nc]!=-1) continue;
            if(coral[nr][nc]==1) continue;

            if(occupied[nr][nc]!=0 && occupied[nr][nc] != id) continue;

            dist[nr][nc] = dist[r][c] + 1;
            q.push({nr,nc});
        }
    }

    int r = turtle.r;
    int c = turtle.c;

    if(dist[r][c]==-1) return;

    for(int dir=0;dir<4;dir++){
        int nr = r +dr[dir];
        int nc = c + dc[dir];

        if(!inRange(nr,nc)) continue;
        if(dist[nr][nc]!=dist[r][c]-1) continue;

        occupied[r][c] = 0;

        turtle.r = nr;
        turtle.c = nc;

        if(nr == destR && nc == destC){
            turtle.state = ARRIVED;
            turtle.arriveTurn = turn;
        }
        else{
            occupied[nr][nc] = id;
        }
        return;
    }
}

void addHeatAndCheck(
    int r,
    int c,
    int amount,
    vector<vector<int>> &heat,
    vector<int> &erupted,
    queue<int> &eruptionQueue
){
    heat[r][c] += amount;
    int nextVolcano = volcanoIdx[r][c];

    if(nextVolcano==-1) return;

    if(erupted[nextVolcano]) return;

    Volcano &v = volcanoes[nextVolcano];
    if(v.pressure+heat[r][c]>=v.threshold){
        erupted[nextVolcano] = true;
        eruptionQueue.push(nextVolcano);
    }
}

void eruptVolcanoes(
    vector<vector<int>> &heat,
    vector<int> &erupted
){
    queue<int> eruptionQueue;
    for(int i=0;i<K;i++){
        if(volcanoes[i].pressure>=volcanoes[i].threshold){
            erupted[i] = true;
            eruptionQueue.push(i);
        }
    }

    while(!eruptionQueue.empty()){
        int idx = eruptionQueue.front(); eruptionQueue.pop();
        Volcano &v = volcanoes[idx];
        addHeatAndCheck(
            v.r,
            v.c,
            v.threshold,
            heat,
            erupted,
            eruptionQueue
        );

        for(int d=0;d<4;d++){
            int nr = v.r + dr[d];
            int nc = v.c + dc[d];

            int currentHeat = v.threshold/2;

            while(inRange(nr,nc)&& currentHeat>0){
                if(coral[nr][nc]==1) break;

                addHeatAndCheck(
                    nr,
                    nc,
                    currentHeat,
                    heat,
                    erupted,
                    eruptionQueue
                );

                nr +=dr[d];
                nc +=dc[d];

                currentHeat /=2;
            }
        }
    }
}

void fossilTurtles(vector<vector<int>> &heat){
    for(int id = 1;id<=M;id++){
        Turtle &turtle = turtles[id];
        if(turtle.state != ACTIVE) continue;
        if(heat[turtle.r][turtle.c]>=20){
            turtle.state = FOSSIL;
        }
    }
}

int main(){
    ios::sync_with_stdio(0),cin.tie(0);

    cin >> N >> M >> K;
    coral.assign(N,vector<int>(N));
    occupied.assign(N,vector<int>(N));
    volcanoIdx.assign(N,vector<int>(N,-1));

    for(int r=0;r<N;r++){
        for(int c=0;c<N;c++) cin >> coral[r][c];
    }

    turtles.resize(M+1);

    for(int id=1;id<=M;id++){
        int r,c;
        cin >> r >> c;

        turtles[id].r = r;
        turtles[id].c = c;

        occupied[r][c] = id;
    }

    volcanoes.resize(K);

    for(int i=0;i<K;i++){
        int r,c;
        int p;

        cin >> r >> c >> p;
        volcanoes[i] = {
            r,c,p,0
        };

        volcanoIdx[r][c] = i;
    }

    for(int turn = 1;turn <=100; turn++){
        for(int id=1;id<=M;id++){
            moveTurtle(id,turn);
        }

        for(Volcano &v : volcanoes){
            v.pressure += 10;
        }

        vector<vector<int>> heat(N,vector<int>(N,0));

        vector<int> erupted(K,false);

        eruptVolcanoes(heat, erupted);
        fossilTurtles(heat);

        for(int i=0;i<K;i++){
            if(erupted[i]) volcanoes[i].pressure=0;
        }
    }

    for(int id=1;id<=M;id++) cout << turtles[id].arriveTurn << '\n';
}