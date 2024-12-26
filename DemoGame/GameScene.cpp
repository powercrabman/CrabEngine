#include "CrabEnginePch.h"
#include "GameScene.h"


void GameScene::SetupScene()
{
	using namespace crab;

	// Vertex Buffer
	struct SimpleVertex
	{
		Vec3 position;
		Vec4 color;
	};

	Ref<IVertexBuffer> vBuf = IVertexBuffer::Create({
		SimpleVertex{Vec3(0.f, 0.5f, 0.5f), Vec4(1.0f, 0.0f, 0.0f, 1.0f)},
		SimpleVertex{Vec3(0.5f, -0.5f, 0.5f), Vec4(0.0f, 1.0f, 0.0f, 1.0f)},
		SimpleVertex{Vec3(-0.5f, -0.5f, 0.5f), Vec4(0.0f, 0.0f, 1.0f, 1.0f)}
		});

	// Index Buffer
	Ref<IIndexBuffer> iBuf = IIndexBuffer::Create({ 0,1,2 });

	// Geometry
	m_geometry = Geometry::Create(vBuf, iBuf);

	// Shader
	const char* singleShader = R"(
			struct VSInput
			{
				float3 position : POSITION;
				float4 color : COLOR;
			};

			struct VSOutput
			{
				float4 position : SV_POSITION;
				float4 color : COLOR;
			};

			VSOutput VSmain(VSInput in_input)
			{
				VSOutput output;
				output.position = float4(in_input.position, 1.0f);
				output.color = in_input.color;
				return output;
			}

			float4 PSmain(VSOutput in_input) : SV_TARGET
			{
				return in_input.color;
			}
		)";

	m_shader = IShader::CreateByString(
		{ { "POSITION", 0, eLayoutFormat::Float3 },
		  { "COLOR", 0, eLayoutFormat::Float4 } }
		, singleShader
	);

}

void GameScene::OnEnterScene()
{
	m_shader->Bind();
}

void GameScene::OnExitScene()
{
}

void GameScene::OnUpdate(float in_deltaTime)
{
	if (crab::Input::IsKeyPress(crab::eKey::Enter))
	{
		crab::Log::Debug("Enter Key Pressed");
		crab::Log::Debug("fps is {:.3f}", 1 / in_deltaTime);
	}
}

void GameScene::OnRender(float in_deltaTime)
{
	crab::Renderer::DrawIndices(m_geometry, Mat::Identity);
}

void GameScene::OnImGuiRender(float in_deltaTime)
{
	ImGui::Begin("GameScene");

	static int s_counter = 0;
	ImGui::Text("Hello, World!");

	if (ImGui::Button("Click")) ++s_counter;
	ImGui::SameLine();
	ImGui::Text("Counter: %d", s_counter);
	ImGui::Text("FPS : %.3f", 1 / in_deltaTime);

	ImGui::End();
}

