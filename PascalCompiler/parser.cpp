#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <algorithm>
#include <iostream>
#include <map>
#include <vector>
#include <sstream>
#include <stack>
#include <queue>
#include "signtable.cpp"
#include "constants.h"
using namespace std;

//Love lemon_TsyD
//Miss lydrainbowcat
//By ITX351

class Parser
{
private:
	typedef enum
	{
		C_X_ASS_Y_OP_Z = 0, C_X_ASS_OP_Y, C_X_ASS_Y, C_GOTO_X,
		C_IF_X_GOTO, C_IF_X_RELOP_Y_GOTO, C_PARAM_X, C_CALL_P_N, 
		C_RET, C_TITLE, C_X_ASS_Y_ARR_Z
	} CommandType;

	typedef pair < int, string > IS;
	typedef pair < int, int > II;
	vector < IS > words;

	struct Reduce
	{
		int aim;
		vector < int > signs;
		Reduce(int _aim) { aim = _aim; signs.clear(); }
	};
	vector < Reduce > reduceTable;
	map < string, int > terminalTable;
	map < II, int > parseTable;

	char str[51], str2[131];

	struct Node
	{
		int lineNumber;
		int status;
		string signName;
		vector < int > truelist, falselist, nextlist;

		int expressionNum;
		int tmpLineNumber;

		vector < string > identifiers;
		int declarationType;

		Node() 
		{ 
			expressionNum = 0; 
			truelist.clear(); 
			falselist.clear(); 
			nextlist.clear();
			identifiers.clear();
		}
	};
	stack < int > s1;
	stack < Node > s2;
	vector < Node > nodes;

	struct Command
	{
		CommandType type;
		string arg1, arg2, op, result;
	};
	vector < Command > commands;

	int nextquad;
	void gencode(CommandType _type, const string& _result, const string& _arg1,
				 const string& _op, const string& _arg2)
	{
		Command cmd;
		cmd.type = _type; cmd.arg1 = _arg1; cmd.arg2 = _arg2;
		cmd.op = _op; cmd.result = _result;
		commands.push_back(cmd);
		nextquad++;
	}

	int tmpVar;
	string nextTmpVar()
	{
		return string("t") + intToString(++tmpVar);
	}

	char iToA[20];
	string intToString(int i)
	{
		sprintf(iToA, "%d", i);
		return string(iToA);
	}

	vector<int> merge(vector<int> a, vector<int> b)
	{
		vector<int> ret;
		for (vector<int> :: iterator iter = a.begin(); iter != a.end(); iter++)
			ret.push_back(*iter);
		for (vector<int> :: iterator iter = b.begin(); iter != b.end(); iter++)
			ret.push_back(*iter);
		return ret;
	}

	void fillback(vector<int> list, int quad)
	{
		for (vector <int> :: iterator iter = list.begin(); iter != list.end(); iter++)
			commands[*iter].result = intToString(quad);
	}

	signtable* nowSignTable; int signTableCount;

public:
	Parser(vector < IS > _words)
	{
		words = _words;
	}

	void init()
	{
		reduceTable.clear();
		terminalTable.clear();
		parseTable.clear();
		commands.clear();
		nextquad = tmpVar = 0;

		nowSignTable = new signtable(NULL, signTableCount++);

		int num = 0; FILE *fp;

		fp = fopen("..\\txt\\terminal.txt", "r");
		while (fscanf(fp, "%s", str) > 0)
			terminalTable[string(str)] = num++;
		fclose(fp);
		fp = fopen("..\\txt\\nonterminal.txt", "r");
		while (fscanf(fp, "%s", str) > 0)
			terminalTable[string(str)] = num++;
		fclose(fp);

		terminalTable["program'"] = num++;

		fp = fopen("..\\txt\\grammar_list.txt", "r");
		while (fgets(str2, 131, fp) != NULL)
		{
			istringstream is(str2);
			is >> str;
			string tmp(str);
			Reduce reduce(terminalTable[tmp]);

			is >> str;

			while (is >> str)
			{
				string tmp(str);
				reduce.signs.push_back(terminalTable[tmp]);
			}
			reduceTable.push_back(reduce);
		}
		fclose(fp);

		fp = fopen("..\\txt\\table.txt", "r");
		int a, b, c;
		while (fscanf(fp, "%d%d%d", &a, &b, &c) > 0)
			parseTable[II(a, b)] = c;
		fclose(fp);
	}

