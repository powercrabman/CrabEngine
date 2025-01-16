#pragma once
#include "AssetPayload.h"
#include "CrabEditorData.h"
#include "PanelDrawHelper.h"

namespace crab
{
	class SpriteEditorPanel
	{
	public:
		enum class eMode { Create, Edit };

		void Draw();
		void OpenPanel(const Sprite* in_sprite_or_null, eMode in_mode);
		bool IsVisible() const { return m_visible; }

	private:
		bool	is_duplicated_sprite_name();
		void	save_and_close() 
		{ 
			if (save_sprite())
				m_visible = false;
		}
		void	save_as_and_close() 
		{
			if (save_as_sprite())
				m_visible = false;
		}
		bool	save_as_sprite();
		bool	save_sprite();

		Sprite	m_editSprite = {};
		float	m_zoomRatio = 1.f;
		ImVec2	m_offset = { 0.f,0.f };
		eMode	m_mode = eMode::Create;
		bool	m_visible = false;
		
		char		m_spriteNameInputBuffer[100];
		std::string m_originalSpriteName = {}; // use only eMode::Edit
		bool		m_spriteNameDirtyBit = false;
	};
}
