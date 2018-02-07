// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'


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
		using Vec2_t		= Vec<T,2,U>;
		using Vec3_t		= Vec<T,3,U>;
		using Vec4_t		= Vec<T,4,U>;
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

		constexpr explicit Vec (const Vec2_t &v): x(v.x) {}
		constexpr explicit Vec (const Vec3_t &v): x(v.x) {}
		constexpr explicit Vec (const Vec4_t &v): x(v.x) {}

		template <typename B>
		constexpr explicit Vec (const Vec<B,1,U> &v): x(T(v.x)) {}

		Self& operator = (T X)								{ x = X; return *this; }

		CHECKRES constexpr explicit operator T () const		{ return x; }
#	endif

#	if I == 2
	// variables
		T	x,
			y;

	// constructors
		constexpr Vec (GX_DEFCTOR): x(0), y(0) {}
		constexpr Vec (const Vec2_t &v): x(v.x), y(v.y) {}

		constexpr explicit Vec (T V): x(V), y(V) {}
		constexpr explicit Vec (const Vec3_t &v): x(v.x), y(v.y) {}
		constexpr explicit Vec (const Vec4_t &v): x(v.x), y(v.y) {}

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
		constexpr Vec (const Vec3_t &v): x(v.x), y(v.y), z(v.z) {}

		constexpr explicit Vec (T V): x(V), y(V), z(V) {}

		template <typename A, typename B, typename C>
		constexpr explicit Vec (A X, B Y, C Z): x(X), y(Y), z(Z) {}

		template <typename A>
		constexpr explicit Vec (A X, const Vec2_t &YZ): x(X), y(YZ[0]), z(YZ[1]) {}

		template <typename A>
		constexpr explicit Vec (const Vec2_t &XY, A Z): x(XY[0]), y(XY[1]), z(Z) {}

		constexpr explicit Vec (const Vec2_t &XY): x(XY[0]), y(XY[1]), z(0) {}
		constexpr explicit Vec (const Vec4_t &v): x(v.x), y(v.y), z(v.z) {}

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
		constexpr Vec (const Vec4_t &v): x(v.x), y(v.y), z(v.z), w(v.w) {}

		constexpr explicit Vec (T V): x(V), y(V), z(V), w(V) {}
		constexpr explicit Vec (const Vec2_t &XY): x(XY[0]), y(XY[1]), z(T(0)), w(T(0)) {}

		template <typename A, typename B, typename C, typename D>
		constexpr explicit Vec (A X, B Y, C Z, D W): x(X), y(Y), z(Z), w(W) {}

		template <typename A, typename B>
		constexpr explicit Vec (A X, const Vec2_t &YZ, B W): x(X), y(YZ[0]), z(YZ[1]), w(W) {}

		template <typename A, typename B>
		constexpr explicit Vec (A X, B Y, const Vec2_t &ZW): x(X), y(Y), z(ZW[0]), w(ZW[1]) {}

		constexpr explicit Vec (const Vec2_t &XY, const Vec2_t &ZW): x(XY[0]), y(XY[1]), z(ZW[0]), w(ZW[1]) {}

		template <typename A>
		constexpr explicit Vec (const Vec3_t &XYZ, A W): x(XYZ[0]), y(XYZ[1]), z(XYZ[2]), w(W) {}
		
		template <typename A>
		constexpr explicit Vec (A X, const Vec3_t &YZW): x(X), y(YZW[0]), z(YZW[1]), w(YZW[2]) {}

		constexpr explicit Vec (const Vec3_t &XYZ): x(XYZ[0]), y(XYZ[1]), z(XYZ[2]), w(T(0)) {}

		template <typename A, typename B>
		constexpr explicit Vec (const Vec2_t &XY, A Z, B W) : x(XY[0]), y(XY[1]), z(Z), w(W) {}

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
		CHECKRES T &				operator [] (usize i)						{ ASSUME(i<I);  return ptr()[i]; }
		CHECKRES T const &			operator [] (usize i) const					{ ASSUME(i<I);  return ptr()[i]; }

		CHECKRES constexpr BVec_t	operator == (const Self & right) const		{ return Equal( right ); }
		CHECKRES constexpr BVec_t	operator != (const Self & right) const		{ return not Equal( right ); }
		CHECKRES constexpr BVec_t	operator >  (const Self & right) const		{ return Greater( right ); }
		CHECKRES constexpr BVec_t	operator >= (const Self & right) const		{ return not Less( right ); }
		CHECKRES constexpr BVec_t	operator <  (const Self & right) const		{ return Less( right ); }
		CHECKRES constexpr BVec_t	operator <= (const Self & right) const		{ return not Greater( right ); }
		
