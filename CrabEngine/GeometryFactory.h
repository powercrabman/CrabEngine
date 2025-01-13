#pragma once

namespace crab::GeometryFactory
{
	//namespace DD
	//{
	//	static Ref<Geometry> CreateRectColor(float width, float height, const std::array<Vec4, 4>& in_colorArray)
	//	{
	//		float widthDiv2 = width * 0.5f;
	//		float heightDiv2 = height * 0.5f;
	//
	//		Ref<IVertexBuffer> vBuf = IVertexBuffer::Create({
	//			                                                VertexPosColor{Vec3{-widthDiv2, heightDiv2, 0.f}, in_colorArray[0]},
	//			                                                VertexPosColor{Vec3{widthDiv2, heightDiv2, 0.f}, in_colorArray[1]},
	//			                                                VertexPosColor{Vec3{widthDiv2, -heightDiv2, 0.f}, in_colorArray[2]},
	//			                                                VertexPosColor{Vec3{-widthDiv2, -heightDiv2, 0.f}, in_colorArray[3]},
	//		                                                }, eBufferType::Immutable);
	//
	//		Ref<IIndexBuffer> iBuf = IIndexBuffer::Create({ 0,1,2, 0,2,3 });
	//		return Geometry::Create(vBuf, iBuf);
	//	}
	//
	//	static Ref<Geometry> CreateRectTex(float width, float height, const std::array<Vec2, 4>& in_uvArray = { Vec2{0.f, 0.f}, Vec2{1.f, 0.f},Vec2{1.f, 1.f},Vec2{0.f, 1.f} })
	//	{
	//		float widthDiv2 = width * 0.5f;
	//		float heightDiv2 = height * 0.5f;
	//
	//		Ref<IVertexBuffer> vBuf = IVertexBuffer::Create({
	//			                                                VertexPosTex{Vec3{-widthDiv2, heightDiv2, 0.f}, in_uvArray[0]},
	//			                                                VertexPosTex{Vec3{widthDiv2, heightDiv2, 0.f}, in_uvArray[1]},
	//			                                                VertexPosTex{Vec3{widthDiv2, -heightDiv2, 0.f}, in_uvArray[2]},
	//			                                                VertexPosTex{Vec3{-widthDiv2, -heightDiv2, 0.f}, in_uvArray[3]},
	//		                                                }, eBufferType::Immutable);
	//
	//		Ref<IIndexBuffer> iBuf = IIndexBuffer::Create({ 0,1,2, 0,2,3 });
	//		return Geometry::Create(vBuf, iBuf);
	//	}
	//
	//	static Ref<Geometry> CreateRectColorTex(
	//		float width, float height,
	//		const std::array<Vec4, 4>& in_colorArray = { Color::WHITE, Color::WHITE ,Color::WHITE ,Color::WHITE },
	//		const std::array<Vec2, 4>& in_uvArray = { Vec2{0.f, 0.f}, Vec2{1.f, 0.f},Vec2{1.f, 1.f},Vec2{0.f, 1.f} }
	//	)
	//	{
	//		float widthDiv2 = width * 0.5f;
	//		float heightDiv2 = height * 0.5f;
	//
	//		Ref<IVertexBuffer> vBuf = IVertexBuffer::Create({
	//			                                                VertexPosTexColor{Vec3{-widthDiv2, heightDiv2, 0.f}, in_colorArray[0], in_uvArray[0]},
	//			                                                VertexPosTexColor{Vec3{widthDiv2, heightDiv2, 0.f}, in_colorArray[1], in_uvArray[1]},
	//			                                                VertexPosTexColor{Vec3{widthDiv2, -heightDiv2, 0.f}, in_colorArray[2], in_uvArray[2]},
	//			                                                VertexPosTexColor{Vec3{-widthDiv2, -heightDiv2, 0.f}, in_colorArray[3], in_uvArray[3]},
	//		                                                }, eBufferType::Immutable);
	//
	//		Ref<IIndexBuffer> iBuf = IIndexBuffer::Create({ 0,1,2, 0,2,3 });
	//		return Geometry::Create(vBuf, iBuf);
	//	}
	//}
	//
	//namespace DDD
	//{
	//
	//}
}