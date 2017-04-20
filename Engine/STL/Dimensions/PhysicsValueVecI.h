// Copyright © 2014-2017  Zhirnov Andrey. All rights reserved.


	//
	// Physics Value Vector
	//

	template <typename T>
	struct PhysicsValueVec<T,I> : public CompileTime::CopyQualifiers< T >
	{
	// types
	public:
		typedef bool						_is_vector;
		typedef PhysicsValueVec<T,I>		Self;
		typedef T							value_t;
		typedef typename T::value_t			inner_value_t;
		typedef typename T::dimensions_t	dimensions_t;
		typedef typename T::value_scale_t	value_scale_t;
		typedef Vec< inner_value_t, I >		vec_t;
		typedef Vec< bool, I >				bvec_t;

		static const usize	STATIC_COUNT = I;
		

	private:
		template <typename D, typename S>
		struct _Add {
			typedef PhysicsValue< inner_value_t, D, S >										Right;
			typedef typename value_scale_t::template Add< typename Right::value_scale_t >	conv_add_op_t;
			typedef PhysicsValue< inner_value_t,
						typename dimensions_t::template Add< Right::dimensions_t >,
						typename conv_add_op_t >											type;
			typedef PhysicsValueVec< type, I >												vec;

			static inner_value_t Get (const inner_value_t &left, const inner_value_t &right) {
				return conv_add_op_t::Get( left, right );
			}
		};
		
		template <typename D, typename S>
		struct _Sub {
			typedef PhysicsValue< inner_value_t, D, S >										Right;
			typedef typename value_scale_t::template Sub< typename Right::value_scale_t >	conv_sub_op_t;
			typedef PhysicsValue< inner_value_t,
						typename dimensions_t::template Sub< Right::dimensions_t >,
						typename conv_sub_op_t >											type;
			typedef PhysicsValueVec< type, I >												vec;

			static inner_value_t Get (const inner_value_t &left, const inner_value_t &right) {
				return conv_sub_op_t::Get( left, right );
			}
		};
		
		template <typename D, typename S>
		struct _Mul {
			typedef PhysicsValue< inner_value_t, D, S >										Right;
			typedef typename value_scale_t::template Mul< typename Right::value_scale_t >	conv_mul_op_t;
			typedef PhysicsValue< inner_value_t,
						typename dimensions_t::template Mul< Right::dimensions_t >,
						typename conv_mul_op_t >											type;
			typedef PhysicsValueVec< type, I >												vec;

			static inner_value_t Get (const inner_value_t &left, const inner_value_t &right) {
				return conv_mul_op_t::Get( left, right );
			}
		};
		
		template <typename D, typename S>
		struct _Div {
			typedef PhysicsValue< inner_value_t, D, S >										Right;
			typedef typename value_scale_t::template Div< typename Right::value_scale_t >	conv_div_op_t;
			typedef PhysicsValue< inner_value_t,
						typename dimensions_t::template Div< Right::dimensions_t >,
						typename conv_div_op_t >											type;
			typedef PhysicsValueVec< type, I >												vec;

			static inner_value_t Get (const inner_value_t &left, const inner_value_t &right) {
				return conv_div_op_t::Get( left, right );
			}
		};
		
		template <isize PowNum, isize PowDenom = 1>
		struct _Pow {
			typedef typename CompileTime::Fractional32< PowNum, PowDenom >			pow_t;
			typedef PhysicsValue< inner_value_t,
						typename dimensions_t::template Power< pow_t >,
						typename value_scale_t::template Pow< pow_t > >				type;
			typedef PhysicsValueVec< type, I >										vec;
		};

		template <typename NewValueType>
		struct _NewConv {
			typedef typename PhysicsDimensionScale< NewValueType >::template 
						BaseConversion< value_scale_t::Scale::MANTISSA, value_scale_t::Scale::EXPONENT,
										value_scale_t::Power::N, value_scale_t::Power::D >	type;
			typedef PhysicsValueVec< type, I >												vec;
		};

		typedef PhysicsValue< inner_value_t, typename dimensions_t::Inverse,
							  typename value_scale_t::Inverse >						_SelfInversedValue;

		typedef PhysicsValueVec< _SelfInversedValue, I >							SelfInversed;

		typedef typename _Pow< 2 >::type											square_value_t;
		typedef typename _Pow< 3 >::type											cube_value_t;


	public:

#	if I == 1
	// variables
		T	x;

	// constructors
		PhysicsValueVec (GX_DEFCTOR): x(0) {}
		PhysicsValueVec (const PhysicsValueVec<T,1> &v): x(v.x) {}
		
		template <typename S>
		PhysicsValueVec (const PhysicsValueVec< PhysicsValue< inner_value_t, dimensions_t, S >, 1 > &v): x(v.x) {}

		explicit PhysicsValueVec (T X): x(X) {}
		explicit PhysicsValueVec (const PhysicsValueVec<T,2> &v): x(v.x) {}
		explicit PhysicsValueVec (const PhysicsValueVec<T,3> &v): x(v.x) {}
		explicit PhysicsValueVec (const PhysicsValueVec<T,4> &v): x(v.x) {}
#	endif

#	if I == 2
	// variables
		T	x,
			y;

	// constructors
		PhysicsValueVec (GX_DEFCTOR): x(0), y(0) {}
		PhysicsValueVec (const PhysicsValueVec<T,2> &v): x(v.x), y(v.y) {}

		template <typename S>
		PhysicsValueVec (const PhysicsValueVec< PhysicsValue< inner_value_t, dimensions_t, S >, 2 > &v): x(v.x), y(v.y) {}

		explicit PhysicsValueVec (T V): x(V), y(V) {}
		explicit PhysicsValueVec (T X, T Y): x(X), y(Y) {}
		explicit PhysicsValueVec (const PhysicsValueVec<T,3> &v): x(v.x), y(v.y) {}
		explicit PhysicsValueVec (const PhysicsValueVec<T,4> &v): x(v.x), y(v.y) {}
#	endif

#	if I == 3
	// variables
		T	x,
			y,
			z;

	// constructors
		PhysicsValueVec (GX_DEFCTOR): x(0), y(0), z(0) {}
		PhysicsValueVec (const PhysicsValueVec<T,3> &v): x(v.x), y(v.y), z(v.z) {}

		template <typename S>
		PhysicsValueVec (const PhysicsValueVec< PhysicsValue< inner_value_t, dimensions_t, S >, 3 > &v): x(v.x), y(v.y), z(v.z) {}

		explicit PhysicsValueVec (T V): x(V), y(V), z(V) {}
		explicit PhysicsValueVec (T X, T Y, T Z): x(X), y(Y), z(Z) {}
		explicit PhysicsValueVec (T X, const PhysicsValueVec<T,2> &YZ): x(X), y(YZ[0]), z(YZ[1]) {}
		explicit PhysicsValueVec (const PhysicsValueVec<T,2> &XY, T Z): x(XY[0]), y(XY[1]), z(Z) {}
		explicit PhysicsValueVec (const PhysicsValueVec<T,2> &XY): x(XY[0]), y(XY[1]), z(0) {}
		explicit PhysicsValueVec (const PhysicsValueVec<T,4> &v): x(v.x), y(v.y), z(v.z) {}

#	endif

#	if I == 4
	// variables
		T	x,
			y,
			z,
			w;

	// constructors
		PhysicsValueVec (GX_DEFCTOR): x(0), y(0), z(0), w(0) {}
		PhysicsValueVec (const PhysicsValueVec<T,4> &v): x(v.x), y(v.y), z(v.z), w(v.w) {}

		template <typename S>
		PhysicsValueVec (const PhysicsValueVec< PhysicsValue< inner_value_t, dimensions_t, S >, 4 > &v): x(v.x), y(v.y), z(v.z), w(v.w) {}

		explicit PhysicsValueVec (T V): x(V), y(V), z(V), w(V) {}
		explicit PhysicsValueVec (T X, T Y, T Z, T W): x(X), y(Y), z(Z), w(W) {}
		explicit PhysicsValueVec (const PhysicsValueVec<T,2> &XY): x(XY[0]), y(XY[1]), z(0), w(0) {}
		explicit PhysicsValueVec (T X, const PhysicsValueVec<T,2> &YZ, T W): x(X), y(YZ[0]), z(YZ[1]), w(W) {}
		explicit PhysicsValueVec (T X, T Y, const PhysicsValueVec<T,2> &ZW): x(X), y(Y), z(ZW[0]), w(ZW[1]) {}
		explicit PhysicsValueVec (const PhysicsValueVec<T,2> &XY, const PhysicsValueVec<T,2> &ZW): x(XY[0]), y(XY[1]), z(ZW[0]), w(ZW[1]) {}
		explicit PhysicsValueVec (const PhysicsValueVec<T,3> &XYZ, T W): x(XYZ[0]), y(XYZ[1]), z(XYZ[2]), w(W) {}
		explicit PhysicsValueVec (const PhysicsValueVec<T,3> &XYZ): x(XYZ[0]), y(XYZ[1]), z(XYZ[2]), w(0) {}
#	endif

#	if I >= 5 or I < 0
		STATIC_WARNING( "not supported!" );
#	endif


		// binary operators
		T &			operator [] (usize i)						{ ASSUME(i<I);  return ptr()[i]; }
		T const &	operator [] (usize i)			const		{ ASSUME(i<I);  return ptr()[i]; }

		bvec_t		operator == (const Self & right) const;
		bvec_t		operator != (const Self & right) const;
		bvec_t		operator >  (const Self & right) const;
		bvec_t		operator >= (const Self & right) const;
		bvec_t		operator <  (const Self & right) const;
		bvec_t		operator <= (const Self & right) const;

		Self		operator -  () const;
		
		Self &		operator += (const Self &right);
		Self &		operator -= (const Self &right);

		Self		operator +  (const Self &right)		const;
		Self		operator -  (const Self &right)		const;
		
		Self &		operator += (value_t right);
		Self &		operator -= (value_t right);

		Self		operator +  (value_t right)			const;
		Self		operator -  (value_t right)			const;

		Self &		operator *= (inner_value_t right);
		Self &		operator /= (inner_value_t right);

		Self		operator *  (inner_value_t right)	const;
		Self		operator /  (inner_value_t right)	const;
		
		Self &		operator *= (const vec_t &right);
		Self &		operator /= (const vec_t &right);

		Self		operator *  (const vec_t &right)	const;
		Self		operator /  (const vec_t &right)	const;

		
		template <typename D, typename S>
		typename _Add< D, S >::vec  operator +  (const PhysicsValueVec< PhysicsValue< inner_value_t, D, S >, I > &right) const;
		
		template <typename D, typename S>
		typename _Sub< D, S >::vec  operator -  (const PhysicsValueVec< PhysicsValue< inner_value_t, D, S >, I > &right) const;
		
		template <typename D, typename S>
		typename _Mul< D, S >::vec  operator *  (const PhysicsValueVec< PhysicsValue< inner_value_t, D, S >, I > &right) const;
		
		template <typename D, typename S>
		typename _Div< D, S >::vec  operator /  (const PhysicsValueVec< PhysicsValue< inner_value_t, D, S >, I > &right) const;

		
		template <typename D, typename S>
		typename _Add< D, S >::vec  operator +  (const PhysicsValue< inner_value_t, D, S > &right) const;
		
		template <typename D, typename S>
		typename _Sub< D, S >::vec  operator -  (const PhysicsValue< inner_value_t, D, S > &right) const;
		
		template <typename D, typename S>
		typename _Mul< D, S >::vec  operator *  (const PhysicsValue< inner_value_t, D, S > &right) const;
		
		template <typename D, typename S>
		typename _Div< D, S >::vec  operator /  (const PhysicsValue< inner_value_t, D, S > &right) const;

		
		template <typename T0, typename D, typename S>
		friend typename _Add< D, S >::vec  operator +  (const PhysicsValue< inner_value_t, D, S > &left, const Self &right);
		
		template <typename T0, typename D, typename S>
		friend typename _Sub< D, S >::vec  operator -  (const PhysicsValue< inner_value_t, D, S > &left, const Self &right);
		
		template <typename T0, typename D, typename S>
		friend typename _Mul< D, S >::vec  operator *  (const PhysicsValue< inner_value_t, D, S > &left, const Self &right);
		
		template <typename T0, typename D, typename S>
		friend typename _Div< D, S >::vec  operator /  (const PhysicsValue< inner_value_t, D, S > &left, const Self &right);


		friend Self			operator * (inner_value_t left, const Self &right);
		friend SelfInversed	operator / (inner_value_t left, const Self &right);
		
		friend Self			operator * (const vec_t &left, const Self &right);
		friend SelfInversed	operator / (const vec_t &left, const Self &right);


		// methods
		T *				ptr ()									{ return PointerCast< T >( this ); }
		T const *		ptr ()	const							{ return PointerCast< T >( this ); }
		vec_t &			ref ()									{ return ReferenceCast< vec_t >( *this ); }
		vec_t const &	ref ()	const							{ return ReferenceCast< vec_t >( *this ); }

		vec_t			Get ()	const;

		static usize	Count ()								{ return I; }

		
		template <isize PowNum, isize PowDenom>
		typename _Pow< PowNum, PowDenom >::vec  Pow ()	const;
		
		template <isize Power>
		typename _Pow< Power >::vec		Pow ()			const;
		
		typename _Pow< 2 >::vec			Square ()		const	{ return Pow< 2 >(); }

		typename _Pow< 1, 2 >::vec		Sqrt ()			const	{ return Pow< 1, 2 >(); }
		
		SelfInversed					Inverse ()		const	{ return SelfInversed( inner_value_t(1) / _value ); }


		T				Length ()						const	{ return value_t( ref().Length() ); }
		square_value_t	LengthSqr ()					const	{ return square_value_t( ref().LengthSqr() ); }

		T				Distance (const Self &right)	const	{ return value_t( ref().Distance( right.ref() ) ); }
		square_value_t	DistanceSqr (const Self &right)	const	{ return square_value_t( ref().DistanceSqr( right.ref() ) ); }


		template <typename T2>
		PhysicsValueVec<T2,I> Convert () const;

		template <typename B>
		const B		To () const;
		
		template <typename ToValueScale>
		PhysicsValueVec< PhysicsValue< inner_value_t, dimensions_t, ToValueScale >, I >	ToScale () const;


		String		ToString ()			const;
		String		ToStringFull ()		const;
		String		ToDebugString ()	const;
		

		// compare methods
		bvec_t		Equal (const Self &right)	const;	// ==
		bvec_t		Less (const Self &right)	const;	// <
		bvec_t		LEqual (const Self &right)	const;	// <=
		bvec_t		Greater (const Self &right)	const;	// >
		bvec_t		GEqual (const Self &right)	const;	// >=
		bvec_t		IsZero ()					const;
		bvec_t		IsNotZero ()				const;

		
#	if I == 1
		value_t					Sum ()		const	{ return x; }
		value_t					SumAbs ()	const	{ return Abs(x); }
		square_value_t			Area ()		const	{ return x * T(1); }
		cube_value_t			Volume ()	const	{ return x * T(1) * T(1); }
#	endif

#	if I == 2
		value_t					Sum ()		const	{ return x + y; }
		value_t					SumAbs ()	const	{ return Abs(x) + Abs(y); }
		cube_value_t			Volume ()	const	{ return x * y * T(1); }
#	endif

#	if I >= 2
		square_value_t			Area ()		const	{ return x * y; }
		PhysicsValueVec<T,2>	yx ()		const	{ return PhysicsValueVec<T,2>( y, x ); }
		PhysicsValueVec<T,2>	xy ()		const	{ return PhysicsValueVec<T,2>( x, y ); }
		PhysicsValueVec<T,2>	ox ()		const	{ return PhysicsValueVec<T,2>( 0, x ); }
		PhysicsValueVec<T,2>	oy ()		const	{ return PhysicsValueVec<T,2>( 0, y ); }
#	endif

#	if I == 3
		value_t					Sum ()		const	{ return x + y + z; }
		value_t					SumAbs ()	const	{ return Abs(x) + Abs(y) + Abs(z); }
#	endif

#	if I >= 3
		cube_value_t			Volume ()	const	{ return x * y * z; }
		PhysicsValueVec<T,2>	yz ()		const	{ return PhysicsValueVec<T,2>( y, z ); }
		PhysicsValueVec<T,2>	xz ()		const	{ return PhysicsValueVec<T,2>( x, z ); }
		PhysicsValueVec<T,3>	xyz ()		const	{ return PhysicsValueVec<T,3>( x, y, z ); }
		PhysicsValueVec<T,3>	zyx ()		const	{ return PhysicsValueVec<T,3>( z, y, x ); }
		PhysicsValueVec<T,3>	xzy ()		const	{ return PhysicsValueVec<T,3>( x, z, y ); }

		PhysicsValueVec<T,3>	xoo ()		const	{ return PhysicsValueVec<T,3>( x, 0, 0 ); }
		PhysicsValueVec<T,3>	oyo ()		const	{ return PhysicsValueVec<T,3>( 0, y, 0 ); }
		PhysicsValueVec<T,3>	ooz ()		const	{ return PhysicsValueVec<T,3>( 0, 0, z ); }

		PhysicsValueVec<T,3>	xoz ()		const	{ return PhysicsValueVec<T,3>( x, 0, z ); }
		PhysicsValueVec<T,3>	xyo ()		const	{ return PhysicsValueVec<T,3>( x, y, 0 ); }
		PhysicsValueVec<T,3>	oyz ()		const	{ return PhysicsValueVec<T,3>( 0, y, z ); }
#	endif

#	if I == 4
		value_t					Sum ()		const	{ return x + y + z + w; }
		value_t					SumAbs ()	const	{ return Abs(x) + Abs(y) + Abs(z) + Abs(w); }
#	endif

#	if I >= 4
		PhysicsValueVec<T,2>	zw ()		const	{ return PhysicsValueVec<T,2>( z, w ); }
		PhysicsValueVec<T,4>	xyzw ()		const	{ return PhysicsValueVec<T,4>( x, y, z, w ); }
		PhysicsValueVec<T,4>	wzyx ()		const	{ return PhysicsValueVec<T,4>( w, z, y, x ); }
#	endif
	};



	
	template <typename T>
	inline Vec<bool,I>  PhysicsValueVec<T,I>::operator == (const Self & right) const
	{
		return Equal( right );
	}
	

	template <typename T>
	inline Vec<bool,I>  PhysicsValueVec<T,I>::operator != (const Self & right) const
	{
		return not Equal( right );
	}

		
	template <typename T>
	inline Vec<bool,I>  PhysicsValueVec<T,I>::operator >  (const Self & right) const
	{
		return Greater( right );
	}

		
	template <typename T>
	inline Vec<bool,I>  PhysicsValueVec<T,I>::operator >= (const Self & right) const
	{
		return GEqual( right );
	}

		
	template <typename T>
	inline Vec<bool,I>  PhysicsValueVec<T,I>::operator <  (const Self & right) const
	{
		return Less( right );
	}

		
	template <typename T>
	inline Vec<bool,I>  PhysicsValueVec<T,I>::operator <= (const Self & right) const
	{
		return LEqual( right );
	}
	
	
	template <typename T>
	inline PhysicsValueVec<T,I>  PhysicsValueVec<T,I>::operator -  () const
	{
		Self	ret;
		FOR( i, ret )		ret[i] = -(*this)[i];
		return ret;
	}


	template <typename T>
	inline PhysicsValueVec<T,I> &  PhysicsValueVec<T,I>::operator += (const Self &right)
	{
		FOR( i, *this )		(*this)[i] += right[i];
		return *this;
	}

	
	template <typename T>
	inline PhysicsValueVec<T,I> &  PhysicsValueVec<T,I>::operator -= (const Self &right)
	{
		FOR( i, *this )		(*this)[i] -= right[i];
		return *this;
	}
	

	template <typename T>
	inline PhysicsValueVec<T,I> &  PhysicsValueVec<T,I>::operator += (value_t right)
	{
		FOR( i, *this )		(*this)[i] += right;
		return *this;
	}

	
	template <typename T>
	inline PhysicsValueVec<T,I> &  PhysicsValueVec<T,I>::operator -= (value_t right)
	{
		FOR( i, *this )		(*this)[i] -= right;
		return *this;
	}


	template <typename T>
	inline PhysicsValueVec<T,I>  PhysicsValueVec<T,I>::operator +  (const Self &right) const
	{
		Self	ret;
		FOR( i, ret )		ret[i] = (*this)[i] + right[i];
		return ret;
	}


	template <typename T>
	inline PhysicsValueVec<T,I>  PhysicsValueVec<T,I>::operator -  (const Self &right) const
	{
		Self	ret;
		FOR( i, ret )		ret[i] = (*this)[i] - right[i];
		return ret;
	}


	template <typename T>
	inline PhysicsValueVec<T,I>  PhysicsValueVec<T,I>::operator +  (value_t right) const
	{
		Self	ret;
		FOR( i, ret )		ret[i] = (*this)[i] + right;
		return ret;
	}


	template <typename T>
	inline PhysicsValueVec<T,I>  PhysicsValueVec<T,I>::operator -  (value_t right) const
	{
		Self	ret;
		FOR( i, ret )		ret[i] = (*this)[i] - right;
		return ret;
	}

	
	template <typename T>
	inline PhysicsValueVec<T,I> &  PhysicsValueVec<T,I>::operator *= (inner_value_t right)
	{
		FOR( i, *this )		(*this)[i] *= right;
		return *this;
	}

	
	template <typename T>
	inline PhysicsValueVec<T,I> &  PhysicsValueVec<T,I>::operator /= (inner_value_t right)
	{
		FOR( i, *this )		(*this)[i] /= right;
		return *this;
	}

	
	template <typename T>
	inline PhysicsValueVec<T,I>  PhysicsValueVec<T,I>::operator *  (inner_value_t right) const
	{
		Self	ret;
		FOR( i, ret )		ret[i] = (*this)[i] * right;
		return ret;
	}

	
	template <typename T>
	inline PhysicsValueVec<T,I>  PhysicsValueVec<T,I>::operator /  (inner_value_t right) const
	{
		Self	ret;
		FOR( i, ret )		ret[i] = (*this)[i] / right;
		return ret;
	}
	

	template <typename T>
	inline PhysicsValueVec<T,I> &  PhysicsValueVec<T,I>::operator *= (const vec_t &right)
	{
		FOR( i, *this )		(*this)[i] *= right[i];
		return *this;
	}

	
	template <typename T>
	inline PhysicsValueVec<T,I> &  PhysicsValueVec<T,I>::operator /= (const vec_t &right)
	{
		FOR( i, *this )		(*this)[i] /= right[i];
		return *this;
	}

	
	template <typename T>
	inline PhysicsValueVec<T,I>  PhysicsValueVec<T,I>::operator *  (const vec_t &right)	const
	{
		Self	ret;
		FOR( i, ret )		ret[i] = (*this)[i] * right[i];
		return ret;
	}

	
	template <typename T>
	inline PhysicsValueVec<T,I>  PhysicsValueVec<T,I>::operator /  (const vec_t &right)	const
	{
		Self	ret;
		FOR( i, ret )		ret[i] = (*this)[i] / right[i];
		return ret;
	}
	
	
	template <typename T>
	template <typename D, typename S>
	inline typename PhysicsValueVec<T,I>::_Add< D, S >::vec
		PhysicsValueVec<T,I>::operator +  (const PhysicsValueVec< PhysicsValue< inner_value_t, D, S >, I > &right) const
	{
		_Add< D, S >::vec	ret;
		FOR( i, ret )		ret[i] = (*this)[i] + right[i];
		return ret;
	}
		

	template <typename T>
	template <typename D, typename S>
	inline typename PhysicsValueVec<T,I>::_Sub< D, S >::vec
		PhysicsValueVec<T,I>::operator -  (const PhysicsValueVec< PhysicsValue< inner_value_t, D, S >, I > &right) const
	{
		_Sub< D, S >::vec	ret;
		FOR( i, ret )		ret[i] = (*this)[i] - right[i];
		return ret;
	}
		

	template <typename T>
	template <typename D, typename S>
	inline typename PhysicsValueVec<T,I>::_Mul< D, S >::vec
		PhysicsValueVec<T,I>::operator *  (const PhysicsValueVec< PhysicsValue< inner_value_t, D, S >, I > &right) const
	{
		_Mul< D, S >::vec	ret;
		FOR( i, ret )		ret[i] = (*this)[i] * right[i];
		return ret;
	}
		

	template <typename T>
	template <typename D, typename S>
	inline typename PhysicsValueVec<T,I>::_Div< D, S >::vec
		PhysicsValueVec<T,I>::operator /  (const PhysicsValueVec< PhysicsValue< inner_value_t, D, S >, I > &right) const
	{
		_Div< D, S >::vec	ret;
		FOR( i, ret )		ret[i] = (*this)[i] / right[i];
		return ret;
	}
	
	
	template <typename T>
	template <typename D, typename S>
	inline typename PhysicsValueVec<T,I>::_Add< D, S >::vec
		PhysicsValueVec<T,I>::operator +  (const PhysicsValue< inner_value_t, D, S > &right) const
	{
		_Add< D, S >::vec	ret;
		FOR( i, ret )		ret[i] = (*this)[i] + right;
		return ret;
	}
		

	template <typename T>
	template <typename D, typename S>
	inline typename PhysicsValueVec<T,I>::_Sub< D, S >::vec
		PhysicsValueVec<T,I>::operator -  (const PhysicsValue< inner_value_t, D, S > &right) const
	{
		_Sub< D, S >::vec	ret;
		FOR( i, ret )		ret[i] = (*this)[i] - right;
		return ret;
	}
		

	template <typename T>
	template <typename D, typename S>
	inline typename PhysicsValueVec<T,I>::_Mul< D, S >::vec
		PhysicsValueVec<T,I>::operator *  (const PhysicsValue< inner_value_t, D, S > &right) const
	{
		_Mul< D, S >::vec	ret;
		FOR( i, ret )		ret[i] = (*this)[i] * right;
		return ret;
	}
		

	template <typename T>
	template <typename D, typename S>
	inline typename PhysicsValueVec<T,I>::_Div< D, S >::vec
		PhysicsValueVec<T,I>::operator /  (const PhysicsValue< inner_value_t, D, S > &right) const
	{
		_Div< D, S >::vec	ret;
		FOR( i, ret )		ret[i] = (*this)[i] / right;
		return ret;
	}
	
	
	template <typename T, typename D, typename S>
	inline typename PhysicsValueVec<T,I>::_Add< D, S >::vec
		operator +  (const PhysicsValue< typename PhysicsValueVec<T,I>::inner_value_t, D, S > &left, const PhysicsValueVec<T,I> &right)
	{
		_Add< D, S >::vec	ret;
		FOR( i, ret )		ret[i] = left + right[i];
		return ret;
	}
		

	template <typename T, typename D, typename S>
	inline typename PhysicsValueVec<T,I>::_Sub< D, S >::vec
		operator -  (const PhysicsValue< typename PhysicsValueVec<T,I>::inner_value_t, D, S > &left, const PhysicsValueVec<T,I> &right)
	{
		_Sub< D, S >::vec	ret;
		FOR( i, ret )		ret[i] = left - right[i];
		return ret;
	}
		

	template <typename T, typename D, typename S>
	inline typename PhysicsValueVec<T,I>::_Mul< D, S >::vec
		operator *  (const PhysicsValue< typename PhysicsValueVec<T,I>::inner_value_t, D, S > &left, const PhysicsValueVec<T,I> &right)
	{
		_Mul< D, S >::vec	ret;
		FOR( i, ret )		ret[i] = left * right[i];
		return ret;
	}
		

	template <typename T, typename D, typename S>
	inline typename PhysicsValueVec<T,I>::_Div< D, S >::vec
		operator /  (const PhysicsValue< typename PhysicsValueVec<T,I>::inner_value_t, D, S > &left, const PhysicsValueVec<T,I> &right)
	{
		_Div< D, S >::vec	ret;
		FOR( i, ret )		ret[i] = left / right[i];
		return ret;
	}
	

	template <typename T>
	inline PhysicsValueVec<T,I>
		operator * (typename PhysicsValueVec<T,I>::inner_value_t left, const PhysicsValueVec<T,I> &right)
	{
		Self	ret;
		FOR( i, ret )		ret[i] = left * right[i];
		return ret;
	}
	

	template <typename T>
	inline typename PhysicsValueVec<T,I>::SelfInversed
		operator / (typename PhysicsValueVec<T,I>::inner_value_t left, const PhysicsValueVec<T,I> &right)
	{
		SelfInversed	ret;
		FOR( i, ret )		ret[i] = left / right[i];
		return ret;
	}
	

	template <typename T>
	inline PhysicsValueVec<T,I>
		operator * (const typename PhysicsValueVec<T,I>::vec_t &left, const PhysicsValueVec<T,I> &right)
	{
		Self	ret;
		FOR( i, ret )		ret[i] = left[i] * right[i];
		return ret;
	}
	

	template <typename T>
	inline typename PhysicsValueVec<T,I>::SelfInversed
		operator / (const typename PhysicsValueVec<T,I>::vec_t &left, const PhysicsValueVec<T,I> &right)
	{
		SelfInversed	ret;
		FOR( i, ret )		ret[i] = left[i] / right[i];
		return ret;
	}
	

	template <typename T>
	inline typename PhysicsValueVec<T,I>::vec_t  PhysicsValueVec<T,I>::Get () const
	{
		vec_t	ret;
		FOR( i, ret )	ret[i] = (*this)[i].Get();
		return ret;
	}
		

	template <typename T>
	inline Vec<bool,I>  PhysicsValueVec<T,I>::Equal(const Self &right) const
	{
		Vec<bool,I>	ret;
		FOR( i, *this )	ret[i] = GXMath::Equals( (*this)[i], right[i] );
		return ret;
	}
		

	template <typename T>
	inline Vec<bool,I>  PhysicsValueVec<T,I>::Less(const Self &right) const
	{
		Vec<bool,I>	ret;
		FOR( i, *this )	ret[i] = ( (*this)[i] < right[i] ) & not GXMath::Equals( (*this)[i], right[i] );
		return ret;
	}
		

	template <typename T>
	inline Vec<bool,I>  PhysicsValueVec<T,I>::LEqual(const Self &right) const
	{
		Vec<bool,I>	ret;
		FOR( i, *this )	ret[i] = ( (*this)[i] < right[i] ) | GXMath::Equals( (*this)[i], right[i] );
		return ret;
	}
		

	template <typename T>
	inline Vec<bool,I>  PhysicsValueVec<T,I>::Greater(const Self &right) const
	{
		Vec<bool,I>	ret;
		FOR( i, *this )	ret[i] = ( (*this)[i] > right[i] ) & not GXMath::Equals( (*this)[i], right[i] );
		return ret;
	}
		

	template <typename T>
	inline Vec<bool,I>  PhysicsValueVec<T,I>::GEqual(const Self &right) const
	{
		Vec<bool,I>	ret;
		FOR( i, *this )	ret[i] = ( (*this)[i] > right[i] ) | GXMath::Equals( (*this)[i], right[i] );
		return ret;
	}
		

	template <typename T>
	inline Vec<bool,I>  PhysicsValueVec<T,I>::IsZero() const
	{
		Vec<bool,I>	ret;
		FOR( i, *this )	ret[i] = (*this)[i].IsZero();
		return ret;
	}
		

	template <typename T>
	inline Vec<bool,I>  PhysicsValueVec<T,I>::IsNotZero() const
	{
		return not IsZero();
	}
	

	template <typename T>
	template <isize PowNum, isize PowDenom>
	inline typename PhysicsValueVec<T,I>::_Pow< PowNum, PowDenom >::vec  PhysicsValueVec<T,I>::Pow () const
	{
		_Pow< PowNum, PowDenom >::vec	ret;
		FOR( i, ret )	ret[i] = (*this)[i].Pow< PowNum, PowDenom >();
		return ret;
	}
	

	template <typename T>
	template <isize Power>
	inline typename PhysicsValueVec<T,I>::_Pow< Power >::vec  PhysicsValueVec<T,I>::Pow () const
	{
		_Pow< Power >::vec	ret;
		FOR( i, ret )	ret[i] = (*this)[i].Pow< Power >();
		return ret;
	}
	
	
	template <typename T>
	template <typename T2>
	inline PhysicsValueVec<T2,I>  PhysicsValueVec<T,I>::Convert () const
	{
		PhysicsValueVec<T2,I>	ret;
		FOR( i, ret )	ret[i] = (*this)[i].Convert< T2 >();
		return ret;
	}

	
	template <typename T>
	template <typename B>
	inline const B  PhysicsValueVec<T,I>::To () const
	{
		STATIC_ASSERT( typename B::_is_vector(true), "type is not vector" );
		
		B				ret;
		const usize		count = GXMath::Min( Count(), B::Count() );

		FOR_range( i, 0, count )	ret[i] = typename B::value_t( (*this)[i] );
		return ret;
	}

		
	template <typename T>
	template <typename ToValueScale>
	inline PhysicsValueVec< PhysicsValue< typename PhysicsValueVec<T,I>::inner_value_t,
										  typename PhysicsValueVec<T,I>::dimensions_t, ToValueScale >, I >
		PhysicsValueVec<T,I>::ToScale () const
	{
		PhysicsValueVec< PhysicsValue< inner_value_t, dimensions_t, ToValueScale >, I >	ret;
		FOR( i, ret )	ret[i] = (*this)[i].ToScale< ToValueScale >();
		return ret;
	}

	
	template <typename T>
	inline String  PhysicsValueVec<T,I>::ToString () const
	{
		return ('(' >> Get().ToString()) << ") * [" << dimensions_t::ToString("*") << ']';
	}

	
	template <typename T>
	inline String  PhysicsValueVec<T,I>::ToStringFull () const
	{
		return ('(' >> Get().ToStringFull()) << ") * [" << dimensions_t::ToString("*") << ']';
	}

	
	template <typename T>
	inline String  PhysicsValueVec<T,I>::ToDebugString () const
	{
		return ('(' >> ref().ToStringFull()) << ") * (" << value_scale_t::Scale::ToString()
					<< ")^" << value_scale_t::Power::ToString() << " [" << dimensions_t::ToString("*") << ']';
	}

