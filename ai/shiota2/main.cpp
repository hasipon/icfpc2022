#include <iostream>
#include <sstream>
#include <fstream>
#include "picojson.h"

using namespace std;

typedef vector<int> vi;
typedef pair<int, int> pii;

class Block{
public:
    string blockId;
    pii bottomLeft, topRight;
    vi color;
    bool canMerge(Block &b) {
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
    std::cout << "Hello, World!" << std::endl;
    auto vb = parseInitJson("26");
    while(vb.size() != 1){
        for(int i = 0; i < vb.size(); i++){
            for(int j = j+1; j<vb.size(); j++){
                if(vb[i].canMerge(vb[j])){
                    cout << i <<' ' << j <<endl;
                }
            }
        }
        break;
    }
    return 0;
}
