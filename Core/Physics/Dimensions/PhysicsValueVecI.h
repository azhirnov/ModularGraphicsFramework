// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'


	//
	// Physics Value Vector
	//

	template <typename T, ulong U>
	struct PhysicsValueVec<T,I,U> : public CompileTime::CopyQualifiers< T >
	{
	// types
	public:
		using _is_vector	= bool;
		using Self			= PhysicsValueVec<T,I,U>;
		using PVec2_t		= PhysicsValueVec<T,2,U>;
		using PVec3_t		= PhysicsValueVec<T,3,U>;
		using PVec4_t		= PhysicsValueVec<T,4,U>;
		using Value_t		= T;
		using InnerValue_t	= typename T::Value_t;
		using Dimensions_t	= typename T::Dimensions_t;
		using ValueScale_t	= typename T::ValueScale_t;
		using Vec_t			= GXMath::Vec< InnerValue_t, I, U >;
		using BVec_t		= GXMath::Vec< bool, I, U >;

		static const usize	STATIC_COUNT = I;
		

	private:
		template <typename D, typename S>
		struct _Add {
			typedef PhysicsValue< InnerValue_t, D, S >										Right;
			typedef typename ValueScale_t::template Add< typename Right::ValueScale_t >	conv_add_op_t;
			typedef PhysicsValue< InnerValue_t,
						typename Dimensions_t::template Add< Right::Dimensions_t >,
						conv_add_op_t >														type;
			typedef PhysicsValueVec< type, I, U >											vec;

			ND_ static InnerValue_t Get (const InnerValue_t &left, const InnerValue_t &right) {
				return conv_add_op_t::Get( left, right );
			}
		};
		
		template <typename D, typename S>
		struct _Sub {
			typedef PhysicsValue< InnerValue_t, D, S >										Right;
			typedef typename ValueScale_t::template Sub< typename Right::ValueScale_t >	conv_sub_op_t;
			typedef PhysicsValue< InnerValue_t,
						typename Dimensions_t::template Sub< Right::Dimensions_t >,
						conv_sub_op_t >														type;
			typedef PhysicsValueVec< type, I, U >											vec;

			ND_ static InnerValue_t Get (const InnerValue_t &left, const InnerValue_t &right) {
				return conv_sub_op_t::Get( left, right );
			}
		};
		
		template <typename D, typename S>
		struct _Mul {
			typedef PhysicsValue< InnerValue_t, D, S >										Right;
			typedef typename ValueScale_t::template Mul< typename Right::ValueScale_t >	conv_mul_op_t;
			typedef PhysicsValue< InnerValue_t,
						typename Dimensions_t::template Mul< Right::Dimensions_t >,
						conv_mul_op_t >														type;
			typedef PhysicsValueVec< type, I, U >											vec;

			ND_ static InnerValue_t Get (const InnerValue_t &left, const InnerValue_t &right) {
				return conv_mul_op_t::Get( left, right );
			}
		};
		
		template <typename D, typename S>
		struct _Div {
			typedef PhysicsValue< InnerValue_t, D, S >										Right;
			typedef typename ValueScale_t::template Div< typename Right::ValueScale_t >	conv_div_op_t;
			typedef PhysicsValue< InnerValue_t,
						typename Dimensions_t::template Div< typename Right::Dimensions_t >,
						conv_div_op_t >														type;
			typedef PhysicsValueVec< type, I, U >											vec;

			ND_ static InnerValue_t Get (const InnerValue_t &left, const InnerValue_t &right) {
				return conv_div_op_t::Get( left, right );
			}
		};
		
		template <isize PowNum, isize PowDenom = 1>
		struct _Pow {
			typedef typename CompileTime::Fractional32< PowNum, PowDenom >			pow_t;
			typedef PhysicsValue< InnerValue_t,
						typename Dimensions_t::template Power< pow_t >,
					typename ValueScale_t::template Pow< pow_t > >					type;
			typedef PhysicsValueVec< type, I, U >									vec;
		};

		template <typename NewValueType>
		struct _NewConv {
			typedef typename PhysicsDimensionScale< NewValueType >::template 
						BaseConversion< ValueScale_t::Scale::MANTISSA, ValueScale_t::Scale::EXPONENT,
										ValueScale_t::Power::N, ValueScale_t::Power::D >	type;
			typedef PhysicsValueVec< type, I, U >											vec;
		};

		typedef PhysicsValue< InnerValue_t, typename Dimensions_t::Inverse,
							  typename ValueScale_t::Inverse >						_SelfInversedValue;

		using SelfInversed	= PhysicsValueVec< _SelfInversedValue, I, U >;
		using SquareValue_t	= typename _Pow< 2 >::type;
		using CubeValue_t	= typename _Pow< 3 >::type;


	public:

#	if I == 1
	// variables
		T	x;

	// constructors
		PhysicsValueVec (GX_DEFCTOR): x(0) {}
		PhysicsValueVec (const PhysicsValueVec<T,1,U> &v): x(v.x) {}
		
		template <typename S>
		PhysicsValueVec (const PhysicsValueVec< PhysicsValue< InnerValue_t, Dimensions_t, S >, 1, U > &v): x(v.x) {}

		explicit PhysicsValueVec (T X): x(X) {}
		explicit PhysicsValueVec (const PVec2_t &v): x(v.x) {}
		explicit PhysicsValueVec (const PVec3_t &v): x(v.x) {}
		explicit PhysicsValueVec (const PVec4_t &v): x(v.x) {}
#	endif

#	if I == 2
	// variables
		T	x,
			y;

	// constructors
		PhysicsValueVec (GX_DEFCTOR): x(0), y(0) {}
		PhysicsValueVec (const PVec2_t &v): x(v.x), y(v.y) {}

		template <typename S>
		PhysicsValueVec (const PhysicsValueVec< PhysicsValue< InnerValue_t, Dimensions_t, S >, 2, U > &v): x(v.x), y(v.y) {}

		explicit PhysicsValueVec (T V): x(V), y(V) {}
		explicit PhysicsValueVec (T X, T Y): x(X), y(Y) {}
		explicit PhysicsValueVec (const PVec3_t &v): x(v.x), y(v.y) {}
		explicit PhysicsValueVec (const PVec4_t &v): x(v.x), y(v.y) {}
#	endif

#	if I == 3
	// variables
		T	x,
			y,
			z;

	// constructors
		PhysicsValueVec (GX_DEFCTOR): x(0), y(0), z(0) {}
		PhysicsValueVec (const PVec3_t &v): x(v.x), y(v.y), z(v.z) {}

		template <typename S>
		PhysicsValueVec (const PhysicsValueVec< PhysicsValue< InnerValue_t, Dimensions_t, S >, 3, U > &v): x(v.x), y(v.y), z(v.z) {}

		explicit PhysicsValueVec (T V): x(V), y(V), z(V) {}
		explicit PhysicsValueVec (T X, T Y, T Z): x(X), y(Y), z(Z) {}
		explicit PhysicsValueVec (T X, const PVec2_t &YZ): x(X), y(YZ[0]), z(YZ[1]) {}
		explicit PhysicsValueVec (const PVec2_t &XY, T Z): x(XY[0]), y(XY[1]), z(Z) {}
		explicit PhysicsValueVec (const PVec2_t &XY): x(XY[0]), y(XY[1]), z(0) {}
		explicit PhysicsValueVec (const PVec4_t &v): x(v.x), y(v.y), z(v.z) {}

#	endif

#	if I == 4
	// variables
		T	x,
			y,
			z,
			w;

	// constructors
		PhysicsValueVec (GX_DEFCTOR): x(0), y(0), z(0), w(0) {}
		PhysicsValueVec (const PVec4_t &v): x(v.x), y(v.y), z(v.z), w(v.w) {}

		template <typename S>
		PhysicsValueVec (const PhysicsValueVec< PhysicsValue< InnerValue_t, Dimensions_t, S >, 4, U > &v): x(v.x), y(v.y), z(v.z), w(v.w) {}

		explicit PhysicsValueVec (T V): x(V), y(V), z(V), w(V) {}
		explicit PhysicsValueVec (T X, T Y, T Z, T W): x(X), y(Y), z(Z), w(W) {}
		explicit PhysicsValueVec (const PVec2_t &XY): x(XY[0]), y(XY[1]), z(0), w(0) {}
		explicit PhysicsValueVec (T X, const PVec2_t &YZ, T W): x(X), y(YZ[0]), z(YZ[1]), w(W) {}
		explicit PhysicsValueVec (T X, T Y, const PVec2_t &ZW): x(X), y(Y), z(ZW[0]), w(ZW[1]) {}
		explicit PhysicsValueVec (const PVec2_t &XY, const PVec2_t &ZW): x(XY[0]), y(XY[1]), z(ZW[0]), w(ZW[1]) {}
		explicit PhysicsValueVec (const PVec3_t &XYZ, T W): x(XYZ[0]), y(XYZ[1]), z(XYZ[2]), w(W) {}
		explicit PhysicsValueVec (const PVec3_t &XYZ): x(XYZ[0]), y(XYZ[1]), z(XYZ[2]), w(0) {}
#	endif

#	if I >= 5 or I < 0
		STATIC_WARNING( "not supported!" );
#	endif


		// binary operators
		ND_ T &			operator [] (usize i)				{ ASSUME(i<I);  return ptr()[i]; }
		ND_ T const &	operator [] (usize i)	const		{ ASSUME(i<I);  return ptr()[i]; }

		BVec_t		operator == (const Self & right) const;
		BVec_t		operator != (const Self & right) const;
		BVec_t		operator >  (const Self & right) const;
		BVec_t		operator >= (const Self & right) const;
		BVec_t		operator <  (const Self & right) const;
		BVec_t		operator <= (const Self & right) const;

		Self		operator -  () const;
		
		Self &		operator += (const Self &right);
		Self &		operator -= (const Self &right);

		Self		operator +  (const Self &right)		const;
		Self		operator -  (const Self &right)		const;
		
		Self &		operator += (Value_t right);
		Self &		operator -= (Value_t right);

		Self		operator +  (Value_t right)			const;
		Self		operator -  (Value_t right)			const;

		Self &		operator *= (InnerValue_t right);
		Self &		operator /= (InnerValue_t right);

		Self		operator *  (InnerValue_t right)	const;
		Self		operator /  (InnerValue_t right)	const;
		
		Self &		operator *= (const Vec_t &right);
		Self &		operator /= (const Vec_t &right);

		Self		operator *  (const Vec_t &right)	const;
		Self		operator /  (const Vec_t &right)	const;

		
		template <typename D, typename S>
		auto  operator +  (const PhysicsValueVec< PhysicsValue< InnerValue_t, D, S >, I, U > &right) const;
		
		template <typename D, typename S>
		auto  operator -  (const PhysicsValueVec< PhysicsValue< InnerValue_t, D, S >, I, U > &right) const;
		
		template <typename D, typename S>
		auto  operator *  (const PhysicsValueVec< PhysicsValue< InnerValue_t, D, S >, I, U > &right) const;
		
		template <typename D, typename S>
		auto  operator /  (const PhysicsValueVec< PhysicsValue< InnerValue_t, D, S >, I, U > &right) const;

		
		template <typename D, typename S>
		auto  operator +  (const PhysicsValue< InnerValue_t, D, S > &right) const;
		
		template <typename D, typename S>
		auto  operator -  (const PhysicsValue< InnerValue_t, D, S > &right) const;
		
		template <typename D, typename S>
		auto  operator *  (const PhysicsValue< InnerValue_t, D, S > &right) const;
		
		template <typename D, typename S>
		auto  operator /  (const PhysicsValue< InnerValue_t, D, S > &right) const;

		
		template <typename T0, typename D, typename S, ulong U0>
		friend auto  operator +  (const PhysicsValue< InnerValue_t, D, S > &left, const Self &right);
		
		template <typename T0, typename D, typename S, ulong U0>
		friend auto  operator -  (const PhysicsValue< InnerValue_t, D, S > &left, const Self &right);
		
		template <typename T0, typename D, typename S, ulong U0>
		friend auto  operator *  (const PhysicsValue< InnerValue_t, D, S > &left, const Self &right);
		
		template <typename T0, typename D, typename S, ulong U0>
		friend auto  operator /  (const PhysicsValue< InnerValue_t, D, S > &left, const Self &right);


		friend Self			operator * (InnerValue_t left, const Self &right);
		friend SelfInversed	operator / (InnerValue_t left, const Self &right);
		
		friend Self			operator * (const Vec_t &left, const Self &right);
		friend SelfInversed	operator / (const Vec_t &left, const Self &right);


		// methods
		T *				ptr ()									{ _CheckAlign();  return &x; }
		T const *		ptr ()	const							{ _CheckAlign();  return &x; }

		//Vec_t &		ref ()									{ return ReferenceCast< Vec_t >( *this ); }
		Vec_t const &	ref ()	const							{ _CheckAlign();  return *Cast< Vec_t const *>( &x ); }

		Vec_t			Get ()	const;

		static usize	Count ()								{ return I; }

		
		template <isize PowNum, isize PowDenom>
		auto  Pow ()	const;
		
		template <isize Power>
		ND_ auto  Pow ()	const
		{
			typename _Pow< Power >::vec		ret;
			FOR( i, ret )	ret[i] = (*this)[i].template Pow< Power >();
			return ret;
		}
		
		ND_ auto			Square ()						const	{ return Pow< 2 >(); }

		ND_ auto			Sqrt ()							const	{ return Pow< 1, 2 >(); }
		
		ND_ SelfInversed	Inverse ()						const	{ return SelfInversed( InnerValue_t(1) / (*this) ); }


		ND_ Value_t			Length ()						const	{ return Value_t( ref().Length() ); }
		ND_ SquareValue_t	LengthSqr ()					const	{ return SquareValue_t( ref().LengthSqr() ); }

		ND_ Value_t			Distance (const Self &right)	const	{ return Value_t( ref().Distance( right.ref() ) ); }
		ND_ SquareValue_t	DistanceSqr (const Self &right)	const	{ return SquareValue_t( ref().DistanceSqr( right.ref() ) ); }


		template <typename T2>
		PhysicsValueVec<T2,I,U> Convert () const;

		template <typename B>
		const B		To () const;
		
		template <typename ToValueScale>
		auto		ToScale () const;


		String		ToString ()			const;
		String		ToStringFull ()		const;
		String		ToDebugString ()	const;
		

		// compare methods
		BVec_t		Equal (const Self &right)	const;	// ==
		BVec_t		Less (const Self &right)	const;	// <
		BVec_t		LEqual (const Self &right)	const;	// <=
		BVec_t		Greater (const Self &right)	const;	// >
		BVec_t		GEqual (const Self &right)	const;	// >=
		BVec_t		IsZero ()					const;
		BVec_t		IsNotZero ()				const;

		
#	if I == 1
		ND_ Value_t			Sum ()		const	{ return x; }
		ND_ Value_t			SumAbs ()	const	{ return Abs(x); }
		ND_ SquareValue_t	Area ()		const	{ return x * T(1); }
		ND_ CubeValue_t		Volume ()	const	{ return x * T(1) * T(1); }
#	endif

#	if I == 2
		ND_ Value_t			Sum ()		const	{ return x + y; }
		ND_ Value_t			SumAbs ()	const	{ return Abs(x) + Abs(y); }
		ND_ CubeValue_t		Volume ()	const	{ return x * y * T(1); }
#	endif

#	if I >= 2
		ND_ SquareValue_t	Area ()		const	{ return x * y; }
		ND_ PVec2_t			yx ()		const	{ return PVec2_t( y, x ); }
		ND_ PVec2_t			xy ()		const	{ return PVec2_t( x, y ); }
		ND_ PVec2_t			ox ()		const	{ return PVec2_t( 0, x ); }
		ND_ PVec2_t			oy ()		const	{ return PVec2_t( 0, y ); }
#	endif

#	if I == 3
		ND_ Value_t			Sum ()		const	{ return x + y + z; }
		ND_ Value_t			SumAbs ()	const	{ return Abs(x) + Abs(y) + Abs(z); }
#	endif

#	if I >= 3
		ND_ CubeValue_t		Volume ()	const	{ return x * y * z; }
		ND_ PVec2_t			yz ()		const	{ return PVec2_t( y, z ); }
		ND_ PVec2_t			xz ()		const	{ return PVec2_t( x, z ); }
		ND_ PVec3_t			xyz ()		const	{ return PVec3_t( x, y, z ); }
		ND_ PVec3_t			zyx ()		const	{ return PVec3_t( z, y, x ); }
		ND_ PVec3_t			xzy ()		const	{ return PVec3_t( x, z, y ); }

		ND_ PVec3_t			xoo ()		const	{ return PVec3_t( x, 0, 0 ); }
		ND_ PVec3_t			oyo ()		const	{ return PVec3_t( 0, y, 0 ); }
		ND_ PVec3_t			ooz ()		const	{ return PVec3_t( 0, 0, z ); }

		ND_ PVec3_t			xoz ()		const	{ return PVec3_t( x, 0, z ); }
		ND_ PVec3_t			xyo ()		const	{ return PVec3_t( x, y, 0 ); }
		ND_ PVec3_t			oyz ()		const	{ return PVec3_t( 0, y, z ); }
#	endif

#	if I == 4
		ND_ Value_t			Sum ()		const	{ return x + y + z + w; }
		ND_ Value_t			SumAbs ()	const	{ return Abs(x) + Abs(y) + Abs(z) + Abs(w); }
#	endif

#	if I >= 4
		ND_ PVec2_t			zw ()		const	{ return PVec2_t( z, w ); }
		ND_ PVec4_t			xyzw ()		const	{ return PVec4_t( x, y, z, w ); }
		ND_ PVec4_t			wzyx ()		const	{ return PVec4_t( w, z, y, x ); }
#	endif


	private:
		static void _CheckAlign ();
	};


	
