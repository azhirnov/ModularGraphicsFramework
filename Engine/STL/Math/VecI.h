// Copyright © 2014-2017  Zhirnov Andrey. All rights reserved.


	//
	// Vector
	//

	template <typename T>
	struct Vec<T,I> : public CompileTime::CopyQualifiers< T >
	{
		STATIC_ASSERT( CompileTime::IsScalar<T> or TypeTraits::IsEnum<T> );

	// types
		typedef bool			_is_vector;
		typedef Vec<T,I>		Self;
		typedef T				value_t;
		typedef Vec<bool,I>		bvec;
		typedef T				arr_t[I];
		enum { STATIC_COUNT = I };


#	if I == 1
	// variables
		T	x;

	// constructors
		Vec (GX_DEFCTOR): x(0) {}
		Vec (const Vec<T,1> &v): x(v.x) {}

		explicit Vec (T X): x(X) {}
		explicit Vec (const Vec<T,2> &v): x(v.x) {}
		explicit Vec (const Vec<T,3> &v): x(v.x) {}
		explicit Vec (const Vec<T,4> &v): x(v.x) {}

		template <typename B>
		explicit Vec (const Vec<B,1> &v): x(T(v.x)) {}
#	endif

#	if I == 2
	// variables
		T	x,
			y;

	// constructors
		Vec (GX_DEFCTOR): x(0), y(0) {}
		Vec (const Vec<T,2> &v): x(v.x), y(v.y) {}

		explicit Vec (T V): x(V), y(V) {}
		explicit Vec (T X, T Y): x(X), y(Y) {}
		explicit Vec (const Vec<T,3> &v): x(v.x), y(v.y) {}
		explicit Vec (const Vec<T,4> &v): x(v.x), y(v.y) {}

		template <typename B>
		explicit Vec (const Vec<B,2> &v): x(T(v.x)), y(T(v.y)) {}
#	endif

#	if I == 3
	// variables
		T	x,
			y,
			z;

	// constructors
		Vec (GX_DEFCTOR): x(0), y(0), z(0) {}
		Vec (const Vec<T,3> &v): x(v.x), y(v.y), z(v.z) {}

		explicit Vec (T V): x(V), y(V), z(V) {}
		explicit Vec (T X, T Y, T Z): x(X), y(Y), z(Z) {}

		template <typename A>
		explicit Vec (A X, const Vec<T,2> &YZ): x(X), y(YZ[0]), z(YZ[1]) {}

		template <typename A>
		explicit Vec (const Vec<T,2> &XY, A Z): x(XY[0]), y(XY[1]), z(Z) {}

		explicit Vec (const Vec<T,2> &XY): x(XY[0]), y(XY[1]), z(0) {}
		explicit Vec (const Vec<T,4> &v): x(v.x), y(v.y), z(v.z) {}

		template <typename B>
		explicit Vec (const Vec<B,3> &v): x(T(v.x)), y(T(v.y)), z(T(v.z)) {}
#	endif

#	if I == 4
	// variables
		T	x,
			y,
			z,
			w;

	// constructors
		Vec (GX_DEFCTOR): x(0), y(0), z(0), w(0) {}
		Vec (const Vec<T,4> &v): x(v.x), y(v.y), z(v.z), w(v.w) {}

		explicit Vec (T V): x(V), y(V), z(V), w(V) {}
		explicit Vec (T X, T Y, T Z, T W): x(X), y(Y), z(Z), w(W) {}
		explicit Vec (const Vec<T,2> &XY): x(XY[0]), y(XY[1]), z(0), w(0) {}

		template <typename A, typename B>
		explicit Vec (A X, const Vec<T,2> &YZ, B W): x(X), y(YZ[0]), z(YZ[1]), w(W) {}

		template <typename A, typename B>
		explicit Vec (A X, B Y, const Vec<T,2> &ZW): x(X), y(Y), z(ZW[0]), w(ZW[1]) {}

		explicit Vec (const Vec<T,2> &XY, const Vec<T,2> &ZW): x(XY[0]), y(XY[1]), z(ZW[0]), w(ZW[1]) {}

		template <typename A>
		explicit Vec (const Vec<T,3> &XYZ, A W): x(XYZ[0]), y(XYZ[1]), z(XYZ[2]), w(W) {}

		explicit Vec (const Vec<T,3> &XYZ): x(XYZ[0]), y(XYZ[1]), z(XYZ[2]), w(0) {}

		template <typename A, typename B>
		explicit Vec (const Vec<T,2> &XY, A Z, B W) : x(XY[0]), y(XY[1]), z(Z), w(W) {}

		template <typename B>
		explicit Vec (const Vec<B,4> &v): x(T(v.x)), y(T(v.y)), z(T(v.z)), w(T(v.w)) {}
#	endif

#	if I >= 5 or I < 0
		STATIC_WARNING( "not supported!" );
#	endif

		
		// unary operators
		VEC_UN_OPERATOR( - );
		VEC_UN_OPERATOR( ~ );
		VEC_UN_OPERATOR( ! );

		Self &		operator ++ ();
		Self &		operator -- ();
		const Self	operator ++ (int);
		const Self	operator -- (int);


		// binary operators
		T &			operator [] (usize i)						{ ASSUME(i<I);  return ptr()[i]; }
		T const &	operator [] (usize i) const					{ ASSUME(i<I);  return ptr()[i]; }

		bvec		operator == (const Self & right) const;
		bvec		operator != (const Self & right) const;
		bvec		operator >  (const Self & right) const;
		bvec		operator >= (const Self & right) const;
		bvec		operator <  (const Self & right) const;
		bvec		operator <= (const Self & right) const;
		
		bvec		operator == (const T& right) const;
		bvec		operator != (const T& right) const;
		bvec		operator >  (const T& right) const;
		bvec		operator >= (const T& right) const;
		bvec		operator <  (const T& right) const;
		bvec		operator <= (const T& right) const;
		
		/*friend bvec	operator == (const T& left, const Self &right)	{ return Self(left) == right; }
		friend bvec	operator != (const T& left, const Self &right)	{ return Self(left) != right; }
		friend bvec	operator >  (const T& left, const Self &right)	{ return Self(left) >  right; }
		friend bvec	operator >= (const T& left, const Self &right)	{ return Self(left) >= right; }
		friend bvec	operator <  (const T& left, const Self &right)	{ return Self(left) <  right; }
		friend bvec	operator <= (const T& left, const Self &right)	{ return Self(left) <= right; }*/

		VEC_BIN_OPERATOR( +  );
		VEC_BIN_OPERATOR( -  );
		VEC_BIN_OPERATOR( *  );
		VEC_BIN_OPERATOR( /  );

		// only for integer types
		VEC_BIN_OPERATOR( &  );
		VEC_BIN_OPERATOR( |  );
		VEC_BIN_OPERATOR( ^  );
		VEC_BIN_OPERATOR( << );
		VEC_BIN_OPERATOR( >> );
		
		VEC_BIN_OPERATOR2( && );
		VEC_BIN_OPERATOR2( || );
		

		Self &		operator %= (const Self& right)
		{
			FOR( i, *this )	(*this)[i] = Mod( (*this)[i], right[i] );
			return *this;
		}
		
		const Self	operator %  (const Self& right) const
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

		const Self	operator %  (const T& right) const
		{
			Self	ret;
			FOR( i, *this )	 ret[i] = Mod( (*this)[i], right );
			return ret;
		}

		friend const Self operator % (const T& left, const Self& right)
		{
			return Self(left) % right;
		}


		// methods
		T *			ptr ()										{ return PointerCast< T >( this ); }
		T const *	ptr ()	const								{ return PointerCast< T >( this ); }
		arr_t &		ref ()										{ return *PointerCast< arr_t >( this ); }
		arr_t const& ref ()	const								{ return *PointerCast< arr_t >( this ); }

		static usize Count ()									{ return I; }

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
		const Vec<T2,I> Convert () const;

		template <typename B>
		const B		To () const;


		// compare methods
		bvec		Equal (const Self &right)	const;	// ==
		bvec		Less (const Self &right)	const;	// <
		bvec		LEqual (const Self &right)	const;	// <=
		bvec		Greater (const Self &right)	const;	// >
		bvec		GEqual (const Self &right)	const;	// >=
		bvec		IsZero ()					const;
		bvec		IsNotZero ()				const;


#	if I == 1
		T			Sum ()		const	{ return x; }
		T			SumAbs ()	const	{ return Abs(x); }
		T			Area ()		const	{ return x*1; }
		T			Volume ()	const	{ return x*1*1; }
#	endif

#	if I == 2
		T			Sum ()		const	{ return x + y; }
		T			SumAbs ()	const	{ return Abs(x) + Abs(y); }
		T			Volume ()	const	{ return x*y*1; }
#	endif

#	if I >= 2
		T			Area ()		const	{ return x*y; }
		Vec<T,2>	xo ()		const	{ return Vec<T,2>( x, 0 ); }
		Vec<T,2>	yo ()		const	{ return Vec<T,2>( y, 0 ); }
		Vec<T,2>	yx ()		const	{ return Vec<T,2>( y, x ); }
		Vec<T,2>	xy ()		const	{ return Vec<T,2>( x, y ); }
		Vec<T,2>	ox ()		const	{ return Vec<T,2>( 0, x ); }
		Vec<T,2>	oy ()		const	{ return Vec<T,2>( 0, y ); }
		Vec<T,2>	xx ()		const	{ return Vec<T,2>( x, x ); }
		Vec<T,2>	yy ()		const	{ return Vec<T,2>( y, y ); }

		Vec<T,3>	xoo ()		const	{ return Vec<T,3>( x, 0, 0 ); }
		Vec<T,3>	oyo ()		const	{ return Vec<T,3>( 0, y, 0 ); }
		Vec<T,3>	xyo ()		const	{ return Vec<T,3>( x, y, 0 ); }
		Vec<T,3>	xoy ()		const	{ return Vec<T,3>( x, 0, y ); }
		Vec<T,3>	oyx ()		const	{ return Vec<T,3>( 0, y, x ); }

		Vec<T,4>	xooo ()		const	{ return Vec<T,4>( x, 0, 0, 0 ); }
		Vec<T,4>	xooy ()		const	{ return Vec<T,4>( x, 0, 0, y ); }
		Vec<T,4>	xyoo ()		const	{ return Vec<T,4>( x, y, 0, 0 ); }
		Vec<T,4>	xyxy ()		const	{ return Vec<T,4>( x, y, x, y ); }
#	endif

#	if I == 3
		T			Sum ()		const	{ return x + y + z; }
		T			SumAbs ()	const	{ return Abs(x) + Abs(y) + Abs(z); }
#	endif

#	if I >= 3
		T			Volume ()	const	{ return x*y*z; }
		Vec<T,2>	yz ()		const	{ return Vec<T,2>( y, z ); }
		Vec<T,2>	xz ()		const	{ return Vec<T,2>( x, z ); }
		Vec<T,2>	yw ()		const	{ return Vec<T,2>( y, w ); }
		Vec<T,3>	xyz ()		const	{ return Vec<T,3>( x, y, z ); }
		Vec<T,3>	zyx ()		const	{ return Vec<T,3>( z, y, x ); }
		Vec<T,3>	xzy ()		const	{ return Vec<T,3>( x, z, y ); }

		Vec<T,3>	ooz ()		const	{ return Vec<T,3>( 0, 0, z ); }
		Vec<T,3>	xoz ()		const	{ return Vec<T,3>( x, 0, z ); }
		Vec<T,3>	oyz ()		const	{ return Vec<T,3>( 0, y, z ); }
		
		Vec<T,4>	xyoz ()		const	{ return Vec<T,4>( x, y, 0, z ); }
		Vec<T,4>	xyzo ()		const	{ return Vec<T,4>( x, y, z, 0 ); }
#	endif

#	if I == 4
		T			Sum ()		const	{ return x + y + z + w; }
		T			SumAbs ()	const	{ return Abs(x) + Abs(y) + Abs(z) + Abs(w); }
#	endif

#	if I >= 4
		T			Volume4D ()	const	{ return x*y*z*w; }
		Vec<T,2>	zw ()		const	{ return Vec<T,2>( z, w ); }
		
		Vec<T,3>	xwo ()		const	{ return Vec<T,3>( x, y, 0 ); }
		Vec<T,3>	xyw ()		const	{ return Vec<T,3>( x, y, w ); }
		Vec<T,3>	yzw ()		const	{ return Vec<T,3>( y, z, w ); }
		
		Vec<T,4>	xyzw ()		const	{ return Vec<T,4>( x, y, z, w ); }
		Vec<T,4>	wzyx ()		const	{ return Vec<T,4>( w, z, y, x ); }
#	endif
	};



	
		
	template <typename T>
	inline Vec<T,I> &  Vec<T,I>::operator ++ ()
	{
		FOR( i, *this )	++(*this)[i];
		return *this;
	}

		
	template <typename T>
	inline Vec<T,I> &  Vec<T,I>::operator -- ()
	{
		FOR( i, *this )	--(*this)[i];
		return *this;
	}

		
	template <typename T>
	inline const Vec<T,I>  Vec<T,I>::operator ++ (int)
	{
		Self	ret(*this);
		++(*this);
		return ret;
	}

		
	template <typename T>
	inline const Vec<T,I>  Vec<T,I>::operator -- (int)
	{
		Self	ret(*this);
		--(*this);
		return ret;
	}
	
	
	template <typename T>
	inline Vec<bool,I>  Vec<T,I>::operator == (const T& right) const
	{
		return Equal( Self(right) );
	}
	
	
	template <typename T>
	inline Vec<bool,I>  Vec<T,I>::operator != (const T& right) const
	{
		return not Equal( Self(right) );
	}

	
	template <typename T>
	inline Vec<bool,I>  Vec<T,I>::operator >  (const T& right) const
	{
		return Greater( Self(right) );
	}

	
	template <typename T>
	inline Vec<bool,I>  Vec<T,I>::operator >= (const T& right) const
	{
		return GEqual( Self(right) );
	}

	
	template <typename T>
	inline Vec<bool,I>  Vec<T,I>::operator <  (const T& right) const
	{
		return Less( Self(right) );
	}

	
	template <typename T>
	inline Vec<bool,I>  Vec<T,I>::operator <= (const T& right) const
	{
		return LEqual( Self(right) );
	}

		
	template <typename T>
	inline Vec<bool,I>  Vec<T,I>::operator == (const Self & right) const
	{
		return Equal( right );
	}
	

	template <typename T>
	inline Vec<bool,I>  Vec<T,I>::operator != (const Self & right) const
	{
		return not Equal( right );
	}

		
	template <typename T>
	inline Vec<bool,I>  Vec<T,I>::operator >  (const Self & right) const
	{
		return Greater( right );
	}

		
	template <typename T>
	inline Vec<bool,I>  Vec<T,I>::operator >= (const Self & right) const
	{
		return GEqual( right );
	}

		
	template <typename T>
	inline Vec<bool,I>  Vec<T,I>::operator <  (const Self & right) const
	{
		return Less( right );
	}

		
	template <typename T>
	inline Vec<bool,I>  Vec<T,I>::operator <= (const Self & right) const
	{
		return LEqual( right );
	}

	
	template <typename T>
	inline T  Vec<T,I>::Min () const
	{
		T	t_min = (*this)[0];
		FOR( i, *this )  (*this)[i] < t_min ?  t_min = (*this)[i]  : 0;
		return t_min;
	}

	
	template <typename T>
	inline T  Vec<T,I>::Max () const
	{
		T	t_max = (*this)[0];
		FOR( i, *this )  (*this)[i] > t_max ?  t_max = (*this)[i]  : 0;
		return t_max;
	}

	
	template <typename T>
	inline T  Vec<T,I>::Length () const
	{
		return Sqrt( LengthSqr() );
	}

	
	template <typename T>
	inline T  Vec<T,I>::LengthSqr () const
	{
		Self	res;
		FOR( i, *this )	res[i] = GXMath::Square( (*this)[i] );
		return res.Sum();
	}

	
	template <typename T>
	inline Vec<T,I>  Vec<T,I>::Normalized () const
	{
		const T		len_sqr = LengthSqr();
		return len_sqr != T(0) ?  Self(*this) * InvSqrt( len_sqr )  :  Self();
	}
	
	
	template <typename T>
	inline void  Vec<T,I>::GetNormLength (OUT Self &norm, OUT T &length) const
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

		
	template <typename T>
	inline Vec<T,I> &  Vec<T,I>::Normalize ()
	{
		return ( *this = Normalized() );
	}

		
	template <typename T>
	inline Vec<T,I> &  Vec<T,I>::SetLength (T len)
	{
		len *= InvSqrt( LengthSqr() );
		FOR( i, *this )  (*this)[i] *= len;
		return *this;
	}
	

	template <typename T>
	inline Vec<T,I>  Vec<T,I>::NewLength (T len) const
	{
		return Self( *this ).SetLength( len );
	}


	template <typename T>
	inline Vec<T,I> &  Vec<T,I>::MakeCeil (const Self &sVec)
	{
		FOR( i, *this )  sVec[i] > (*this)[i]  ?  (*this)[i] = sVec[i]  : 0;
		return *this;
	}
		

	template <typename T>
	inline Vec<T,I> &  Vec<T,I>::MakeFloor (const Self &sVec)
	{
		FOR( i, *this )  sVec[i] < (*this)[i]  ?  (*this)[i] = sVec[i]  : 0;
		return *this;
	}

		
	template <typename T>
	inline T  Vec<T,I>::Dot (const Self &right) const
	{
		Self	res;
		FOR( i, *this )	res[i] = (*this)[i] * right[i];
		return res.Sum();
	}
		

	template <typename T>
	inline T  Vec<T,I>::DotAbs (const Self &right) const
	{
		Self	res;
		FOR( i, *this )	res[i] = Abs( (*this)[i] * right[i] );
		return res.Sum();
	}

		
	template <typename T>
	inline T  Vec<T,I>::Distance (const Self &sVec) const
	{
		return ( (*this) - sVec ).Length();
	}
		

	template <typename T>
	inline T  Vec<T,I>::DistanceSqr (const Self &sVec) const
	{
		return ( (*this) - sVec ).LengthSqr();
	}

		
	template <typename T>
	template <typename T2>
	inline const Vec<T2,I>  Vec<T,I>::Convert () const
	{
		Vec<T2,I>	ret;
		FOR( i, *this )	ret[i] = T2( (*this)[i] );
		return ret;
	}
	
		
	template <typename T>
	template <typename B>
	inline const B  Vec<T,I>::To () const
	{
		STATIC_ASSERT( typename B::_is_vector(true), "type is not vector" );
		
		B				ret;
		const usize		count = GXMath::Min( Count(), B::Count() );

		FOR_range( i, 0, count )	ret[i] = typename B::value_t( (*this)[i] );
		return ret;
	}
	
	/*
	template <typename T>
	template <typename B, typename VT, usize C>
	inline const B  Vec<T,I>::To () const
	{
		B				ret;
		const usize		count = GXMath::Min( Count(), C );

		FOR_range( i, 0, count )	ret[i] = VT( (*this)[i] );
		return ret;
	}
	*/

	template <typename T>
	inline Vec<bool,I>  Vec<T,I>::Equal (const Self &right) const
	{
		Vec<bool,I>	ret;
		FOR( i, *this )	ret[i] = GXMath::Equals( (*this)[i], right[i] );
		return ret;
	}
		

	template <typename T>
	inline Vec<bool,I>  Vec<T,I>::Less (const Self &right) const
	{
		Vec<bool,I>	ret;
		FOR( i, *this )	ret[i] = ( (*this)[i] < right[i] ) & not GXMath::Equals( (*this)[i], right[i] );
		return ret;
	}
		

	template <typename T>
	inline Vec<bool,I>  Vec<T,I>::LEqual (const Self &right) const
	{
		Vec<bool,I>	ret;
		FOR( i, *this )	ret[i] = ( (*this)[i] < right[i] ) | GXMath::Equals( (*this)[i], right[i] );
		return ret;
	}
		

	template <typename T>
	inline Vec<bool,I>  Vec<T,I>::Greater (const Self &right) const
	{
		Vec<bool,I>	ret;
		FOR( i, *this )	ret[i] = ( (*this)[i] > right[i] ) & not GXMath::Equals( (*this)[i], right[i] );
		return ret;
	}
		

	template <typename T>
	inline Vec<bool,I>  Vec<T,I>::GEqual (const Self &right) const
	{
		Vec<bool,I>	ret;
		FOR( i, *this )	ret[i] = ( (*this)[i] > right[i] ) | GXMath::Equals( (*this)[i], right[i] );
		return ret;
	}
		

	template <typename T>
	inline Vec<bool,I>  Vec<T,I>::IsZero () const
	{
		Vec<bool,I>	ret;
		FOR( i, *this )	ret[i] = GXMath::IsZero( (*this)[i] );
		return ret;
	}
		

	template <typename T>
	inline Vec<bool,I>  Vec<T,I>::IsNotZero () const
	{
		Vec<bool,I>	ret;
		FOR( i, *this )	ret[i] = GXMath::IsNotZero( (*this)[i] );
		return ret;
	}
	
		
	template <typename T>
	inline Vec<T,I>  Vec<T,I>::Reflect (const Self &normal) const
	{
		return (*this) - T(2) * normal.Dot( *this ) * normal;
	}

		
	template <typename T>
	inline Vec<T,I>  Vec<T,I>::Refract (const Self &normal, T eta) const
	{
		T const		k = T(1) - eta * eta * ( T(1) - GXMath::Square( normal.Dot( *this ) ) );
		Self		r;

		if ( k >= T(0) )
			r = eta * (*this) - ( eta * normal.Dot( *this ) + Sqrt( k ) ) * normal;

		return r;
	}
