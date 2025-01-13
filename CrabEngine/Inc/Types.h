#pragma once

using int8 = __int8;
using int16 = __int16;
using int32 = __int32;
using int64 = __int64;

using uint8 = unsigned __int8;
using uint16 = unsigned __int16;
using uint32 = unsigned __int32;
using uint64 = unsigned __int64;

constexpr uint8		UINT8_LIMIT  = -1;
constexpr uint16	UINT16_LIMIT = -1;
constexpr uint32	UINT32_LIMIT = -1;
constexpr uint64	UINT64_LIMIT = -1;

using Vec2            = DirectX::SimpleMath::Vector2;
using Vec3            = DirectX::SimpleMath::Vector3;
using Vec4            = DirectX::SimpleMath::Vector4;
using Mat             = DirectX::SimpleMath::Matrix;
using Quat            = DirectX::SimpleMath::Quaternion;
using AABB            = DirectX::BoundingBox; // AABB
using OBB             = DirectX::BoundingOrientedBox; // AABB

template <typename Ty>
using ComPtr = Microsoft::WRL::ComPtr<Ty>;
using HR = HRESULT;

struct Vec2Int
{
	Vec2Int() : x(0), y(0) {}
	Vec2Int(int32 x_, int32 y_) : x(x_), y(y_) {}
	Vec2Int operator+(const Vec2Int& other) const { return Vec2Int(x + other.x, y + other.y); }
	Vec2Int operator-(const Vec2Int& other) const { return Vec2Int(x - other.x, y - other.y); }
	Vec2Int operator*(int32 scalar) const { return Vec2Int(x * scalar, y * scalar); }
	Vec2Int operator/(int32 scalar) const { return Vec2Int(x / scalar, y / scalar); }
	bool operator==(const Vec2Int& other) const { return x == other.x && y == other.y; }
	bool operator!=(const Vec2Int& other) const { return !(*this == other); }

	int32 x;
	int32 y;
};