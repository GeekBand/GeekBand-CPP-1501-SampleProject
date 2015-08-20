//*************************************************
// Module:  GeekErrorCode.h
// Notices: Copyright (c) 2015 Zhang Wenjie
//*************************************************

#pragma once

namespace geek
{
	typedef long GeekResult;

	#define GEEK_SUCCESS		((GeekResult)0x22000000L)
	#define GEEK_ERROR			((GeekResult)0xE2000000L)

	#define GEEKFAILED(gr)		((gr) != GEEK_SUCCESS)
	#define GEEKSUCCEEDED(gr)	((gr) == GEEK_SUCCESS)

	#define GEEK_ERROR_OUTOFMEMORY		((GeekResult)(GEEK_ERROR + 1L))
	#define GEEK_ERROR_INVALIDPARAM		((GeekResult)(GEEK_ERROR + 2L))
	#define GEEK_ERROR_NULLPOINTER		((GeekResult)(GEEK_ERROR + 3L))
	#define GEEK_ERROR_EMPTY			((GeekResult)(GEEK_ERROR + 4L))
	#define GEEK_ERROR_NAMEALREADYEXIST	((GeekResult)(GEEK_ERROR + 8L))
	#define GEEK_ERROR_FILENOTFOUND		((GeekResult)(GEEK_ERROR + 9L))
	#define GEEK_ERROR_ENTRYNOTFOUND	((GeekResult)(GEEK_ERROR + 10L))
	

	#define GEEK_ERROR_CREATESTGFAILED			((GeekResult)(GEEK_ERROR + 51L))
	#define GEEK_ERROR_CREATESTMFAILED			((GeekResult)(GEEK_ERROR + 52L))
	#define GEEK_ERROR_OPENSTGFAILED			((GeekResult)(GEEK_ERROR + 53L))
	#define GEEK_ERROR_OPENSTMFAILED			((GeekResult)(GEEK_ERROR + 54L))
	#define GEEK_ERROR_READSTMFAILED			((GeekResult)(GEEK_ERROR + 55L))
	#define GEEK_ERROR_WRITESTMFAILED			((GeekResult)(GEEK_ERROR + 56L))
	#define GEEK_ERROR_ENUMSTGELEMENTSFAILED	((GeekResult)(GEEK_ERROR + 57L))
	#define GEEK_ERROR_ENUMSTGNEXTFAILED		((GeekResult)(GEEK_ERROR + 58L))
	#define GEEK_ERROR_COMMITSTGFAILED			((GeekResult)(GEEK_ERROR + 59L))
}