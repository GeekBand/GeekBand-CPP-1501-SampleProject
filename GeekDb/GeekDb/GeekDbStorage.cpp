//*************************************************
// Module:  GeekDbStorage.cpp
// Notices: Copyright (c) 2015 Zhang Wenjie
//*************************************************

#include "stdafx.h"
#include "GeekDbStorage.h"

namespace geek
{
	GeekResult GeekDbStorage::CreateStorage()
	{
		GeekResult	result				= GEEK_SUCCESS;
		HRESULT		hr					= S_OK;
		IStorage*	pstgGeekPackage		= NULL;
		IStorage*	pstgGeekMetadata	= NULL;
		IStream*	pstm				= NULL;

		// Create the root storage
		hr = ::StgCreateStorageEx(
			this->m_wszFileName.c_str(),
			STGM_CREATE | STGM_SHARE_EXCLUSIVE | STGM_READWRITE,
			STGFMT_STORAGE,
			0, 
			NULL, 
			NULL,
			IID_IStorage,
			reinterpret_cast<void**>(&this->m_pstg));
		ON_FAILED_RETURN(hr, result, GEEK_ERROR_CREATESTGFAILED);
		
		// Create the "GeekMetadata" storage.
		this->m_pstg->CreateStorage(
			s_wszStorageNameGeekMetadata,
			STGM_READWRITE | STGM_SHARE_EXCLUSIVE,
			0, 0, &pstgGeekMetadata);
		ON_FAILED_RETURN(hr, result, GEEK_ERROR_CREATESTGFAILED);

		// Create the "GeekDatabasePackage" storage.
		this->m_pstg->CreateStorage(
			s_wszStorageNameGeekPackage,
			STGM_READWRITE | STGM_SHARE_EXCLUSIVE,
			0, 0, &pstgGeekPackage);
		ON_FAILED_RETURN(hr, result, GEEK_ERROR_CREATESTGFAILED);

		// Add metadata to the "GeekMetadata" storage.
		hr = pstgGeekMetadata->CreateStream(
			s_wszStreamNameDatabaseName,
			STGM_CREATE | STGM_READWRITE | STGM_SHARE_EXCLUSIVE,
			0,
			0,
			&pstm);
		ON_FAILED_RETURN(hr, result, GEEK_ERROR_CREATESTMFAILED);

		// Write metadata to the stream.
		result = this->WriteStringEntry(pstm, m_Metadata.wszName);
		ON_GEEKFAILED_RETURN(result);

		// Commit all changes.
		hr = m_pstg->Commit(STGC_DEFAULT);
		ON_FAILED_RETURN(hr, result, GEEK_ERROR_COMMITSTGFAILED);

	e_Exit:
		RELEASE_OLE_OBJECT(pstgGeekPackage);
		RELEASE_OLE_OBJECT(pstgGeekMetadata);
		RELEASE_OLE_OBJECT(pstm);
		return result;
	}
	
	GeekResult GeekDbStorage::OpenStorage()
	{
		GeekResult result = GEEK_SUCCESS;
		
		// Opens an existing storage file. 
		HRESULT hr = StgOpenStorageEx(
			this->m_wszFileName.c_str(),
			STGM_SHARE_EXCLUSIVE | STGM_READWRITE,
			STGFMT_STORAGE,
			0, 
			NULL, 
			NULL,
			IID_IStorage,
			reinterpret_cast<void**>(&this->m_pstg));
		if (FAILED(hr))
			result = GEEK_ERROR_OPENSTGFAILED;

		return result;
	}

	GeekResult GeekDbStorage::WriteStringEntry(
		INPARAM IStream* pstm,
		INPARAM const std::wstring& wsz)
	{
		GeekResult result = GEEK_SUCCESS;
		if (pstm == NULL || IsNullOrEmpty(wsz))
			return GEEK_ERROR_INVALIDPARAM;

		const wchar_t* wszString = wsz.c_str();

		// write the length of the wszString.
		DWORD cbString = (DWORD)::wcslen(wszString) * sizeof(wchar_t);
		result = this->WriteExact(pstm, &cbString, sizeof(DWORD), NULL);
		
		// write the content of the wszString.
		result = this->WriteExact(pstm, wszString, cbString, NULL);
		
		return result;
	}

