//////////////////////////////////////////////////////////////////////////////////////
// 
// File: Tools.cpp
// Desc: implementation of common helper functins
// Author: Chris Miiler
// Date 6/12/o8
//
//////////////////////////////////////////////////////////////////////////////////////

#include "Global.h"

void logError(std::string msg)
{
	// open the file to write to it
	std::fstream errFile("debug.txt", std::ios::out | std::ios::app);

	// make sure the file is open
	if (errFile.is_open())
	{
		errFile << msg;
	}
}
