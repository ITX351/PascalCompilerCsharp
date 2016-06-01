#include <cstdio>
#include <cstring>
#include <iostream>
#include "wordscanner.cpp"
#include "parser.cpp"
using namespace std;

//Love lemon_TsyD
//Miss lydrainbowcat
//By ITX351

int main()
{
	Scanner scanner;
	scanner.init();
	scanner.scan();
	scanner.close();

	Parser parser(scanner.result);
	parser.init();
	int ret = parser.parse();
	if (ret < 0)
	{
		printf("SUCCESSFULLY ACCEPTED.\n");
		parser.printSignTable();
		parser.printcode();
	}
	else
		printf("ERROR ON LINE %d, %s.\n", ret, scanner.result[ret].second.c_str());

	return 0;
}
