// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'


	//
	// Matrix
	//
	
	template <typename T, ulong U>
	struct Matrix<T,C,R,U> : public CompileTime::CopyQualifiers< T >
	{
	// types
	public:
		using Self			= Matrix< T, C, R, U >;
		using Transposed_t	= Matrix< T, R, C, U >;
		using Value_t		= T;
		using Col_t			= Vec< T, R, U >;
		using Row_t			= Vec< T, C, U >;
		using Col3_t		= Vec< T, (R > 2 ? 3 : R), U >;
		using Arr2_t		= T[C][R];
		using Arr_t			= T[C*R];
		using Cols_t		= Col_t[C];
		using Vec2_t		= Vec< T, 2, U >;
		using Vec3_t		= Vec< T, 3, U >;
		using Vec4_t		= Vec< T, 4, U >;

		using Scale_t		= Vec< T, (R > C ? (C < 3 ? C : 3) : (R < 3 ? R : 3)), U >;
		using _is_matrix	= bool;


	// variables
	private:
		Arr2_t	_v;

		//		  c0  c1  c2  c3
		//	r0	| 1 | 2 | 3 | X |	1 - left
		//	r1	|   |   |   | Y |	2 - up
		//	r2	|   |   |   | Z |	3 - forward
		//	r3	| 0 | 0 | 0 | W |


	// methods
	private:
		void _GenNoise ();

	public:
		// constructors
		Matrix (GX_DEFCTOR);
		Matrix (const Self &m);
		explicit Matrix (const T& val);
		//explicit Matrix (const Cols_t &columns);

		MATRIX_METHODS();


		// unary operators
		Self			operator -  () const;
		Self			operator ~  () const;
		Self			operator !  () const;

		Self &			operator ++ ();
		Self &			operator -- ();
		Self			operator ++ (int) const;
		Self			operator -- (int) const;


		// binary operators
		T &				operator [] (usize i);
		T const &		operator [] (usize i) const;
		Col_t &			operator () (usize c);
		Col_t const &	operator () (usize c) const;
		T &				operator () (usize c, usize r);
		T const &		operator () (usize c, usize r) const;

		bool			operator == (const Self &right) const;
		bool			operator != (const Self &right) const;


		MAT_BIN_OPERATOR( + );
		MAT_BIN_OPERATOR( - );
		MAT_BIN_OPERATOR( & );
		MAT_BIN_OPERATOR( | );
		MAT_BIN_OPERATOR( ^ );

		MAT_BIN_OPERATOR_SCALAR( * );
		MAT_BIN_OPERATOR_SCALAR( / );
		MAT_BIN_OPERATOR_SCALAR( % );

		template <usize Q>
		Self &			operator *= (const Matrix<T,Q,C,U> &right);

		template <usize Q>
		Matrix<T,Q,R,U>	operator *  (const Matrix<T,Q,C,U> &right) const;

		Col_t			operator *  (const Row_t &v) const;

		friend Row_t	operator *  (const Col_t &v, const Self &m)
		{
			Row_t	ret;
		
			for (usize c = 0; c < C; ++c)
			for (usize r = 0; r < R; ++r)
				ret[c] += m(c,r) * v[r];

			return ret;
		}


		// methods
		static usize	NumRows ()								{ return R; }
		static usize	NumColumns ()							{ return C; }
		static usize	Count ()								{ return R*C; }

		T *				ptr ()									{ return PointerCast< T >( this ); }
		T const *		ptr ()		const						{ return PointerCast< T >( this ); }

		Arr2_t &		ref ()									{ return _v; }
		Arr2_t const &	ref ()		const						{ return _v; }

		//Cols_t &		columns ()								{ return *PointerCast< Cols_t >( this ); }
		//Cols_t const &	columns ()	const					{ return *PointerCast< Cols_t >( this ); }

		//Arr_t &			array_ref ()						{ return *PointerCast< Arr_t >( this ); }
		//Arr_t const &	array_ref ()	const					{ return *PointerCast< Arr_t >( this ); }
		
		Row_t			Row (uint i)	const;
		Col_t const &	Column (uint i) const;

		template <usize nA>
		static Self	FromEuler (const Vec<T,nA,U> &a);

		template <usize nC, usize nR, usize nA>
		static Self	FromEuler (const Vec<T,nA,U> &a);

		template <usize nC, usize nR>
		static Self	RotationX (const T& a);
		static Self	RotationX (const T& a);
		
		template <usize nC, usize nR>
		static Self	RotationY (const T& a);
		static Self	RotationY (const T& a);
		
		template <usize nC, usize nR>
		static Self	RotationZ (const T& a);
		static Self	RotationZ (const T& a);

		template <usize nC, usize nR>
		static Self	FromQuat (const Quaternion<T,U> &q);
		static Self	FromQuat (const Quaternion<T,U> &q);

		static Self	Scale (const Scale_t &s);

		static Self	Translate (const Col3_t &t);

		static Self	Identity ();
		static Self	Zero ();
		
		Col3_t const &	Translation () const	{ return *PointerCast< Col3_t >( &((*this)(C-1)) ); }
		Col3_t		 &	Translation ()			{ return *PointerCast< Col3_t >( &((*this)(C-1)) ); }
		
		Transposed_t		Transpose () const;

		template <typename B>
		Matrix<B,C,R,U>	Convert () const;

		template <typename B>
		B				To () const;

		template <typename B, usize X, usize Y>
		Self &			Inject (const Matrix<B,X,Y,U> &m);
	
		template <typename B, usize X, usize Y>
		Self &			InjectTr (const Matrix<B,X,Y,U> &m);	// copy transform to last column
	};



	
	template <typename T, ulong U>
	inline Matrix<T,C,R,U>::Matrix (UninitializedType)
	{
		DEBUG_ONLY( _GenNoise() );
	}

		
	template <typename T, ulong U>
	inline void Matrix<T,C,R,U>::_GenNoise ()
	{
		// set noise to matrix elements
		DEBUG_ONLY(
			FOR( i, *this ) {
				(*this)[i] = (T) ReferenceCast<float>( uint( (i << 28) | ((C*R-i) << 24) | ((i&C) << 20) |
															 ((i&R) << 16) | ((~i << 12) & 0xF) |
															 (((i << 8) | (i << 4) | (i)) & 0x123) ) );
			}
		)
	}


	template <typename T, ulong U>
	inline Matrix<T,C,R,U>::Matrix (const T& val)
	{
		for (usize i = 0; i < Count(); ++i)
			(*this)[i] = val;
	}


	template <typename T, ulong U>
	inline Matrix<T,C,R,U>::Matrix (const Self &m)
	{
		UnsafeMem::MemCopy( ptr(), m.ptr(), SizeOf(m) );
	}
	

	//template <typename T, ulong U>
	//inline Matrix<T,C,R,U>::Matrix (const Cols_t &columns)
	//{
	//	UnsafeMem::MemCopy( ptr(), &columns[0], SizeOf(columns) );
	//}


	template <typename T, ulong U>
	inline T &  Matrix<T,C,R,U>::operator [] (usize i)
	{
		ASSUME( i < Count() );
		return ptr()[i];
	}

		
	template <typename T, ulong U>
	inline T const &  Matrix<T,C,R,U>::operator [] (usize i)	const
	{
		ASSUME( i < Count() );
		return ptr()[i];
	}

		
	template <typename T, ulong U>
	inline typename Matrix<T,C,R,U>::Col_t &  Matrix<T,C,R,U>::operator () (usize c)
	{
		ASSUME( c < NumColumns() );
		return *PointerCast< Col_t >( _v[c] );
	}

		
	template <typename T, ulong U>
	inline typename Matrix<T,C,R,U>::Col_t const &  Matrix<T,C,R,U>::operator () (usize c) const
	{
		ASSUME( c < NumColumns() );
		return *PointerCast< Col_t >( _v[c] );
	}
		

	template <typename T, ulong U>
	inline T &  Matrix<T,C,R,U>::operator () (usize c, usize r)
	{
		ASSUME( r < NumRows() and c < NumColumns() );
		return _v[c][r];
	}

		
	template <typename T, ulong U>
	inline T const &  Matrix<T,C,R,U>::operator () (usize c, usize r) const
	{
		ASSUME( r < NumRows() and c < NumColumns() );
		return _v[c][r];
	}
	
		
	template <typename T, ulong U>
	inline Matrix<T,C,R,U>  Matrix<T,C,R,U>::operator -  () const
	{
		Self	ret;
		FOR( i, *this )		ret[i] = -(*this)[i];
		return ret;
	}

		
	template <typename T, ulong U>
	inline Matrix<T,C,R,U>  Matrix<T,C,R,U>::operator ~  () const
	{
		Self	ret;
		FOR( i, *this )		ret[i] = ~(*this)[i];
		return ret;
	}

		
	template <typename T, ulong U>
	inline Matrix<T,C,R,U>  Matrix<T,C,R,U>::operator !  () const
	{
		Self	ret;
		FOR( i, *this )		ret[i] = not (*this)[i];
		return ret;
	}

		
	template <typename T, ulong U>
	inline Matrix<T,C,R,U> &  Matrix<T,C,R,U>::operator ++ ()
	{
		FOR( i, *this )		++(*this)[i];
		return *this;
	}

		
	template <typename T, ulong U>
	inline Matrix<T,C,R,U> &  Matrix<T,C,R,U>::operator -- ()
	{
		FOR( i, *this )		--(*this)[i];
		return *this;
	}

		
	template <typename T, ulong U>
	inline Matrix<T,C,R,U>  Matrix<T,C,R,U>::operator ++ (int) const
	{
		Self	ret;
		++(*this);
		return ret;
	}

		
	template <typename T, ulong U>
	inline Matrix<T,C,R,U>  Matrix<T,C,R,U>::operator -- (int) const
	{
		Self	ret;
		--(*this);
		return ret;
	}

		
	template <typename T, ulong U>
	inline bool  Matrix<T,C,R,U>::operator == (const Self &right) const
	{
		bool			ret = true;
		FOR( i, *this )	ret &= (*this)[i] == right[i];
		return ret;
	}

		
	template <typename T, ulong U>
	inline bool  Matrix<T,C,R,U>::operator != (const Self &right) const
	{
		return not ( *this == right );
	}

		
	template <typename T, ulong U>
	template <usize Q>
	inline Matrix<T,C,R,U> &  Matrix<T,C,R,U>::operator *= (const Matrix<T,Q,C,U> &right)
	{
		return ( *this = *this * right );
	}

		
	template <typename T, ulong U>
	template <usize Q>
	inline Matrix<T,Q,R,U>  Matrix<T,C,R,U>::operator *  (const Matrix<T,Q,C,U> &right) const
	{
		Matrix<T,Q,R>	ret;
		TMatMul<T,C,R,Q>( ret.ref(), ref(), right.ref() );
		return ret;
	}

		
	template <typename T, ulong U>
	inline typename Matrix<T,C,R,U>::Col_t  Matrix<T,C,R,U>::operator *  (const Row_t &v) const
	{
		Col_t	ret;
		
		for (usize r = 0; r < R; ++r)
		for (usize c = 0; c < C; ++c)
			ret[r] += (*this)(c,r) * v[c];

		return ret;
	}
	
		
	template <typename T, ulong U>
	inline typename Matrix<T,C,R,U>::Row_t  Matrix<T,C,R,U>::Row (uint r) const
	{
		ASSERT( r < R );

		Row_t	row;

		for (usize c = 0; c < C; ++c)
			row[c] = (*this)(c, r);

		return row;
	}
	

	template <typename T, ulong U>
	inline typename Matrix<T,C,R,U>::Col_t const &  Matrix<T,C,R,U>::Column (uint c) const
	{
		ASSERT( c < C );
		return *PointerCast< Cols_t >( this )[c];
	}

	
	template <typename T, ulong U>
	template <usize nA>
	inline Matrix<T,C,R,U>  Matrix<T,C,R,U>::FromEuler (const Vec<T,nA,U> &a)
	{
		return FromEuler<C,R>( a );
	}
	
	
	template <typename T, ulong U>
	template <usize nC, usize nR, usize nA>
	inline Matrix<T,C,R,U>  Matrix<T,C,R,U>::FromEuler (const Vec<T,nA,U> &a)
	{
		Self	ret;
		MatFromEuler::Get< nC, nR >( ret._v, a );
		return ret;
	}
	

	template <typename T, ulong U>
	template <usize nC, usize nR>
	inline Matrix<T,C,R,U>  Matrix<T,C,R,U>::RotationX (const T& a)
	{
		Self	ret;
		MatRotationX::Get< nC, nR >( ret._v, a );
		return ret;
	}

	
	template <typename T, ulong U>
	inline Matrix<T,C,R,U>  Matrix<T,C,R,U>::RotationX (const T& a)
	{
		return RotationX< C, R >( a );
	}
		
	
	template <typename T, ulong U>
	template <usize nC, usize nR>
	inline Matrix<T,C,R,U>  Matrix<T,C,R,U>::RotationY (const T& a)
	{
		Self	ret;
		MatRotationY::Get< nC, nR >( ret._v, a );
		return ret;
	}

	
	template <typename T, ulong U>
	inline Matrix<T,C,R,U>  Matrix<T,C,R,U>::RotationY (const T& a)
	{
		return RotationY< C, R >( a );
	}
		
	
	template <typename T, ulong U>
	template <usize nC, usize nR>
	inline Matrix<T,C,R,U>  Matrix<T,C,R,U>::RotationZ (const T& a)
	{
		Self	ret;
		MatRotationZ::Get< nC, nR >( ret._v, a );
		return ret;
	}

	
	template <typename T, ulong U>
	inline Matrix<T,C,R,U>  Matrix<T,C,R,U>::RotationZ (const T& a)
	{
		return RotationZ< C, R >( a );
	}
	

	template <typename T, ulong U>
	template <usize nC, usize nR>
	inline Matrix<T,C,R,U>  Matrix<T,C,R,U>::FromQuat (const Quaternion<T,U> &q)
	{
		Self	ret;
		MatFromQuat::Get< nC, nR >( ret._v, q );
		return ret;
	}
	

	template <typename T, ulong U>
	inline Matrix<T,C,R,U>  Matrix<T,C,R,U>::FromQuat (const Quaternion<T,U> &q)
	{
		return FromQuat< C, R >( q );
	}
	

	template <typename T, ulong U>
	inline Matrix<T,C,R,U>  Matrix<T,C,R,U>::Identity ()
	{
		Self	ret;
		MatIdentity::Get( ret._v );
		return ret;
	}

	
	template <typename T, ulong U>
	inline Matrix<T,C,R,U>  Matrix<T,C,R,U>::Zero ()
	{
		Self	ret;
		UnsafeMem::ZeroMem( &ret, sizeof(ret) );
		return ret;
	}
	

	template <typename T, ulong U>
	inline Matrix<T,R,C,U>  Matrix<T,C,R,U>::Transpose () const
	{
		Transposed_t	ret;
		MatTranspose::Get( ret._v, _v );
		return ret;
	}

	
	template <typename T, ulong U>
	inline Matrix<T,C,R,U>  Matrix<T,C,R,U>::Scale (const Scale_t &s)
	{
		Self	ret;
		MatScale::Get( ret._v, s );
		return ret;
	}
	

	template <typename T, ulong U>
	inline Matrix<T,C,R,U>  Matrix<T,C,R,U>::Translate (const Col3_t &t)
	{
		Self	ret;
		MatTranslate::Get( ret._v, t );
		return ret;
	}
	
	
	template <typename T, ulong U>
	template <typename B>
	inline Matrix<B,C,R,U>  Matrix<T,C,R,U>::Convert () const
	{
		Matrix<B,C,R,U>	ret;
		FOR( i, *this )	ret[i] = B( (*this)[i] );
		return ret;
	}
	

	template <typename T, ulong U>
	template <typename B>
	inline B  Matrix<T,C,R,U>::To () const
	{
		STATIC_ASSERT( typename B::_is_matrix(true), "type is not matrix type" );

		B	ret = B::Identity();
		ret.Inject( *this );
		return ret;
	}

	
	template <typename T, ulong U>
	template <typename B, usize X, usize Y>
	inline Matrix<T,C,R,U> &  Matrix<T,C,R,U>::Inject (const Matrix<B,X,Y,U> &m)
	{
		const usize	col = Min<usize>( C, X );
		const usize	row = Min<usize>( R, Y );

		for (usize c = 0; c < col; ++c)
		for (usize r = 0; r < row; ++r)
			(*this)(c,r) = T( m(c,r) );

		return *this;
	}
	

	template <typename T, ulong U>
	template <typename B, usize X, usize Y>
	inline Matrix<T,C,R,U> &  Matrix<T,C,R,U>::InjectTr (const Matrix<B,X,Y,U> &m)
	{
		STATIC_ASSERT( C > 1 and R > 1 and X > 1 and Y > 1, "incorrect parameters" );

		// copy rotation
		const usize col = Min<usize>( C, X );
		const usize row = Min<usize>( R, Y );
	
		for (usize c = 0; c < col-1; ++c)
		for (usize r = 0; r < row-1; ++r)
			(*this)(c,r) = T( m(c,r) );


		// copy transform
		const usize n = Min<usize>( row, 3 );

		for (usize r = 0; r < row; ++r)
			(*this)(C-1,r) = T( m(X-1,r) );

		return *this;
	}
