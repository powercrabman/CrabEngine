#pragma once

namespace crab
{
	class ImWindow
	{
	public:
		ImWindow(const std::string_view in_name, bool* in_open = (bool*)0, ImGuiWindowFlags in_flags = 0)
		{
			ImGui::Begin(in_name.data(), in_open, in_flags);
		}

		~ImWindow()
		{
			ImGui::End();
		}
	};

	class ImChildWindow
	{
	public:
		ImChildWindow(const std::string_view in_name, const ImVec2& in_size = { 0.f,0.f }, ImGuiChildFlags in_cFlags = NULL, ImGuiWindowFlags in_wFlags = NULL)
		{
			ImGui::BeginChild(in_name.data(), in_size, in_cFlags, in_wFlags);
		}

		~ImChildWindow()
		{
			ImGui::EndChild();
		}
	};

	class ImMenu
	{
	public:
		ImMenu(const std::string_view in_name, bool in_enable = true)
		{
			m_enable = ImGui::BeginMenu(in_name.data(), in_enable);
		}

		~ImMenu()
		{
			if (m_enable)
			{
				ImGui::EndMenu();
			}
		}

		operator bool() const noexcept { return m_enable; }

	private:
		bool m_enable = false;
	};

	class ImMenuBar
	{
	public:
		ImMenuBar()
		{
			m_enable = ImGui::BeginMenuBar();
		}

		~ImMenuBar()
		{
			if (m_enable)
			{
				ImGui::EndMenuBar();
			}
		}

		operator bool() const noexcept { return m_enable; }

	private:
		bool m_enable = false;
	};


	class ImMainMenuBar
	{
	public:
		ImMainMenuBar()
		{
			m_enable = ImGui::BeginMainMenuBar();
		}

		~ImMainMenuBar()
		{
			if (m_enable)
			{
				ImGui::EndMainMenuBar();
			}
		}

		operator bool() const noexcept { return m_enable; }

	private:
		bool m_enable = false;
	};

	class ImPopupWindow
	{
	public:
		ImPopupWindow(const char* in_str = "", ImGuiPopupFlags in_flags = NULL)
		{
			m_enable = ImGui::BeginPopupContextWindow(in_str, in_flags);
		}

		~ImPopupWindow()
		{
			if (m_enable)
			{
				ImGui::EndPopup();
			}
		}

		operator bool() const noexcept { return m_enable; }

	private:
		bool m_enable = false;
	};


	class ImPopupItem
	{
	public:
		ImPopupItem(const char* in_str = nullptr, ImGuiPopupFlags in_flags = 0)
		{
			m_enable = ImGui::BeginPopupContextItem(in_str, in_flags);
		}

		~ImPopupItem()
		{
			if (m_enable)
			{
				ImGui::EndPopup();
			}
		}

		void Close()
		{
			ImGui::CloseCurrentPopup();
			if (m_enable)
			{
				ImGui::EndPopup();
				m_enable = false;
			}
		}

		operator bool() const noexcept { return m_enable; }

	private:
		bool m_enable = false;
	};

	enum class eImPopupResult
	{
		None = 0,
		Button1,
		Button2
	};

	class ImModal
	{
	public:
		ImModal(std::string_view name)
		{
			ImGui::SetNextWindowPos(ImGui::GetMainViewport()->GetCenter(), ImGuiCond_Appearing, ImVec2{ 0.5f,0.5f });
			mEnableBit = ImGui::BeginPopupModal(name.data(), NULL, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoScrollbar);
		}
		~ImModal() { if (mEnableBit) { ImGui::EndPopup(); } }
		explicit operator bool() const { return mEnableBit; }

		eImPopupResult DrawButton(const std::string_view button1)
		{
			ImGui::Spacing();
			ImGui::Separator();
			ImGui::Spacing();

			if (ImGui::Button(button1.data()))
			{
				ImGui::CloseCurrentPopup();
				return eImPopupResult::Button1;
			}

			return eImPopupResult::None;
		}

		eImPopupResult DrawButton2(const std::string_view button1, const std::string_view button2)
		{
			ImGui::Separator();

			if (ImGui::Button(button1.data()))
			{
				ImGui::CloseCurrentPopup();
				return eImPopupResult::Button1;
			}

			if (!button2.empty())
			{
				ImGui::SameLine();
				if (ImGui::Button(button2.data()))
				{
					ImGui::CloseCurrentPopup();
					return eImPopupResult::Button2;
				}
			}

			return eImPopupResult::None;
		}

	private:
		bool mEnableBit = false;
	};
}

class ImDisable
{
public:
	ImDisable(bool bit)
	{
		ImGui::BeginDisabled(bit);
	}

	~ImDisable()
	{
		ImGui::EndDisabled();
	}

private:
};