/*
=================================================
	operator ==
=================================================
*/
	template <typename T, ulong U>
	ND_ inline GXMath::Vec<bool,I,U>  PhysicsValueVec<T,I,U>::operator == (const Self & right) const
	{
		return Equal( right );
	}
	
/*
=================================================
	operator !=
=================================================
*/
	template <typename T, ulong U>
	ND_ inline GXMath::Vec<bool,I,U>  PhysicsValueVec<T,I,U>::operator != (const Self & right) const
	{
		return not Equal( right );
	}
	
/*
=================================================
	operator >
=================================================
*/
	template <typename T, ulong U>
	ND_ inline GXMath::Vec<bool,I,U>  PhysicsValueVec<T,I,U>::operator >  (const Self & right) const
	{
		return Greater( right );
	}
	
/*
=================================================
	operator >=
=================================================
*/
	template <typename T, ulong U>
	ND_ inline GXMath::Vec<bool,I,U>  PhysicsValueVec<T,I,U>::operator >= (const Self & right) const
	{
		return GEqual( right );
	}
	
/*
=================================================
	operator <
=================================================
*/
	template <typename T, ulong U>
	ND_ inline GXMath::Vec<bool,I,U>  PhysicsValueVec<T,I,U>::operator <  (const Self & right) const
	{
		return Less( right );
	}
	
