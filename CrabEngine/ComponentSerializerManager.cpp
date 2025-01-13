#include "CrabEnginePch.h"
#include "ComponentSerializerManager.h"


namespace crab
{
	void ComponentSerializerManager::Shutdown()
	{
		Destroy();
	}
}
