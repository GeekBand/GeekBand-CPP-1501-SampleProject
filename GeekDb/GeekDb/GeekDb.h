//*************************************************
// Module:  GeekDB.h
// Notices: Copyright (c) 2015 Zhang Wenjie
//*************************************************
#pragma once

#include "GeekDefs.h"
#include "GeekErrorCode.h"
#include <vector>

namespace geek
{
	//
	// Abstract class for Geek database.
	//
	class GeekDb
	{
	public:
		GeekDb(const GeekDbMetadata& metadata) 
			: m_Metadata(metadata)
		{
		}

	public:
		virtual GeekResult InsertKeyValue(INPARAM const GeekKeyValue& entry) = 0;
		virtual GeekResult UpdateKeyValue(INPARAM const GeekKeyValue& entry) = 0;
		virtual GeekResult DeleteKeyValue(INPARAM const std::wstring& wszKey) = 0;
		
		virtual GeekResult QueryKeyValue(
			INPARAM const std::wstring& wszKey, 
			OUTPARAM std::vector<GeekKeyValue>& entries
		) = 0;

		virtual GeekResult DumpKeyValues(INPARAM const std::wstring& wszFileName) = 0;

		virtual GeekResult LoadKeyValues(
			INPARAM const std::wstring& wszFileName, 
			OUTPARAM std::wstring& wszName
		) = 0;

		virtual void TraverseKeyVaues(void) = 0;

		virtual const std::size_t GetSize(void) const = 0;

	protected:
		GeekDbMetadata m_Metadata;

	private:
		GeekDb(void);
		GeekDb(const GeekDb&);
		GeekDb(const GeekDb*);
	};

	//
	// Traits for Geek database.
	// Traits is used for Creating concrete database implementation.
	//
	template<typename T> 
		struct GeekDbTraits
	{
	};
}