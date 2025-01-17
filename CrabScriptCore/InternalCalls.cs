using System;
using System.Collections.Generic;
using System.Linq;
using System.Numerics;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading.Tasks;

namespace crab
{
	internal static class InternalCalls
	{

		#region Log
		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		extern public static void Log_Trace(String in_msg);

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		extern public static void Log_Warn(String in_msg);

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		extern public static void Log_Error(String in_msg);

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		extern public static void Log_Debug(String in_msg);

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		extern public static void Log_Info(String in_msg);
		#endregion

		#region Entity

		// Create

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		extern public static object Entity_TryCreateComponent(UInt32 in_entityID, Type in_componentType);

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		extern public static object Entity_CreateComponent(UInt32 in_entityID, Type in_componentType);

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		extern public static object Entity_ReplaceComponent(UInt32 in_entityID, Type in_componentType);

		// Find

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		extern public static bool Entity_HasComponent(UInt32 in_entityID, Type in_componentType);

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		extern public static object Entity_TryFindComponent(UInt32 in_entityID, Type in_componentType);

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		extern public static object Entity_FindComponent(UInt32 in_entityID, Type in_componentType);

		// Remove

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		extern public static object Entity_RemoveComponent(UInt32 in_entityID, Type in_componentType);

		// Ect

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		extern public static bool IsValid(UInt32 in_entityID);

		#endregion

		#region Input
		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		extern public static bool Input_IsKeyPress(eKey in_keyCode);

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		extern public static bool Input_IsKeyRelease(eKey in_keyCode);

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		extern public static bool Input_IsKeyDown(eKey in_keyCode);

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		extern public static bool Input_IsKeyUp(eKey in_keyCode);

		#endregion

		#region Transform

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		extern public static void Transform_SetPosition(UInt32 in_entityID, Vector3 in_position);

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		extern public static void Transform_GetPosition(UInt32 in_entityID, out Vector3 in_position);

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		extern public static void Transform_SetRotation(UInt32 in_entityID, Vector3 in_position);

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		extern public static void Transform_GetRotation(UInt32 in_entityID, out Vector3 in_position);

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		extern public static void Transform_SetScale(UInt32 in_entityID, Vector3 in_position);

		[MethodImplAttribute(MethodImplOptions.InternalCall)]
		extern public static void Transform_GetScale(UInt32 in_entityID, out Vector3 in_position);

		#endregion

	}
}
