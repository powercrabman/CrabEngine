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

	inline auto stoi(const std::string& in_string)
	{
		for (char c : in_string) 
		{
			if (!std::isdigit(c)) return std::make_pair(false, -1);
		}

		return std::make_pair(true, std::stoi(in_string));
	}
}