#	if I == 1
		CHECKRES constexpr bool		operator == (const T& right) const			{ return x == right; }
		CHECKRES constexpr bool		operator != (const T& right) const			{ return x != right; }
		CHECKRES constexpr bool		operator >  (const T& right) const			{ return x >  right; }
		CHECKRES constexpr bool		operator >= (const T& right) const			{ return x >= right; }
		CHECKRES constexpr bool		operator <  (const T& right) const			{ return x <  right; }
		CHECKRES constexpr bool		operator <= (const T& right) const			{ return x <= right; }
		
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
		
		Self &  operator %= (const Self& right)
		{
			FOR( i, *this )	(*this)[i] = Mod( (*this)[i], right[i] );
			return *this;
		}
		
		CHECKRES constexpr const Self  operator %  (const Self& right) const
		{
			Self	ret;
			FOR( i, *this )	 ret[i] = Mod( (*this)[i], right[i] );
			return ret;
		}

		Self &  operator %= (const T& right)
		{
			FOR( i, *this )	 (*this)[i] = Mod( (*this)[i], right );
			return *this;
		}

		CHECKRES constexpr const Self  operator %  (const T& right) const
		{
			Self	ret;
			FOR( i, *this )	 ret[i] = Mod( (*this)[i], right );
			return ret;
		}

		CHECKRES friend constexpr const Self  operator % (const T& left, const Self& right)
		{
			return Self(left) % right;
		}

#	else
		CHECKRES constexpr BVec_t		operator == (const T& right) const				{ return Equal( Self(right) ); }
		CHECKRES constexpr BVec_t		operator != (const T& right) const				{ return not Equal( Self(right) ); }
		CHECKRES constexpr BVec_t		operator >  (const T& right) const				{ return Greater( Self(right) ); }
		CHECKRES constexpr BVec_t		operator >= (const T& right) const				{ return GEqual( Self(right) ); }
		CHECKRES constexpr BVec_t		operator <  (const T& right) const				{ return Less( Self(right) ); }
		CHECKRES constexpr BVec_t		operator <= (const T& right) const				{ return LEqual( Self(right) ); }

		CHECKRES constexpr BVec_t		operator == (const Scalar_t& right) const		{ return Equal( Self(right.x) ); }
		CHECKRES constexpr BVec_t		operator != (const Scalar_t& right) const		{ return not Equal( Self(right.x) ); }
		CHECKRES constexpr BVec_t		operator >  (const Scalar_t& right) const		{ return Greater( Self(right.x) ); }
		CHECKRES constexpr BVec_t		operator >= (const Scalar_t& right) const		{ return GEqual( Self(right.x) ); }
		CHECKRES constexpr BVec_t		operator <  (const Scalar_t& right) const		{ return Less( Self(right.x) ); }
		CHECKRES constexpr BVec_t		operator <= (const Scalar_t& right) const		{ return LEqual( Self(right.x) ); }

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
		

		Self &  operator %= (const Self& right)
		{
			FOR( i, *this )	(*this)[i] = Mod( (*this)[i], right[i] );
			return *this;
		}
		
		CHECKRES constexpr const Self  operator %  (const Self& right) const
		{
			Self	ret;
			FOR( i, *this )	 ret[i] = Mod( (*this)[i], right[i] );
			return ret;
		}

		Self &  operator %= (const T& right)
		{
			FOR( i, *this )	 (*this)[i] = Mod( (*this)[i], right );
			return *this;
		}

		CHECKRES constexpr const Self  operator %  (const T& right) const
		{
			Self	ret;
			FOR( i, *this )	 ret[i] = Mod( (*this)[i], right );
			return ret;
		}

		CHECKRES friend constexpr const Self  operator % (const T& left, const Self& right)
		{
			return Self(left) % right;
		}