/*
=================================================
	operator <=
=================================================
*/
	template <typename T, ulong U>
	ND_ inline GXMath::Vec<bool,I,U>  PhysicsValueVec<T,I,U>::operator <= (const Self & right) const
	{
		return LEqual( right );
	}
	
/*
=================================================
	operator -
=================================================
*/
	template <typename T, ulong U>
	ND_ inline PhysicsValueVec<T,I,U>  PhysicsValueVec<T,I,U>::operator -  () const
	{
		Self	ret;
		FOR( i, ret )		ret[i] = -(*this)[i];
		return ret;
	}
	
/*
=================================================
	operator +=
=================================================
*/
	template <typename T, ulong U>
	inline PhysicsValueVec<T,I,U> &  PhysicsValueVec<T,I,U>::operator += (const Self &right)
	{
		FOR( i, *this )		(*this)[i] += right[i];
		return *this;
	}
	
/*
=================================================
	operator -=
=================================================
*/
	template <typename T, ulong U>
	inline PhysicsValueVec<T,I,U> &  PhysicsValueVec<T,I,U>::operator -= (const Self &right)
	{
		FOR( i, *this )		(*this)[i] -= right[i];
		return *this;
	}
	
/*
=================================================
	operator +=
=================================================
*/
	template <typename T, ulong U>
	inline PhysicsValueVec<T,I,U> &  PhysicsValueVec<T,I,U>::operator += (Value_t right)
	{
		FOR( i, *this )		(*this)[i] += right;
		return *this;
	}
	
