#include <iostream>
#include<cmath>
#include <sstream>
#include <fstream>
#include<map>
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
typedef vector<pii> vii;
typedef vi Color;


int globalcnt = 0;
class Canvas{
public:
    vector<vector<Color > > vvc;
    Canvas(){
        this->vvc = vector<vector<Color > >(400, vector<Color>(400, Color(4)));
    }

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

Canvas inputPng(ifstream &ifs){
    int N;
    ifs >> N;
    Canvas can;
    rep(i, N){
        int y = 400 - (i/400) - 1;

        int x = i %400;
        rep(j, 4){
            ifs >> can.vvc[x][y][j];
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

double dist(Color c1, Color c2){
    int diff0 = c1[0] - c2[0];
    int diff1 = c1[1] - c2[1];
    int diff2 = c1[2] - c2[2];
    double r = 0;
    r += diff0*diff0;
    r += diff1*diff1;
    r += diff2*diff2;
    //r += x3*x3;
    return r;
}

Color solveColor(const vii &points, const Canvas &canvas){
    int sum[3] = {0};

    for(auto p : points) {
        rep(i, 3){
            sum[i] += canvas.vvc[p.first][p.second][i];
        }
    }

    double res[3];

    int area = points.size();
    for (int k = 0; k < 3; ++ k) {
        res[k] = (sum[k] + area/2) / area;
    }

    // Weiszfeld's algorithm
    // https://en.wikipedia.org/wiki/Geometric_median
    for (auto it = 0; it < 1000; it++) {
        // result = numer / denom
        double numer[3] = {0};
        double denom = 0;

        for(auto p : points){
            double norm = 0;

            for (int k = 0; k < 3; ++k) {
                auto d = canvas.vvc[p.first][p.second][k]- res[k];
                norm += d * d;
            }
            norm = sqrt(fmax(norm, 1e-6));

            denom += 1 / norm;
            for (int k = 0; k < 3; ++k) {
                numer[k] += canvas.vvc[p.first][p.second][k] / norm;
            }
        }

        double difference = 0;
        for (int k = 0; k < 3; ++k) {
            double previous = res[k];
            double next = numer[k] / denom;
            difference += fabs(previous - next);
            res[k] = next;
        }

        if (difference < 0.1) {
            break;
        }
    }

    Color  resInt(4);
    resInt[3] = 255;
    for (int k = 0; k < 3; ++ k) {
        int rounded = round(res[k]);
        if (rounded < 0) {
            resInt[k] = 0;
        } else if (255 < rounded) {
            resInt[k] = 255;
        } else {
            resInt[k] = rounded;
        }
    }

    double score = 0;
    for(auto p : points){
        score += sqrt(dist(canvas.vvc[p.first][p.second], resInt));
    }

    // 山登りで最後まで収束させる
    for (;;) {
        for (int k = 0; k < 3; ++ k) {
            if (resInt[k] > 0) {
                -- resInt[k];
                double score2 = 0;
                for(auto p : points) {
                    score2 += sqrt(dist(canvas.vvc[p.first][p.second], resInt));

                }
                if (score2 < score) {
                    score = score2;
                    goto next;
                }
                ++ resInt[k];
            }
            if (resInt[k] < 255) {
                ++ resInt[k];
                double score2 = 0;
                for(auto p : points) {
                    score2 += sqrt(dist(canvas.vvc[p.first][p.second], resInt));

                }
                if (score2 < score) {
                    score = score2;
                    goto next;
                }
                -- resInt[k];
            }
        }
        break;
    next:;
    }
    return resInt;
}

map<string, vii>  getBlockIdInfo(){
    map<string, vii> ret;
    auto ifs = fstream(string(getenv("BLOCK_ID_INFO")));
    rep(i, 400)rep(j,400){
        int y = 400 - i -1;
        int x = j;
       string id;
        ifs >> id;
        ret["["+id+"]"].push_back(make_pair(x, y));
    }
    return ret;
}

int main() {
    string problemId = string(getenv("PROBLEM_ID"));
    string filename = "../../problems.kyopro/" + problemId + ".kyopro";
    ifstream ifs(filename);
    auto vb = inputInit(ifs);

    Canvas canvas;
    for(auto b : vb){
        REP(x, b.bottomLeft.first, b.topRight.first){
            REP(y, b.bottomLeft.second, b.topRight.second){
                canvas.vvc[x][y] = b.color;
            }
        }
    }

    vii points;
    rep(i, 400){
        rep(j, 400){
            points.push_back(make_pair(i, j));
        }
    }

    auto blockIdInfo = getBlockIdInfo();

    Canvas target = inputPng(ifs);

    string MERGE_ISL = string(getenv("MERGE_ISL"));
    ifs = ifstream (MERGE_ISL);
    string cmd;
    string line;
    while(getline(ifs, line)){
        stringstream ss(line);
        if(!(ss >> cmd)){
            continue;
        }
        if(cmd != "color"){
            cout << line <<endl;
            continue;
        }
        // color
        string bid;
        ss >> bid;
        auto points = blockIdInfo[bid];
        if(points.empty()){
            continue;
        }
        cout << cmd <<' ';
        cout << bid;
        string colorStr;
        ss >> colorStr;
        auto c = solveColor(points, target);
        cout << " [" << c[0] <<","<<c[1]<<","<<c[2]<<","<<c[3]<<"]"<<endl;
    }
    return 0;
}
