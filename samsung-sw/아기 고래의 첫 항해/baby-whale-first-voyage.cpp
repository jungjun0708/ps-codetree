#include <bits/stdc++.h>
using namespace std;

int n;
vector<vector<int>> board;
vector<vector<bool>> visited;

int dr[4] = {-1,0,1,0};
int dc[4] = {0,1,0,-1};

bool inRange(int r, int c){
    return 0 <= r && r<n && 0<=c && c<n;
}

tuple<int,int,int> findNearUnvis(int sr, int sc){
    int moveOrder[4] = {3,2,1,0};
    vector<vector<bool>> seen(n, vector<bool>(n,false));

    queue<pair<int,int>> q;
    q.push({sr,sc});
    seen[sr][sc] = true;

    while(!q.empty()){
        int levelSize = q.size();

        bool found = false;

        int bestR=-1, bestC=-1, bestLastDir = -1;

        while(levelSize--){
            auto[r,c] = q.front(); q.pop();

            for(int dir : moveOrder){
                int nr = r+dr[dir];
                int nc = c + dc[dir];
                if(!inRange(nr,nc)) continue;
                if(board[nr][nc]==1) continue;
                if(seen[nr][nc]) continue;

                seen[nr][nc] = true;

                if(!visited[nr][nc]){
                    if(!found || pair{nr,nc} < pair{bestR,bestC}){
                        found = true;
                        bestR = nr;
                        bestC = nc;
                        bestLastDir = dir;
                    }
                }
                else q.push({nr,nc});
            }
        }

        if(found) return {bestR,bestC,bestLastDir};
    }
    return {-1,-1,-1};
}

int main(){
    ios::sync_with_stdio(0), cin.tie(0);
    int r,c,inputDir;
    cin >> n >> r >> c >> inputDir;

    --r; --c;

    board.assign(n,vector<int>(n));
    
    int totalSea = 0;

    for(int i=0;i<n;i++){
        for(int j=0;j<n;j++){
            cin >> board[i][j];
            if(board[i][j]==0) totalSea++;
        }
    }

    int dirMap[5] = {-1,0,2,3,1};
    int dir = dirMap[inputDir];

    visited.assign(n, vector<bool>(n,false));

    visited[r][c] = true;
    int visitedCount = 1;
    vector<pair<int,int>> ans;
    ans.push_back({r,c});

    int turnOrder[4] = {0,3,1,2};
    while(visitedCount<totalSea){
        bool moved = false;
        for(int turn : turnOrder){
            int nextDir = (dir + turn) % 4;

            int nr = r + dr[nextDir];
            int nc = c + dc[nextDir];

            if(!inRange(nr,nc)) continue;
            if(board[nr][nc]==1) continue;
            if(visited[nr][nc]) continue;

            r = nr;
            c = nc;
            dir = nextDir;

            visited[r][c] = true;
            visitedCount++;
            ans.push_back({r,c});
            moved = true;
            break;
        }
        if(moved) continue;
        auto [targetR, targetC, lastDir] = findNearUnvis(r,c);

        if(targetR == -1) break;
        r = targetR;
        c = targetC;
        dir = lastDir;

        visited[r][c] = true;
        visitedCount++;

        ans.push_back({r,c});
    }

    for(auto [ansR,ansC] : ans){
        cout << ansR + 1 << ' ' << ansC+1 << '\n';
    }
}