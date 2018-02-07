// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'


#if defined( _TIME_IMPL_ )

	template <>
	struct Time< T >
	{
	// types
	public:
		typedef T			Value_t;
		typedef Time< T >	Self;


	// variables
	private:
		T		_time;


	// methods
	public:
		constexpr Time (GX_DEFCTOR) : _time(0) {}

		explicit
		constexpr Time (const T &value) : _time(value) {}
		
		constexpr Time (Self &&other) = default;
		constexpr Time (const Self &other) = default;

		Self& operator = (Self &&right) = default;
		Self& operator = (const Self &right) = default;

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
		CHECKRES static constexpr Self	FromTime (const B &time)			{ return Self().SetTime( time ); }

		CHECKRES static constexpr Self	FromSeconds (const T &value)		{ return Self().SetSeconds( value ); }
		CHECKRES static constexpr Self	FromMilliSeconds (const T &value)	{ return Self().SetMilliSeconds( value ); }
		CHECKRES static constexpr Self	FromMicroSeconds (const T &value)	{ return Self().SetMicroSeconds( value ); }
		CHECKRES static constexpr Self	FromNanoSeconds (const T &value)	{ return Self().SetNanoSeconds( value ); }
		
		constexpr Self&	SetSeconds (const T &value)							{ _time = value * T(1000000000);	return *this; }
		constexpr Self&	SetMilliSeconds (const T &value)					{ _time = value * T(1000000);		return *this; }
		constexpr Self&	SetMicroSeconds (const T &value)					{ _time = value * T(1000);			return *this; }
		constexpr Self&	SetNanoSeconds (const T &value)						{ _time = value;					return *this; }

		CHECKRES constexpr T		Seconds ()					const		{ return GXMath::RoundTo<T>( _time * 1.0e-9 ); }
		CHECKRES constexpr T		MilliSeconds ()				const		{ return GXMath::RoundTo<T>( _time * 1.0e-6 ); }
		CHECKRES constexpr T		MicroSeconds ()				const		{ return GXMath::RoundTo<T>( _time * 1.0e-3 ); }
		CHECKRES constexpr T		NanoSeconds ()				const		{ return _time; }

		CHECKRES constexpr bool	IsZero ()						const		{ return GXMath::IsZero( _time ); }

		CHECKRES explicit operator T ()							const		{ return NanoSeconds(); }
		
		_GX_DIM_CMP_OPERATORS_SELF( _time );
		_GX_DIM_CMP_OPERATORS_TYPE( _time, const Value_t&, );
		_GX_DIM_OPERATORS_SELF( +, _time );
		_GX_DIM_OPERATORS_SELF( -, _time );
		_GX_DIM_OPERATORS_TYPE( *, _time, const Value_t&, );
		_GX_DIM_OPERATORS_TYPE( /, _time, const Value_t&, );
		
		template <typename B>
		CHECKRES B		To ()					const	{ return B().FromTime( *this ); }

		// seconds = value * 10 ^ power
		static constexpr int GetSecondsPowerOf10 ()		{ return 9; }
	};


	template <>
	struct Hash< Time<T> >
	{
		CHECKRES HashResult  operator () (const Time<T> &x) const noexcept
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

		CHECKRES Time_t  GetTimeDelta () const
		{
			return GetCurrentTIme() - _start;
		}

		CHECKRES Time_t  GetStartTIme () const
		{
			return _start;
		}

		CHECKRES Time_t  GetCurrentTIme () const
		{
			return Time_t().FromTime( _timer.GetTimeMicroSec() );
		}
	};

#endif	// _TIME_PROFILER_IMPL_
	
#undef T
