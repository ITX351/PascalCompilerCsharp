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
private:
	const int MAXLONGINT = 0x7f7f7f7f;
public:
	struct Type
	{
		int type;
		int low, high;
		Type() { }
		Type(int _type, int _low, int _high)
		{ type = _type, low = _low, high = _high; }
	};

	struct sign
	{
		string name;
		Type type; // int 0, real 1, signtable 2
		int offset;
		int array_num;
		signtable* table;

		sign(const string& _name, const Type& _type, int _offset, signtable* _table)
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

	bool enter(const string& _name, const Type& _type, signtable* _table) // return if error happens
	{
		for (vector < sign > :: iterator iter = signs.begin(); iter != signs.end(); iter++)
			if (iter->name == _name)
				return true;
		signs.push_back(sign(_name, _type, offset, _table));
		
		int var_num;
		if (_type.high == MAXLONGINT)
			var_num = 1;
		else
			var_num = _type.high - _type.low + 1;

		int size;
		switch (_type.type)
		{
		case 0: case 1: // INT or REAL
			size = 4 * var_num;
			break;
		default: // signTable
			size = 0;
			break;
		}
		offset += size;

		return false;
	}
};
