using Microsoft.SqlServer.Server;
using System;
using System.Collections.Specialized;
using System.Runtime.CompilerServices;
using System.Runtime.InteropServices;

namespace crab
{
	public static class Log
	{

		/*=================================
			 	Internal Function
		==================================*/
		public static void Trace(string in_str)
		{
			InternalCalls.Log_Trace(in_str);
		}

		public static void Debug(string in_str)
		{
			InternalCalls.Log_Debug(in_str);
		}

		public static void Warn(string in_str)
		{
			InternalCalls.Log_Warn(in_str);
		}

		public static void Error(string in_str)
		{
			InternalCalls.Log_Error(in_str);
		}
		public static void Info(string in_str)
		{
			InternalCalls.Log_Info(in_str);
		}
	}
}
