#pragma once
#include "SceneSerializer.h"

// Shared Data for edit
namespace crab
{
	struct EditorCamera
	{
		Mat GetProj(float in_aspect) const
		{
			if (mode == eProjection::Orthographic)
			{
				float z = transform.position.z == 0 ? math::SMALL_NUM : transform.position.z;
				return DirectX::XMMatrixOrthographicLH(-z * in_aspect, -z, 0.01f, 100.f);
			}
			else
			{
				return DirectX::XMMatrixPerspectiveFovLH(fov, in_aspect, 0.01f, 100.f);
			}
		}

		Mat GetView() const
		{
			Mat rMat = Mat::CreateFromYawPitchRoll(transform.rotation.x, transform.rotation.y, transform.rotation.z);
			return DirectX::XMMatrixLookToLH(transform.position, rMat.Backward(), rMat.Up());
		}

		Mat GetViewProj(float in_aspect) const
		{
			return GetView() * GetProj(in_aspect);
		}

		eProjection mode    = eProjection::Orthographic;
		Transform transform = { Vec3{0.f,0.f,-1.f}, Vec3::Zero, Vec3::One };
		float fov           = math::DegToRad(45.f);
	};

	struct CrabEditorData
	{
		Ref<IRenderFrame>	editRenderFrame;
		SceneSerializer		tempSceneData;
		Entity				selectedEntity      = Entity::s_null;
		EditorCamera		editorCamera        = {};
		eEditorState		editorState         = eEditorState::Edit;
		ImGuizmo::OPERATION	guizmoOp            = ImGuizmo::OPERATION::TRANSLATE;

		ImFont*				editorSmallFont;
		ImFont*				editorFont;
		ImFont*				editorBoldFont;

		Ref<ITexture>		brandIcon;
		Ref<ITexture>		playIcon;
		Ref<ITexture>		simulateIcon;
		Ref<ITexture>		pauseIcon;
		Ref<ITexture>		resetIcon;
		Ref<ITexture>		nextFrameIcon;
		Ref<ITexture>		cameraIcon;
		Ref<ITexture>		translateIcon;
		Ref<ITexture>		rotateIcon;
		Ref<ITexture>		scaleIcon;
		Ref<ITexture>		entityIcon;
		Ref<ITexture>		plusIcon;
		Ref<ITexture>		gearIcon;
		Ref<ITexture>		tagIcon;
		Ref<ITexture>		transformIcon;
		Ref<ITexture>		spriteIcon;
		Ref<ITexture>		flipbookIcon;
		Ref<ITexture>		meshIcon;
		Ref<ITexture>		monoScriptIcon;
		Ref<ITexture>		docIcon;
		Ref<ITexture>		directoryIcon;
		Ref<ITexture>		refreshIcon;
		Ref<ITexture>		searchIcon;
		Ref<ITexture>		pencilIcon;
		Ref<ITexture>		checkerBoard;

	};

	inline CrabEditorData& GetCrabEditorData()
	{
		assert(GetCrabEngine().IsEnableEditor());
		static CrabEditorData g_CrabEditorData;
		return g_CrabEditorData;
	}
}

