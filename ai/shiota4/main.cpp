#include <iostream>
#include <sstream>
#include <fstream>
#include<algorithm>
#include<numeric>
#include <set>
#include <map>
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

vector<Block> inputInit(ifstream &ifs){
    int N;
    ifs >> N;
    vector<Block> vb(N);
    rep(i, N){
        ifs >> vb[i].blockId;
        ifs >> vb[i].bottomLeft.first >> vb[i].bottomLeft.second;
        ifs >> vb[i].topRight.first >> vb[i].topRight.second;
        int tmp;
        rep(j, 4){
            ifs >> tmp;
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


string convBid(string s){
    stringstream ss(s), oss;
    string tmp;
    int bid;
    char c;
    ss >> c >> bid >> tmp;
    oss << c <<bid + globalcnt<<tmp;
    return oss.str();
}



int main() {
    string problemId = string(getenv("PROBLEM_ID"));
    string filename = "../../problems.kyopro/" + problemId + ".kyopro";
    ifstream ifs(filename);
    auto vb = inputInit(ifs);
    globalcnt = max(int(vb.size() -1), 0);
    {
	    map<int, map<int, Block>> aaa;
	    for (auto b : vb) {
	    	aaa[b.bottomLeft.first][b.bottomLeft.second] = b;
	    }
	    vector<Block> bbb;
	    for (auto& aa : aaa) {
	    	vector<Block> aa2;
	    	for (auto& b : aa.second) {
	    		aa2.push_back(b.second);
	    	}
	    	auto b = aa2[0];
	    	for (int i = 1; i < (int)aa2.size(); ++ i) {
                cout << "merge [" << b.blockId << "] [" << aa2[i].blockId<< "]" <<endl;
                b = b.merge(aa2[i]);
	    	}
	    	bbb.push_back(b);
	    }
	    {
	    	auto b = bbb[0];
	    	for (int i = 1; i < (int)bbb.size(); ++ i) {
                cout << "merge [" << b.blockId << "] [" << bbb[i].blockId<< "]" <<endl;
                b = b.merge(bbb[i]);
	    	}
	    }
    }

    string MERGE_ISL = string(getenv("MERGE_ISL"));
    ifs = ifstream (MERGE_ISL);
    string cmd;
    string line;
    bool first = true;
    while(getline(ifs, line)){
        stringstream ss(line);
        if(!(ss >> cmd)){
            continue;
        }
        if(cmd[0] == '#'){
            cout << line <<endl;
            continue;
        }
        if(first){
            first = false;
            if(cmd != "color"){
                cout << "color" << ' ';
                string bid = "[0]";
                cout << convBid(bid);
                cout << " " << "[255,255,255,255]" << endl;
            }
        }
        if(cmd == "cut" || cmd=="color"){
            cout << cmd <<' ';
            string bid;
            ss >> bid;
            cout << convBid(bid);
            string tmp;
            while(ss >> tmp){
                cout <<' ' <<tmp;
            }
            cout <<endl;
            continue;
        } else if(cmd == "swap" || cmd=="merge"){
            cout << cmd <<' ';
            string bid;
            ss >> bid;
            cout << convBid(bid);
            ss >> bid;
            cout << ' ' << convBid(bid);
            string tmp;
            while(ss >> tmp){
                cout <<' ' <<tmp;
            }
            cout <<endl;
            continue;
        }else {
            cout << "unknown cmd: " << cmd <<endl;
            break;
        }
    }
    return 0;
}