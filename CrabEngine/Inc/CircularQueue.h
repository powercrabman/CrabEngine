#pragma once

template <typename Ty, uint32 SIZE>
class CircularQueue
{
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

	bool				empty() const { return m_front == m_rear; }
	bool				full() const { return (m_rear + 1) % SIZE == m_front; }
	constexpr uint32	capacity() const { return SIZE; }

	uint32 size() const
	{
		if (m_rear >= m_front) return m_rear - m_front;
		return SIZE - (m_front - m_rear);
	}

	Ty front() const
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

	Ty back() const
	{
		if (empty())
		{
			ASSERT(false, "buffer overflow");
		}
		else
		{
			return m_buffer[m_rear - 1];
		}
	}

private:
	std::array<Ty, SIZE> m_buffer = {};
	uint32 m_front = 0;
	uint32 m_rear = 0;
};