	int parse()
	{
		FILE *fp_parser = fopen("parser_result.txt", "w");

		while (!s1.empty())
			s1.pop();
		while (!s2.empty())
			s2.pop();
		s1.push(0);

		int i = 0;
		while (i < (int)words.size())
		{
			int a = s1.top(), b = words[i].first;
			if (a == 1 && b == 38) // 1, 38 ACC
			{
				fprintf(fp_parser, "ACCEPT!\n");
				return -1;
			}

			if (parseTable.find(II(a, b)) == parseTable.end())
			{
				fprintf(fp_parser, "ERROR PARSE(ACTION) %d, %d\n", a, b);
				return i;
			}

			int c = parseTable[II(a, b)];
			if (c > 0) // shift
			{
				Node node;
				node.lineNumber = nextquad;
				node.status = b;

				signtable* newSignTable;
				switch (TokenType(b))
				{
				case T_FUNCTION: case T_PRODEDURE:
					newSignTable = new signtable(nowSignTable, signTableCount);
					nowSignTable->enter(string("SignTable") + intToString(signTableCount), 2, newSignTable);
					nowSignTable = newSignTable;
					signTableCount++;
					node.tmpLineNumber = nextquad;
					if (TokenType(b) == T_FUNCTION)
						gencode(C_TITLE, "FUNC ", "FunctionNotDone", "", "");
					else
						gencode(C_TITLE, "PROC ", "ProcedureNotDone", "", "");
					break;
				case T_DO:
					gencode(C_IF_X_GOTO, intToString(nextquad + 2), "DoExpressionNotDone", "", "");
					node.tmpLineNumber = nextquad;
					gencode(C_GOTO_X, "DoGotoNotDone", "", "", "");
					break;
				case T_THEN:
					gencode(C_IF_X_GOTO, intToString(nextquad + 2), "ThenExpressionNotDone", "", "");
					node.tmpLineNumber = nextquad;
					gencode(C_GOTO_X, "ThenGotoNotDone", "", "", "");
					break;
				case T_ELSE:
					node.tmpLineNumber = nextquad;
					gencode(C_GOTO_X, "ElseGotoNotDone", "", "", "");
					break;
				case T_IDN: case T_INT: case T_REAL:
					node.signName = words[i].second;
					break;
				}

				s1.push(c);
				s2.push(node);
				i++;

				fprintf(fp_parser, "Status move in %d, Alphas move in %d.\n", a, b);
			}
			else // reduce
			{
				c = -c;
				if (c < 0 || c >= (int)reduceTable.size())
				{
					fprintf(fp_parser, "ERROR IN FOUND REDUCE %d.\n", c);
					return i;
				}
				fprintf(fp_parser, "REDUCED By %d.\n", c);

				Reduce reduce = reduceTable[c];
				fprintf(fp_parser, "POP");

				nodes.clear();
				for (int j = 0; j < (int)reduce.signs.size(); j++)
				{
					fprintf(fp_parser, " (%d, %d)", s1.top(), s2.top().status);
					nodes.push_back(s2.top());
					s1.pop(); s2.pop();
				}
				reverse(nodes.begin(), nodes.end());

				int non_terminal = reduce.aim;
				Node node = translate(c);
				node.status = non_terminal;

				s2.push(node);
				if (parseTable.find(II(s1.top(), non_terminal)) == parseTable.end())
				{
					fprintf(fp_parser, "\nERROR PARSE(GOTO) (%d, %d)", s1.top(), non_terminal);
					return i;
				}
				int Goto = parseTable[II(s1.top(), non_terminal)];
				fprintf(fp_parser, " and PUSH %d, GOTO %d.\n", non_terminal, Goto);
				s1.push(Goto);
			}
		}
		fclose(fp_parser);
		return 0;
	}

