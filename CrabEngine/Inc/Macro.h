#pragma once

#define DX_ASSERT(hr, msg, ...) ASSERT(SUCCEEDED(hr), msg, __VA_ARGS__)

#define CRAB_INTERFACE		class
#define FORCE_INLINE	__forceinline
#define NODISCARD		[[nodiscard]]

#define CARB_API

#define BIT(x) (1 << x)

#define IS_SAME_TYPE(Type1, Type2)			std::is_same<Type1, Type2>::value 
#define IS_BASE_OF(BaseType, DerivedType)	std::is_base_of<BaseType, DerivedType>::value
