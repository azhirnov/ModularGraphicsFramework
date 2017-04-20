// Copyright © 2014-2017  Zhirnov Andrey. All rights reserved.


	//
	// Matrix
	//
	
	template <typename T>
	struct Matrix<T,C,R> : public CompileTime::CopyQualifiers< T >
	{
	// types
	public:
		typedef Matrix< T, C, R >			Self;
		typedef Matrix< T, R, C >			transpose_t;
		typedef T							value_t;
		typedef Vec< T, R >					col_t;
		typedef Vec< T, C >					row_t;
		typedef Vec< T, (R > 2 ? 3 : R) >	col3_t;
		typedef T							arr2_t[C][R];
		typedef T							arr_t[C*R];
		typedef col_t						cols_t[C];

		typedef Vec< T, (R > C ? (C < 3 ? C : 3) : (R < 3 ? R : 3)) >	scale_t;
		typedef bool						_is_matrix;


	// variables
	private:
		arr2_t	_v;

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
		//explicit Matrix (const cols_t &columns);

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
		col_t &			operator () (usize c);
		col_t const &	operator () (usize c) const;
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
		Self &			operator *= (const Matrix<T,Q,C> &right);

		template <usize Q>
		Matrix<T,Q,R>	operator *  (const Matrix<T,Q,C> &right) const;

		row_t			operator *  (const col_t &v) const;

		friend col_t	operator *  (const row_t &v, const Self &m)
		{
			col_t	ret;
		
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

		arr2_t &		ref ()									{ return _v; }
		arr2_t const &	ref ()		const						{ return _v; }

		//cols_t &		columns ()								{ return *PointerCast< cols_t >( this ); }
		//cols_t const &	columns ()	const						{ return *PointerCast< cols_t >( this ); }

		//arr_t &			array_ref ()								{ return *PointerCast< arr_t >( this ); }
		//arr_t const &	array_ref ()	const						{ return *PointerCast< arr_t >( this ); }
		
		row_t			Row (uint i)	const;
		col_t const &	Column (uint i) const;

		template <usize nA>
		static Self	FromEuler (const Vec<T,nA> &a);

		template <usize nC, usize nR, usize nA>
		static Self	FromEuler (const Vec<T,nA> &a);

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
		static Self	FromQuat (const Quaternion<T> &q);
		static Self	FromQuat (const Quaternion<T> &q);

		static Self	Scale (const scale_t &s);

		static Self	Translate (const col3_t &t);

		static Self	Identity ();
		static Self	Zero ();
		
		col3_t const &	Translation () const	{ return *PointerCast< col3_t >( &((*this)(C-1)) ); }
		col3_t		 &	Translation ()			{ return *PointerCast< col3_t >( &((*this)(C-1)) ); }
		
		transpose_t		Transpose () const;

		template <typename B>
		Matrix<B,C,R>	Convert () const;

		template <typename B>
		B				To () const;

		template <typename B, usize X, usize Y>
		Self &			Inject (const Matrix<B,X,Y> &m);
	
		template <typename B, usize X, usize Y>
		Self &			InjectTr (const Matrix<B,X,Y> &m);	// copy transform to last column
	};



	
	template <typename T>
	inline Matrix<T,C,R>::Matrix (UninitializedType)
	{
		DEBUG_ONLY( _GenNoise() );
	}

		
	template <typename T>
	inline void Matrix<T,C,R>::_GenNoise ()
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


	template <typename T>
	inline Matrix<T,C,R>::Matrix (const T& val)
	{
		for (usize i = 0; i < Count(); ++i)
			(*this)[i] = val;
	}


	template <typename T>
	inline Matrix<T,C,R>::Matrix (const Self &m)
	{
		UnsafeMem::MemCopy( ptr(), m.ptr(), SizeOf(m) );
	}
	

	//template <typename T>
	//inline Matrix<T,C,R>::Matrix (const cols_t &columns)
	//{
	//	UnsafeMem::MemCopy( ptr(), &columns[0], SizeOf(columns) );
	//}


	template <typename T>
	inline T &  Matrix<T,C,R>::operator [] (usize i)
	{
		ASSUME( i < Count() );
		return ptr()[i];
	}

		
	template <typename T>
	inline T const &  Matrix<T,C,R>::operator [] (usize i)	const
	{
		ASSUME( i < Count() );
		return ptr()[i];
	}

		
	template <typename T>
	inline typename Matrix<T,C,R>::col_t &  Matrix<T,C,R>::operator () (usize c)
	{
		ASSUME( c < NumColumns() );
		return *PointerCast< col_t >( _v[c] );
	}

		
	template <typename T>
	inline typename Matrix<T,C,R>::col_t const &  Matrix<T,C,R>::operator () (usize c) const
	{
		ASSUME( c < NumColumns() );
		return *PointerCast< col_t >( _v[c] );
	}
		

	template <typename T>
	inline T &  Matrix<T,C,R>::operator () (usize c, usize r)
	{
		ASSUME( r < NumRows() and c < NumColumns() );
		return _v[c][r];
	}

		
	template <typename T>
	inline T const &  Matrix<T,C,R>::operator () (usize c, usize r) const
	{
		ASSUME( r < NumRows() and c < NumColumns() );
		return _v[c][r];
	}
	
		
	template <typename T>
	inline Matrix<T,C,R>  Matrix<T,C,R>::operator -  () const
	{
		Self	ret;
		FOR( i, *this )		ret[i] = -(*this)[i];
		return ret;
	}

		
	template <typename T>
	inline Matrix<T,C,R>  Matrix<T,C,R>::operator ~  () const
	{
		Self	ret;
		FOR( i, *this )		ret[i] = ~(*this)[i];
		return ret;
	}

		
	template <typename T>
	inline Matrix<T,C,R>  Matrix<T,C,R>::operator !  () const
	{
		Self	ret;
		FOR( i, *this )		ret[i] = not (*this)[i];
		return ret;
	}

		
	template <typename T>
	inline Matrix<T,C,R> &  Matrix<T,C,R>::operator ++ ()
	{
		FOR( i, *this )		++(*this)[i];
		return *this;
	}

		
	template <typename T>
	inline Matrix<T,C,R> &  Matrix<T,C,R>::operator -- ()
	{
		FOR( i, *this )		--(*this)[i];
		return *this;
	}

		
	template <typename T>
	inline Matrix<T,C,R>  Matrix<T,C,R>::operator ++ (int) const
	{
		Self	ret;
		++(*this);
		return ret;
	}

		
	template <typename T>
	inline Matrix<T,C,R>  Matrix<T,C,R>::operator -- (int) const
	{
		Self	ret;
		--(*this);
		return ret;
	}

		
	template <typename T>
	inline bool  Matrix<T,C,R>::operator == (const Self &right) const
	{
		bool			ret = true;
		FOR( i, *this )	ret &= (*this)[i] == right[i];
		return ret;
	}

		
	template <typename T>
	inline bool  Matrix<T,C,R>::operator != (const Self &right) const
	{
		return not ( *this == right );
	}

		
	template <typename T>
	template <usize Q>
	inline Matrix<T,C,R> &  Matrix<T,C,R>::operator *= (const Matrix<T,Q,C> &right)
	{
		return ( *this = *this * right );
	}

		
	template <typename T>
	template <usize Q>
	inline Matrix<T,Q,R>  Matrix<T,C,R>::operator *  (const Matrix<T,Q,C> &right) const
	{
		Matrix<T,Q,R>	ret;
		TMatMul<T,C,R,Q>( ret.ref(), ref(), right.ref() );
		return ret;
	}

		
	template <typename T>
	inline typename Matrix<T,C,R>::row_t  Matrix<T,C,R>::operator *  (const col_t &v) const
	{
		row_t	ret;
		
		for (usize r = 0; r < R; ++r)
		for (usize c = 0; c < C; ++c)
			ret[r] += (*this)(c,r) * v[c];

		return ret;
	}
	
		
	template <typename T>
	inline typename Matrix<T,C,R>::row_t  Matrix<T,C,R>::Row (uint r) const
	{
		ASSERT( r < R );

		row_t	row;

		for (usize c = 0; c < C; ++c)
			row[c] = (*this)(c, r);

		return row;
	}
	

	template <typename T>
	inline typename Matrix<T,C,R>::col_t const &  Matrix<T,C,R>::Column (uint c) const
	{
		ASSERT( c < C );
		return *PointerCast< cols_t >( this )[c];
	}

	
	template <typename T>
	template <usize nA>
	inline Matrix<T,C,R>  Matrix<T,C,R>::FromEuler (const Vec<T,nA> &a)
	{
		return FromEuler<C,R>( a );
	}
	
	
	template <typename T>
	template <usize nC, usize nR, usize nA>
	inline Matrix<T,C,R>  Matrix<T,C,R>::FromEuler (const Vec<T,nA> &a)
	{
		Self	ret;
		MatFromEuler::Get< nC, nR >( ret._v, a );
		return ret;
	}
	

	template <typename T>
	template <usize nC, usize nR>
	inline Matrix<T,C,R>  Matrix<T,C,R>::RotationX (const T& a)
	{
		Self	ret;
		MatRotationX::Get< nC, nR >( ret._v, a );
		return ret;
	}

	
	template <typename T>
	inline Matrix<T,C,R>  Matrix<T,C,R>::RotationX (const T& a)
	{
		return RotationX< C, R >( a );
	}
		
	
	template <typename T>
	template <usize nC, usize nR>
	inline Matrix<T,C,R>  Matrix<T,C,R>::RotationY (const T& a)
	{
		Self	ret;
		MatRotationY::Get< nC, nR >( ret._v, a );
		return ret;
	}

	
	template <typename T>
	inline Matrix<T,C,R>  Matrix<T,C,R>::RotationY (const T& a)
	{
		return RotationY< C, R >( a );
	}
		
	
	template <typename T>
	template <usize nC, usize nR>
	inline Matrix<T,C,R>  Matrix<T,C,R>::RotationZ (const T& a)
	{
		Self	ret;
		MatRotationZ::Get< nC, nR >( ret._v, a );
		return ret;
	}

	
	template <typename T>
	inline Matrix<T,C,R>  Matrix<T,C,R>::RotationZ (const T& a)
	{
		return RotationZ< C, R >( a );
	}
	

	template <typename T>
	template <usize nC, usize nR>
	inline Matrix<T,C,R>  Matrix<T,C,R>::FromQuat (const Quaternion<T> &q)
	{
		Self	ret;
		MatFromQuat::Get< nC, nR >( ret._v, q );
		return ret;
	}
	

	template <typename T>
	inline Matrix<T,C,R>  Matrix<T,C,R>::FromQuat (const Quaternion<T> &q)
	{
		return FromQuat< C, R >( q );
	}
	

	template <typename T>
	inline Matrix<T,C,R>  Matrix<T,C,R>::Identity ()
	{
		Self	ret;
		MatIdentity::Get( ret._v );
		return ret;
	}

	
	template <typename T>
	inline Matrix<T,C,R>  Matrix<T,C,R>::Zero ()
	{
		Self	ret;
		UnsafeMem::ZeroMem( &ret, sizeof(ret) );
		return ret;
	}
	

	template <typename T>
	inline Matrix<T,R,C>  Matrix<T,C,R>::Transpose () const
	{
		transpose_t	ret;
		MatTranspose::Get( ret._v, _v );
		return ret;
	}

	
	template <typename T>
	inline Matrix<T,C,R>  Matrix<T,C,R>::Scale (const scale_t &s)
	{
		Self	ret;
		MatScale::Get( ret._v, s );
		return ret;
	}
	

	template <typename T>
	inline Matrix<T,C,R>  Matrix<T,C,R>::Translate (const col3_t &t)
	{
		Self	ret;
		MatTranslate::Get( ret._v, t );
		return ret;
	}
	
	
	template <typename T>
	template <typename B>
	inline Matrix<B,C,R>  Matrix<T,C,R>::Convert () const
	{
		Matrix<B,C,R>	ret;
		FOR( i, *this )	ret[i] = B( (*this)[i] );
		return ret;
	}
	

	template <typename T>
	template <typename B>
	inline B  Matrix<T,C,R>::To () const
	{
		STATIC_ASSERT( typename B::_is_matrix(true), "type is not matrix type" );

		B	ret = B::Identity();
		ret.Inject( *this );
		return ret;
	}

	
	template <typename T>
	template <typename B, usize X, usize Y>
	inline Matrix<T,C,R> &  Matrix<T,C,R>::Inject (const Matrix<B,X,Y> &m)
	{
		const usize	col = Min<usize>( C, X );
		const usize	row = Min<usize>( R, Y );

		for (usize c = 0; c < col; ++c)
		for (usize r = 0; r < row; ++r)
			(*this)(c,r) = T( m(c,r) );

		return *this;
	}
	

	template <typename T>
	template <typename B, usize X, usize Y>
	inline Matrix<T,C,R> &  Matrix<T,C,R>::InjectTr (const Matrix<B,X,Y> &m)
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
