#pragma once
#include "IVertexBuffer.h"
#include "IIndexBuffer.h"

namespace crab
{
	class Geometry
	{
	public:
		Geometry() = default;
		~Geometry() = default;

		void Bind();
		
		Ref<IVertexBuffer>	GetVertexBuffer() const { return m_vertexBuffer; }
		Ref<IIndexBuffer>	GetIndexBuffer() const { return m_indexBuffer; }

		uint32				GetIndexCount() const { return m_indexBuffer->GetIndexCount(); }

		static Ref<Geometry> Create(const Ref<IVertexBuffer>& in_vertexBuffer, const Ref<IIndexBuffer>& in_indexBuffer);

	private:
		Ref<IVertexBuffer>	m_vertexBuffer;
		Ref<IIndexBuffer>	m_indexBuffer;
	};

	//===================================================
	//                      Inline
	//===================================================

	inline void Geometry::Bind()
	{
		m_vertexBuffer->Bind();
		m_indexBuffer->Bind();
	}
}