	Node translate(int reduceId)
	{
		Node node;
		node.lineNumber = nextquad;

		string var;

		signtable* oldSignTable;
		switch (reduceId)
		{
		case 0: // program' => program
			break;
		case 1: // program => T_PROGRAM T_IDN T_LPAR identifier_list T_RPAR T_SEMICL declarations subprogram_declarations compound_statement
			break;
		case 2: // identifier_list => T_IDN
			node.identifiers.push_back(nodes[0].signName);
			break;
		case 3: // identifier_list => identifier_list T_COMMA T_IDN
			node.identifiers = nodes[0].identifiers;
			node.identifiers.push_back(nodes[2].signName);
			break;
		case 4: // declarations => T_VAR declaration T_SEMICL
			break;
		case 5: // declarations =>
			break;
		case 6: // declaration => declaration T_SEMICL identifier_list T_COLON type
			for (vector<string>::iterator iter = nodes[2].identifiers.begin(); 
				iter != nodes[2].identifiers.end(); iter++)
				nowSignTable->enter(*iter, nodes[4].declarationType, NULL);
			break;
		case 7: // declaration => identifier_list T_COLON type
			for (vector<string>::iterator iter = nodes[0].identifiers.begin(); 
				iter != nodes[0].identifiers.end(); iter++)
				nowSignTable->enter(*iter, nodes[2].declarationType, NULL);
			break;
		case 8: // type => standard_type
			node.declarationType = nodes[0].declarationType;
			break;
		case 9: // type => T_ARRAY T_LBRKPAR T_INT T_DOUBLEPERIOD T_INT T_RBRKPAR T_OF standard_type
			// TODO: ARRAY DECLARATION
			break;
		case 10: // standard_type => T_INTTYPE
			node.declarationType = 0;
			break;
		case 11: // standard_type => T_REALTYPE
			node.declarationType = 1;
			break;
		case 12: // subprogram_declarations => subprogram_declarations subprogram_declaration T_SEMICL
			break;
		case 13: // subprogram_declarations =>
			break;
		case 14: // subprogram_declaration => subprogram_head declarations compound_statement
			gencode(C_RET, "", "", "", "");
			break;
		case 15: // subprogram_head => T_FUNCTION T_IDN arguments T_COLON standard_type T_SEMICL
			oldSignTable = nowSignTable->fatherTable;
			oldSignTable->offset += nowSignTable->offset;
			nowSignTable = oldSignTable;
			commands[nodes[0].tmpLineNumber].arg1 = nodes[1].signName;
			break;
		case 16: // subprogram_head => T_PRODEDURE T_IDN arguments T_SEMICL
			oldSignTable = nowSignTable->fatherTable;
			oldSignTable->offset += nowSignTable->offset;
			nowSignTable = oldSignTable;
			commands[nodes[0].tmpLineNumber].arg1 = nodes[1].signName;
			break;
		case 17: // arguments => T_LPAR parameter_list T_RPAR
			break;
		case 18: // arguments =>
			break;
		case 19: // parameter_list => identifier_list T_COLON type
			for (vector<string>::iterator iter = nodes[0].identifiers.begin(); 
				iter != nodes[0].identifiers.end(); iter++)
				nowSignTable->enter(*iter, nodes[2].declarationType, NULL);
			break;
		case 20: // parameter_list => parameter_list T_SEMICL identifier_list T_COLON type
			for (vector<string>::iterator iter = nodes[2].identifiers.begin(); 
				iter != nodes[2].identifiers.end(); iter++)
				nowSignTable->enter(*iter, nodes[4].declarationType, NULL);
			break;
		case 21: // compound_statement => T_BEGIN optional_statements T_END
			break;
		case 22: // optional_statements => statement_list
			break;
		case 23: // optional_statements =>
			break;
		case 24: // statement_list => statement
			break;
		case 25: // statement_list => statement_list T_SEMICL statement
			break;
		case 26: // statement => variable T_ASS expression
			gencode(C_X_ASS_Y, nodes[0].signName, nodes[2].signName, "", "");
			break;
		case 27: // statement => procedure_statement
			break;
		case 28: // statement => compound_statement
			break;
		case 29: // statement => T_IF expression T_THEN statement T_ELSE statement
			commands[nodes[2].tmpLineNumber - 1].arg1 = nodes[1].signName;
			commands[nodes[2].tmpLineNumber].result = intToString(nodes[5].lineNumber);
			commands[nodes[4].tmpLineNumber].result = intToString(nextquad);
			break;
		case 30: // statement => T_WHILE expression T_DO statement
			gencode(C_GOTO_X, intToString(nodes[0].lineNumber), "", "", "");
			commands[nodes[2].tmpLineNumber - 1].arg1 = nodes[1].signName;
			commands[nodes[2].tmpLineNumber].result = intToString(nextquad);
			break;
		case 31: // variable => T_IDN
			node.signName = nodes[0].signName;
			break;
		case 32: // variable => T_IDN T_LBRKPAR expression T_RBRKPAR
			node.signName = nodes[0].signName + string("[") + nodes[2].signName + string("]");
			break;
		case 33: // procedure_statement => T_IDN
			gencode(C_CALL_P_N, "", nodes[0].signName, "", "0");
			break;
		case 34: // procedure_statement => T_IDN T_LPAR expression_list T_RPAR
			gencode(C_CALL_P_N, "", nodes[0].signName, "", intToString(nodes[2].expressionNum));
			break;
		case 35: // expression_list => expression
			node.expressionNum = 1;
			gencode(C_PARAM_X, "", nodes[0].signName, "", "");
			break;
		case 36: // expression_list => expression_list T_COMMA expression
			node.expressionNum = nodes[0].expressionNum + 1;
			gencode(C_PARAM_X, "", nodes[2].signName, "", "");
			break;
		case 37: // expression => simple_expression
			node.signName = nodes[0].signName;
			break;
		case 38: // expression => simple_expression relop simple_expression
			var = nextTmpVar();
			gencode(C_X_ASS_Y_OP_Z, var, nodes[0].signName, nodes[1].signName, nodes[2].signName);
			node.signName = var;
			break;
		case 39: // simple_expression => term
			node.signName = nodes[0].signName;
			break;
		case 40: // simple_expression => sign term
			var = nextTmpVar();
			gencode(C_X_ASS_OP_Y, var, nodes[1].signName, nodes[0].signName, "");
			node.signName = var;
			break;
		case 41: // simple_expression => simple_expression addop term
			var = nextTmpVar();
			gencode(C_X_ASS_Y_OP_Z, var, nodes[0].signName, nodes[1].signName, nodes[2].signName);
			node.signName = var;
			break;
		case 42: // term => factor
			node.signName = nodes[0].signName;
			break;
		case 43: // term => term mulop factor
			var = nextTmpVar();
			gencode(C_X_ASS_Y_OP_Z, var, nodes[0].signName, nodes[1].signName, nodes[2].signName);
			node.signName = var;
			break;
		case 44: // factor => T_IDN
			node.signName = nodes[0].signName;
			break;
		case 45: // factor => T_IDN T_LPAR expression_list T_RPAR
			gencode(C_CALL_P_N, "", nodes[0].signName, "", intToString(nodes[2].expressionNum));
			node.signName = string("F");
			break;
		case 46: // factor => num
			node.signName = nodes[0].signName;
			break;
		case 47: // factor => T_LPAR expression T_RPAR
			node.signName = nodes[1].signName;
			break;
		case 48: // factor => T_NOT factor
			var = nextTmpVar();
			gencode(C_X_ASS_OP_Y, var, nodes[1].signName, "not", "");
			node.signName = var;
			break;
		case 49: // sign => T_ADD
			node.signName = "add";
			break;
		case 50: // sign => T_SUB
			node.signName = "minus";
			break;
		case 51: // relop => T_EQL
			node.signName = "=";
			break;
		case 52: // relop => T_NEQ
			node.signName = "<>";
			break;
		case 53: // relop => T_GT
			node.signName = ">";
			break;
		case 54: // relop => T_GTE
			node.signName = ">=";
			break;
		case 55: // relop => T_LT
			node.signName = "<";
			break;
		case 56: // relop => T_LTE
			node.signName = "<=";
			break;
		case 57: // addop => T_ADD
			node.signName = "+";
			break;
		case 58: // addop => T_SUB
			node.signName = "-";
			break;
		case 59: // mulop => T_MUL
			node.signName = "*";
			break;
		case 60: // mulop => T_SLASH
			node.signName = "/";
			break;
		case 61: // num => T_INT
			node.signName = nodes[0].signName;
			break;
		case 62: // num => T_REAL
			node.signName = nodes[0].signName;
			break;
		case 63: // factor => T_IDN T_LBRKPAR expression T_RBRKPAR
			var = nextTmpVar();
			gencode(C_X_ASS_Y_ARR_Z, var, nodes[0].signName, "", nodes[2].signName);
			node.signName = var;
			break;
		}
		return node;
	}