	//
	// Attention: ppwz is allocated memory 
	// and must be released out of this function.
	//
	GeekResult GeekDbStorage::ReadStringEntry(
		INPARAM  IStream*	pstm,
		OUTPARAM WCHAR**	ppwz,
		OUTPARAM DWORD*		pcch)
	{
		HRESULT hr = S_OK;
		DWORD   dwLen = 0;
		DWORD   cch = 0;
		WCHAR*	wsz = NULL;
		GeekResult result = GEEK_SUCCESS;

		if (pstm == NULL || ppwz == NULL)
			return GEEK_ERROR_NULLPOINTER;

		*ppwz = NULL;
		
		// Read the length of the string.
		result = this->ReadExact(pstm, &dwLen, sizeof(DWORD), NULL);
		
		cch = dwLen / sizeof(WCHAR);
		wsz = new WCHAR[cch + 1];
		if (wsz == NULL)
			return GEEK_ERROR_OUTOFMEMORY;
		
		// Read the contents of the string.
		wsz[cch] = L'\0';
		hr = ReadExact(pstm, wsz, dwLen, NULL);

		// Assign the output parameters.
		*ppwz = wsz;
		*pcch = cch;

		wsz = NULL;
		return GEEK_SUCCESS;
	}

	GeekResult GeekDbStorage::Add(INPARAM const GeekKeyValue& entry)
	{
		if (m_pstg == NULL)
			return GEEK_ERROR_NULLPOINTER;

		HRESULT		hr				= S_OK;
		GeekResult	result			= GEEK_SUCCESS;
		IStorage*	pstgGeekPackage = NULL;
		IStream*	pstm			= NULL;

		// Locate the "GeekDatabasePackage" storage.
		hr = this->m_pstg->OpenStorage(
			s_wszStorageNameGeekPackage,
			NULL,
			STGM_READWRITE | STGM_SHARE_EXCLUSIVE,
			NULL, 0, &pstgGeekPackage);
		ON_FAILED_RETURN(hr, result, GEEK_ERROR_CREATESTGFAILED);

		// Create a stream for storing the key/value entry.
		const OLECHAR* wszName = entry.first.c_str();
		hr = pstgGeekPackage->CreateStream(
			wszName,
			STGM_CREATE | STGM_READWRITE | STGM_SHARE_EXCLUSIVE,
			0,
			0,
			&pstm);
		ON_FAILED_RETURN(hr, result, GEEK_ERROR_CREATESTMFAILED);

		result = this->WriteStringEntry(pstm, entry.second);
		ON_GEEKFAILED_RETURN(result);

	e_Exit:
		RELEASE_OLE_OBJECT(pstgGeekPackage);
		RELEASE_OLE_OBJECT(pstm);
		return result;
	}

	GeekResult GeekDbStorage::AddRange(INPARAM const std::vector<GeekKeyValue>& entries)
	{
		if (m_pstg == NULL)
			return GEEK_ERROR_NULLPOINTER;

		HRESULT		hr = S_OK;
		GeekResult	result = GEEK_SUCCESS;
		IStorage*	pstgGeekPackage = NULL;
		IStream*	pstm = NULL;
		std::vector<GeekKeyValue>::const_iterator it = entries.begin();

		// Locate the "GeekDatabasePackage" storage.
		hr = this->m_pstg->OpenStorage(
			s_wszStorageNameGeekPackage,
			NULL,
			STGM_READWRITE | STGM_SHARE_EXCLUSIVE,
			NULL, 0, &pstgGeekPackage);
		ON_FAILED_RETURN(hr, result, GEEK_ERROR_CREATESTGFAILED);
		
		while (it != entries.end())
		{
			const OLECHAR* wszName = (*it).first.c_str();
			hr = pstgGeekPackage->CreateStream(
				wszName,
				STGM_CREATE | STGM_READWRITE | STGM_SHARE_EXCLUSIVE,
				0,
				0,
				&pstm);
			ON_FAILED_RETURN(hr, result, GEEK_ERROR_CREATESTMFAILED);

			result = this->WriteStringEntry(pstm, (*it).second);
			ON_GEEKFAILED_RETURN(result);

			it++;
		}

		// Commit all changes.
		hr = pstgGeekPackage->Commit(STGC_DEFAULT);
		ON_FAILED_RETURN(hr, result, GEEK_ERROR_COMMITSTGFAILED);

	e_Exit:
		RELEASE_OLE_OBJECT(pstgGeekPackage);
		RELEASE_OLE_OBJECT(pstm);
		return result;
	}

