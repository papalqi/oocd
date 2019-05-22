#include"OdRegex.h"

int oocd::Retest()
{
	regex reg1(R"(_*)");
	smatch r2;
	string s1("");
	regex_match(s1, r2, reg1);
	cout << r2.str();
	return 0;
}