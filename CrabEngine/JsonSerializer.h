#pragma once
#include "json/single_include/nlohmann/json.hpp"

namespace crab
{
	using Json = nlohmann::json;

	class JsonSerializerBase
	{
	public:
		virtual bool	LoadJsonFromFile(std::filesystem::path in_path);
		virtual bool	SaveJsonToFile(std::filesystem::path in_path);
		Json&			GetJson() { return m_json; }
		bool			IsJsonLoaded() const { return !m_json.is_null(); }

	private:
		bool			_handle_error_(std::filesystem::path& in_path);

	protected:
		Json m_json;
	};

	template <typename DataType>
	class JsonSerializer : public JsonSerializerBase
	{
	public:
		DataType LoadFromJson()
		{
			return m_json.get<DataType>();
		}

		void SaveToJson(const DataType& in_data)
		{
			m_json = in_data;
		}

		operator DataType()
		{
			return LoadFromJson();
		}

		JsonSerializer<DataType>& operator=(const DataType& in_data)
		{
			SaveToJson(in_data);
			return *this;
		}
	};
}
