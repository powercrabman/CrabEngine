#pragma once

template <typename Ty, uint32 SIZE>
class CircularQueue
{
	static_assert(SIZE > 0, "Size of CircularQueue must be greater than 0");
public:
	void push_back(const Ty& in_item)
	{
		if (full())
		{
			ASSERT(false, "buffer overflow");
		}
		else
		{
			m_buffer[m_rear] = in_item;
			m_rear = (m_rear + 1) % SIZE;
		}
	}

	void pop_back()
	{
		if (empty())
		{
			ASSERT(false, "buffer is empty");
		}
		else
		{
			m_rear = (m_rear - 1 + SIZE) % SIZE;
		}
	}

	void push_front(const Ty& in_item)
	{
		if (full())
		{
			ASSERT(false, "buffer overflow");
		}
		else
		{
			m_front = (m_front - 1 + SIZE) % SIZE;
			m_buffer[m_front] = in_item;
		}
	}

	void pop_front()
	{
		if (empty())
		{
			ASSERT(false, "buffer is empty");
		}
		else
		{
			m_front = (m_front + 1) % SIZE;
		}
	}

	template <typename Func>
	void for_each(const Func& in_func)
	{
		for (uint32 idx = m_front; idx != m_rear; idx = (idx + 1) % SIZE)
		{
			in_func(m_buffer[idx]);
		}
	}

	template <typename Func>
	void for_each_reverse(const Func& in_func)
	{
		for (uint32 idx = (m_rear - 1 + SIZE) % SIZE;
			idx != (m_front - 1 + SIZE) % SIZE;
			idx = (idx - 1 + SIZE) % SIZE)
		{
			in_func(m_buffer[idx]);
		}
	}

	bool				empty() const { return m_front == m_rear; }
	bool				full() const { return (m_rear + 1) % SIZE == m_front; }
	constexpr uint32	capacity() const { return SIZE; }

	Ty& operator[](uint32 idx)
	{
		assert(m_front <= idx && idx < m_rear);
		return m_buffer[idx];
	}

	uint32 size() const
	{
		if (m_rear >= m_front) return m_rear - m_front;
		return SIZE - (m_front - m_rear);
	}

	Ty& front() const
	{
		if (empty())
		{
			ASSERT(false, "buffer is empty");
		}
		else
		{
			return m_buffer[m_front];
		}
	}

	Ty& back() const
	{
		if (empty())
		{
			ASSERT(false, "buffer is empty");
		}
		else
		{
			return m_buffer[(m_rear - 1 + SIZE) % SIZE];
		}
	}

private:
	std::array<Ty, SIZE>	m_buffer = {};
	uint32					m_front = 0;
	uint32					m_rear = 0;
};