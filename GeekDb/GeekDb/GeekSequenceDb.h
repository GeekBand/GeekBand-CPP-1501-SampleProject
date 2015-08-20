//*************************************************
// Module:  GeekSequenceDb.h
// Notices: Copyright (c) 2015 Zhang Wenjie
//*************************************************

#pragma once

#include "GeekDb.h"
#include "GeekDbStorage.h"
#include <vector>

namespace geek
{
	//
	// Geek database implementation using std::vector
	//
	class GeekSequenceDb : public GeekDb
	{
	public:
		GeekSequenceDb(const GeekDbMetadata& metadata)
			: GeekDb(metadata)
		{
		}

		~GeekSequenceDb(void)
		{
		}

	public:
		GeekResult InsertKeyValue(INPARAM const GeekKeyValue& entry);

		GeekResult UpdateKeyValue(INPARAM const GeekKeyValue& entry);

		GeekResult DeleteKeyValue(INPARAM const std::wstring& wszKey);

		GeekResult QueryKeyValue(
			INPARAM const std::wstring& wszKey,
			OUTPARAM std::vector<GeekKeyValue>& entries
		);

		GeekResult DumpKeyValues(INPARAM const std::wstring& wszFileName);

		GeekResult LoadKeyValues(
			INPARAM const std::wstring& wszFileName,
			OUTPARAM std::wstring& wszName
		);

		void TraverseKeyVaues(void);

		const std::size_t GetSize(void) const;
				
	private:
		typedef std::vector<GeekKeyValue> KeyValueContainer;
		typedef KeyValueContainer::iterator KeyValueIterator;
		KeyValueContainer m_kvContainer;
	};

	//
	// Traits returns the concrete database type.
	//
	template<>
	struct GeekDbTraits<GeekSequenceDb>
	{
	public:
		typedef GeekSequenceDb Database;
	};
}