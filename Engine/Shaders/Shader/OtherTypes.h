// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Shaders/Shader/VecTypes.h"

namespace ShaderEditor
{
namespace ShaderNodes
{
	//
	// Array (static)
	//

	template <typename T, uint I>
	struct Array : Node
	{
	// types
	public:
		using Self		= Array< T, I >;
		using Value_t	= T;
		

	// variables
	private:
		GX_STL::GXTypes::StaticArray< T, I >		_values;


	// methods
	public:
		Array (GX_DEFCTOR) {}


		Array (Node *parent, StringCRef name) :
			Node( parent, name, _Typename(), ENodeType::Array )
		{
			TODO( "" );
			/*FOR( i, _values ) {
				_values[i] = RVREF( T( this, Name_t("[") << i << "]" ) );
			}*/
		}


		Array (Self &&node)
		{
			TODO( "" );
			/*FOR( i, _values ) {
				_values[i] = RVREF( other._values[i] );
			}
			_Move( node );*/
		}


		T const&	operator [] (uint index) const;

		static Name_t  _Typename ();
	};



	//
	// Shader Input
	//

	template <typename T>
	struct Input : Node
	{
	};



	//
	// Shader Output
	//

	template <typename T>
	struct Output : Node
	{
	};



	//
	// Local Shared
	//

	template <typename T>
	struct LocalShared : Node
	{
	// types
	public:
		using Self		= LocalShared<T>;
		using Value_t	= T;


	// variables
	private:
		T		_data;


	// methods
	public:
		LocalShared (Node *parent, StringCRef name) :
			Node( parent, name, "local_shared", ENodeType::LocalShared ),
			_data{ this, "data" }
		{}
	};

	
	
	template <typename T, uint I>
	inline Node::Name_t  Array<T,I>::_Typename ()
	{
		return T::_Typename() << '[' << Name_t().FormatI( I, 10 ) << ']';
	}
	
	
	template <typename T, uint I>
	inline T const&  Array<T,I>::operator [] (uint index) const
	{
		ASSERT( index < I );
		ASSERT( I == _SelfNode()->Fields().Count() );

		return	index < I ? _SelfNode()->Fields()[ index ] :
				*((T const*) null);
	}

}	// ShaderNodes
}	// ShaderEditor
