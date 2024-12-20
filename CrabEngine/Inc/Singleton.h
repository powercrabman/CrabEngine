#pragma once

namespace crab
{
	template <typename T>
	class Singleton
	{
	public:
		static T& Get()
		{
			if (!m_instance) { m_instance = new T(); }
			return *m_instance;
		}

		static void Destroy()
		{
			if (m_instance)
			{
				delete m_instance;
				m_instance = nullptr;
			}
		}

	protected:
		Singleton() = default;
		~Singleton() = default;

		Singleton(const Singleton&) = delete;
		Singleton& operator=(const Singleton&) = delete;
		Singleton(Singleton&&) = delete;
		Singleton& operator=(Singleton&&) = delete;

	private:
		inline static T* m_instance = nullptr;
	};
}
