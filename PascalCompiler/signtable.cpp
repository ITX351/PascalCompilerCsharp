#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <algorithm>
#include <iostream>
#include <vector>
using namespace std;

//Love lemon_TsyD
//Miss lydrainbowcat
//By ITX351

class signtable
{
public:
    struct sign
    {
        string name;
        int type; // int 0, real 1
        int offset;

        sign(const string& _name, int _type, int _offset)
        { name = _name, type = _type, offset = _offset; }
    };

    vector < sign > signs;

    signtable()
    {
        clear();
    }

    void clear()
    {
        signs.clear();
    }

    ~signtable()
    {
        clear();
    }

    bool enter(const string& _name, int _type, int _offset) // return if error happens
    {
        for (vector < sign > :: iterator iter = signs.begin(); iter != signs.end(); iter++)
            if (iter->name == _name)
                return true;
        signs.push_back(sign(_name, _type, _offset));
        return false;
    }
};
