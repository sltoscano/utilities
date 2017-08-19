#include "StdAfx.h"
#include "StrSafe.h"

#include "helpers.h"


namespace Utils
{
	void GetTruncatedMessage(const TCHAR* source, const size_t maxLen, CString& dest)
	{
		size_t len = 0;
		StringCbLength(source, MAX_PATH * sizeof(TCHAR), &len);
		len /= sizeof(TCHAR);

		dest.Empty();
		if (len > maxLen)
		{
			dest.Append(_T(".."));
			dest.Append((source + len) - maxLen);
		}
		else
		{
			dest.Append(source);
		}
	}

	int GetPercent(const __int64 amount, const __int64 total)
	{
		double percent = total ? static_cast<double>(amount) / static_cast<double>(total) : 0;
		return static_cast<int>(percent * 100);
	}

	void ConvertToMagnitude(const __int64 rawValue, Magnitude& magnitude)
	{
		if (rawValue < 1024)
		{
			magnitude.unit = _T("bytes");
			magnitude.value = rawValue;
		}
		else if (rawValue < 1048576)
		{
			magnitude.unit = _T("KB");
			magnitude.value = rawValue >> 10;
		}
		else if (rawValue < 1073741824)
		{
			magnitude.unit = _T("MB");
			magnitude.value = rawValue >> 20;
		}
		else
		{
			magnitude.unit = _T("GB");
			magnitude.value = rawValue >> 30;
		}
	}
}
