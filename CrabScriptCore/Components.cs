using System;
using System.Collections.Generic;
using System.Linq;
using System.Numerics;
using System.Text;
using System.Threading.Tasks;

namespace crab
{
	public class Component
	{
		public Entity entity { get; set; }
	}

	public class Transform : Component
	{
		public Vector3 position 
		{
			get
			{
				InternalCalls.Transform_GetPosition(entity.ID, out Vector3 out_position);
				return out_position;
			}
			set 
			{
				InternalCalls.Transform_SetPosition(entity.ID, value);
			}
		}
		public Vector3 rotation
		{
			get
			{
				InternalCalls.Transform_GetRotation(entity.ID, out Vector3 out_rotation);
				return out_rotation;
			}
			set
			{
				InternalCalls.Transform_SetRotation(entity.ID, value);
			}
		}
		public Vector3 scaling
		{
			get
			{
				InternalCalls.Transform_GetScale(entity.ID, out Vector3 out_scale);
				return out_scale;
			}
			set
			{
				InternalCalls.Transform_SetScale(entity.ID, value);
			}
		}
	}
}
