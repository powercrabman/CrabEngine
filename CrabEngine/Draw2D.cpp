#include "CrabEnginePch.h"
#include "Draw2D.h"

namespace crab
{
	BatchData::BatchData()
	{
		vertices.reserve(BUFFER_CAPACITY);
		indices.reserve(BUFFER_CAPACITY);

		vb = IDynamicVertexBuffer::Create<BatchVertex>(BUFFER_CAPACITY);
		ib = IDynamicIndexBuffer::Create(BUFFER_CAPACITY);
	}

	//===================================================
	//		         Batch Renderer 2D
	//===================================================

	void Draw2D::Init()
	{
		m_batches = MakeScope<Batches>();

		// TODO: this code not apply in openGL
		m_batches->shader = IShader::CreateByFile(
			{
				LayoutElement{"POSITION", 0, eDataFormat::Float3},
				LayoutElement{"TEXCOORD", 0, eDataFormat::Float2},
				LayoutElement{"COLOR", 0, eDataFormat::Float4},
				LayoutElement{"ENTITY_ID", 0, eDataFormat::UInt32}
			},
			GetAssetManager().GetEngineDirectory("DXBatchRenderer2D.hlsl")
		);
	}

	void Draw2D::BeginBatch()
	{
		m_batches->spriteBatch.vertices.clear();
		m_batches->spriteBatch.indices.clear();
		m_batches->triBatch.vertices.clear();
		m_batches->triBatch.indices.clear();

		m_batches->m_beginBatch = true;
	}

	void Draw2D::EndBatch()
	{
		assert(m_batches->m_beginBatch);

		Flush();
		m_batches->m_beginBatch = false;
	}

	void Draw2D::Flush()
	{
		_draw_sprites_();
		_draw_triangles_();
	}

	void Draw2D::AddRect(const Mat& in_world, const Vec2& in_size, const Vec4& in_color, float in_thickness)
	{
		AddRect(in_world, in_size, in_color, in_thickness, UINT32_LIMIT);
	}

	void Draw2D::AddRect(const Mat& in_world, const Vec2& in_size, const Vec4& in_color, float in_thickness, uint32 in_entityID)
	{
		Vec2 halfSize = in_size * 0.f;
		Vec2 p1 = Vec2::Transform({ -halfSize.x ,  halfSize.y }, in_world);
		Vec2 p2 = Vec2::Transform({ halfSize.x ,  halfSize.y }, in_world);
		Vec2 p3 = Vec2::Transform({ halfSize.x , -halfSize.y }, in_world);
		Vec2 p4 = Vec2::Transform({ -halfSize.x , -halfSize.y }, in_world);

		AddLine(p1, p2, in_color, in_thickness, in_entityID);
		AddLine(p2, p3, in_color, in_thickness, in_entityID);
		AddLine(p3, p4, in_color, in_thickness, in_entityID);
		AddLine(p4, p1, in_color, in_thickness, in_entityID);
	}

	void Draw2D::AddSprite(
		const Mat& in_world,
		const Vec2& in_size,
		const Vec4& in_color,
		const Ref<ITexture>& in_tex
	)
	{
		AddSprite(in_world, in_size, in_color, in_tex, { 0.f, 0.f }, { 1.f, 1.f }, UINT32_LIMIT);
	}

	void Draw2D::AddSprite(
		const Mat& in_world,
		const Vec2& in_size,
		const Vec4& in_color,
		const Ref<ITexture>& in_tex,
		const Vec2& in_uv0,
		const Vec2& in_uv1
	)
	{
		AddSprite(in_world, in_size, in_color, in_tex, { 0.f,0.f }, { 1.f,1.f }, UINT32_LIMIT);
	}

	void Draw2D::AddSprite(const Mat& in_world, const Vec2& in_size, const Vec4& in_color, const Ref<ITexture>& in_tex, uint32 in_entityID)
	{
		AddSprite(in_world, in_size, in_color, in_tex, { 0.f,0.f }, { 1.f,1.f }, in_entityID);
	}

