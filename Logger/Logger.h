#pragma once

//Simple logger made by me LOL, DonSimonetti

#include <cstdio>
#include <cstdarg>
#include <iostream>
#include <fstream>
#include <sstream>
#include <ctime>

#define LOG_FILENAME "SampRandomPlugin.log"
#define LOG_TAG "[SRP]"

using namespace std;

namespace Logger
{
	static ofstream * logFile;

	void Debug(const char* str, ...);
	void Error(const char* str,...);
	void Print(stringstream & stream);
	void Close();
}