	GeekResult GeekDbStorage::LoadAll(
		OUTPARAM GeekDbMetadata& metadata,
		OUTPARAM std::vector<GeekKeyValue>& entries)
	{
		if (m_pstg == NULL)
			return GEEK_ERROR_NULLPOINTER;

		HRESULT			hr					= S_OK;
		GeekResult		result				= GEEK_SUCCESS;
		ULONG			ulFetched			= 0;
		const ULONG		ulCelt				= 1UL;
		IEnumSTATSTG*	pEnumStat			= NULL;
		IStorage*		pstgGeekMetadata	= NULL;
		IStorage*		pstgGeekPackage		= NULL;
		IStream*		pstm				= NULL;
		PWSTR			pwszValue			= NULL;
		PWSTR			pwszName			= NULL;
		DWORD			dwRead = 0;
		STATSTG			stg;
		
		// Locate the "GeekMetadata" storage.
		hr = this->m_pstg->OpenStorage(
			s_wszStorageNameGeekMetadata,
			NULL,
			STGM_READ | STGM_SHARE_EXCLUSIVE,
			NULL,
			0,
			&pstgGeekMetadata);
		ON_FAILED_RETURN(hr, result, GEEK_ERROR_OPENSTGFAILED);

		// Get the metadata info.
		hr = pstgGeekMetadata->OpenStream(
			s_wszStreamNameDatabaseName,
			NULL,
			STGM_READ | STGM_SHARE_EXCLUSIVE,
			0,
			&pstm);
		ON_FAILED_RETURN(hr, result, GEEK_ERROR_OPENSTMFAILED);
		
		result = this->ReadStringEntry(pstm, &pwszName, &dwRead);
		ON_GEEKFAILED_RETURN(result);
		metadata.wszName = std::wstring(pwszName);

		// Locate the "GeekDatabasePackage" storage.
		hr = this->m_pstg->OpenStorage(
			s_wszStorageNameGeekPackage,
			NULL,
			STGM_READ | STGM_SHARE_EXCLUSIVE,
			NULL, 
			0, 
			&pstgGeekPackage);
		ON_FAILED_RETURN(hr, result, GEEK_ERROR_OPENSTGFAILED);

		hr = pstgGeekPackage->EnumElements(0, NULL, 0, &pEnumStat);
		ON_FAILED_RETURN(hr, result, GEEK_ERROR_ENUMSTGELEMENTSFAILED);

		hr = pEnumStat->Next(ulCelt, &stg, &ulFetched);
		ON_FAILED_RETURN(hr, result, GEEK_ERROR_ENUMSTGNEXTFAILED);

		while (hr == S_OK)
		{
			// Get stream name.
			std::wstring wszKey = stg.pwcsName;
			
			// Read stream contents.
			hr = pstgGeekPackage->OpenStream(
				stg.pwcsName, 
				NULL, 
				STGM_READ | STGM_SHARE_EXCLUSIVE, 
				0,
				&pstm);
			ON_FAILED_RETURN(hr, result, GEEK_ERROR_OPENSTMFAILED);

			DWORD dwRead;
			result = this->ReadStringEntry(pstm, &pwszValue, &dwRead);
			if (GEEKSUCCEEDED(result))
			{
				std::wstring wszValue(pwszValue);
				entries.push_back(GeekKeyValue(wszKey, wszValue));

				// Attention: array must be released here.
				DISPOSE_ARRAY(pwszValue);
			}

			hr = pEnumStat->Next(ulCelt, &stg, &ulFetched);
			ON_FAILED_RETURN(hr, result, GEEK_ERROR_ENUMSTGNEXTFAILED);
		}
		
	e_Exit:
		RELEASE_OLE_OBJECT(pstgGeekPackage);
		RELEASE_OLE_OBJECT(pstgGeekMetadata);
		RELEASE_OLE_OBJECT(pEnumStat);
		RELEASE_OLE_OBJECT(pstm);
		DISPOSE_ARRAY(pwszName);
		DISPOSE_ARRAY(pwszValue);
		return result;
	}
}