	void Draw2D::AddSprite(const Mat& in_world, const Vec2& in_size, const Vec4& in_color, const Ref<ITexture>& in_tex, const Vec2& in_uv0, const Vec2& in_uv1, uint32 in_entityID)
	{
		if (m_batches->currentTexture != in_tex ||
			m_batches->spriteBatch.indices.size() + 6 >= BatchData::BUFFER_CAPACITY ||
			m_batches->spriteBatch.vertices.size() + 4 >= BatchData::BUFFER_CAPACITY)
		{
			_draw_sprites_();
		}

		Vec2 halfSize = in_size * 0.5f;
		Vec3 p1 = Vec3::Transform({ -halfSize.x ,  halfSize.y, 0.f }, in_world);
		Vec3 p2 = Vec3::Transform({ halfSize.x ,  halfSize.y, 0.f }, in_world);
		Vec3 p3 = Vec3::Transform({ halfSize.x , -halfSize.y, 0.f }, in_world);
		Vec3 p4 = Vec3::Transform({ -halfSize.x , -halfSize.y, 0.f }, in_world);

		uint32 offset = static_cast<uint32>(m_batches->spriteBatch.vertices.size());

		m_batches->spriteBatch.vertices.emplace_back(p1, in_uv0, in_color, in_entityID);
		m_batches->spriteBatch.vertices.emplace_back(p2, Vec2{ in_uv1.x, in_uv0.y }, in_color, in_entityID);
		m_batches->spriteBatch.vertices.emplace_back(p3, in_uv1, in_color, in_entityID);
		m_batches->spriteBatch.vertices.emplace_back(p4, Vec2{ in_uv0.x, in_uv1.y }, in_color, in_entityID);

		m_batches->spriteBatch.indices.insert(m_batches->spriteBatch.indices.end(), {
			offset, offset + 1, offset + 2,
			offset, offset + 2, offset + 3
			});

		m_batches->currentTexture = in_tex;
	}

	void Draw2D::AddRectFill(const Mat& in_world, const Vec2& in_size, const Vec4& in_color)
	{
		AddRectFill(in_world, in_size, in_color, UINT32_LIMIT);
	}

	void Draw2D::AddRectFill(const Mat& in_world, const Vec2& in_size, const Vec4& in_color, uint32 in_entityID)
	{
		if (m_batches->triBatch.indices.size() + 6 >= BatchData::BUFFER_CAPACITY ||
			m_batches->triBatch.vertices.size() + 4 >= BatchData::BUFFER_CAPACITY)
		{
			_draw_triangles_();
		}

		Vec2 halfSize = in_size * 0.5f;
		Vec3 p1 = Vec3::Transform({ -halfSize.x ,  halfSize.y , 0.f }, in_world);
		Vec3 p2 = Vec3::Transform({ halfSize.x ,  halfSize.y , 0.f }, in_world);
		Vec3 p3 = Vec3::Transform({ halfSize.x , -halfSize.y , 0.f }, in_world);
		Vec3 p4 = Vec3::Transform({ -halfSize.x , -halfSize.y , 0.f }, in_world);

		uint32 offset = static_cast<uint32>(m_batches->triBatch.vertices.size());

		m_batches->triBatch.vertices.emplace_back(p1, Vec2::Zero, in_color, in_entityID);
		m_batches->triBatch.vertices.emplace_back(p2, Vec2::Zero, in_color, in_entityID);
		m_batches->triBatch.vertices.emplace_back(p3, Vec2::Zero, in_color, in_entityID);
		m_batches->triBatch.vertices.emplace_back(p4, Vec2::Zero, in_color, in_entityID);

		m_batches->triBatch.indices.insert(m_batches->triBatch.indices.end(), {
			offset, offset + 1, offset + 2,
			offset, offset + 2, offset + 3
			});
	}

	void Draw2D::AddCircle(const Mat& in_world, float radius, const Vec4& in_color, float in_thickness)
	{
		AddCircle(in_world, radius, in_color, in_thickness, UINT32_LIMIT);
	}

	void Draw2D::AddCircle(const Mat& in_world, float radius, const Vec4& in_color, float in_thickness, uint32 in_entityID)
	{
		constexpr uint32 CIRCLE_SEGMENT = 64;
		constexpr float ANGLE_STEP = math::PI2 / CIRCLE_SEGMENT;

		Vec2 prevPoint = { radius, 0.0f };
		prevPoint = Vec2::Transform(prevPoint, in_world);

		for (uint32 i = 1; i <= CIRCLE_SEGMENT; i++)
		{
			float rad = static_cast<float>(i) * ANGLE_STEP;
			auto [sin, cos] = math::CalcSinCos(-rad);

			Vec2 currentPoint = { cos * radius, sin * radius };
			currentPoint = Vec2::Transform(currentPoint, in_world);

			AddLine(prevPoint, currentPoint, in_color, in_thickness, in_entityID);

			prevPoint = currentPoint;
		}
	}

