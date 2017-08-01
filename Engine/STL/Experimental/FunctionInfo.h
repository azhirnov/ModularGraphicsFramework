// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/STL/CompileTime/TypeListHelpers.h"
#include "Engine/STL/Types/Delegate.h"

namespace GX_STL
{
namespace CompileTime
{

	namespace _ctime_hidden_
	{
		template <typename T>
		struct _FunctionInfo			{};
		
		template <typename T>
		struct _FunctionInfo< T * >		{};

		
		template <typename Result, typename ...Args>
		struct _FunctionInfo< Result (Args...) >
		{
			using args		= TypeListFrom< Args..., TypeListEnd >;
			using result	= Result;
			using type		= Result (*) (Args...);
			using clazz		= void;
		};

		template <typename Class, typename Result, typename ...Args>
		struct _FunctionInfo< Result (Class::*) (Args...) >
		{
			using args		= TypeListFrom< Args..., TypeListEnd >;
			using result	= Result;
			using type		= Result (Class::*) (Args...);
			using clazz		= Class;
		};
		
		template <typename Class, typename Result, typename ...Args>
		struct _FunctionInfo< Result (Class::*) (Args...) const >
		{
			using args		= TypeListFrom< Args..., TypeListEnd >;
			using result	= Result;
			using type		= Result (Class::*) (Args...) const;
			using clazz		= Class;
		};

		template <typename Result, typename ...Args>
		struct _FunctionInfo< Result (*) (Args...) >
		{
			using args		= TypeListFrom< Args..., TypeListEnd >;
			using result	= Result;
			using type		= Result (*) (Args...);
			using clazz		= void;
		};

		template <typename Result, typename ...Args>
		struct _FunctionInfo< Delegate< Result (Args...) > >
		{
			using _del		= Delegate< Result (Args...) >;

			using args		= typename _del::Args_t;
			using result	= typename _del::Result_t;
			using type		= typename _del::Function_t;
			using clazz		= void;
		};

		template <typename Result, typename ...Args>
		struct _FunctionInfo< Event< Result (Args...) > >
		{
			using _ev		= Event< Result (Args...) >;

			using args		= typename _ev::Args_t;
			using result	= typename _ev::Result_t;
			using type		= typename _ev::Function_t;
			using clazz		= void;
		};
		
		template <typename Result, typename ...Args>
		struct _FunctionInfo< std::function< Result (Args...) > >
		{
			using args		= TypeListFrom< Args..., TypeListEnd >;
			using result	= Result;
			using type		= Result (*) (Args...);
			using clazz		= void;
		};

	}	// _ctime_hidden_

	
	template <typename T>
	using FunctionInfo		= _ctime_hidden_::_FunctionInfo< T >;


}	// CompileTime
}	// GX_STL
