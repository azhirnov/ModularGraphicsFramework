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
	protected:
		T	_time;


	// methods
	public:
		constexpr Time (GX_DEFCTOR) : _time(0.0) {}

		explicit
		constexpr Time (const T &value) : _time(value) {}
		
		constexpr Time (Self &&other) = default;
		constexpr Time (const Self &other) = default;

		constexpr Self& operator = (Self &&right) = default;
		constexpr Self& operator = (const Self &right) = default;

		template <typename B>
		constexpr explicit
		Time (const Time<B> &value) : _time(0)
		{
			SetTime( value );
		}

		template <typename B>
		forceinline constexpr Self&	SetTime (const B &time)
		{
			if ( time.GetSecondsPowerOf10() <= 0 )		return SetSeconds( (T) time.Seconds() );
			if ( time.GetSecondsPowerOf10() <= 3 )		return SetMilliSeconds( (T) time.MilliSeconds() );
			if ( time.GetSecondsPowerOf10() <= 6 )		return SetMicroSeconds( (T) time.MicroSeconds() );

			return SetNanoSeconds( (T) time.NanoSeconds() );
		}

		template <typename B>
		static constexpr Self	FromTime (const B &time)			{ return Self().SetTime( time ); }

		static constexpr Self	FromSeconds (const T &value)		{ return Self().SetSeconds( value ); }
		static constexpr Self	FromMilliSeconds (const T &value)	{ return Self().SetMilliSeconds( value ); }
		static constexpr Self	FromMicroSeconds (const T &value)	{ return Self().SetMicroSeconds( value ); }
		static constexpr Self	FromNanoSeconds (const T &value)	{ return Self().SetNanoSeconds( value ); }
		
		constexpr Self&	SetSeconds (const T &value)					{ _time = value;				return *this; }
		constexpr Self&	SetMilliSeconds (const T &value)			{ _time = value * T(1.0e-3);	return *this; }
		constexpr Self&	SetMicroSeconds (const T &value)			{ _time = value * T(1.0e-6);	return *this; }
		constexpr Self&	SetNanoSeconds (const T &value)				{ _time = value * T(1.0e-9);	return *this; }

		constexpr T		Hours ()						const		{ return _time / T(3600); }
		constexpr T		Minutes ()						const		{ return _time / T(60); }
		constexpr T		Seconds ()						const		{ return _time; }
		constexpr T		MilliSeconds ()					const		{ return _time * T(1.0e+3); }
		constexpr T		MicroSeconds ()					const		{ return _time * T(1.0e+6); }
		constexpr T		NanoSeconds ()					const		{ return _time * T(1.0e+9); }
		
		constexpr bool	IsZero ()						const		{ return GXMath::IsZero( _time ); }
		
		explicit operator T ()							const		{ return Seconds(); }

		_GX_DIM_CMP_OPERATORS_SELF( _time );
		_GX_DIM_CMP_OPERATORS_TYPE( _time, const Value_t&, );
		_GX_DIM_OPERATORS_SELF( +, _time );
		_GX_DIM_OPERATORS_SELF( -, _time );
		_GX_DIM_OPERATORS_TYPE( *, _time, const Value_t&, );
		_GX_DIM_OPERATORS_TYPE( /, _time, const Value_t&, );

		template <typename B>
		B		To ()							const	{ return B().FromTime( *this ); }

		// seconds = value * 10 ^ power
		static constexpr int GetSecondsPowerOf10 ()		{ return 0; }
	};


	template <>
	struct Hash< Time<T> > : private Hash<T>
	{
		typedef Time<T>				Key_t;
		typedef Hash<T>				Base_t;
		typedef Base_t::Result_t	Result_t;

		Result_t operator () (const Key_t &x) const noexcept
		{
			return Base_t::operator ()( x.Seconds() );
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
			_start = GetCurrentTime();
		}

		Time_t GetTimeDelta () const
		{
			return GetCurrentTime() - _start;
		}

		Time_t GetStartTime () const
		{
			return _start;
		}

		Time_t GetCurrentTime () const
		{
			return Time_t().FromTime( _timer.GetTime() );
		}
	};

#endif	// _TIME_PROFILER_IMPL_

#undef T