	void printSignTable()
	{
		FILE *fp = fopen("signtable_result.txt", "w");

		queue<signtable*> q;
		q.push(nowSignTable);
		
		while (!q.empty())
		{
			signtable* st = q.front(); q.pop();
			fprintf(fp, "Table %d, \n\tfather table: ", st->number);
			if (st->fatherTable == NULL)
				fprintf(fp, "NULL");
			else
				fprintf(fp, "%d", st->fatherTable->number);
			fprintf(fp, "\n");

			for (vector<signtable::sign>::iterator iter = st->signs.begin(); 
				iter != st->signs.end(); iter++)
			{
				fprintf(fp, "Name: %s, Offset: %d, Type: ", iter->name.c_str(), iter->offset);
				switch (iter->type)
				{
				case 0:
					fprintf(fp, "integer");
					break;
				case 1:
					fprintf(fp, "real");
					break;
				case 2:
					fprintf(fp, "signTable");
					q.push(iter->table);
					break;
				}
				fprintf(fp, "\n");
			}
			fprintf(fp, "\n");
		}
	}

	void printcode()
	{
		FILE *fp = fopen("translater_result.txt", "w");

		for (int i = 0; i <= (int)commands.size(); i++)
		{
			fprintf(fp, "%3d: ", i);
			if (i < (int)commands.size())
			{
				Command cmd = commands[i];
				switch (cmd.type)
				{
				case C_X_ASS_Y_OP_Z:
					fprintf(fp, "%s := %s %s %s", cmd.result.c_str(), cmd.arg1.c_str(), cmd.op.c_str(), cmd.arg2.c_str());
					break;
				case C_X_ASS_OP_Y:
					fprintf(fp, "%s := %s %s", cmd.result.c_str(), cmd.op.c_str(), cmd.arg1.c_str());
					break;
				case C_X_ASS_Y:
					fprintf(fp, "%s := %s", cmd.result.c_str(), cmd.arg1.c_str());
					break;
				case C_GOTO_X:
					fprintf(fp, "goto %s", cmd.result.c_str());
					break;
				case C_IF_X_GOTO:
					fprintf(fp, "if %s goto %s", cmd.arg1.c_str(), cmd.result.c_str());
					break;
				case C_IF_X_RELOP_Y_GOTO:
					fprintf(fp, "if %s %s %s goto %s", cmd.arg1.c_str(), cmd.op.c_str(), cmd.arg2.c_str(), cmd.result.c_str());
					break;
				case C_PARAM_X:
					fprintf(fp, "param %s", cmd.arg1.c_str());
					break;
				case C_CALL_P_N:
					fprintf(fp, "call %s, %s", cmd.arg1.c_str(), cmd.arg2.c_str());
					break;
				case C_RET:
					fprintf(fp, "ret\n");
					break;
				case C_TITLE:
					fprintf(fp, "%s%s%s%s", cmd.result.c_str(), cmd.arg1.c_str(), cmd.op.c_str(), cmd.arg2.c_str());
					break;
				case C_X_ASS_Y_ARR_Z:
					fprintf(fp, "%s := %s[%s]", cmd.result.c_str(), cmd.arg1.c_str(), cmd.arg2.c_str());
					break;
				}
			}
			fprintf(fp, "\n");
		}
		fclose(fp);
	}
};
