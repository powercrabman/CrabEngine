#pragma once

#include <algorithm>
#include <array>
#include <format>
#include <cassert>
#include <chrono>
#include <filesystem>
#include <string>
#include <fstream>
#include <string_view>
#include <SDL/include/SDL.h>

#ifdef _DEBUG
#pragma comment(lib, "SDL2d.lib")
#pragma comment(lib, "SDL2maind.lib")
#else
#pragma comment(lib, "SDL2.lib")
#pragma comment(lib, "SDL2main.lib")
#endif

// DX
#include <d3d11.h>
#include <d3dcompiler.h>
#include <d3dcompiler.inl>
#include <wrl/client.h>

#include <DirectXTK/Inc/SimpleMath.h>
#include <DirectXTK/Inc/SimpleMath.inl>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "uuid.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "dxcompiler.lib")
#pragma comment(lib, R"(C:\Users\Ahnjiwoo\Desktop\Projects\DirectXTK\Bin\Desktop_2022\x64\Debug\DirectXTK.lib)")
#pragma comment(lib, R"(C:\Users\Ahnjiwoo\Desktop\Projects\DirectXTex\DirectXTex\Bin\Desktop_2022\x64\Debug\DirectXTex.lib)")
#pragma comment(lib, "windowsapp.lib")
#pragma comment(lib, "runtimeobject.lib")

#include <entt/single_include/entt/entt.hpp>

#include "Enums.h"
#include "StringHelper.h"
#include "Macro.h"
#include "Pointer.h"
#include "Types.h"
#include "TypeID.h"
#include "Log.h"
#include "Timer.h"
#include "EngineAssert.h"
#include "EventDispatcher.h"
#include "CrabMath.h"
#include "CrabColor.h"

#include "CrabEngine.h"

#include "RendererSingleInclude.h"

#include "ImGuiInclude.h"
#include "Input.h"

#include "SceneManager.h"

#include "AssetSingleInclude.h"

#include "JsonSerializer.h"
#include "SerializerHelper.h"

#include "Components.h"
#include "ComponentSerializer.h"

#include "RenderSystem.h"

#include "EntryPoint.h"
