//*************************************************
// Module:  GeekDefs.h
// Notices: Copyright (c) 2015 Zhang Wenjie
//*************************************************

#pragma once

#include <string>
#include <utility>

namespace geek
{
	//
	// Defines the input parameter label.
	// This label indicates that a parameter is input to a function.
	//
	#ifndef INPARAM
	#define INPARAM
	#endif

	//
	// Defines the output parameter label.
	// This label indicates that a parameter is ouput from a function.
	//
	#ifndef OUTPARAM
	#define OUTPARAM
	#endif

	//
	// Defines the key/value pair used for the database entry.
	// Key: wstring
	// Value : wstring
	//
	typedef std::pair<std::wstring, std::wstring> GeekKeyValue;

	//
	// Defines the metadata stucture for the geek database.
	// Currently only the database name is added.
	//
	struct GeekDbMetadata
	{
	public:
		std::wstring wszName;	
	} ;
}