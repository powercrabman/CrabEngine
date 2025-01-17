using crab;
using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace DemoGameScript
{
	public class Rocket : Entity
	{
		private Transform m_transform;

		public override void OnCreate()
		{
			m_transform = TryFindComponent<Transform>();
		}

		public override void OnUpdate(float in_detalTime)
		{
			if(Input.IsKeyDown(eKey.LeftArrow))
			{

			}
		}
	}
}
