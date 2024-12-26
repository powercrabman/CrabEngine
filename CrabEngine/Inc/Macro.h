#pragma once

#define DX_ASSERT(hr, msg, ...) ASSERT(SUCCEEDED(hr), msg, __VA_ARGS__)

#define CRAB_INTERFACE		class
#define FORCE_INLINE	__forceinline
#define NO_DISCARD		[[nodiscard]]

#define CARB_API

#define BIND_FN_CALLBACK(object, method)         std::bind(&method, object)
#define BIND_FN_CALLBACK_ARG1(object, method)    std::bind(&method, object, std::placeholders::_1)
#define BIND_FN_CALLBACK_ARG2(object, method)    std::bind(&method, object, std::placeholders::_1, std::placeholders::_2)
#define BIND_FN_CALLBACK_ARG3(object, method)    std::bind(&method, object, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)

#define BIT(x) (1 << x)

#define ASSET_MANAGER		crab::AssetManager::Get()
#define CRAB_ENGINE			crab::CrabEngine::Get()
#define SCENE_MANAGER		crab::SceneManager::Get()
