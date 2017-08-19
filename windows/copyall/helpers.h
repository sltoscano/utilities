#pragma once


namespace Utils
{
	struct Magnitude
	{
		__int64 value;
		const TCHAR* unit;
	};

	void GetTruncatedMessage(const TCHAR* source, const size_t maxLen, CString& dest);

	int GetPercent(const __int64 amount, const __int64 total);

	void ConvertToMagnitude(const __int64 rawValue, Magnitude& magnitude);
}