/*
=================================================
	operator -=
=================================================
*/
	template <typename T, ulong U>
	inline PhysicsValueVec<T,I,U> &  PhysicsValueVec<T,I,U>::operator -= (Value_t right)
	{
		FOR( i, *this )		(*this)[i] -= right;
		return *this;
	}
	
/*
=================================================
	operator +
=================================================
*/
	template <typename T, ulong U>
	ND_ inline PhysicsValueVec<T,I,U>  PhysicsValueVec<T,I,U>::operator +  (const Self &right) const
	{
		Self	ret;
		FOR( i, ret )		ret[i] = (*this)[i] + right[i];
		return ret;
	}
	
/*
=================================================
	operator -
=================================================
*/
	template <typename T, ulong U>
	ND_ inline PhysicsValueVec<T,I,U>  PhysicsValueVec<T,I,U>::operator -  (const Self &right) const
	{
		Self	ret;
		FOR( i, ret )		ret[i] = (*this)[i] - right[i];
		return ret;
	}
	
/*
=================================================
	operator +
=================================================
*/
	template <typename T, ulong U>
	ND_ inline PhysicsValueVec<T,I,U>  PhysicsValueVec<T,I,U>::operator +  (Value_t right) const
	{
		Self	ret;
		FOR( i, ret )		ret[i] = (*this)[i] + right;
		return ret;
	}
	
