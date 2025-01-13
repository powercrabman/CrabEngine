#pragma once

namespace crab
{
	struct BatchVertex
	{
		Vec3	position;
		Vec2	texCoord;
		Vec4	color;
		uint32	entityID;
	};

	struct BatchData
	{
		BatchData();
		enum { BUFFER_CAPACITY = 30000 };
		
		std::vector<BatchVertex>	vertices;
		std::vector<uint32>			indices;

		// internal
		Ref<IDynamicVertexBuffer>	vb = nullptr;
		Ref<IDynamicIndexBuffer>	ib = nullptr;
	};

	//===================================================

	class Draw2D
	{
	public:
		static void Init();
		
		static void BeginBatch();
		static void EndBatch();
		static void Flush();

		static void AddRect(const Mat& in_world, const Vec2& in_size, const Vec4& in_color, float in_thickness);
		static void AddRect(const Mat& in_world, const Vec2& in_size, const Vec4& in_color, float in_thickness, uint32 in_entityID);
		static void AddRectFill(const Mat& in_world, const Vec2& in_size, const Vec4& in_color);
		static void AddRectFill(const Mat& in_world, const Vec2& in_size, const Vec4& in_color, uint32 in_entityID);

		static void AddCircle(const Mat& in_world, float radius, const Vec4& in_color, float in_thickness);
		static void AddCircle(const Mat& in_world, float radius, const Vec4& in_color, float in_thickness, uint32 in_entityID);
		static void AddCircleFill(const Mat& in_world, float radius, const Vec4& in_color);
		static void AddCircleFill(const Mat& in_world, float radius, const Vec4& in_color, uint32 in_entityID);

		static void AddTriangle(const Mat& in_world, const Vec2& in_point1, const Vec2& in_point2, const Vec2& in_point3, const Vec4& in_color, float in_thickness);
		static void AddTriangle(const Mat& in_world, const Vec2& in_point1, const Vec2& in_point2, const Vec2& in_point3, const Vec4& in_color, float in_thickness, uint32 in_entityID);
		static void AddTriangleFill(const Mat& in_world, const Vec2& in_point1, const Vec2& in_point2, const Vec2& in_point3, const Vec4& in_color);
		static void AddTriangleFill(const Mat& in_world, const Vec2& in_point1, const Vec2& in_point2, const Vec2& in_point3, const Vec4& in_color, uint32 in_entityID);

		static void AddLine(const Vec2& in_point1, const Vec2& in_point2, const Vec4& in_color, float in_thickness);
		static void AddLine(const Vec2& in_point1, const Vec2& in_point2, const Vec4& in_color, float in_thickness, uint32 in_entityID);

		static void AddSprite(const Mat& in_world, const Vec2& in_size, const Vec4& in_color, const Ref<ITexture>& in_tex);
		static void AddSprite(const Mat& in_world, const Vec2& in_size, const Vec4& in_color, const Ref<ITexture>& in_tex, const Vec2& in_uv0, const Vec2& in_uv1);
		static void AddSprite(const Mat& in_world, const Vec2& in_size, const Vec4& in_color, const Ref<ITexture>& in_tex, uint32 in_entityID);
		static void AddSprite(const Mat& in_world, const Vec2& in_size, const Vec4& in_color, const Ref<ITexture>& in_tex, const Vec2& in_uv0, const Vec2& in_uv1, uint32 in_entityID);

	private:
		static void _draw_sprites_();
		static void _draw_triangles_();

		struct Batches
		{
			BatchData		triBatch;
			BatchData		spriteBatch;

			Ref<ITexture>	currentTexture;
			uint32			spriteTextureSlot = 0;

			bool			m_beginBatch   = false;

			Ref<IShader>	shader;
		};

		inline static Scope<Batches> m_batches;
	};
}