	void Draw2D::AddCircleFill(const Mat& in_world, float radius, const Vec4& in_color)
	{
		AddCircleFill(in_world, radius, in_color, UINT32_LIMIT);
	}

	void Draw2D::AddCircleFill(const Mat& in_world, float radius, const Vec4& in_color, uint32 in_entityID)
	{
		constexpr uint32 CIRCLE_SEGMENT = 64;
		constexpr float ANGLE_STEP = math::PI2 / CIRCLE_SEGMENT;

		if (m_batches->triBatch.indices.size() + CIRCLE_SEGMENT + 1 >= BatchData::BUFFER_CAPACITY ||
			m_batches->triBatch.vertices.size() + CIRCLE_SEGMENT * 3 >= BatchData::BUFFER_CAPACITY)
		{
			_draw_triangles_();
		}

		uint32 offset = static_cast<uint32>(m_batches->triBatch.vertices.size());

		m_batches->triBatch.vertices.emplace_back(in_world.Translation(), Vec2::Zero, in_color, in_entityID);

		for (uint32 i = 0; i <= CIRCLE_SEGMENT; i++)
		{
			const float rad = static_cast<float>(i) * ANGLE_STEP;
			auto [sin, cos] = math::CalcSinCos(-rad);

			m_batches->triBatch.vertices.emplace_back(Vec3::Transform({ cos * radius, sin * radius, 0.f }, in_world), Vec2::Zero, in_color, in_entityID);
		}

		for (uint32 i = 1; i <= CIRCLE_SEGMENT; i++)
		{
			m_batches->triBatch.indices.insert(m_batches->triBatch.indices.end(), {
				offset, offset + i, offset + i + 1
				});
		}
	}

	void Draw2D::AddTriangle(const Mat& in_world, const Vec2& in_point1, const Vec2& in_point2,
	                         const Vec2& in_point3, const Vec4& in_color, float in_thickness)
	{
		AddTriangle(in_world, in_point1, in_point2, in_point3, in_color, in_thickness, UINT32_LIMIT);
	}

	void Draw2D::AddTriangle(const Mat& in_world, const Vec2& in_point1, const Vec2& in_point2, const Vec2& in_point3, const Vec4& in_color, float
	                         in_thickness, uint32 in_entityID)
	{
		Vec2 p1 = Vec2::Transform(in_point1, in_world);
		Vec2 p2 = Vec2::Transform(in_point2, in_world);
		Vec2 p3 = Vec2::Transform(in_point3, in_world);

		AddLine(p1, p2, in_color, in_thickness, in_entityID);
		AddLine(p2, p3, in_color, in_thickness, in_entityID);
		AddLine(p3, p1, in_color, in_thickness, in_entityID);
	}

	void Draw2D::AddTriangleFill(
		const Mat& in_world,
		const Vec2& in_point1,
		const Vec2& in_point2,
		const Vec2& in_point3,
		const Vec4& in_color
	)
	{
		AddTriangleFill(in_world, in_point1, in_point2, in_point3, in_color, UINT32_LIMIT);
	}

	void Draw2D::AddTriangleFill(const Mat& in_world, const Vec2& in_point1, const Vec2& in_point2, const Vec2& in_point3, const Vec4& in_color, uint32 in_entityID)
	{
		if (m_batches->triBatch.indices.size() + 3 >= BatchData::BUFFER_CAPACITY ||
			m_batches->triBatch.vertices.size() + 3 >= BatchData::BUFFER_CAPACITY)
		{
			_draw_triangles_();
		}

		uint32 offset = static_cast<uint32>(m_batches->triBatch.vertices.size());

		m_batches->triBatch.vertices.emplace_back(Vec3::Transform({ in_point1.x, in_point1.y, 0.f }, in_world), Vec2::Zero, in_color, in_entityID);
		m_batches->triBatch.vertices.emplace_back(Vec3::Transform({ in_point2.x, in_point2.y, 0.f }, in_world), Vec2::Zero, in_color, in_entityID);
		m_batches->triBatch.vertices.emplace_back(Vec3::Transform({ in_point3.x, in_point3.y, 0.f }, in_world), Vec2::Zero, in_color, in_entityID);

		m_batches->triBatch.indices.insert(m_batches->triBatch.indices.end(), {
			offset, offset + 1, offset + 2
			});
	}

