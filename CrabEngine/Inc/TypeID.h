#pragma once

namespace crab
{
	using TypeID = uint64;

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
	
	template <typename Ty>
	constexpr TypeID GenerateTypeID()
	{
		return TypeIDGen<Ty>::Get();
	}

}
