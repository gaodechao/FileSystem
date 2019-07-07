#pragma once
#include <iostream>
#include<time.h>
#include<fstream>
using namespace std;
class Log
{
public:
	Log();
	void write(string content);
	void read(string content);
	~Log();
};