/*
=================================================
	operator -
=================================================
*/
	template <typename T, ulong U>
	ND_ inline PhysicsValueVec<T,I,U>  PhysicsValueVec<T,I,U>::operator -  (Value_t right) const
	{
		Self	ret;
		FOR( i, ret )		ret[i] = (*this)[i] - right;
		return ret;
	}
	
/*
=================================================
	operator *=
=================================================
*/
	template <typename T, ulong U>
	inline PhysicsValueVec<T,I,U> &  PhysicsValueVec<T,I,U>::operator *= (InnerValue_t right)
	{
		FOR( i, *this )		(*this)[i] *= right;
		return *this;
	}
	
/*
=================================================
	operator /=
=================================================
*/
	template <typename T, ulong U>
	inline PhysicsValueVec<T,I,U> &  PhysicsValueVec<T,I,U>::operator /= (InnerValue_t right)
	{
		FOR( i, *this )		(*this)[i] /= right;
		return *this;
	}
	
/*
=================================================
	operator *
=================================================
*/
	template <typename T, ulong U>
	ND_ inline PhysicsValueVec<T,I,U>  PhysicsValueVec<T,I,U>::operator *  (InnerValue_t right) const
	{
		Self	ret;
		FOR( i, ret )		ret[i] = (*this)[i] * right;
		return ret;
	}
	
