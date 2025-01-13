#pragma once

namespace crab
{
	template <typename Ty>
	class TypeIDGen
	{
	public:
		static uint64 Get()
		{
			static const int id;
			return reinterpret_cast<uint64>(&id);
		}
	};

	struct TypeID
	{
		bool operator==(const TypeID in_id) const { return id == in_id.id; }
		bool operator!=(const TypeID in_id) const { return id != in_id.id; }

		template <typename Ty>
		static TypeID Generate() { return TypeID{ TypeIDGen<Ty>::Get() }; }

		uint64 id;
	};
}

template <>
struct std::hash<crab::TypeID>
{
	uint64 operator()(const crab::TypeID in_id) const noexcept
	{
		return std::hash<uint64>{}(in_id.id);
	}
};
