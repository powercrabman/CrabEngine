#include "CrabEnginePch.h"
#include "Components.h"
#include "IRenderTarget.h"

namespace crab
{
	Transform Transform::s_identity = {
		Vec3::Zero,
		Vec3::Zero,
		Vec3::One
	};
}
