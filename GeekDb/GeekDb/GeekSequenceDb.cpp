//*************************************************
// Module:  GeekSequenceDb.cpp
// Notices: Copyright (c) 2015 Zhang Wenjie
//*************************************************

#include "stdafx.h"
#include "GeekSequenceDb.h"
#include "GeekUtils.h"
#include <algorithm>
#include <functional>

namespace geek
{
	GeekResult GeekSequenceDb::InsertKeyValue(INPARAM const GeekKeyValue& entry)
	{
		m_kvContainer.push_back(entry);
		return GEEK_SUCCESS;
	}

	GeekResult GeekSequenceDb::UpdateKeyValue(INPARAM const GeekKeyValue& entry)
	{
		KeyValueIterator it = std::find(m_kvContainer.begin(), m_kvContainer.end(), entry);
		if (it == m_kvContainer.end())
			return GEEK_ERROR_ENTRYNOTFOUND;

		(*it).first = entry.first;
		(*it).second = entry.second;
		return GEEK_SUCCESS;
	}

	GeekResult GeekSequenceDb::DeleteKeyValue(INPARAM const std::wstring& wszKey)
	{
		if (IsNullOrEmpty(wszKey))
			return GEEK_ERROR_INVALIDPARAM;

		GeekKeyValue entry;
		for(KeyValueIterator it = m_kvContainer.begin(); it != m_kvContainer.end(); it++)
		{
			if ((*it).first == wszKey)
			{	
				entry = (*it);
				break;
			}
		}

		m_kvContainer.erase(std::remove(m_kvContainer.begin(), m_kvContainer.end(), entry));
		return GEEK_SUCCESS;
	}

	GeekResult GeekSequenceDb::QueryKeyValue(
		INPARAM const std::wstring& wszKey,
		OUTPARAM std::vector<GeekKeyValue>& entries)
	{
		if (IsNullOrEmpty(wszKey))
			return GEEK_ERROR_INVALIDPARAM;

		for (KeyValueIterator it = m_kvContainer.begin(); it != m_kvContainer.end(); it++)
		{
			if ((*it).first == wszKey)
			{
				entries.push_back((*it));
			}
		}

		return GEEK_SUCCESS;
	}

	GeekResult GeekSequenceDb::DumpKeyValues(INPARAM const std::wstring & wszFileName)
	{
		if (IsNullOrEmpty(wszFileName))
			return GEEK_ERROR_INVALIDPARAM;

		GeekDbStorage* pstg = new GeekDbStorage(wszFileName, this->m_Metadata, true);
		if (pstg == NULL)
			return GEEK_ERROR_OUTOFMEMORY;

		GeekResult result = pstg->AddRange(m_kvContainer);
		DISPOSE_OBJECT(pstg);
		return result;
	}

	GeekResult GeekSequenceDb::LoadKeyValues(
		INPARAM const std::wstring & wszFileName,
		OUTPARAM std::wstring& wszName)
	{
		if (IsNullOrEmpty(wszFileName))
			return GEEK_ERROR_INVALIDPARAM;

		GeekDbStorage* pstg = new GeekDbStorage(wszFileName, m_Metadata, false);
		if (pstg == NULL)
			return GEEK_ERROR_OUTOFMEMORY;

		// Ensure the container's size and capacity is 0 by shrinking it!
		KeyValueContainer().swap(m_kvContainer);
		wszName.clear();

		GeekResult result = pstg->LoadAll(m_Metadata, m_kvContainer);
		wszName.assign(m_Metadata.wszName);
		DISPOSE_OBJECT(pstg);
		return result;
	}

	void GeekSequenceDb::TraverseKeyVaues(void)
	{
		std::for_each(m_kvContainer.begin(), m_kvContainer.end(),
			geek::PrintKeyValue());
	}

	const std::size_t GeekSequenceDb::GetSize(void) const
	{
		return m_kvContainer.size();
	}
}