#	endif


		// methods
		T *			ptr ()										{ _CheckAlign();  return &x; }
		T const *	ptr ()	const								{ _CheckAlign();  return &x; }

		static constexpr usize Count ()							{ return I; }

		T			Min () const;
		T			Max () const;

		T			Length () const;
		T			LengthSqr () const;
		Self		Normalized () const;
		void		GetNormLength (OUT Self &norm, OUT T &length) const;
		Self &		Normalize ();
		Self &		SetLength (T len);
		Self		WithLength (T len) const;

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
		CHECKRES constexpr T		Sum ()		const	{ return x; }
		CHECKRES constexpr T		SumAbs ()	const	{ return Abs(x); }
		CHECKRES constexpr T		Area ()		const	{ return x*1; }
		CHECKRES constexpr T		Volume ()	const	{ return x*1*1; }
#	endif

#	if I >= 1
		CHECKRES constexpr Vec2_t	xo ()		const	{ return Vec2_t( x, T(0) ); }
		CHECKRES constexpr Vec2_t	ox ()		const	{ return Vec2_t( T(0), x ); }
		CHECKRES constexpr Vec2_t	xx ()		const	{ return Vec2_t( x, x ); }

		CHECKRES constexpr Vec3_t	xoo ()		const	{ return Vec3_t( x, T(0), T(0) ); }

		CHECKRES constexpr Vec4_t	xooo ()		const	{ return Vec4_t( x, T(0), T(0), T(0) ); }
#	endif

#	if I == 2
		CHECKRES constexpr T		Sum ()		const	{ return x + y; }
		CHECKRES constexpr T		SumAbs ()	const	{ return Abs(x) + Abs(y); }
		CHECKRES constexpr T		Volume ()	const	{ return x*y*1; }
#	endif

#	if I >= 2
		CHECKRES constexpr T		Area ()		const	{ return x*y; }
		CHECKRES constexpr Vec2_t	yo ()		const	{ return Vec2_t( y, T(0) ); }
		CHECKRES constexpr Vec2_t	yx ()		const	{ return Vec2_t( y, x ); }
		CHECKRES constexpr Vec2_t	xy ()		const	{ return Vec2_t( x, y ); }
		CHECKRES constexpr Vec2_t	oy ()		const	{ return Vec2_t( T(0), y ); }
		CHECKRES constexpr Vec2_t	yy ()		const	{ return Vec2_t( y, y ); }

		CHECKRES constexpr Vec3_t	oyo ()		const	{ return Vec3_t( T(0), y, T(0) ); }
		CHECKRES constexpr Vec3_t	xyo ()		const	{ return Vec3_t( x, y, T(0) ); }
		CHECKRES constexpr Vec3_t	xoy ()		const	{ return Vec3_t( x, T(0), y ); }
		CHECKRES constexpr Vec3_t	oyx ()		const	{ return Vec3_t( T(0), y, x ); }

		CHECKRES constexpr Vec4_t	xooy ()		const	{ return Vec4_t( x, T(0), T(0), y ); }
		CHECKRES constexpr Vec4_t	xyoo ()		const	{ return Vec4_t( x, y, T(0), T(0) ); }
		CHECKRES constexpr Vec4_t	xyxy ()		const	{ return Vec4_t( x, y, x, y ); }
#	endif

#	if I == 3
		CHECKRES constexpr T		Sum ()		const	{ return x + y + z; }
		CHECKRES constexpr T		SumAbs ()	const	{ return Abs(x) + Abs(y) + Abs(z); }
#	endif

#	if I >= 3
		CHECKRES constexpr T		Volume ()	const	{ return x*y*z; }
		CHECKRES constexpr Vec2_t	yz ()		const	{ return Vec2_t( y, z ); }
		CHECKRES constexpr Vec2_t	xz ()		const	{ return Vec2_t( x, z ); }
		CHECKRES constexpr Vec3_t	xyz ()		const	{ return Vec3_t( x, y, z ); }
		CHECKRES constexpr Vec3_t	zyx ()		const	{ return Vec3_t( z, y, x ); }
		CHECKRES constexpr Vec3_t	xzy ()		const	{ return Vec3_t( x, z, y ); }

		CHECKRES constexpr Vec3_t	ooz ()		const	{ return Vec3_t( T(0), T(0), z ); }
		CHECKRES constexpr Vec3_t	xoz ()		const	{ return Vec3_t( x, T(0), z ); }
		CHECKRES constexpr Vec3_t	oyz ()		const	{ return Vec3_t( T(0), y, z ); }
		
		CHECKRES constexpr Vec4_t	xyoz ()		const	{ return Vec4_t( x, y, T(0), z ); }
		CHECKRES constexpr Vec4_t	xyzo ()		const	{ return Vec4_t( x, y, z, T(0) ); }
