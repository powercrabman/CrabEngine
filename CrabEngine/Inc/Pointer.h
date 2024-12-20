#pragma once

#include <memory>

namespace crab
{
	template <typename Ty>
	using Scope = std::unique_ptr<Ty>;

	template <typename Ty, typename... Args>
	Scope<Ty> MakeScope(Args&& ...in_args)
	{
		return std::unique_ptr<Ty>(new Ty(std::forward<Args>(in_args)...));
	}

	template <typename Ty>
	using Ref = std::shared_ptr<Ty>;

	template <typename Ty, typename... Args>
	Ref<Ty> MakeRef(Args&& ...in_args)
	{
		return std::shared_ptr<Ty>(new Ty(std::forward<Args>(in_args)...));
	}
}
