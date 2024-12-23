#include "CrabEnginePch.h"
#include "TextureDemoScene.h"

void TextureDemoScene::OnEnterScene()
{
	using namespace crab;

	struct SimpleVertex
	{
		Vec3 position;
		Vec2 texCoord;
	};

	Ref<IVertexBuffer> vBuf = IVertexBuffer::Create({
		SimpleVertex{Vec3{0.5f,0.5f,0.5f}, Vec2{1.f,0.f}},
		SimpleVertex{Vec3{0.5f,-0.5f,0.5f}, Vec2{1.f,1.f}},
		SimpleVertex{Vec3{-0.5f,-0.5f,0.5f}, Vec2{0.f,1.f}},
		SimpleVertex{Vec3{-0.5f,0.5f,0.5f}, Vec2{0.f,0.f}},
		});

	Ref<IIndexBuffer> iBuf = IIndexBuffer::Create({ 0,1,2, 0,2,3 });

	m_geometry = Geometry::Create(vBuf, iBuf);
	m_geometry->Bind();

	m_texture = ITexture::Create(R"(sampleTexture.jpg)");
	m_texture->Bind(0);
	
	Ref<IShader> shader = IShader::CreateByFile(
		{ { "POSITION", 0, eLayoutFormat::Float3 },
		  { "TEXCOORD", 0, eLayoutFormat::Float2 } }
		, L"TextureDemoShader.hlsli"
	);

	shader->Bind();
}

void TextureDemoScene::OnExitScene()
{

}

void TextureDemoScene::OnUpdate(float in_deltaTime)
{

}

void TextureDemoScene::OnRender(float in_deltaTime)
{
	crab::Renderer::BindSamplerState(crab::eSamplerState::LinearWrap);

	Mat finalMat = DirectX::XMMatrixAffineTransformation(
		m_scale,
		Vec3::Zero,
		DirectX::SimpleMath::Quaternion::CreateFromYawPitchRoll(m_rot),
		m_position
	);

	crab::Renderer::DrawIndices(m_geometry, finalMat);
}

void TextureDemoScene::OnImGuiRender(float in_deltaTime)
{
}
