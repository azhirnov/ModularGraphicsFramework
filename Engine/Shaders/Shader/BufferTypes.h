// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Shaders/Shader/VecTypes.h"

namespace ShaderEditor
{
namespace ShaderNodes
{


	//
	// Buffer
	//
	
	template <typename AccessType, typename DataType>
	struct Buffer final : Node
	{
	// types
	private:
		using Self		= Buffer< AccessType, DataType >;
		using Access_t	= AccessType;
		using Data_t	= DataType;
		using ModulePtr	= Engine::Base::ModulePtr;
		

	// methods
	public:
		Buffer (Node *parent, StringCRef name) :
			Node( parent, name, _Typename(), ENodeType::Buffer )
		{}

		Self &	operator = (const ModulePtr &value);

		DataType const&		Read ()		const;
		DataType &			Write ()	const;

		static Name_t		_Typename ()		{ return "Buffer"; }
	};



	//
	// PushConstants
	//

	template <typename T>
	struct PushConstants final : Node
	{
	// types
	public:
		using Self	= PushConstants<T>;


	// variables
	private:
		T		_data;


	// methods
	public:
		PushConstants (Node *parent, StringCRef name) :
			Node( parent, name, "push_constants", ENodeType::PushConstants ),
			_data{ this, "data" }
		{}

		T const *	operator -> () const	{ return &_data; }
		T *			operator -> ()			{ return &_data; }
	};


}	// ShaderNodes
}	// ShaderEditor