/*
=================================================
	operator /
=================================================
*/
	template <typename T, ulong U>
	ND_ inline PhysicsValueVec<T,I,U>  PhysicsValueVec<T,I,U>::operator /  (InnerValue_t right) const
	{
		Self	ret;
		FOR( i, ret )		ret[i] = (*this)[i] / right;
		return ret;
	}
	
/*
=================================================
	operator *=
=================================================
*/
	template <typename T, ulong U>
	inline PhysicsValueVec<T,I,U> &  PhysicsValueVec<T,I,U>::operator *= (const Vec_t &right)
	{
		FOR( i, *this )		(*this)[i] *= right[i];
		return *this;
	}
	
/*
=================================================
	operator /=
=================================================
*/
	template <typename T, ulong U>
	inline PhysicsValueVec<T,I,U> &  PhysicsValueVec<T,I,U>::operator /= (const Vec_t &right)
	{
		FOR( i, *this )		(*this)[i] /= right[i];
		return *this;
	}
	
/*
=================================================
	operator *
=================================================
*/
	template <typename T, ulong U>
	ND_ inline PhysicsValueVec<T,I,U>  PhysicsValueVec<T,I,U>::operator *  (const Vec_t &right) const
	{
		Self	ret;
		FOR( i, ret )		ret[i] = (*this)[i] * right[i];
		return ret;
	}
	
/*
=================================================
	operator /
=================================================
*/
	template <typename T, ulong U>
	ND_ inline PhysicsValueVec<T,I,U>  PhysicsValueVec<T,I,U>::operator /  (const Vec_t &right)	const
	{
		Self	ret;
		FOR( i, ret )		ret[i] = (*this)[i] / right[i];
		return ret;
	}
	
/*
=================================================
	operator +
=================================================
*/
	template <typename T, ulong U>
	template <typename D, typename S>
	ND_ inline auto  PhysicsValueVec<T,I,U>::operator +  (const PhysicsValueVec< PhysicsValue< InnerValue_t, D, S >, I, U > &right) const
	{
		typename _Add< D, S >::vec	ret;
		FOR( i, ret )		ret[i] = (*this)[i] + right[i];
		return ret;
	}
		
/*
=================================================
	operator -
=================================================
*/
	template <typename T, ulong U>
	template <typename D, typename S>
	ND_ inline auto  PhysicsValueVec<T,I,U>::operator -  (const PhysicsValueVec< PhysicsValue< InnerValue_t, D, S >, I, U > &right) const
	{
		typename _Sub< D, S >::vec	ret;
		FOR( i, ret )		ret[i] = (*this)[i] - right[i];
		return ret;
	}
		
/*
=================================================
	operator *
=================================================
*/
	template <typename T, ulong U>
	template <typename D, typename S>
	ND_ inline auto  PhysicsValueVec<T,I,U>::operator *  (const PhysicsValueVec< PhysicsValue< InnerValue_t, D, S >, I, U > &right) const
	{
		typename _Mul< D, S >::vec	ret;
		FOR( i, ret )		ret[i] = (*this)[i] * right[i];
		return ret;
	}
		
/*
=================================================
	operator /
=================================================
*/
	template <typename T, ulong U>
	template <typename D, typename S>
	ND_ inline auto  PhysicsValueVec<T,I,U>::operator /  (const PhysicsValueVec< PhysicsValue< InnerValue_t, D, S >, I, U > &right) const
	{
		typename _Div< D, S >::vec	ret;
		FOR( i, ret )		ret[i] = (*this)[i] / right[i];
		return ret;
	}
	
/*
=================================================
	operator +
=================================================
*/
	template <typename T, ulong U>
	template <typename D, typename S>
	ND_ inline auto  PhysicsValueVec<T,I,U>::operator +  (const PhysicsValue< InnerValue_t, D, S > &right) const
	{
		typename _Add< D, S >::vec	ret;
		FOR( i, ret )		ret[i] = (*this)[i] + right;
		return ret;
	}
		
/*
=================================================
	operator -
=================================================
*/
	template <typename T, ulong U>
	template <typename D, typename S>
	ND_ inline auto  PhysicsValueVec<T,I,U>::operator -  (const PhysicsValue< InnerValue_t, D, S > &right) const
	{
		typename _Sub< D, S >::vec	ret;
		FOR( i, ret )		ret[i] = (*this)[i] - right;
		return ret;
	}
		
/*
=================================================
	operator *
=================================================
*/
	template <typename T, ulong U>
	template <typename D, typename S>
	ND_ inline auto  PhysicsValueVec<T,I,U>::operator *  (const PhysicsValue< InnerValue_t, D, S > &right) const
	{
		typename _Mul< D, S >::vec	ret;
		FOR( i, ret )		ret[i] = (*this)[i] * right;
		return ret;
	}
		
