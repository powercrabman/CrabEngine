#pragma once
#include "IDepthStencil.h"

namespace crab
{
	class DX11DepthStencil : public IDepthStencil
	{
	public:
		 DX11DepthStencil()          = default;
		~DX11DepthStencil() override = default;

		void*	GetNativeDepthStencil() override { return m_dsv.Get(); }
		void	Clear(bool in_clearDepth, bool in_clearStencil) override;

		::ComPtr<ID3D11DepthStencilView> GetDepthStencilView() const { return m_dsv; }

	private:
		void	_create_(uint32 in_width, uint32 in_height) override;

		::ComPtr<ID3D11DepthStencilView>	m_dsv;
	};
}