#	endif

#	if I == 4
		CHECKRES constexpr T		Sum ()		const	{ return x + y + z + w; }
		CHECKRES constexpr T		SumAbs ()	const	{ return Abs(x) + Abs(y) + Abs(z) + Abs(w); }
#	endif

#	if I >= 4
		CHECKRES constexpr T		Volume4D ()	const	{ return x * y * z * w; }
		CHECKRES constexpr Vec2_t	xw ()		const	{ return Vec2_t( x, w ); }
		CHECKRES constexpr Vec2_t	yw ()		const	{ return Vec2_t( y, w ); }
		CHECKRES constexpr Vec2_t	zw ()		const	{ return Vec2_t( z, w ); }
		
		CHECKRES constexpr Vec3_t	xwo ()		const	{ return Vec3_t( x, y, T(0) ); }
		CHECKRES constexpr Vec3_t	xyw ()		const	{ return Vec3_t( x, y, w ); }
		CHECKRES constexpr Vec3_t	yzw ()		const	{ return Vec3_t( y, z, w ); }
		
		CHECKRES constexpr Vec4_t	xyzw ()		const	{ return Vec4_t( x, y, z, w ); }
		CHECKRES constexpr Vec4_t	wzyx ()		const	{ return Vec4_t( w, z, y, x ); }
#	endif

		template <Swizzle::type SW>
		CHECKRES constexpr auto Swizzle () const -> Vec< T, _math_hidden_::SWLength<SW>, U >;

		CHECKRES Vec4_t			Swizzle (Swizzle::type sw) const;


	private:
		static void _CheckAlign ();
	};



/*
=================================================
	operator ++
=================================================
*/
	template <typename T, ulong U>
	inline Vec<T,I,U> &  Vec<T,I,U>::operator ++ ()
	{
		FOR( i, *this )	++(*this)[i];
		return *this;
	}

/*
=================================================
	operator --
=================================================
*/
	template <typename T, ulong U>
	inline Vec<T,I,U> &  Vec<T,I,U>::operator -- ()
	{
		FOR( i, *this )	--(*this)[i];
		return *this;
	}

/*
=================================================
	operator ++
=================================================
*/
	template <typename T, ulong U>
	CHECKRES inline const Vec<T,I,U>  Vec<T,I,U>::operator ++ (int)
	{
		Self	ret(*this);
		++(*this);
		return ret;
	}

/*
=================================================
	operator --
=================================================
*/
	template <typename T, ulong U>
	CHECKRES inline const Vec<T,I,U>  Vec<T,I,U>::operator -- (int)
	{
		Self	ret(*this);
		--(*this);
		return ret;
	}

/*
=================================================
	Min
=================================================
*/
	template <typename T, ulong U>
	CHECKRES inline T  Vec<T,I,U>::Min () const
	{
		T	t_min = (*this)[0];
		FOR( i, *this )  (*this)[i] < t_min ?  t_min = (*this)[i]  : 0;
		return t_min;
	}

/*
=================================================
	Max
=================================================
*/
	template <typename T, ulong U>
	CHECKRES inline T  Vec<T,I,U>::Max () const
	{
		T	t_max = (*this)[0];
		FOR( i, *this )  (*this)[i] > t_max ?  t_max = (*this)[i]  : 0;
		return t_max;
	}

/*
=================================================
	Length
=================================================
*/
	template <typename T, ulong U>
	CHECKRES inline T  Vec<T,I,U>::Length () const
	{
		return Sqrt( LengthSqr() );
	}

/*
=================================================
	LengthSqr
=================================================
*/
	template <typename T, ulong U>
	CHECKRES inline T  Vec<T,I,U>::LengthSqr () const
	{
		Self	res;
		FOR( i, *this )	res[i] = GXMath::Square( (*this)[i] );
		return res.Sum();
	}

