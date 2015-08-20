//*************************************************
// Module:  GeekDbManager.cpp
// Notices: Copyright (c) 2015 Zhang Wenjie
//*************************************************

#include "stdafx.h"
#include "GeekDbManager.h"
#include "GeekUtils.h"

namespace geek
{
	const size_t GeekDbManager::GetDatabaseSize(INPARAM const std::wstring & wszName)
	{
		if (geek::IsNullOrEmpty(wszName))
			return GEEK_ERROR_INVALIDPARAM;

		GeekDb* db = this->GetDatabaseByName(wszName);
		if (db == NULL)
			return GEEK_ERROR_NULLPOINTER;

		return (db->GetSize());
	}

	GeekResult GeekDbManager::InsertDatabase(
		INPARAM const std::wstring& wszName,
		INPARAM const GeekKeyValue& entry)
	{
		if (geek::IsNullOrEmpty(wszName))
			return GEEK_ERROR_INVALIDPARAM;

		GeekDb* db = this->GetDatabaseByName(wszName);
		if (db == NULL)
			return GEEK_ERROR_NULLPOINTER;

		GeekResult result = db->InsertKeyValue(entry);
		return result;
	}

	GeekResult GeekDbManager::InsertDatabase(
		INPARAM const std::wstring& wszName,
		INPARAM const std::vector<GeekKeyValue>& entries)
	{
		if (geek::IsNullOrEmpty(wszName))
			return GEEK_ERROR_INVALIDPARAM;

		if (entries.empty())
			return GEEK_ERROR_EMPTY;

		GeekDb* db = this->GetDatabaseByName(wszName);
		if (db == NULL)
			return GEEK_ERROR_NULLPOINTER;

		GeekResult result = GEEK_SUCCESS;
		for (std::vector<GeekKeyValue>::const_iterator it = entries.begin();
			it != entries.end(); it++)
		{
			GeekResult result = db->InsertKeyValue((*it));
		}

		return result;
	}

	GeekResult GeekDbManager::UpdateDatabase(
		INPARAM const std::wstring& wszName,
		INPARAM const GeekKeyValue& entry)
	{
		if (geek::IsNullOrEmpty(wszName))
			return GEEK_ERROR_INVALIDPARAM;

		GeekDb* db = this->GetDatabaseByName(wszName);
		if (db == NULL)
			return GEEK_ERROR_NULLPOINTER;

		GeekResult result = db->UpdateKeyValue(entry);
		return result;
	}

	GeekResult GeekDbManager::DeleteDatabase(
		INPARAM const std::wstring& wszName,
		INPARAM const std::wstring& wszKey)
	{
		if (geek::IsNullOrEmpty(wszName) || geek::IsNullOrEmpty(wszKey))
			return GEEK_ERROR_INVALIDPARAM;

		GeekDb* db = this->GetDatabaseByName(wszName);
		if (db == NULL)
			return GEEK_ERROR_NULLPOINTER;

		GeekResult result = db->DeleteKeyValue(wszKey);
		return result;
	}

	GeekResult GeekDbManager::DisposeDatabase(
		INPARAM const std::wstring& wszName)
	{
		if (geek::IsNullOrEmpty(wszName))
			return GEEK_ERROR_INVALIDPARAM;

		GeekDb* db = this->GetDatabaseByName(wszName);
		if (db == NULL)
			return GEEK_ERROR_NULLPOINTER;

		delete db;
		m_DbCollection.erase(wszName);
		
		return GEEK_SUCCESS;
	}

	GeekResult GeekDbManager::DumpDatabase(
		INPARAM const std::wstring& wszName,
		INPARAM const std::wstring& wszFileName)
	{
		if (geek::IsNullOrEmpty(wszName) || geek::IsNullOrEmpty(wszFileName))
			return GEEK_ERROR_INVALIDPARAM;

		GeekDb* db = this->GetDatabaseByName(wszName);
		if (db == NULL)
			return GEEK_ERROR_NULLPOINTER;

		GeekResult result = db->DumpKeyValues(wszFileName);
		return result;
	}

	GeekResult GeekDbManager::TraverseDatabase(INPARAM const std::wstring & wszName)
	{
		if (geek::IsNullOrEmpty(wszName))
			return GEEK_ERROR_INVALIDPARAM;

		GeekDb* db = this->GetDatabaseByName(wszName);
		if (db == NULL)
			return GEEK_ERROR_NULLPOINTER;

		db->TraverseKeyVaues();
		return GEEK_SUCCESS;
	}
}