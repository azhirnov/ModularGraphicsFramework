// Copyright © 2014-2017  Zhirnov Andrey. All rights reserved.

#pragma once

		
// from https://en.wikibooks.org/wiki/More_C++_Idioms/Member_Detector
#define GX_CREATE_MEMBER_DETECTOR( _X_ )													\
	template<typename T> class Detect_##_X_ {												\
		struct Fallback { int _X_; };														\
		struct Derived : T, Fallback { };													\
																							\
		template<typename U, U> struct Check;												\
																							\
		typedef char ArrayOfOne[1];															\
		typedef char ArrayOfTwo[2];															\
																							\
		template<typename U> static ArrayOfOne & func(Check<int Fallback::*, &U::_X_> *);	\
		template<typename U> static ArrayOfTwo & func(...);									\
		public:																				\
		typedef Detect_##_X_ type;															\
		enum { value = sizeof(func<Derived>(0)) == 2 };										\
	};