/*
=================================================
	Normalized
=================================================
*/
	template <typename T, ulong U>
	CHECKRES inline Vec<T,I,U>  Vec<T,I,U>::Normalized () const
	{
		const T		len_sqr = LengthSqr();
		return len_sqr != T(0) ?  Self(*this) * InvSqrt( len_sqr )  :  Self();
	}
	
/*
=================================================
	GetNormLength
=================================================
*/
	template <typename T, ulong U>
	inline void  Vec<T,I,U>::GetNormLength (OUT Self &norm, OUT T &length) const
	{
		length = LengthSqr();

		if ( GXMath::IsNotZero( length ) )
		{
			T	l	= InvSqrt( length );
			norm	= (*this) * l;
			length	= T(1) / l;
		}
		else
		{
			norm	= Self();
			length	= T(0);
		}
	}

/*
=================================================
	Normalize
=================================================
*/
	template <typename T, ulong U>
	inline Vec<T,I,U> &  Vec<T,I,U>::Normalize ()
	{
		return ( *this = Normalized() );
	}

/*
=================================================
	SetLength
=================================================
*/
	template <typename T, ulong U>
	inline Vec<T,I,U> &  Vec<T,I,U>::SetLength (T len)
	{
		len *= InvSqrt( LengthSqr() );
		FOR( i, *this )  (*this)[i] *= len;
		return *this;
	}
	
/*
=================================================
	WithLength
=================================================
*/
	template <typename T, ulong U>
	CHECKRES inline Vec<T,I,U>  Vec<T,I,U>::WithLength (T len) const
	{
		return Self( *this ).SetLength( len );
	}

/*
=================================================
	MakeCeil
=================================================
*/
	template <typename T, ulong U>
	inline Vec<T,I,U> &  Vec<T,I,U>::MakeCeil (const Self &sVec)
	{
		FOR( i, *this )  sVec[i] > (*this)[i]  ?  ((*this)[i] = sVec[i])  : T(0);
		return *this;
	}

/*
=================================================
	MakeFloor
=================================================
*/
	template <typename T, ulong U>
	inline Vec<T,I,U> &  Vec<T,I,U>::MakeFloor (const Self &sVec)
	{
		FOR( i, *this )  sVec[i] < (*this)[i]  ?  ((*this)[i] = sVec[i])  : T(0);
		return *this;
	}

/*
=================================================
	Dot
=================================================
*/
	template <typename T, ulong U>
	CHECKRES inline T  Vec<T,I,U>::Dot (const Self &right) const
	{
		Self	res;
		FOR( i, *this )	res[i] = (*this)[i] * right[i];
		return res.Sum();
	}

/*
=================================================
	DotAbs
=================================================
*/
	template <typename T, ulong U>
	CHECKRES inline T  Vec<T,I,U>::DotAbs (const Self &right) const
	{
		Self	res;
		FOR( i, *this )	res[i] = Abs( (*this)[i] * right[i] );
		return res.Sum();
	}
	
/*
=================================================
	Distance
=================================================
*/
	template <typename T, ulong U>
	CHECKRES inline T  Vec<T,I,U>::Distance (const Self &sVec) const
	{
		return ( (*this) - sVec ).Length();
	}

/*
=================================================
	DistanceSqr
=================================================
*/
	template <typename T, ulong U>
	CHECKRES inline T  Vec<T,I,U>::DistanceSqr (const Self &sVec) const
	{
		return ( (*this) - sVec ).LengthSqr();
	}
	
/*
=================================================
	Convert
=================================================
*/
	template <typename T, ulong U>
	template <typename T2>
	CHECKRES inline const Vec<T2,I,U>  Vec<T,I,U>::Convert () const
	{
		Vec<T2,I,U>	ret;
		FOR( i, *this )	ret[i] = T2( (*this)[i] );
		return ret;
	}
	
/*
=================================================
	To
=================================================
*/
	template <typename T, ulong U>
	template <typename B>
	CHECKRES inline const B  Vec<T,I,U>::To () const
	{
		STATIC_ASSERT( typename B::_is_vector(true), "type is not vector" );
		
		B				ret;
		const usize		count = GXMath::Min( Count(), B::Count() );

		FOR_range( i, 0, count )	ret[i] = typename B::Value_t( (*this)[i] );
		return ret;
	}

