// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'


#if defined( _TIME_IMPL_ )

	template <>
	struct Time< T >
	{
	// types
	public:
		using Value_t	= T;
		using Self		= Time< T >;


	// variables
	private:
		T		_time;


	// methods
	public:
		constexpr Time (GX_DEFCTOR) : _time(0) {}

		explicit
		constexpr Time (const T &value) : _time(value) {}
		
		GX_DEFCOPYCTOR_ASSIGN( Time );

		template <typename B>
		constexpr explicit
		Time (const Time<B> &value) : _time(0)
		{
			SetTime( value );
		}
		
		template <typename B>
		forceinline constexpr Self&	SetTime (const B &time)
		{
			return SetNanoSeconds( GXMath::RoundTo<T>( time.NanoSeconds() ) );
		}
		
		template <typename B>
		ND_ static constexpr Self	FromTime (const B &time)			{ return Self().SetTime( time ); }

		ND_ static constexpr Self	FromSeconds (const T &value)		{ return Self().SetSeconds( value ); }
		ND_ static constexpr Self	FromMilliSeconds (const T &value)	{ return Self().SetMilliSeconds( value ); }
		ND_ static constexpr Self	FromMicroSeconds (const T &value)	{ return Self().SetMicroSeconds( value ); }
		ND_ static constexpr Self	FromNanoSeconds (const T &value)	{ return Self().SetNanoSeconds( value ); }
		
		constexpr Self&	SetSeconds (const T &value)						{ _time = value * T(1000000000);	return *this; }
		constexpr Self&	SetMilliSeconds (const T &value)				{ _time = value * T(1000000);		return *this; }
		constexpr Self&	SetMicroSeconds (const T &value)				{ _time = value * T(1000);			return *this; }
		constexpr Self&	SetNanoSeconds (const T &value)					{ _time = value;					return *this; }

		ND_ constexpr T		Seconds ()					const			{ return GXMath::RoundTo<T>( _time * 1.0e-9 ); }
		ND_ constexpr T		MilliSeconds ()				const			{ return GXMath::RoundTo<T>( _time * 1.0e-6 ); }
		ND_ constexpr T		MicroSeconds ()				const			{ return GXMath::RoundTo<T>( _time * 1.0e-3 ); }
		ND_ constexpr T		NanoSeconds ()				const			{ return _time; }

		ND_ constexpr bool	IsZero ()					const			{ return GXMath::IsZero( _time ); }

		ND_ explicit operator const T ()				const			{ return NanoSeconds(); }
		
		_GX_DIM_CMP_OPERATORS_SELF( _time );
		_GX_DIM_CMP_OPERATORS_TYPE( _time, const Value_t&, );
		_GX_DIM_OPERATORS_SELF( +, _time );
		_GX_DIM_OPERATORS_SELF( -, _time );
		_GX_DIM_OPERATORS_TYPE( *, _time, const Value_t&, );
		_GX_DIM_OPERATORS_TYPE( /, _time, const Value_t&, );
		
		template <typename B>
		ND_ B		To ()					const	{ return B().FromTime( *this ); }

		// seconds = value * 10 ^ power
		static constexpr int GetSecondsPowerOf10 ()		{ return 9; }
	};


	template <>
	struct Hash< Time<T> >
	{
		ND_ HashResult  operator () (const Time<T> &x) const noexcept
		{
			return HashOf( x.NanoSeconds() );
		}
	};
	
#endif	// _TIME_IMPL_


#if defined( _TIME_PROFILER_IMPL_ )

	template <>
	struct TimeProfiler< T >
	{
	// types
	public:
		typedef T					Value_t;
		typedef Time< T >			Time_t;
		typedef TimeProfiler< T >	Self;


	// variables
	private:
		OS::PerformanceTimer	_timer;
		Time_t					_start;


	// methods
	public:
		explicit
		TimeProfiler (bool start = false)
		{
			if ( start ) {
				Start();
			}
		}

		void Start ()
		{
			_start = GetCurrentTIme();
		}

		ND_ Time_t  GetTimeDelta () const
		{
			return GetCurrentTIme() - _start;
		}

		ND_ Time_t  GetStartTIme () const
		{
			return _start;
		}

		ND_ Time_t  GetCurrentTIme () const
		{
			return Time_t().FromTime( _timer.GetTimeMicroSec() );
		}
	};

#endif	// _TIME_PROFILER_IMPL_
	
#undef T
