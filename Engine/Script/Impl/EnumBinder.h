// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Script/Impl/ClassBinder.h"

namespace GX_STL
{
namespace GXScript
{

	//
	// Enum Binder
	//
	template <typename T>
	class EnumBinder final
	{
	// types
	public:
		using Self		= EnumBinder<T>;
		using Enum_t	= T;


	// variables
	private:
		ScriptEnginePtr		_engine;
		String				_name;


	// methods
	public:
		explicit EnumBinder (const ScriptEnginePtr &eng);

		bool Create ();

		bool AddValue (StringCRef name, T value);
	};



	//
	// Enum Bitfield Binder
	//
	template <typename T>
	class EnumBitfieldBinder
	{
	// types
	public:
		using Self			= EnumBitfieldBinder<T>;
		using Bitfield_t	= T;
		using Enum_t		= typename Bitfield_t::Index_t;


	// variables
	private:
		ClassBinder<T>	_binder;


	// methods
	public:
		explicit EnumBitfieldBinder (const ScriptEnginePtr &eng);

		// operators
		Self&	Op_And ();
		Self&	Op_Or ();
		Self&	Op_Xor ();

		// methods
		Self&	Op_Set ();
		Self&	Op_Reset ();
		Self&	Op_Get ();

		Self&	BindDefaults ();
	};
//-----------------------------------------------------------------------------


	
/*
=================================================
	constructor
=================================================
*/
	template <typename T>
	inline EnumBinder<T>::EnumBinder (const ScriptEnginePtr &eng) :
		_engine{ eng }
	{
		ScriptTypeInfo< T >::Name( OUT _name );
	}

/*
=================================================
	Create
=================================================
*/
	template <typename T>
	inline bool EnumBinder<T>::Create ()
	{
		int	res = _engine->Get()->RegisterEnum( _name.cstr() );
		
		if ( res < 0 and res != AngelScript::asALREADY_REGISTERED )
		{
			AS_CALL( res );
			RETURN_ERR( "enum '" << _name << "' already registerd" );
		}
		return true;
	}
	
/*
=================================================
	AddValue
=================================================
*/
	template <typename T>
	inline bool EnumBinder<T>::AddValue (StringCRef valueName, T value)
	{
		ASSERT( ilong(value) >= MinValue<int>() and ilong(value) <= MaxValue<int>() );

		AS_CALL_R( _engine->Get()->RegisterEnumValue( _name.cstr(), (String(_name) << '_' << valueName).cstr(), value ) );
		return true;
	}
//-----------------------------------------------------------------------------

	

/*
=================================================
	constructor
=================================================
*/
	template <typename T>
	inline EnumBitfieldBinder<T>::EnumBitfieldBinder (const ScriptEnginePtr &eng) :
		_binder( eng )
	{
		_binder.CreateClassValue();

		struct Ctors {
			static void FromEnum (void *dst, Enum_t e) {
				new(dst) Bitfield_t{ e };
			}
		};

		_binder.AddConstructor( &Ctors::FromEnum );
	}

/*
=================================================
	operators
=================================================
*/
	template <typename T>
	inline EnumBitfieldBinder<T>&  EnumBitfieldBinder<T>::Op_And ()
	{
		_binder.Operators()
			.BinaryRH( EBinaryOperator::And, static_cast< Bitfield_t (*)(Enum_t, const Bitfield_t&) >( &operator & ))
			.And<Enum_t>()		.AndAssign<Enum_t>()
			.And<Bitfield_t>()	.AndAssign<Bitfield_t>();

		return *this;
	}

	template <typename T>
	inline EnumBitfieldBinder<T>&  EnumBitfieldBinder<T>::Op_Or ()
	{
		_binder.Operators()
			.BinaryRH( EBinaryOperator::Or, static_cast< Bitfield_t (*)(Enum_t, const Bitfield_t&) >( &operator | ))
			.Or<Enum_t>()		.OrAssign<Enum_t>()
			.Or<Bitfield_t>()	.OrAssign<Bitfield_t>();

		return *this;
	}

	template <typename T>
	inline EnumBitfieldBinder<T>&  EnumBitfieldBinder<T>::Op_Xor ()
	{
		_binder.Operators()
			.BinaryRH( EBinaryOperator::Xor, static_cast< Bitfield_t (*)(Enum_t, const Bitfield_t&) >( &operator ^ ))
			.Xor<Enum_t>()		.XorAssign<Enum_t>()
			.Xor<Bitfield_t>()	.XorAssign<Bitfield_t>();

		return *this;
	}
	
/*
=================================================
	methods
=================================================
*/
	template <typename T>
	inline EnumBitfieldBinder<T>&  EnumBitfieldBinder<T>::Op_Set ()
	{
		_binder.AddMethod( &Bitfield_t::Set, "Set" );
		return *this;
	}

	template <typename T>
	inline EnumBitfieldBinder<T>&  EnumBitfieldBinder<T>::Op_Reset ()
	{
		_binder.AddMethod( &Bitfield_t::Reset, "Reset" );
		return *this;
	}

	template <typename T>
	inline EnumBitfieldBinder<T>&  EnumBitfieldBinder<T>::Op_Get ()
	{
		_binder.AddMethod( &Bitfield_t::Get, "Get" );
		return *this;
	}
	
/*
=================================================
	BindDefaults
=================================================
*/
	template <typename T>
	inline EnumBitfieldBinder<T>&  EnumBitfieldBinder<T>::BindDefaults ()
	{
		return (*this).Op_Set().Op_Reset().Op_Get().Op_Or();
	}

}	// GXScript
}	// GX_STL
