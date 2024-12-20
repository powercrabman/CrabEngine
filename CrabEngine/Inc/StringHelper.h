#pragma once


namespace crab::str
{
	inline std::string CvtToString(const std::wstring& in_string)
	{
		return std::string(in_string.begin(), in_string.end());
	}

	inline std::wstring CvtToWideString(const std::string& in_string)
	{
		return std::wstring(in_string.begin(), in_string.end());
	}
}