#pragma once
#include "json/single_include/nlohmann/json.hpp"

namespace crab
{
	using Json = nlohmann::json;

	class JsonSerializerBase
	{
	public:
		bool	LoadJsonFromFile(std::filesystem::path in_path);
		bool	SaveJsonToFile(std::filesystem::path in_path);

		Json&	GetJson() { return m_json; }

	private:
		bool	_handle_error_(std::filesystem::path& in_path);

	protected:
		Json m_json;
	};

	template <typename DataType>
	class JsonSerializer : public JsonSerializerBase
	{
	public:
		DataType FromJson()
		{
			return m_json.get<DataType>();
		}

		void ToJson(const DataType& in_data)
		{
			m_json = in_data;
		}

		operator DataType()
		{
			return FromJson();
		}

		JsonSerializer<DataType>& operator=(const DataType& in_data)
		{
			ToJson(in_data);
			return *this;
		}
	};
}
