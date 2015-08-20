//*************************************************
// Module:  GeekDbManager.h
// Notices: Copyright (c) 2015 Zhang Wenjie
//*************************************************
#pragma once

#include "GeekDb.h"
#include "GeekSequenceDb.h"
#include "GeekUtils.h"
#include "GeekErrorCode.h"
#include <string>
#include <map>
#include <algorithm>

namespace geek
{
	//
	// Manages all geek database created in memory.
	//
	class GeekDbManager
	{
	public:
		GeekDbManager(void) { }
		~GeekDbManager(void) { }

	public:
		template <typename TConcrete>
		GeekResult CreateDatabase(
			INPARAM const std::wstring& wszName
		);

		//
		// Load database into memory from file system.
		// 
		template <typename TConcrete>
		GeekResult LoadDatabase(
			INPARAM const std::wstring& wszFileName,
			OUTPARAM std::wstring& wszName
		);

		//
		// Get the size the database by the given name.
		//
		const size_t GetDatabaseSize(
			INPARAM const std::wstring& wszName
		);

		GeekResult InsertDatabase(
			INPARAM const std::wstring& wszName,
			INPARAM const GeekKeyValue& entry
		);

		//
		// Batch insert
		//
		GeekResult InsertDatabase(
			INPARAM const std::wstring& wszName, 
			INPARAM const std::vector<GeekKeyValue>& entries
		);
		
		GeekResult UpdateDatabase(
			INPARAM const std::wstring& wszName,
			INPARAM const GeekKeyValue& entry
		);

		GeekResult DeleteDatabase(
			INPARAM const std::wstring& wszName,
			INPARAM const std::wstring& wszKey
		);

		GeekResult DisposeDatabase(INPARAM const std::wstring& wszName);

		GeekResult DumpDatabase(
			INPARAM const std::wstring& wszName,
			INPARAM const std::wstring& wszFileName
		);

		GeekResult TraverseDatabase(INPARAM const std::wstring& wszName);

	private:
		inline GeekDb* GetDatabaseByName(const std::wstring& wszName)
		{
			CollectionIterator it = m_DbCollection.find(wszName);
			if (it == m_DbCollection.end())
				return NULL;

			return (*it).second;
		}

	private:
		typedef std::map<std::wstring, GeekDb*>::iterator CollectionIterator;
		std::map<std::wstring, GeekDb*> m_DbCollection;
	};

	template <typename TConcrete>
	GeekResult GeekDbManager::CreateDatabase(
		INPARAM const std::wstring& wszName)
	{
		typedef typename GeekDbTraits<TConcrete>::Database Database;
		GeekDbMetadata metadata;
		metadata.wszName = wszName;

		Database* pdb = new Database(metadata);
		if (pdb == NULL)
			return GEEK_ERROR_OUTOFMEMORY;

		m_DbCollection.insert(std::make_pair(wszName, pdb));
		return GEEK_SUCCESS;
	}

	template <typename TConcrete>
	GeekResult GeekDbManager::LoadDatabase(
		INPARAM const std::wstring & wszFileName,
		OUTPARAM std::wstring& wszName)
	{
		if (geek::IsNullOrEmpty(wszFileName))
			return GEEK_ERROR_INVALIDPARAM;

		if (!geek::IsFileExists(wszFileName))
			return GEEK_ERROR_FILENOTFOUND;

		typedef typename GeekDbTraits<TConcrete>::Database Database;
		Database* pdb = new Database(GeekDbMetadata());
		if (pdb == NULL)
			return GEEK_ERROR_OUTOFMEMORY;			
		
		GeekResult result = pdb->LoadKeyValues(wszFileName, wszName);

		// does the database name already exists ?
		CollectionIterator it = m_DbCollection.find(wszName);
		if (it != m_DbCollection.end())
		{
			// already exists, replace with the loaded db.
			m_DbCollection[wszName] = pdb;
		}
		else
		{
			// add the loaded db.
			m_DbCollection.insert(std::make_pair(wszName, pdb));
		}
		
		return result;
	}
}