/*
=================================================
	Equal
=================================================
*/
	template <typename T, ulong U>
	CHECKRES inline Vec<bool,I,U>  Vec<T,I,U>::Equal (const Self &right) const
	{
		Vec<bool,I,U>	ret;
		FOR( i, *this )	ret[i] = GXMath::Equals( (*this)[i], right[i] );
		return ret;
	}

/*
=================================================
	Less
=================================================
*/
	template <typename T, ulong U>
	CHECKRES inline Vec<bool,I,U>  Vec<T,I,U>::Less (const Self &right) const
	{
		Vec<bool,I,U>	ret;
		FOR( i, *this )	ret[i] = ( (*this)[i] < right[i] ) & not GXMath::Equals( (*this)[i], right[i] );
		return ret;
	}

/*
=================================================
	LEqual
=================================================
*/
	template <typename T, ulong U>
	CHECKRES inline Vec<bool,I,U>  Vec<T,I,U>::LEqual (const Self &right) const
	{
		Vec<bool,I,U>	ret;
		FOR( i, *this )	ret[i] = ( (*this)[i] < right[i] ) | GXMath::Equals( (*this)[i], right[i] );
		return ret;
	}

/*
=================================================
	Greater
=================================================
*/
	template <typename T, ulong U>
	CHECKRES inline Vec<bool,I,U>  Vec<T,I,U>::Greater (const Self &right) const
	{
		Vec<bool,I,U>	ret;
		FOR( i, *this )	ret[i] = ( (*this)[i] > right[i] ) & not GXMath::Equals( (*this)[i], right[i] );
		return ret;
	}

/*
=================================================
	GEqual
=================================================
*/
	template <typename T, ulong U>
	CHECKRES inline Vec<bool,I,U>  Vec<T,I,U>::GEqual (const Self &right) const
	{
		Vec<bool,I,U>	ret;
		FOR( i, *this )	ret[i] = ( (*this)[i] > right[i] ) | GXMath::Equals( (*this)[i], right[i] );
		return ret;
	}

/*
=================================================
	IsZero
=================================================
*/
	template <typename T, ulong U>
	CHECKRES inline Vec<bool,I,U>  Vec<T,I,U>::IsZero () const
	{
		Vec<bool,I,U>	ret;
		FOR( i, *this )	ret[i] = GXMath::IsZero( (*this)[i] );
		return ret;
	}

/*
=================================================
	IsNotZero
=================================================
*/
	template <typename T, ulong U>
	CHECKRES inline Vec<bool,I,U>  Vec<T,I,U>::IsNotZero () const
	{
		Vec<bool,I,U>	ret;
		FOR( i, *this )	ret[i] = GXMath::IsNotZero( (*this)[i] );
		return ret;
	}

/*
=================================================
	Reflect
=================================================
*/
	template <typename T, ulong U>
	CHECKRES inline Vec<T,I,U>  Vec<T,I,U>::Reflect (const Self &normal) const
	{
		return (*this) - T(2) * normal.Dot( *this ) * normal;
	}
	
/*
=================================================
	Refract
=================================================
*/
	template <typename T, ulong U>
	CHECKRES inline Vec<T,I,U>  Vec<T,I,U>::Refract (const Self &normal, T eta) const
	{
		T const		k = T(1) - eta * eta * ( T(1) - GXMath::Square( normal.Dot( *this ) ) );
		Self		r;

		if ( k >= T(0) )
			r = eta * (*this) - ( eta * normal.Dot( *this ) + Sqrt( k ) ) * normal;

		return r;
	}
	
/*
=================================================
	Swizzle
=================================================
*/
	template <typename T, ulong U>
	template <Swizzle::type SW>
	CHECKRES inline constexpr auto  Vec<T,I,U>::Swizzle () const -> Vec< T, _math_hidden_::SWLength<SW>, U >
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
	
/*
=================================================
	Swizzle
=================================================
*/
	template <typename T, ulong U>
	CHECKRES inline Vec<T,4,U>  Vec<T,I,U>::Swizzle (Swizzle::type sw) const
	{
		Vec4_t	res;
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
	
/*
=================================================
	_CheckAlign
=================================================
*/
	template <typename T, ulong U>
	inline void Vec<T,I,U>::_CheckAlign ()
	{
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