/*
=================================================
	operator /
=================================================
*/
	template <typename T, ulong U>
	template <typename D, typename S>
	ND_ inline auto  PhysicsValueVec<T,I,U>::operator /  (const PhysicsValue< InnerValue_t, D, S > &right) const
	{
		typename _Div< D, S >::vec	ret;
		FOR( i, ret )		ret[i] = (*this)[i] / right;
		return ret;
	}

/*
=================================================
	operator *
=================================================
*/
	template <typename T, ulong U>
	ND_ inline auto  operator * (typename PhysicsValueVec<T,I,U>::InnerValue_t left, const PhysicsValueVec<T,I,U> &right)
	{
		PhysicsValueVec<T,I,U>	ret;
		FOR( i, ret )		ret[i] = left * right[i];
		return ret;
	}
	
/*
=================================================
	operator /
=================================================
*/
	template <typename T, ulong U>
	ND_ inline auto  operator / (typename PhysicsValueVec<T,I,U>::InnerValue_t left, const PhysicsValueVec<T,I,U> &right)
	{
		typename PhysicsValueVec<T,I,U>::SelfInversed	ret;
		FOR( i, ret )		ret[i] = left / right[i];
		return ret;
	}
	
/*
=================================================
	operator *
=================================================
*/
	template <typename T, ulong U>
	ND_ inline auto  operator * (const typename PhysicsValueVec<T,I,U>::Vec_t &left, const PhysicsValueVec<T,I,U> &right)
	{
		PhysicsValueVec<T,I,U>	ret;
		FOR( i, ret )		ret[i] = left[i] * right[i];
		return ret;
	}
	
/*
=================================================
	operator /
=================================================
*/
	template <typename T, ulong U>
	ND_ inline auto  operator / (const typename PhysicsValueVec<T,I,U>::Vec_t &left, const PhysicsValueVec<T,I,U> &right)
	{
		typename PhysicsValueVec<T,I,U>::SelfInversed	ret;
		FOR( i, ret )		ret[i] = left[i] / right[i];
		return ret;
	}
	
/*
=================================================
	Get
=================================================
*/
	template <typename T, ulong U>
	ND_ inline typename PhysicsValueVec<T,I,U>::Vec_t  PhysicsValueVec<T,I,U>::Get () const
	{
		typename PhysicsValueVec<T,I,U>::Vec_t	ret;
		FOR( i, ret )	ret[i] = (*this)[i].Get();
		return ret;
	}
		
/*
=================================================
	Equal
=================================================
*/
	template <typename T, ulong U>
	ND_ inline GXMath::Vec<bool,I,U>  PhysicsValueVec<T,I,U>::Equal (const Self &right) const
	{
		GXMath::Vec<bool,I,U>	ret;
		FOR( i, *this )	ret[i] = GXMath::Equals( (*this)[i], right[i] );
		return ret;
	}
		
/*
=================================================
	Less
=================================================
*/
	template <typename T, ulong U>
	ND_ inline GXMath::Vec<bool,I,U>  PhysicsValueVec<T,I,U>::Less (const Self &right) const
	{
		GXMath::Vec<bool,I,U>	ret;
		FOR( i, *this )	ret[i] = ( (*this)[i] < right[i] ) & not GXMath::Equals( (*this)[i], right[i] );
		return ret;
	}
		
/*
=================================================
	LEqual
=================================================
*/
	template <typename T, ulong U>
	ND_ inline GXMath::Vec<bool,I,U>  PhysicsValueVec<T,I,U>::LEqual (const Self &right) const
	{
		GXMath::Vec<bool,I,U>	ret;
		FOR( i, *this )	ret[i] = ( (*this)[i] < right[i] ) | GXMath::Equals( (*this)[i], right[i] );
		return ret;
	}

/*
=================================================
	Greater
=================================================
*/
	template <typename T, ulong U>
	ND_ inline GXMath::Vec<bool,I,U>  PhysicsValueVec<T,I,U>::Greater (const Self &right) const
	{
		GXMath::Vec<bool,I,U>	ret;
		FOR( i, *this )	ret[i] = ( (*this)[i] > right[i] ) & not GXMath::Equals( (*this)[i], right[i] );
		return ret;
	}
		
/*
=================================================
	GEqual
=================================================
*/
	template <typename T, ulong U>
	ND_ inline GXMath::Vec<bool,I,U>  PhysicsValueVec<T,I,U>::GEqual (const Self &right) const
	{
		GXMath::Vec<bool,I,U>	ret;
		FOR( i, *this )	ret[i] = ( (*this)[i] > right[i] ) | GXMath::Equals( (*this)[i], right[i] );
		return ret;
	}
		
