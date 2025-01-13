#pragma once

namespace crab
{
	class ITexture
	{
	public:
		ITexture()  = default;
		virtual ~ITexture() = default;

		virtual void*							GetNativeTexture() const = 0;
		virtual std::pair<float, float>			GetSize() const          = 0;
		virtual float							GetAspect() const        = 0;
		virtual std::wstring_view				GetName() const          = 0;
		virtual const std::filesystem::path&	GetPath() const          = 0;

		static Ref<ITexture>					Create(const std::filesystem::path& in_path);

	private:
		virtual void _create_(const std::filesystem::path& in_path) = 0;
	};
}
