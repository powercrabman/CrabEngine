#include "CrabEnginePch.h"
#include "JsonSerializer.h"
#include <fstream>

namespace crab
{

	bool JsonSerializerBase::LoadJsonFromFile(std::filesystem::path in_path)
	{
		if (!_handle_error_(in_path))
		{
			return false;
		}

		std::ifstream fs(in_path);
		if (!fs.is_open())
		{
			assert(false);
			return false;
		}

		m_json = Json::parse(fs);
		return !m_json.is_discarded();
	}

	bool JsonSerializerBase::SaveJsonToFile(std::filesystem::path in_path)
	{
		if (!_handle_error_(in_path))
		{
			return false;
		}

		// 디렉토리가 없을 경우 만든다.
		std::filesystem::create_directories(in_path.parent_path());

		std::ofstream os(in_path);
		if (!os.is_open())
		{
			assert(false);
			return false;
		}

		os << m_json.dump(4);
		return true;
	}

	bool JsonSerializerBase::_handle_error_(std::filesystem::path& in_path)
	{
		std::filesystem::path ext = in_path.extension();
		if (ext.empty())
		{
			in_path += ".json";
		}
		else if (ext != ".json")
		{
			assert(false);
			Log::Error("Only files with the json extension can be read. {}", in_path.string());
			return false;
		}

		return true;
	}

}
