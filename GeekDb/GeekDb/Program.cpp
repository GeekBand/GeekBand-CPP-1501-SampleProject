//******************************************************************
// Module:  GeekSequenceDb.cpp
//          Defines the entry point for the console application.
//
// Notices: Copyright (c) 2015 Zhang Wenjie
//******************************************************************

#include "stdafx.h"
#include "GeekDbManager.h"
#include <iostream>
using namespace geek;

// The global database manager.
geek::GeekDbManager g_DbManager;

void CreateSequenceDatabase(const std::wstring& wszDumpFileName)
{
	std::wstring wszFileName = wszDumpFileName;
	if (geek::IsNullOrEmpty(wszFileName))
		wszFileName = L"sequence01.geekdb";
		
	geek::GeekKeyValue gkv[] = {
		geek::GeekKeyValue(L"name", L"zhangwenjie"),
		geek::GeekKeyValue(L"age", L"30"),
		geek::GeekKeyValue(L"class", L"AABBCCC"),
		geek::GeekKeyValue(L"grade", L"900"),
		geek::GeekKeyValue(L"language", L"us-en"),
		geek::GeekKeyValue(L"sex", L"male"),
	};

	g_DbManager.CreateDatabase<geek::GeekSequenceDb>(L"Testdb01");
	g_DbManager.InsertDatabase(L"Testdb01", 
		std::vector<geek::GeekKeyValue>(gkv, gkv + 6));

	geek::GeekResult r = g_DbManager.DumpDatabase(L"Testdb01", 
		wszFileName);
}

int main()
{
	CreateSequenceDatabase(L"D:\\Boolan\\dump02.geekdb");
	
	std::wstring wszName;
	geek::GeekResult r = g_DbManager.LoadDatabase<geek::GeekSequenceDb>(
		L"D:\\Boolan\\dump02.geekdb", wszName);
	g_DbManager.TraverseDatabase(wszName);

    return 0;
}

