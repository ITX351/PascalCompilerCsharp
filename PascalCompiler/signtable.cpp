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
		int type; // int 0, real 1, signtable 2
		int offset;
		signtable* table;

		sign(const string& _name, int _type, int _offset, signtable* _table)
		{ name = _name, type = _type, offset = _offset, table = _table; }
	};

	vector < sign > signs;
	int offset, number;
	signtable* fatherTable;

	signtable(signtable* _fatherTable, int _number)
	{
		clear();
		fatherTable = _fatherTable;
		number = _number;
	}

	void clear()
	{
		signs.clear();
		offset = 0;
	}

	~signtable()
	{
		clear();
	}

	bool enter(const string& _name, int _type, signtable* _table) // return if error happens
	{
		for (vector < sign > :: iterator iter = signs.begin(); iter != signs.end(); iter++)
			if (iter->name == _name)
				return true;
		signs.push_back(sign(_name, _type, offset, _table));
		
		int size;
		switch (_type)
		{
		case 0: case 1: // INT or REAL
			size = 4; 
			break;
		default: // signTable
			size = 0;
			break;
		}
		offset += size;

		return false;
	}
};