	void Draw2D::AddLine(const Vec2& in_point1, const Vec2& in_point2, const Vec4& in_color, float in_thickness)
	{
		AddLine(in_point1, in_point2, in_color, in_thickness, UINT32_LIMIT);
	}

	void Draw2D::AddLine(const Vec2& in_point1, const Vec2& in_point2, const Vec4& in_color, float in_thickness, uint32 in_entityID)
	{
		if (m_batches->triBatch.indices.size() + 6 >= BatchData::BUFFER_CAPACITY ||
			m_batches->triBatch.vertices.size() + 4 >= BatchData::BUFFER_CAPACITY)
		{
			_draw_triangles_();
		}

		constexpr float THICKNESS_SCALE_FACTORY = 0.001f;
		float halfThickness = 0.5f * in_thickness * THICKNESS_SCALE_FACTORY;

		Vec2 direction = in_point2 - in_point1;
		direction.Normalize();

		Vec2 thick = Vec2(-direction.y, direction.x) * halfThickness;

		Vec2 p1 = in_point1 - thick;
		Vec2 p2 = in_point1 + thick;
		Vec2 p3 = in_point2 + thick;
		Vec2 p4 = in_point2 - thick;

		uint32 offset = static_cast<uint32>(m_batches->triBatch.vertices.size());

		m_batches->triBatch.vertices.emplace_back(Vec3{ p1.x, p1.y, 0.f }, Vec2::Zero, in_color, in_entityID);
		m_batches->triBatch.vertices.emplace_back(Vec3{ p2.x, p2.y, 0.f }, Vec2::Zero, in_color, in_entityID);
		m_batches->triBatch.vertices.emplace_back(Vec3{ p3.x, p3.y, 0.f }, Vec2::Zero, in_color, in_entityID);
		m_batches->triBatch.vertices.emplace_back(Vec3{ p4.x, p4.y, 0.f }, Vec2::Zero, in_color, in_entityID);

		m_batches->triBatch.indices.insert(m_batches->triBatch.indices.end(), {
			offset, offset + 1, offset + 2,
			offset, offset + 2, offset + 3
			});
	}

	void Draw2D::_draw_sprites_()
	{
		if (m_batches->spriteBatch.vertices.empty() || m_batches->spriteBatch.indices.empty() || !m_batches->currentTexture)
			return;

		Renderer::BindShader(m_batches->shader);

		m_batches->spriteBatch.ib->ClearBuffer();
		m_batches->spriteBatch.ib->PushIndices(m_batches->spriteBatch.indices);

		m_batches->spriteBatch.vb->ClearBuffer();
		m_batches->spriteBatch.vb->PushVertices(
			m_batches->spriteBatch.vertices.data(),
			sizeof(BatchVertex),
			m_batches->spriteBatch.vertices.size()
		);

		Renderer::BindTexture(m_batches->spriteTextureSlot, m_batches->currentTexture);
		Renderer::DrawIndices(m_batches->spriteBatch.vb, m_batches->spriteBatch.ib);

		m_batches->spriteBatch.indices.clear();
		m_batches->spriteBatch.vertices.clear();
	}

	void Draw2D::_draw_triangles_()
	{
		if (m_batches->triBatch.vertices.empty() || m_batches->triBatch.indices.empty())
			return;

		Renderer::BindShader(m_batches->shader);

		m_batches->triBatch.ib->ClearBuffer();
		m_batches->triBatch.ib->PushIndices(m_batches->triBatch.indices);

		m_batches->triBatch.vb->ClearBuffer();
		m_batches->triBatch.vb->PushVertices(
			m_batches->triBatch.vertices.data(),
			sizeof(BatchVertex),
			m_batches->triBatch.vertices.size()
		);

		// 기존 텍스처 바인딩 해제
		if (m_batches->currentTexture)
		{
			m_batches->currentTexture = nullptr;
			Renderer::UnbindTexture(m_batches->spriteTextureSlot);
		}

		Renderer::DrawIndices(m_batches->triBatch.vb, m_batches->triBatch.ib);

		m_batches->triBatch.indices.clear();
		m_batches->triBatch.vertices.clear();
	}
}

