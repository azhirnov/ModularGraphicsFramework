// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'


	//
	// Vector
	//

	template <typename T, ulong U>
	struct Vec<T,I,U> : public CompileTime::CopyQualifiers< T >
	{
		STATIC_ASSERT( CompileTime::IsScalar<T> or TypeTraits::IsEnum<T> );

	// types
		using _is_vector	= bool;
		using Self			= Vec<T,I,U>;
		using Value_t		= T;
		using BVec_t		= Vec<bool,I,U>;
		using Scalar_t		= Vec<T,1,U>;
		using Arr_t			= T[I];

		static constexpr uint	STATIC_COUNT = I;
		static constexpr ulong	UID			 = U;


#	if I == 1
	// variables
		T	x;

	// constructors
		constexpr Vec (GX_DEFCTOR): x(0) {}
		constexpr Vec (const Vec<T,1,U> &v): x(v.x) {}
		constexpr explicit Vec (T X): x(X) {}

		constexpr explicit Vec (const Vec<T,2,U> &v): x(v.x) {}
		constexpr explicit Vec (const Vec<T,3,U> &v): x(v.x) {}
		constexpr explicit Vec (const Vec<T,4,U> &v): x(v.x) {}

		template <typename B>
		constexpr explicit Vec (const Vec<B,1,U> &v): x(T(v.x)) {}

		Self& operator = (T X)				{ x = X; return *this; }

		constexpr explicit operator T () const		{ return x; }
#	endif

#	if I == 2
	// variables
		T	x,
			y;

	// constructors
		constexpr Vec (GX_DEFCTOR): x(0), y(0) {}
		constexpr Vec (const Vec<T,2,U> &v): x(v.x), y(v.y) {}

		constexpr explicit Vec (T V): x(V), y(V) {}
		constexpr explicit Vec (const Vec<T,3,U> &v): x(v.x), y(v.y) {}
		constexpr explicit Vec (const Vec<T,4,U> &v): x(v.x), y(v.y) {}

		template <typename A, typename B>
		constexpr explicit Vec (A X, B Y) : x(X), y(Y) {}

		template <typename B>
		constexpr explicit Vec (const Vec<B,2,U> &v): x(T(v.x)), y(T(v.y)) {}
#	endif

#	if I == 3
	// variables
		T	x,
			y,
			z;

	// constructors
		constexpr Vec (GX_DEFCTOR): x(0), y(0), z(0) {}
		constexpr Vec (const Vec<T,3,U> &v): x(v.x), y(v.y), z(v.z) {}

		constexpr explicit Vec (T V): x(V), y(V), z(V) {}

		template <typename A, typename B, typename C>
		constexpr explicit Vec (A X, B Y, C Z): x(X), y(Y), z(Z) {}

		template <typename A>
		constexpr explicit Vec (A X, const Vec<T,2,U> &YZ): x(X), y(YZ[0]), z(YZ[1]) {}

		template <typename A>
		constexpr explicit Vec (const Vec<T,2,U> &XY, A Z): x(XY[0]), y(XY[1]), z(Z) {}

		constexpr explicit Vec (const Vec<T,2,U> &XY): x(XY[0]), y(XY[1]), z(0) {}
		constexpr explicit Vec (const Vec<T,4,U> &v): x(v.x), y(v.y), z(v.z) {}

		template <typename B>
		constexpr explicit Vec (const Vec<B,3,U> &v): x(T(v.x)), y(T(v.y)), z(T(v.z)) {}
#	endif

#	if I == 4
	// variables
		T	x,
			y,
			z,
			w;

	// constructors
		constexpr Vec (GX_DEFCTOR): x(0), y(0), z(0), w(0) {}
		constexpr Vec (const Vec<T,4,U> &v): x(v.x), y(v.y), z(v.z), w(v.w) {}

		constexpr explicit Vec (T V): x(V), y(V), z(V), w(V) {}
		constexpr explicit Vec (const Vec<T,2,U> &XY): x(XY[0]), y(XY[1]), z(T(0)), w(T(0)) {}

		template <typename A, typename B, typename C, typename D>
		constexpr explicit Vec (A X, B Y, C Z, D W): x(X), y(Y), z(Z), w(W) {}

		template <typename A, typename B>
		constexpr explicit Vec (A X, const Vec<T,2,U> &YZ, B W): x(X), y(YZ[0]), z(YZ[1]), w(W) {}

		template <typename A, typename B>
		constexpr explicit Vec (A X, B Y, const Vec<T,2,U> &ZW): x(X), y(Y), z(ZW[0]), w(ZW[1]) {}

		constexpr explicit Vec (const Vec<T,2,U> &XY, const Vec<T,2,U> &ZW): x(XY[0]), y(XY[1]), z(ZW[0]), w(ZW[1]) {}

		template <typename A>
		constexpr explicit Vec (const Vec<T,3,U> &XYZ, A W): x(XYZ[0]), y(XYZ[1]), z(XYZ[2]), w(W) {}
		
		template <typename A>
		constexpr explicit Vec (A X, const Vec<T,3,U> &YZW): x(X), y(YZW[0]), z(YZW[1]), w(YZW[2]) {}

		constexpr explicit Vec (const Vec<T,3,U> &XYZ): x(XYZ[0]), y(XYZ[1]), z(XYZ[2]), w(T(0)) {}

		template <typename A, typename B>
		constexpr explicit Vec (const Vec<T,2,U> &XY, A Z, B W) : x(XY[0]), y(XY[1]), z(Z), w(W) {}

		template <typename B>
		constexpr explicit Vec (const Vec<B,4,U> &v): x(T(v.x)), y(T(v.y)), z(T(v.z)), w(T(v.w)) {}
#	endif

#	if I >= 5 or I < 0
		STATIC_WARNING( "not supported!" );
#	endif

		Self& operator = (Self &&) = default;
		Self& operator = (const Self &) = default;

		
		// unary operators
		VEC_OP_UNARY( - );
		VEC_OP_UNARY( ~ );
		VEC_OP_UNARY( ! );

		Self &		operator ++ ();
		Self &		operator -- ();
		const Self	operator ++ (int);
		const Self	operator -- (int);


		// binary operators
		T &			operator [] (usize i)								{ ASSUME(i<I);  return PointerCast< T >( this )[i]; }
		T const &	operator [] (usize i) const							{ ASSUME(i<I);  return PointerCast< T >( this )[i]; }
		//constexpr T	operator [] (usize i) const	noexcept				{ /*ASSUME(i<I);*/  return PointerCast< T >( this )[i]; }

		constexpr BVec_t	operator == (const Self & right) const		{ return Equal( right ); }
		constexpr BVec_t	operator != (const Self & right) const		{ return not Equal( right ); }
		constexpr BVec_t	operator >  (const Self & right) const		{ return Greater( right ); }
		constexpr BVec_t	operator >= (const Self & right) const		{ return not Less( right ); }
		constexpr BVec_t	operator <  (const Self & right) const		{ return Less( right ); }
		constexpr BVec_t	operator <= (const Self & right) const		{ return not Greater( right ); }
		
#	if I == 1
		constexpr bool		operator == (const T& right) const			{ return x == right; }
		constexpr bool		operator != (const T& right) const			{ return x != right; }
		constexpr bool		operator >  (const T& right) const			{ return x >  right; }
		constexpr bool		operator >= (const T& right) const			{ return x >= right; }
		constexpr bool		operator <  (const T& right) const			{ return x <  right; }
		constexpr bool		operator <= (const T& right) const			{ return x <= right; }
		
		VEC_OP_BINARY_SCALAR( +,  T );
		VEC_OP_BINARY_SCALAR( -,  T );
		VEC_OP_BINARY_SCALAR( *,  T );
		VEC_OP_BINARY_SCALAR( /,  T );

		VEC_OP_BINARY_SCALAR( &,  T );
		VEC_OP_BINARY_SCALAR( |,  T );
		VEC_OP_BINARY_SCALAR( ^,  T );
		VEC_OP_BINARY_SCALAR( <<, T );
		VEC_OP_BINARY_SCALAR( >>, T );
		VEC_OP_BINARY_SCALAR_( &&, T );
		VEC_OP_BINARY_SCALAR_( ||, T );

		VEC_OP_BINARY( +  );
		VEC_OP_BINARY( -  );
		VEC_OP_BINARY( *  );
		VEC_OP_BINARY( /  );

		// only for integer types
		VEC_OP_BINARY( &  );
		VEC_OP_BINARY( |  );
		VEC_OP_BINARY( ^  );
		VEC_OP_BINARY( << );
		VEC_OP_BINARY( >> );
		
		VEC_OP_BINARY_( && );
		VEC_OP_BINARY_( || );
		
		Self &		operator %= (const Self& right)
		{
			FOR( i, *this )	(*this)[i] = Mod( (*this)[i], right[i] );
			return *this;
		}
		
		constexpr const Self	operator %  (const Self& right) const
		{
			Self	ret;
			FOR( i, *this )	 ret[i] = Mod( (*this)[i], right[i] );
			return ret;
		}

		Self &		operator %= (const T& right)
		{
			FOR( i, *this )	 (*this)[i] = Mod( (*this)[i], right );
			return *this;
		}

		constexpr const Self	operator %  (const T& right) const
		{
			Self	ret;
			FOR( i, *this )	 ret[i] = Mod( (*this)[i], right );
			return ret;
		}

		friend constexpr const Self operator % (const T& left, const Self& right)
		{
			return Self(left) % right;
		}

#	else
		constexpr BVec_t		operator == (const T& right) const				{ return Equal( Self(right) ); }
		constexpr BVec_t		operator != (const T& right) const				{ return not Equal( Self(right) ); }
		constexpr BVec_t		operator >  (const T& right) const				{ return Greater( Self(right) ); }
		constexpr BVec_t		operator >= (const T& right) const				{ return GEqual( Self(right) ); }
		constexpr BVec_t		operator <  (const T& right) const				{ return Less( Self(right) ); }
		constexpr BVec_t		operator <= (const T& right) const				{ return LEqual( Self(right) ); }

		constexpr BVec_t		operator == (const Scalar_t& right) const		{ return Equal( Self(right.x) ); }
		constexpr BVec_t		operator != (const Scalar_t& right) const		{ return not Equal( Self(right.x) ); }
		constexpr BVec_t		operator >  (const Scalar_t& right) const		{ return Greater( Self(right.x) ); }
		constexpr BVec_t		operator >= (const Scalar_t& right) const		{ return GEqual( Self(right.x) ); }
		constexpr BVec_t		operator <  (const Scalar_t& right) const		{ return Less( Self(right.x) ); }
		constexpr BVec_t		operator <= (const Scalar_t& right) const		{ return LEqual( Self(right.x) ); }

		VEC_OP_BINARY_SCALAR( +,  T );
		VEC_OP_BINARY_SCALAR( -,  T );
		VEC_OP_BINARY_SCALAR( *,  T );
		VEC_OP_BINARY_SCALAR( /,  T );

		VEC_OP_BINARY_SCALAR( +,  Scalar_t );
		VEC_OP_BINARY_SCALAR( -,  Scalar_t );
		VEC_OP_BINARY_SCALAR( *,  Scalar_t );
		VEC_OP_BINARY_SCALAR( /,  Scalar_t );

		VEC_OP_BINARY_SCALAR( &,  T );
		VEC_OP_BINARY_SCALAR( |,  T );
		VEC_OP_BINARY_SCALAR( ^,  T );
		VEC_OP_BINARY_SCALAR( <<, T );
		VEC_OP_BINARY_SCALAR( >>, T );

		VEC_OP_BINARY_SCALAR( &,  Scalar_t );
		VEC_OP_BINARY_SCALAR( |,  Scalar_t );
		VEC_OP_BINARY_SCALAR( ^,  Scalar_t );
		VEC_OP_BINARY_SCALAR( <<, Scalar_t );
		VEC_OP_BINARY_SCALAR( >>, Scalar_t );

		VEC_OP_BINARY_SCALAR_( &&, T );
		VEC_OP_BINARY_SCALAR_( ||, T );

		VEC_OP_BINARY_SCALAR_( &&, Scalar_t );
		VEC_OP_BINARY_SCALAR_( ||, Scalar_t );
		/*
		friend BVec_t	operator == (const T& left, const Self &right)		{ return Self(left) == right; }
		friend BVec_t	operator != (const T& left, const Self &right)		{ return Self(left) != right; }
		friend BVec_t	operator >  (const T& left, const Self &right)		{ return Self(left) >  right; }
		friend BVec_t	operator >= (const T& left, const Self &right)		{ return Self(left) >= right; }
		friend BVec_t	operator <  (const T& left, const Self &right)		{ return Self(left) <  right; }
		friend BVec_t	operator <= (const T& left, const Self &right)		{ return Self(left) <= right; }
		*/
		VEC_OP_BINARY( +  );
		VEC_OP_BINARY( -  );
		VEC_OP_BINARY( *  );
		VEC_OP_BINARY( /  );

		// only for integer types
		VEC_OP_BINARY( &  );
		VEC_OP_BINARY( |  );
		VEC_OP_BINARY( ^  );
		VEC_OP_BINARY( << );
		VEC_OP_BINARY( >> );

		VEC_OP_BINARY_( && );
		VEC_OP_BINARY_( || );
		

		Self &		operator %= (const Self& right)
		{
			FOR( i, *this )	(*this)[i] = Mod( (*this)[i], right[i] );
			return *this;
		}
		
		constexpr const Self	operator %  (const Self& right) const
		{
			Self	ret;
			FOR( i, *this )	 ret[i] = Mod( (*this)[i], right[i] );
			return ret;
		}

		Self &		operator %= (const T& right)
		{
			FOR( i, *this )	 (*this)[i] = Mod( (*this)[i], right );
			return *this;
		}

		constexpr const Self	operator %  (const T& right) const
		{
			Self	ret;
			FOR( i, *this )	 ret[i] = Mod( (*this)[i], right );
			return ret;
		}

		friend constexpr const Self operator % (const T& left, const Self& right)
		{
			return Self(left) % right;
		}
#	endif


		// methods
		T *			ptr ()										{ return PointerCast< T >( this ); }
		T const *	ptr ()	const								{ return PointerCast< T >( this ); }
		Arr_t &		ref ()										{ return *PointerCast< Arr_t >( this ); }
		Arr_t const& ref ()	const								{ return *PointerCast< Arr_t >( this ); }

		static constexpr usize Count ()							{ return I; }

		T			Min () const;
		T			Max () const;

		T			Length () const;
		T			LengthSqr () const;
		Self		Normalized () const;
		void		GetNormLength (OUT Self &norm, OUT T &length) const;
		Self &		Normalize ();
		Self &		SetLength (T len);
		Self		NewLength (T len) const;

		// 
		Self &		MakeCeil (const Self &sVec);
		Self &		MakeFloor (const Self &sVec);

		T			Dot (const Self &right) const;
		T			DotAbs (const Self &right) const;

		T			Distance (const Self &sVec) const;
		T			DistanceSqr (const Self &sVec) const;

		Self		Reflect (const Self &normal) const;
		Self		Refract (const Self &normal, T eta) const;

		template <typename T2>
		const Vec<T2,I,U> Convert () const;

		template <typename B>
		const B		To () const;


		// compare methods
		BVec_t		Equal (const Self &right)	const;	// ==
		BVec_t		Less (const Self &right)	const;	// <
		BVec_t		LEqual (const Self &right)	const;	// <=
		BVec_t		Greater (const Self &right)	const;	// >
		BVec_t		GEqual (const Self &right)	const;	// >=
		BVec_t		IsZero ()					const;
		BVec_t		IsNotZero ()				const;


#	if I == 1
		constexpr T				Sum ()		const	{ return x; }
		constexpr T				SumAbs ()	const	{ return Abs(x); }
		constexpr T				Area ()		const	{ return x*1; }
		constexpr T				Volume ()	const	{ return x*1*1; }
#	endif

#	if I >= 1
		constexpr Vec<T,2,U>	xo ()		const	{ return Vec<T,2,U>( x, T(0) ); }
		constexpr Vec<T,2,U>	ox ()		const	{ return Vec<T,2,U>( T(0), x ); }
		constexpr Vec<T,2,U>	xx ()		const	{ return Vec<T,2,U>( x, x ); }

		constexpr Vec<T,3,U>	xoo ()		const	{ return Vec<T,3,U>( x, T(0), T(0) ); }

		constexpr Vec<T,4,U>	xooo ()		const	{ return Vec<T,4,U>( x, T(0), T(0), T(0) ); }
#	endif

#	if I == 2
		constexpr T				Sum ()		const	{ return x + y; }
		constexpr T				SumAbs ()	const	{ return Abs(x) + Abs(y); }
		constexpr T				Volume ()	const	{ return x*y*1; }
#	endif

#	if I >= 2
		constexpr T				Area ()		const	{ return x*y; }
		constexpr Vec<T,2,U>	yo ()		const	{ return Vec<T,2,U>( y, T(0) ); }
		constexpr Vec<T,2,U>	yx ()		const	{ return Vec<T,2,U>( y, x ); }
		constexpr Vec<T,2,U>	xy ()		const	{ return Vec<T,2,U>( x, y ); }
		constexpr Vec<T,2,U>	oy ()		const	{ return Vec<T,2,U>( T(0), y ); }
		constexpr Vec<T,2,U>	yy ()		const	{ return Vec<T,2,U>( y, y ); }

		constexpr Vec<T,3,U>	oyo ()		const	{ return Vec<T,3,U>( T(0), y, T(0) ); }
		constexpr Vec<T,3,U>	xyo ()		const	{ return Vec<T,3,U>( x, y, T(0) ); }
		constexpr Vec<T,3,U>	xoy ()		const	{ return Vec<T,3,U>( x, T(0), y ); }
		constexpr Vec<T,3,U>	oyx ()		const	{ return Vec<T,3,U>( T(0), y, x ); }

		constexpr Vec<T,4,U>	xooy ()		const	{ return Vec<T,4,U>( x, T(0), T(0), y ); }
		constexpr Vec<T,4,U>	xyoo ()		const	{ return Vec<T,4,U>( x, y, T(0), T(0) ); }
		constexpr Vec<T,4,U>	xyxy ()		const	{ return Vec<T,4,U>( x, y, x, y ); }
#	endif

#	if I == 3
		constexpr T				Sum ()		const	{ return x + y + z; }
		constexpr T				SumAbs ()	const	{ return Abs(x) + Abs(y) + Abs(z); }
#	endif

#	if I >= 3
		constexpr T				Volume ()	const	{ return x*y*z; }
		constexpr Vec<T,2,U>	yz ()		const	{ return Vec<T,2,U>( y, z ); }
		constexpr Vec<T,2,U>	xz ()		const	{ return Vec<T,2,U>( x, z ); }
		constexpr Vec<T,3,U>	xyz ()		const	{ return Vec<T,3,U>( x, y, z ); }
		constexpr Vec<T,3,U>	zyx ()		const	{ return Vec<T,3,U>( z, y, x ); }
		constexpr Vec<T,3,U>	xzy ()		const	{ return Vec<T,3,U>( x, z, y ); }

		constexpr Vec<T,3,U>	ooz ()		const	{ return Vec<T,3,U>( T(0), T(0), z ); }
		constexpr Vec<T,3,U>	xoz ()		const	{ return Vec<T,3,U>( x, T(0), z ); }
		constexpr Vec<T,3,U>	oyz ()		const	{ return Vec<T,3,U>( T(0), y, z ); }
		
		constexpr Vec<T,4,U>	xyoz ()		const	{ return Vec<T,4,U>( x, y, T(0), z ); }
		constexpr Vec<T,4,U>	xyzo ()		const	{ return Vec<T,4,U>( x, y, z, T(0) ); }
#	endif

#	if I == 4
		constexpr T				Sum ()		const	{ return x + y + z + w; }
		constexpr T				SumAbs ()	const	{ return Abs(x) + Abs(y) + Abs(z) + Abs(w); }
#	endif

#	if I >= 4
		constexpr T				Volume4D ()	const	{ return x*y*z*w; }
		constexpr Vec<T,2,U>	yw ()		const	{ return Vec<T,2,U>( y, w ); }
		constexpr Vec<T,2,U>	zw ()		const	{ return Vec<T,2,U>( z, w ); }
		
		constexpr Vec<T,3,U>	xwo ()		const	{ return Vec<T,3,U>( x, y, T(0) ); }
		constexpr Vec<T,3,U>	xyw ()		const	{ return Vec<T,3,U>( x, y, w ); }
		constexpr Vec<T,3,U>	yzw ()		const	{ return Vec<T,3,U>( y, z, w ); }
		
		constexpr Vec<T,4,U>	xyzw ()		const	{ return Vec<T,4,U>( x, y, z, w ); }
		constexpr Vec<T,4,U>	wzyx ()		const	{ return Vec<T,4,U>( w, z, y, x ); }
#	endif

		template <Swizzle::type SW>
		constexpr auto Swizzle () const -> Vec< T, _math_hidden_::SWLength<SW>, U >;

		Vec<T,4,U> Swizzle (Swizzle::type sw) const;
	};



	
		
	template <typename T, ulong U>
	inline Vec<T,I,U> &  Vec<T,I,U>::operator ++ ()
	{
		FOR( i, *this )	++(*this)[i];
		return *this;
	}

		
	template <typename T, ulong U>
	inline Vec<T,I,U> &  Vec<T,I,U>::operator -- ()
	{
		FOR( i, *this )	--(*this)[i];
		return *this;
	}

		
	template <typename T, ulong U>
	inline const Vec<T,I,U>  Vec<T,I,U>::operator ++ (int)
	{
		Self	ret(*this);
		++(*this);
		return ret;
	}

		
	template <typename T, ulong U>
	inline const Vec<T,I,U>  Vec<T,I,U>::operator -- (int)
	{
		Self	ret(*this);
		--(*this);
		return ret;
	}
	
	
	template <typename T, ulong U>
	inline T  Vec<T,I,U>::Min () const
	{
		T	t_min = (*this)[0];
		FOR( i, *this )  (*this)[i] < t_min ?  t_min = (*this)[i]  : 0;
		return t_min;
	}

	
	template <typename T, ulong U>
	inline T  Vec<T,I,U>::Max () const
	{
		T	t_max = (*this)[0];
		FOR( i, *this )  (*this)[i] > t_max ?  t_max = (*this)[i]  : 0;
		return t_max;
	}

	
	template <typename T, ulong U>
	inline T  Vec<T,I,U>::Length () const
	{
		return Sqrt( LengthSqr() );
	}

	
	template <typename T, ulong U>
	inline T  Vec<T,I,U>::LengthSqr () const
	{
		Self	res;
		FOR( i, *this )	res[i] = GXMath::Square( (*this)[i] );
		return res.Sum();
	}

	
	template <typename T, ulong U>
	inline Vec<T,I,U>  Vec<T,I,U>::Normalized () const
	{
		const T		len_sqr = LengthSqr();
		return len_sqr != T(0) ?  Self(*this) * InvSqrt( len_sqr )  :  Self();
	}
	
	
	template <typename T, ulong U>
	inline void  Vec<T,I,U>::GetNormLength (OUT Self &norm, OUT T &length) const
	{
		length = LengthSqr();

		if ( GXMath::IsNotZero( length ) )
		{
			float l = InvSqrt( length );
			norm	= (*this) * l;
			length	= T(1) / l;
		}
		else
		{
			norm	= Self();
			length	= T(0);
		}
	}

		
	template <typename T, ulong U>
	inline Vec<T,I,U> &  Vec<T,I,U>::Normalize ()
	{
		return ( *this = Normalized() );
	}

		
	template <typename T, ulong U>
	inline Vec<T,I,U> &  Vec<T,I,U>::SetLength (T len)
	{
		len *= InvSqrt( LengthSqr() );
		FOR( i, *this )  (*this)[i] *= len;
		return *this;
	}
	
	
	template <typename T, ulong U>
	inline Vec<T,I,U>  Vec<T,I,U>::NewLength (T len) const
	{
		return Self( *this ).SetLength( len );
	}

	
	template <typename T, ulong U>
	inline Vec<T,I,U> &  Vec<T,I,U>::MakeCeil (const Self &sVec)
	{
		FOR( i, *this )  sVec[i] > (*this)[i]  ?  (*this)[i] = sVec[i]  : T(0);
		return *this;
	}
		
	
	template <typename T, ulong U>
	inline Vec<T,I,U> &  Vec<T,I,U>::MakeFloor (const Self &sVec)
	{
		FOR( i, *this )  sVec[i] < (*this)[i]  ?  (*this)[i] = sVec[i]  : T(0);
		return *this;
	}

		
	template <typename T, ulong U>
	inline T  Vec<T,I,U>::Dot (const Self &right) const
	{
		Self	res;
		FOR( i, *this )	res[i] = (*this)[i] * right[i];
		return res.Sum();
	}
		
	
	template <typename T, ulong U>
	inline T  Vec<T,I,U>::DotAbs (const Self &right) const
	{
		Self	res;
		FOR( i, *this )	res[i] = Abs( (*this)[i] * right[i] );
		return res.Sum();
	}

		
	template <typename T, ulong U>
	inline T  Vec<T,I,U>::Distance (const Self &sVec) const
	{
		return ( (*this) - sVec ).Length();
	}
		
	
	template <typename T, ulong U>
	inline T  Vec<T,I,U>::DistanceSqr (const Self &sVec) const
	{
		return ( (*this) - sVec ).LengthSqr();
	}

		
	template <typename T, ulong U>
	template <typename T2>
	inline const Vec<T2,I,U>  Vec<T,I,U>::Convert () const
	{
		Vec<T2,I,U>	ret;
		FOR( i, *this )	ret[i] = T2( (*this)[i] );
		return ret;
	}
	
		
	template <typename T, ulong U>
	template <typename B>
	inline const B  Vec<T,I,U>::To () const
	{
		STATIC_ASSERT( typename B::_is_vector(true), "type is not vector" );
		
		B				ret;
		const usize		count = GXMath::Min( Count(), B::Count() );

		FOR_range( i, 0, count )	ret[i] = typename B::Value_t( (*this)[i] );
		return ret;
	}
	
	/*
	template <typename T, ulong U>
	template <typename B, typename VT, usize C>
	inline const B  Vec<T,I,U>::To () const
	{
		B				ret;
		const usize		count = GXMath::Min( Count(), C );

		FOR_range( i, 0, count )	ret[i] = VT( (*this)[i] );
		return ret;
	}
	*/
	
	template <typename T, ulong U>
	inline Vec<bool,I,U>  Vec<T,I,U>::Equal (const Self &right) const
	{
		Vec<bool,I,U>	ret;
		FOR( i, *this )	ret[i] = GXMath::Equals( (*this)[i], right[i] );
		return ret;
	}
		
	
	template <typename T, ulong U>
	inline Vec<bool,I,U>  Vec<T,I,U>::Less (const Self &right) const
	{
		Vec<bool,I,U>	ret;
		FOR( i, *this )	ret[i] = ( (*this)[i] < right[i] ) & not GXMath::Equals( (*this)[i], right[i] );
		return ret;
	}
		
	
	template <typename T, ulong U>
	inline Vec<bool,I,U>  Vec<T,I,U>::LEqual (const Self &right) const
	{
		Vec<bool,I,U>	ret;
		FOR( i, *this )	ret[i] = ( (*this)[i] < right[i] ) | GXMath::Equals( (*this)[i], right[i] );
		return ret;
	}
		
	
	template <typename T, ulong U>
	inline Vec<bool,I,U>  Vec<T,I,U>::Greater (const Self &right) const
	{
		Vec<bool,I,U>	ret;
		FOR( i, *this )	ret[i] = ( (*this)[i] > right[i] ) & not GXMath::Equals( (*this)[i], right[i] );
		return ret;
	}
		
	
	template <typename T, ulong U>
	inline Vec<bool,I,U>  Vec<T,I,U>::GEqual (const Self &right) const
	{
		Vec<bool,I,U>	ret;
		FOR( i, *this )	ret[i] = ( (*this)[i] > right[i] ) | GXMath::Equals( (*this)[i], right[i] );
		return ret;
	}
		
	
	template <typename T, ulong U>
	inline Vec<bool,I,U>  Vec<T,I,U>::IsZero () const
	{
		Vec<bool,I,U>	ret;
		FOR( i, *this )	ret[i] = GXMath::IsZero( (*this)[i] );
		return ret;
	}
		
	
	template <typename T, ulong U>
	inline Vec<bool,I,U>  Vec<T,I,U>::IsNotZero () const
	{
		Vec<bool,I,U>	ret;
		FOR( i, *this )	ret[i] = GXMath::IsNotZero( (*this)[i] );
		return ret;
	}
	
		
	template <typename T, ulong U>
	inline Vec<T,I,U>  Vec<T,I,U>::Reflect (const Self &normal) const
	{
		return (*this) - T(2) * normal.Dot( *this ) * normal;
	}

		
	template <typename T, ulong U>
	inline Vec<T,I,U>  Vec<T,I,U>::Refract (const Self &normal, T eta) const
	{
		T const		k = T(1) - eta * eta * ( T(1) - GXMath::Square( normal.Dot( *this ) ) );
		Self		r;

		if ( k >= T(0) )
			r = eta * (*this) - ( eta * normal.Dot( *this ) + Sqrt( k ) ) * normal;

		return r;
	}

	
	template <typename T, ulong U>
	template <Swizzle::type SW>
	inline constexpr auto  Vec<T,I,U>::Swizzle () const -> Vec< T, _math_hidden_::SWLength<SW>, U >
	{
		Vec< T, _math_hidden_::SWLength<SW>, U >	res;
		constexpr uint								swizzle = _math_hidden_::_ParseSwizzle( SW, I );
		STATIC_ASSERT( swizzle != -1 );

		FOR( i, res )
		{
			const uint j = (swizzle >> (i*4)) & 0xF;
			res[i] =	j > 0 and j <= I ? (*this)[j-1] :
						j == 5 ? T(0) :
						j == 6 ? T(1) :
						GXTypes::MaxValue<T>();
		}
		return res;
	}
	

	template <typename T, ulong U>
	inline Vec<T,4,U>  Vec<T,I,U>::Swizzle (Swizzle::type sw) const
	{
		Vec<T,4,U>	res;
		const uint	swizzle = _math_hidden_::_ParseSwizzle( sw, I );
		
		FOR( i, res )
		{
			const uint j = (swizzle >> (i*4)) & 0xF;
			res[i] =	j > 0 and j <= I ? (*this)[j-1] :
						j == 5 ? T(0) :
						j == 6 ? T(1) :
						GXTypes::MaxValue<T>();
		}
		return res;
	}
