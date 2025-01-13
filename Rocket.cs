using System;
using System.Numerics;

namespace crab
{
	public class Rocket : Entity
	{
		private Transform m_transform;
		private float m_speed;

		public override void OnCreate()
		{
			m_transform = FindComponent<Transform>();
			m_speed = 1f;
		}

		public override void OnUpdate(float in_detalTime)
		{
			if (Input.IsKeyDown(eKey.RightArrow))
			{
				m_transform.position += Vector3.UnitX * m_speed * in_detalTime;
			}
			if (Input.IsKeyDown(eKey.LeftArrow))
			{
				m_transform.position += -Vector3.UnitX * m_speed * in_detalTime;
			}
			if (Input.IsKeyDown(eKey.UpArrow))
			{
				m_transform.position += Vector3.UnitY * m_speed * in_detalTime;
			}
			if (Input.IsKeyDown(eKey.DownArrow))
			{
				m_transform.position += -Vector3.UnitY * m_speed * in_detalTime;
			}
		}
	}
}
