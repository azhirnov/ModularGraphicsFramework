// Copyright © 2014-2017  Zhirnov Andrey. All rights reserved.

#pragma once

#include "Engine/STL/Containers/CircularQueue.h"
#include "Engine/STL/Math/MathFunc.h"

namespace GX_STL
{
namespace GXFilters
{

	//
	// Gaussian Filter for 1D data
	//

	template <typename T, typename ValueType = T, typename ParamType = T, usize MaxSize = 16>
	struct GaussianFilter
	{
	// types
	public:
		typedef GaussianFilter< T, ValueType, ParamType, MaxSize >		Self;

		struct Sample
		{
		// variables
			ValueType	value;
			ParamType	param;

		// methods
			Sample () {}
			Sample (const ValueType &value, const ParamType &param) : value(value), param(param) {}
		};

	private:
		typedef typename FixedSizeCircularQueue< Sample, MaxSize >		Queue_t;


	// variables
	private:
		Queue_t		_values;
		ValueType	_lastFiltered;
		ValueType	_defaultValue;
		T			_sigma;
		bool		_changed;


	// methods
	public:
		GaussianFilter () :
			_lastFiltered(), _defaultValue(), _sigma(), _changed(true)
		{
		}


		void SetSigma (const T &sigma)
		{
			_sigma	 = sigma;
			_changed = true;
		}


		void SetDefault (const ValueType &value)
		{
			_defaultValue	= value;
			_changed		= _changed or _values.Empty();
		}


		Self & Add (const ValueType &value, const ParamType &param)
		{
			if ( _values.Count() >= MaxSize )
				_values.PopFront();

			_values.PushBack( Sample( value, param ) );
			_changed = true;

			return *this;
		}


		Sample const &  operator [] (usize index) const
		{
			return _values[ index ];
		}


		ValueType const &  Filter (const ParamType &refParam)
		{
			if ( _values.Empty() )
			{
				_lastFiltered	= _defaultValue;
				_changed		= false;
				return _lastFiltered;
			}

			ValueType	value_sum	= ValueType();
			T			weight_sum	= T(0);
			T const		sigma2		= Square( sigma ) * T(-2);
			T const		sigma3		= T(1) / (sigma * Sqrt( T(2) * Pi<T> ));


			FOR( i, _values )
			{
				T	x		= refParam - _values[i].param;
				T	weight	= Exp( Square( x ) / sigma2 ) * sigma3;

				weight_sum	+= weight;
				value_sum	+= _values[i].value * weight;
			}

			_lastFiltered	= SafeDiv( value_sum, weight_sum, _defaultValue );
			_changed		= false;

			return _lastFiltered;
		}


		ValueType const &  FilterToLast (const ParamType &refParam)
		{
			if ( _values.Empty() )
			{
				_lastFiltered	= _defaultValue;
				_changed		= false;
				return _lastFiltered;
			}

			ValueType	value_sum	= ValueType();
			T			weight_sum	= T(0);
			T const		sigma2		= Square( sigma ) * T(-2);
			T const		sigma3		= T(1) / (sigma * Sqrt( T(2) * Pi<T> ));


			for (usize i = 0; i < _values.LastIndex(); ++i)
			{
				T	x		= refParam - _values[i].param;
				T	weight	= Exp( Square( x ) / sigma2 ) * sigma3;

				weight_sum	+= weight;
				value_sum	+= _values[i].value * weight;
			}

			weight_sum	+= T(1);
			value_sum	+= _values.Back().value;

			_lastFiltered	= value_sum / weight_sum;
			_changed		= false;

			return _lastFiltered;
		}


		ValueType const &  Get () const
		{
			ASSERT( not _changed );
			return _lastFiltered;
		}


		void	Clear ()			{ _values.Clear(); }

		usize	Count ()	const	{ return _values.Count(); }

		bool	Empty ()	const	{ return _values.Empty(); }
	};


}	// GXFilters
}	// GX_STL