/*
=================================================
	IsZero
=================================================
*/
	template <typename T, ulong U>
	ND_ inline GXMath::Vec<bool,I,U>  PhysicsValueVec<T,I,U>::IsZero () const
	{
		GXMath::Vec<bool,I,U>	ret;
		FOR( i, *this )	ret[i] = (*this)[i].IsZero();
		return ret;
	}
		
/*
=================================================
	IsNotZero
=================================================
*/
	template <typename T, ulong U>
	ND_ inline GXMath::Vec<bool,I,U>  PhysicsValueVec<T,I,U>::IsNotZero () const
	{
		return not IsZero();
	}
	
/*
=================================================
	Pow
=================================================
*/
	template <typename T, ulong U>
	template <isize PowNum, isize PowDenom>
	ND_ inline auto  PhysicsValueVec<T,I,U>::Pow () const
	{
		typename _Pow< PowNum, PowDenom >::vec	ret;
		FOR( i, ret )	ret[i] = (*this)[i].template Pow< PowNum, PowDenom >();
		return ret;
	}
	
/*
=================================================
	Convert
=================================================
*/
	template <typename T, ulong U>
	template <typename T2>
	ND_ inline PhysicsValueVec<T2,I,U>  PhysicsValueVec<T,I,U>::Convert () const
	{
		PhysicsValueVec<T2,I,U>	ret;
		FOR( i, ret )	ret[i] = (*this)[i].template Convert< T2 >();
		return ret;
	}
	
/*
=================================================
	To
=================================================
*/
	template <typename T, ulong U>
	template <typename B>
	ND_ inline const B  PhysicsValueVec<T,I,U>::To () const
	{
		STATIC_ASSERT( typename B::_is_vector(true), "type is not vector" );
		
		B				ret;
		const usize		count = GXMath::Min( Count(), B::Count() );

		FOR_range( i, 0, count )	ret[i] = typename B::Value_t( (*this)[i] );
		return ret;
	}
	
/*
=================================================
	ToScale
=================================================
*/
	template <typename T, ulong U>
	template <typename ToValueScale>
	ND_ inline auto  PhysicsValueVec<T,I,U>::ToScale () const
	{
		PhysicsValueVec< PhysicsValue< InnerValue_t, Dimensions_t, ToValueScale >, I, U >	ret;
		FOR( i, ret )	ret[i] = (*this)[i].template ToScale< ToValueScale >();
		return ret;
	}
	
/*
=================================================
	ToString
=================================================
*/
	template <typename T, ulong U>
	ND_ inline String  PhysicsValueVec<T,I,U>::ToString () const
	{
		return ('(' >> Get().ToString()) << ") * [" << Dimensions_t::ToString("*") << ']';
	}
	
/*
=================================================
	ToStringFull
=================================================
*/
	template <typename T, ulong U>
	ND_ inline String  PhysicsValueVec<T,I,U>::ToStringFull () const
	{
		return ('(' >> Get().ToStringFull()) << ") * [" << Dimensions_t::ToString("*") << ']';
	}
	
/*
=================================================
	ToDebugString
=================================================
*/
	template <typename T, ulong U>
	ND_ inline String  PhysicsValueVec<T,I,U>::ToDebugString () const
	{
		return ('(' >> ref().ToStringFull()) << ") * (" << ValueScale_t::Scale::ToString()
					<< ")^" << ValueScale_t::Power::ToString() << " [" << Dimensions_t::ToString("*") << ']';
	}
	
/*
=================================================
	_CheckAlign
=================================================
*/
	template <typename T, ulong U>
	inline void PhysicsValueVec<T,I,U>::_CheckAlign ()
	{
		STATIC_ASSERT( sizeof(T) == sizeof(InnerValue_t) );

		#if I == 1
			// do nothing

		#elif I == 2
			STATIC_ASSERT( offsetof(Self, x) + sizeof(T) == offsetof(Self, y) );
			STATIC_ASSERT( sizeof(T[I-1]) == (offsetof(Self, y) - offsetof(Self, x)) );

		#elif I == 3
			STATIC_ASSERT( offsetof(Self, x) + sizeof(T) == offsetof(Self, y) );
			STATIC_ASSERT( offsetof(Self, y) + sizeof(T) == offsetof(Self, z) );
			STATIC_ASSERT( sizeof(T[I-1]) == (offsetof(Self, z) - offsetof(Self, x)) );

		#elif I == 4
			STATIC_ASSERT( offsetof(Self, x) + sizeof(T) == offsetof(Self, y) );
			STATIC_ASSERT( offsetof(Self, y) + sizeof(T) == offsetof(Self, z) );
			STATIC_ASSERT( offsetof(Self, z) + sizeof(T) == offsetof(Self, w) );
			STATIC_ASSERT( sizeof(T[I-1]) == (offsetof(Self, w) - offsetof(Self, x)) );

		#endif
	}

