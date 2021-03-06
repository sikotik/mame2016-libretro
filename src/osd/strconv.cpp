// license:BSD-3-Clause
// copyright-holders:Aaron Giles
//============================================================
//
//  strconv.c - Win32 string conversion
//
//============================================================

#if defined(SDLMAME_WIN32) || defined(OSD_WINDOWS)
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

// MAMEOS headers
#include "strconv.h"

#if defined(SDLMAME_WIN32) || defined(OSD_WINDOWS)
//============================================================
//  astring_from_utf8
//============================================================

CHAR *astring_from_utf8(const char *utf8string)
{
	WCHAR *wstring;
	int char_count;
	CHAR *result;

	// convert MAME string (UTF-8) to UTF-16
	char_count = MultiByteToWideChar(CP_UTF8, 0, utf8string, -1, nullptr, 0);
	wstring = (WCHAR *)alloca(char_count * sizeof(*wstring));
	MultiByteToWideChar(CP_UTF8, 0, utf8string, -1, wstring, char_count);

	// convert UTF-16 to "ANSI code page" string
	char_count = WideCharToMultiByte(CP_ACP, 0, wstring, -1, nullptr, 0, nullptr, nullptr);
	result = (CHAR *)osd_malloc_array(char_count * sizeof(*result));
	if (result != nullptr)
		WideCharToMultiByte(CP_ACP, 0, wstring, -1, result, char_count, nullptr, nullptr);

	return result;
}


//============================================================
//  utf8_from_astring
//============================================================

char *utf8_from_astring(const CHAR *astring)
{
	WCHAR *wstring;
	int char_count;
	CHAR *result;

	// convert "ANSI code page" string to UTF-16
	char_count = MultiByteToWideChar(CP_ACP, 0, astring, -1, nullptr, 0);
	wstring = (WCHAR *)alloca(char_count * sizeof(*wstring));
	MultiByteToWideChar(CP_ACP, 0, astring, -1, wstring, char_count);

	// convert UTF-16 to MAME string (UTF-8)
	char_count = WideCharToMultiByte(CP_UTF8, 0, wstring, -1, nullptr, 0, nullptr, nullptr);
	result = (CHAR *)osd_malloc_array(char_count * sizeof(*result));
	if (result != nullptr)
		WideCharToMultiByte(CP_UTF8, 0, wstring, -1, result, char_count, nullptr, nullptr);

	return result;
}


//============================================================
//  wstring_from_utf8
//============================================================

WCHAR *wstring_from_utf8(const char *utf8string)
{
	int char_count;
	WCHAR *result;

	// convert MAME string (UTF-8) to UTF-16
	char_count = MultiByteToWideChar(CP_UTF8, 0, utf8string, -1, nullptr, 0);
	result = (WCHAR *)osd_malloc_array(char_count * sizeof(*result));
	if (result != nullptr)
		MultiByteToWideChar(CP_UTF8, 0, utf8string, -1, result, char_count);

	return result;
}


//============================================================
//  utf8_from_wstring
//============================================================

char *utf8_from_wstring(const WCHAR *wstring)
{
	int char_count;
	char *result;

	// convert UTF-16 to MAME string (UTF-8)
	char_count = WideCharToMultiByte(CP_UTF8, 0, wstring, -1, nullptr, 0, nullptr, nullptr);
	result = (char *)osd_malloc_array(char_count * sizeof(*result));
	if (result != nullptr)
		WideCharToMultiByte(CP_UTF8, 0, wstring, -1, result, char_count, nullptr, nullptr);

	return result;
}

//============================================================
//  osd_uchar_from_osdchar
//============================================================

int osd_uchar_from_osdchar(UINT32 *uchar, const char *osdchar, size_t count)
{
	WCHAR wch;
	CPINFO cp;

	if (!GetCPInfo(CP_ACP, &cp))
		goto error;

	// The multibyte char can't be bigger than the max character size
	count = MIN(count, cp.MaxCharSize);

	if (MultiByteToWideChar(CP_ACP, 0, osdchar, static_cast<DWORD>(count), &wch, 1) == 0)
		goto error;

	*uchar = wch;
	return static_cast<int>(count);

error:
	*uchar = 0;
	return static_cast<int>(count);
}

#else
#if !defined(ANDROID) && !defined(__ANDROID__)
#include "unicode.h"
//============================================================
//  osd_uchar_from_osdchar
//============================================================

int osd_uchar_from_osdchar(unicode_char *uchar, const char *osdchar, size_t count)
{
	wchar_t wch;

	count = mbstowcs(&wch, (char *)osdchar, 1);
	if (count != -1)
		*uchar = wch;
	else
		*uchar = 0;

	return count;
}
#endif
#endif
