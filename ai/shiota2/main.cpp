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
    cout << vb.size() <<endl;
    return 0;
}
