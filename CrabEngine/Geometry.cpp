#include "CrabEnginePch.h"
#include "Geometry.h"
#include "IVertexBuffer.h"
#include "IIndexBuffer.h"

namespace crab
{
	Ref<Geometry> Geometry::Create(const Ref<IVertexBuffer>& in_vertexBuffer, const Ref<IIndexBuffer>& in_indexBuffer)
	{
		Ref<Geometry> geometry = MakeRef<Geometry>();
		geometry->m_vertexBuffer = in_vertexBuffer;
		geometry->m_indexBuffer = in_indexBuffer;
		return geometry;
	}
}

