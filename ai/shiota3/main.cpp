#include <iostream>
#include <sstream>
#include <fstream>
#include<algorithm>
#include<numeric>
#include <set>
#include<vector>

using namespace std;

#define REP(i, b, n) for(int i = b; i<n; i++)
#define rep(i, n) REP(i, 0, n)

typedef vector<int> vi;
typedef vector<vi> vvi;
typedef vector<vvi> vvvi;
typedef pair<int, int> pii;
typedef vi Color;


int globalcnt = 0;
class Canvas{
public:
    vector<vector<Color > > vvc;
};

class Block{
public:
    string blockId;
    pii bottomLeft, topRight;
    Color color;
    Block merge(const Block &b){
        globalcnt++;
        stringstream ss;
        ss << globalcnt;
        Block bl = min(*this, b);
        Block tr = max(*this, b);
        return (Block){
                ss.str(),
                bl.bottomLeft,
                tr.topRight,
                // TODO: invalid
                color};
    }
    int size()const{
        return (topRight.second - bottomLeft.second) * (topRight.first - bottomLeft.first);
    }
    bool canMerge(const Block &b) const {
        Block bl = min(*this, b);
        Block tr = max(*this, b);
        // tate
        if(bl.topRight.second == tr.bottomLeft.second
            && bl.bottomLeft.first == tr.bottomLeft.first
            && bl.topRight.first == tr.topRight.first){
            return true;
        }
        // yoko
        if(bl.topRight.first == tr.bottomLeft.first
            && bl.bottomLeft.second == tr.bottomLeft.second
            && bl.topRight.second == tr.topRight.second){
            return true;
        }
        return false;
    }
    bool operator<(const Block &b) const {
        return this->bottomLeft < b.bottomLeft;
    }

};

vector<Block> inputInit(){
    int N;
    cin >> N;
    vector<Block> vb(N);
    rep(i, N){
        cin >> vb[i].blockId;
        cin >> vb[i].bottomLeft.first >> vb[i].bottomLeft.second;
        cin >> vb[i].topRight.first >> vb[i].topRight.second;
        int tmp;
        rep(j, 4){
            cin >> tmp;
            vb[i].color.push_back(tmp);
        }
    }
    return vb;
}

Canvas inputPng(){
    int N;
    cin >> N;
    Canvas can;
    can.vvc = vector<vector<Color > >(400, vector<Color>(400));
    rep(i, N){
        int y = 400 - (i/400) - 1;

        int x = i %400;
        rep(j, 4){
            cin >> can.vvc[x][y][j];
        }
    }
    return can;
}






int main() {
    auto vb = inputInit();
    globalcnt = max(int(vb.size() -1), 1);
    while(vb.size() != 1){
        sort(vb.begin(), vb.end(), [](const Block & l, const Block& r) -> bool
             {
            return l.size() > r.size();
        });
        vector<Block> next;
        set<string> merged;
        for(int i = 0; i < vb.size(); i++){
            if(merged.count(vb[i].blockId) != 0){
                continue ;
            }
            for(int j = i+1; j<vb.size(); j++){
                if(merged.count(vb[j].blockId) != 0){
                    continue ;
                }
                if(vb[i].canMerge(vb[j])){
                    cout << "merge [" << vb[i].blockId << "] [" << vb[j].blockId<< "]" <<endl;
                    next.push_back(vb[i].merge(vb[j]));
                    merged.insert(vb[i].blockId);
                    merged.insert(vb[j].blockId);
                    goto NEXT;
                }
            }
        }
        if(merged.size() == 0){
            break;
        }
        NEXT:;
        for(auto b : vb){
            if(merged.count(b.blockId))continue;
            next.push_back(b);
        }
        vb = next;
    }
    cout << "color ["<< vb[0].blockId <<"] [255,255,255,255]" <<endl;
    return 0;
}
