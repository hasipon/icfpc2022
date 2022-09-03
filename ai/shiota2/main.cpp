#include <iostream>
#include <sstream>
#include <fstream>
#include <set>
#include "picojson.h"

using namespace std;

typedef vector<int> vi;
typedef pair<int, int> pii;

int globalcnt = 0;

class Block{
public:
    string blockId;
    pii bottomLeft, topRight;
    vi color;
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
    bool canMerge(const Block &b) {
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

pii parsePoint(picojson::array p){
    return pii(p[0].get<double>(), p[1].get<double>() );
}

vi parseColor(picojson::array c){
    vi ret;
    for(auto a : c){
        ret.push_back(a.get<double>());
    }
    return ret;
}

vector<Block> parseInitJson(string problemId){
    string filename ="../../problems/" + problemId + ".initial.json";
    ifstream ifs(filename);
    std::string json((std::istreambuf_iterator<char>(ifs)), std::istreambuf_iterator<char>());
    picojson::value v;
    picojson::parse(v, json);
    picojson::object root = v.get<picojson::object>();
    auto vb = root["blocks"].get<picojson::array>();

    vector<Block> ret;

    for(auto jb : vb){
        auto b = jb.get<picojson::object>();
        string id = b["blockId"].get<string>();
        globalcnt = max(atoi(id.c_str()), globalcnt);
        auto bl = parsePoint(b["bottomLeft"].get<picojson::array >());
        auto tr = parsePoint(b["topRight"].get<picojson::array>());
        auto c = parseColor(b["color"].get<picojson::array>());
        ret.push_back((Block){
                id, bl, tr, c
        });
    }
    return ret;
}


int main() {
    auto vb = parseInitJson("26");
    while(vb.size() != 1){
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
                    cout << vb[i].blockId << ' ' << vb[j].blockId <<endl;
                    next.push_back(vb[i].merge(vb[j]));
                    merged.insert(vb[i].blockId);
                    merged.insert(vb[j].blockId);
                }
            }
        }
        if(merged.size() == 0){
            break;
        }
        for(auto b : vb){
            if(merged.count(b.blockId))continue;
            next.push_back(b);
        }
        vb = next;
    }
    return 0;
}
