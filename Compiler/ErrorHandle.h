#ifndef ERRORHANDLE_H
#define ERRORHANDLE_H

#include <iostream>
#include <string>
using namespace std;

class ErrorHandle
{
public:
	ErrorHandle(void);
	static void Error(int no, int linecount, string line, int chno);
	static void Fatal(int no);
	static bool getHaveError();
	static void setHaveError();
	static bool haveError;
};

#endif //ERRORHANDLE_H