// Copyright © 2014-2016  Zhirnov Andrey. All rights reserved.


#	ifndef F_FIRST_DECLARATION
#		define F_TEMPLATE_OVERRIDE
#	endif

#	if F_UID != 0
#		define F_DECL_PARAMS_REF_WITH_COMMA			, F_DECL_PARAMS_WITH_REF
#		define F_FUNC_ARGS_WITH_COMMA				, F_FUNC_ARGS
#	else
#		define F_DECL_PARAMS_REF_WITH_COMMA
#		define F_FUNC_ARGS_WITH_COMMA
#	endif
	
#	define F_TEMPLATE_TYPES_VAR_WITH_COMMA			, F_TEMPLATE_TYPES_VAR
#	define F_TEMPLATE_TYPES_WITH_COMMA				, F_TEMPLATE_TYPES


#	ifdef F_FIRST_DECLARATION
#		undef  F_TEMPLATE_OVERRIDE
#		define F_TEMPLATE_OVERRIDE		<BufSize, F_TEMPLATE_TYPES>
#	endif

	namespace _types_hidden_
	{
		//
		// Function Implementation
		//
		template <usize BufSize, F_TEMPLATE_PARAMS>
		struct TStaticFunctionImpl F_TEMPLATE_OVERRIDE : Function< F_RETURN, BufSize >::Interface
		{
		public:
			typedef F_RETURN (* PFunction_t) ( F_DECL_PARAMS );
			typedef TStaticFunctionImpl< BufSize, F_TEMPLATE_TYPES >			Self;
			typedef typename Function< F_RETURN, BufSize >::interface_ptr_t		ptr_t;
		
		private:
			PFunction_t		_func;
			F_MEMBER_PARAMS
		
		public:
			TStaticFunctionImpl (PFunction_t func F_DECL_PARAMS_REF_WITH_COMMA):
				_func(func) F_ARGS_TO_MEMBERS
			{}
		
			TStaticFunctionImpl (TStaticFunctionImpl &&o):
				_func(o._func) F_MEMBERS_MOVE_CTOR
			{}

			TStaticFunctionImpl (const TStaticFunctionImpl &) = default;
		
			bool		IsValid ()			const override	{ return _func != null; }
			F_RETURN	Call ()				const override	{ return _func( F_ARG_PARAMS ); }
			void		CloneTo (ptr_t &p)	const override	{ p.Set( *this ); }
			void		MoveTo (ptr_t &p)		override	{ p.Set( RVREF( *this ) ); }
			TypeId_t	TypeId ()			const override	{ return TypeIdOf( *this ); }
			usize		Size ()				const override	{ return sizeof( this ); }
		};
	}

	namespace FunctionBuilder
	{
		template <F_TEMPLATE_PARAMS>
		struct Type < F_RETURN (*) (F_DECL_PARAMS) >
		{
			typedef F_RETURN (* PFunction_t) (F_DECL_PARAMS);
		
			template <usize BufSize>
			static void Create (OUT Function< F_RETURN, BufSize > &fn, PFunction_t func F_DECL_PARAMS_REF_WITH_COMMA)
			{
				typedef _types_hidden_::TStaticFunctionImpl< BufSize, F_TEMPLATE_TYPES >		fn_impl_t;
				fn._func.Set( RVREF( fn_impl_t( func F_FUNC_ARGS_WITH_COMMA ) ) );
			}
		};
	
		template <usize BufSize, F_TEMPLATE_PARAMS>
		inline void Create (OUT Function< F_RETURN, BufSize > &fn, F_RETURN (* func) (F_DECL_PARAMS) F_DECL_PARAMS_REF_WITH_COMMA)
		{
			Type< F_RETURN (*)(F_DECL_PARAMS) >::Create( fn, func F_FUNC_ARGS_WITH_COMMA );
		}
	}

	namespace _types_hidden_
	{
		//
		// Delegate Interface
		//
		template <F_TEMPLATE_PARAMS>
		struct TDelegateInterface < F_RETURN (F_DECL_PARAMS) >
		{
			typedef TDelegateInterface< F_RETURN (F_DECL_PARAMS) >						Interface;
			typedef BufferedPtr< Interface, GlobalConst::STL_DelegateInterfaceSize >	ptr_t;
		
			virtual ~TDelegateInterface () {}
			virtual bool		IsValid ()						const = 0;
			virtual F_RETURN	Call (F_DECL_PARAMS_WITH_REF)	const = 0;
			virtual void		CloneTo (ptr_t &)				const = 0;
			virtual TypeId_t	TypeId ()						const = 0;
			virtual usize		Size ()							const = 0;
		
			bool Cmp (const Interface *p) const
			{
				const usize size = Size();
				return	( size == p->Size() ) and
						( TypeId() == p->TypeId() ) and
						UnsafeMem::MemCmp( (void *)this, (void *)p, size ) == 0;
			}
		
			bool Less (const Interface *p) const
			{
				const usize		size0 = Size();
				const usize		size1 = p->Size();
				const TypeId_t	type0 = TypeId();
				const TypeId_t	type1 = p->TypeId();
			
				if ( type0 == type1 ) {
					if ( size0 == size1 )
						return UnsafeMem::MemCmp( (void *)this, (void *)p, size0 ) < 0;
					else
						return size0 < size1;
				}
				return type0 < type1;
			}
		
			bool Greater (const Interface *p) const
			{
				const usize		size0 = Size();
				const usize		size1 = p->Size();
				const TypeId_t	type0 = TypeId();
				const TypeId_t	type1 = p->TypeId();
			
				if ( type0 == type1 ) {
					if ( size0 == size1 )
						return UnsafeMem::MemCmp( (void *)this, (void *)p, size0 ) > 0;
					else
						return size0 > size1;
				}
				return type0 > type1;
			}
		};
	}

	namespace DelegateBuilder
	{
		template <F_TEMPLATE_PARAMS>
		inline void Create (OUT Delegate < F_RETURN (F_DECL_PARAMS) > &del, F_RETURN (*func) (F_DECL_PARAMS));
	
		template <typename C, typename CP, F_TEMPLATE_PARAMS>
		inline void Create (OUT Delegate < F_RETURN (F_DECL_PARAMS) > &del, const CP &ptr, F_RETURN (C:: *func) (F_DECL_PARAMS));
	
		template <typename C, typename CP, F_TEMPLATE_PARAMS>
		inline void Create (OUT Delegate < F_RETURN (F_DECL_PARAMS) > &del, const CP &ptr, F_RETURN (C:: *func) (F_DECL_PARAMS) const);
	}

	//
	// Delegate
	//
	template <F_TEMPLATE_PARAMS>
	struct Delegate < F_RETURN (F_DECL_PARAMS) >
	{
		template <F_TEMPLATE_PARAMS>
		friend void DelegateBuilder::Create (OUT Delegate < F_RETURN (F_DECL_PARAMS) > &del, F_RETURN (*func) (F_DECL_PARAMS));
	
		template <typename C, typename CP, F_TEMPLATE_PARAMS>
		friend void DelegateBuilder::Create (OUT Delegate < F_RETURN (F_DECL_PARAMS) > &del, const CP &ptr, F_RETURN (C:: *func) (F_DECL_PARAMS));
	
		template <typename C, typename CP, F_TEMPLATE_PARAMS>
		friend void DelegateBuilder::Create (OUT Delegate < F_RETURN (F_DECL_PARAMS) > &del, const CP &ptr, F_RETURN (C:: *func) (F_DECL_PARAMS) const);

	public:
		typedef F_RETURN (* PFunction_t) (F_DECL_PARAMS);
		typedef _types_hidden_::TDelegateInterface< F_RETURN (F_DECL_PARAMS) >	Interface;
		typedef Delegate< F_RETURN (F_DECL_PARAMS) >							Self;
		typedef typename CompileTime::TypeListFrom< F_TEMPLATE_TYPES >			arg_types_t;
		typedef typename Interface::ptr_t										interface_ptr_t;
	
	private:
		interface_ptr_t		_func;
	
	public:
		Delegate (GX_DEFCTOR)
		{}
	
		Delegate (const Self &other)
		{
			if ( other._func ) {
				other._func->CloneTo( _func );
			}
		}

		//Delegate (Self &&other) : _func( RVREF( other._func ) )
		//{}
	
	
		Self & operator = (const Self &right)
		{
			if ( right._func ) {
				right._func->CloneTo( _func );
			}
			return *this;
		}
	
	
		bool	IsValid ()								const	{ return _func and _func->IsValid(); }
	
		F_RETURN Call (F_DECL_PARAMS_WITH_REF)			const	{ ASSERT( IsValid() );  return _func->Call(F_FUNC_ARGS); }
		F_RETURN SafeCall (F_DECL_PARAMS_WITH_REF)		const	{ return IsValid() ? _func->Call(F_FUNC_ARGS) : F_RETURN(); }
	
		F_RETURN operator () (F_DECL_PARAMS_WITH_REF)	const	{ ASSERT( IsValid() );  return _func->Call(F_FUNC_ARGS); }
	
		bool operator == (const Self &right)			const	{ ASSERT( IsValid() );  return _func->Cmp( right._func.ptr() );  }
		bool operator <  (const Self &right)			const	{ ASSERT( IsValid() );  return _func->Less( right._func.ptr() ); }
		bool operator >  (const Self &right)			const	{ ASSERT( IsValid() );  return _func->Greater( right._func.ptr() ); }
		bool operator != (const Self &right)			const	{ return not (*this == right); }
		bool operator <= (const Self &right)			const	{ return not (*this >  right); }
		bool operator >= (const Self &right)			const	{ return not (*this <  right); }
	};
	

#	ifdef F_FIRST_DECLARATION
#		undef  F_TEMPLATE_OVERRIDE
#		define F_TEMPLATE_OVERRIDE		<F_TEMPLATE_TYPES>
#	endif

	namespace _types_hidden_ 
	{
		//
		// Delegate Implementation
		//
		template <F_TEMPLATE_PARAMS>
		struct TStaticDelegateImpl F_TEMPLATE_OVERRIDE : TDelegateInterface< F_RETURN (F_DECL_PARAMS) >
		{
		public:
			typedef F_RETURN (* PFunction_t) (F_DECL_PARAMS);
			typedef TStaticDelegateImpl< F_TEMPLATE_TYPES >			Self;
			typedef TDelegateInterface< F_RETURN (F_DECL_PARAMS) >	Interface;
		
		private:
			PFunction_t		_func;
		
		public:
			explicit
			TStaticDelegateImpl (PFunction_t func): _func(func) {}
		
			bool		IsValid ()						const override	{ return _func != null; }
			F_RETURN	Call (F_DECL_PARAMS_WITH_REF)	const override	{ return _func(F_FUNC_ARGS); }
			void		CloneTo (ptr_t &p)				const override	{ p.Set( *this ); }
			TypeId_t	TypeId ()						const override	{ return TypeIdOf( *this ); }
			usize		Size ()							const override	{ return sizeof(this); }
		};
	}

	namespace DelegateBuilder
	{
		template <F_TEMPLATE_PARAMS>
		inline void Create (OUT Delegate < F_RETURN (F_DECL_PARAMS) > &del, F_RETURN (*func) (F_DECL_PARAMS))
		{
			typedef _types_hidden_::TStaticDelegateImpl< F_TEMPLATE_TYPES >	del_impl_t;
			del._func.Set( RVREF( del_impl_t( func ) ) );
		}
	
		template <F_TEMPLATE_PARAMS>
		inline typename RValueRef< Delegate < F_RETURN (F_DECL_PARAMS) > >::type  Create (F_RETURN (*func) (F_DECL_PARAMS))
		{
			Delegate < F_RETURN (F_DECL_PARAMS) >	del;
			Create( del, func );
			return RVREF( del );
		}
	}


	//
	// Event Delegate Implementation
	//
	template <F_TEMPLATE_PARAMS>
	struct Event < F_RETURN (F_DECL_PARAMS) > : public CompileTime::FastCopyable
	{
	// types
	public:
		typedef F_RETURN (* PFunction_t) ( F_DECL_PARAMS );
		typedef Delegate< F_RETURN (F_DECL_PARAMS) >			delegate_t;
		typedef Event< F_RETURN (F_DECL_PARAMS) >				Self;
		typedef Set< delegate_t >								delegate_set_t;
	

	// variables
	private:
		delegate_set_t	_delegates;
	

	// methods
	public:
		Event (GX_DEFCTOR)
		{}

		Event (const Self &other) : _delegates(other._delegates)
		{}

		Event (Self &&other) : _delegates( RVREF( other._delegates ) )
		{}
	
		void Add (const delegate_t &del)
		{
			_delegates.Add( del );
		}
	
		void Remove (const delegate_t &del)
		{
			_delegates.Erase( del );
		}
	
		template <typename C, typename CP>
		void Add (const CP &ptr, F_RETURN (C:: *func) (F_DECL_PARAMS))
		{
			return Add( DelegateBuilder::Create( ptr, func ) );
		}
	
		void Add (F_RETURN (*func) (F_DECL_PARAMS))
		{
			return Add( DelegateBuilder::Create( func ) );
		}
	
		template <typename C, typename CP>
		void Remove (const CP &ptr, F_RETURN (C:: *func) (F_DECL_PARAMS))
		{
			return Remove( DelegateBuilder::Create( ptr, func ) );
		}
	
		void Remove (F_RETURN (*func) (F_DECL_PARAMS))
		{
			return Remove( DelegateBuilder::Create( func ) );
		}

		void Reserve (usize size)
		{
			_delegates.Reserve( size );
		}
	
		void Clear ()
		{
			_delegates.Clear();
		}
	
		void Call (F_DECL_PARAMS_WITH_REF)			const	{ FOR( i, _delegates ) { _delegates[i].Call( F_FUNC_ARGS ); } }
	
		void operator () (F_DECL_PARAMS_WITH_REF)	const 	{ FOR( i, _delegates ) { _delegates[i].Call( F_FUNC_ARGS ); } }
	
		usize Count ()								const	{ return _delegates.Count(); }
	
		bool Empty()								const	{ return _delegates.Empty(); }
	
		delegate_t const & Get (usize i)			const	{ return _delegates[i]; }

		friend void SwapValues (Self &left, Self &right)
		{
			SwapValues( left._delegates, right._delegates );
		}
	};


	template <F_TEMPLATE_PARAMS>
	struct Event < Delegate< F_RETURN (F_DECL_PARAMS) > > : Event< F_RETURN (F_DECL_PARAMS) >
	{
	};

	
#	ifdef F_FIRST_DECLARATION
#		undef  F_TEMPLATE_OVERRIDE
#		define F_TEMPLATE_OVERRIDE	<C, CP, BufSize, F_TEMPLATE_TYPES>
#	endif

	namespace _types_hidden_
	{
		//
		// Function Implementation
		//
		template <typename C, typename CP, usize BufSize, F_TEMPLATE_PARAMS>
		struct TMemberFunctionImpl F_TEMPLATE_OVERRIDE : Function< F_RETURN, BufSize >::Interface
		{
		public:
			typedef F_RETURN (C:: *PFunction_t)(F_DECL_PARAMS);
			typedef TMemberFunctionImpl< C, CP, BufSize, F_TEMPLATE_TYPES >		Self;
			typedef typename Function< F_RETURN, BufSize >::interface_ptr_t		ptr_t;
	
		private:
			PFunction_t		_func;
			CP				_classPtr;
			F_MEMBER_PARAMS
	
		public:
			TMemberFunctionImpl (const CP &ptr, PFunction_t func F_DECL_PARAMS_REF_WITH_COMMA):
				_func(func), _classPtr(ptr) F_ARGS_TO_MEMBERS
			{}
		
			TMemberFunctionImpl (TMemberFunctionImpl &&o):
				_func(o._func), _classPtr( RVREF( o._classPtr ) ) F_MEMBERS_MOVE_CTOR
			{}

			TMemberFunctionImpl (const TMemberFunctionImpl &) = default;
		
			bool		IsValid ()			const override	{ return _func != null and _classPtr != null; }
			F_RETURN	Call ()				const override	{ return ((*_classPtr).*_func)( F_ARG_PARAMS ); }
			void		CloneTo (ptr_t &p)	const override	{ p.Set( *this ); }
			void		MoveTo (ptr_t &p)		override	{ p.Set( RVREF( *this ) ); }
			TypeId_t	TypeId ()			const override	{ return TypeIdOf( *this ); }
			usize		Size ()				const override	{ return sizeof( this ); }
		};
	
		//
		// Function Implementation
		//
		template <typename C, typename CP, usize BufSize, F_TEMPLATE_PARAMS>
		struct TMemberConstFunctionImpl F_TEMPLATE_OVERRIDE : Function< F_RETURN, BufSize >::Interface
		{
		public:
			typedef F_RETURN (C:: *PFunction_t)(F_DECL_PARAMS) const;
			typedef TMemberConstFunctionImpl< C, CP, BufSize, F_TEMPLATE_TYPES >		Self;
			typedef typename Function< F_RETURN, BufSize >::interface_ptr_t			ptr_t;
	
		private:
			PFunction_t		_func;
			CP				_classPtr;
			F_MEMBER_PARAMS
	
		public:
			TMemberConstFunctionImpl (const CP &ptr, PFunction_t func F_DECL_PARAMS_REF_WITH_COMMA):
				_func(func), _classPtr(ptr) F_ARGS_TO_MEMBERS
			{}
		
			TMemberConstFunctionImpl (TMemberConstFunctionImpl &&o):
				_func(o._func), _classPtr( RVREF( o._classPtr ) ) F_MEMBERS_MOVE_CTOR
			{}

			TMemberConstFunctionImpl (const TMemberConstFunctionImpl &) = default;
		
			bool		IsValid ()			const override	{ return _func != null and _classPtr != null; }
			F_RETURN	Call ()				const override	{ return ((*_classPtr).*_func)( F_ARG_PARAMS ); }
			void		CloneTo (ptr_t &p)	const override	{ p.Set( *this ); }
			void		MoveTo (ptr_t &p)		  override	{ p.Set( RVREF( *this ) ); }
			TypeId_t	TypeId ()			const override	{ return TypeIdOf( *this ); }
			usize		Size ()				const override	{ return sizeof( this ); }
		};
	}

	namespace FunctionBuilder
	{
		template <typename C, F_TEMPLATE_PARAMS>
		struct Type < F_RETURN (C:: *) (F_DECL_PARAMS) >
		{
			typedef F_RETURN (C:: *PFunction_t) (F_DECL_PARAMS);
		
			template <typename CP, usize BufSize>
			static void Create (OUT Function< F_RETURN, BufSize > &fn, const CP &ptr, PFunction_t func F_DECL_PARAMS_REF_WITH_COMMA)
			{
				typedef _types_hidden_::TMemberFunctionImpl< C, CP, BufSize, F_TEMPLATE_TYPES >	fn_impl_t;
				fn._func.Set( RVREF( fn_impl_t( ptr, func F_FUNC_ARGS_WITH_COMMA ) ) );
			}
		};
	
		template <typename C, F_TEMPLATE_PARAMS>
		struct Type < F_RETURN (C:: *) (F_DECL_PARAMS) const >
		{
			typedef F_RETURN (C:: *PFunction_t) (F_DECL_PARAMS) const;
		
			template <typename CP, usize BufSize>
			static void Create (OUT Function< F_RETURN, BufSize > &fn, const CP &ptr, PFunction_t func F_DECL_PARAMS_REF_WITH_COMMA)
			{
				typedef _types_hidden_::TMemberConstFunctionImpl< C, CP, BufSize, F_TEMPLATE_TYPES >	fn_impl_t;
				fn._func.Set( RVREF( fn_impl_t( ptr, func F_FUNC_ARGS_WITH_COMMA ) ) );
			}
		};
	
		template <typename C, typename CP, usize BufSize, F_TEMPLATE_PARAMS>
		inline void Create (OUT Function< F_RETURN, BufSize > &fn, const CP &ptr, F_RETURN (C:: *func) (F_DECL_PARAMS) F_DECL_PARAMS_REF_WITH_COMMA)
		{
			Type< F_RETURN (C:: *)(F_DECL_PARAMS) >::Create( fn, ptr, func F_FUNC_ARGS_WITH_COMMA );
		}
	
		template <typename C, typename CP, usize BufSize, F_TEMPLATE_PARAMS>
		inline void Create (OUT Function< F_RETURN, BufSize > &fn, const CP &ptr, F_RETURN (C:: *func) (F_DECL_PARAMS) const F_DECL_PARAMS_REF_WITH_COMMA)
		{
			Type< F_RETURN (C:: *)(F_DECL_PARAMS) const >::Create( fn, ptr, func F_FUNC_ARGS_WITH_COMMA );
		}
	}
	
	
#	ifdef F_FIRST_DECLARATION
#		undef  F_TEMPLATE_OVERRIDE
#		define F_TEMPLATE_OVERRIDE		<C, F_TEMPLATE_TYPES>
#	endif

	namespace _types_hidden_
	{
		//
		// Delegate Interface
		//
		template <typename C, F_TEMPLATE_PARAMS>
		struct AUXDEF_UNITE_RAW( TDelegateInterface, F_UID ) : TDelegateInterface< F_RETURN (F_DECL_PARAMS) >
		{
		};
	
		//
		// Delegate Implementation
		//
		template <typename C, F_TEMPLATE_PARAMS>
		struct TMemberDelegateImpl F_TEMPLATE_OVERRIDE :
			AUXDEF_UNITE_RAW( TDelegateInterface, F_UID )< typename RawPointer< C >::type F_TEMPLATE_TYPES_VAR_WITH_COMMA >
		{
		public:
			typedef typename RawPointer<C>::type								class_t;
			typedef F_RETURN (class_t:: * PFunction_t)( F_DECL_PARAMS );
			typedef TMemberDelegateImpl< C F_TEMPLATE_TYPES_WITH_COMMA >		Self;
			typedef typename RawPointer< C >::ptr_t								class_ptr_t;
			typedef TDelegateInterface< F_RETURN (F_DECL_PARAMS) >				Interface;
			typedef typename Interface::ptr_t									ptr_t;
		
		private:
			PFunction_t		_func;
			class_ptr_t		_classPtr;
		
		public:
			TMemberDelegateImpl (const class_ptr_t &ptr, PFunction_t func): _classPtr(ptr), _func(func) {}
			TMemberDelegateImpl (class_ptr_t &&ptr, PFunction_t func): _classPtr( RVREF(ptr) ), _func(func) {}
		
			bool		IsValid ()						const override	{ return _func != null and _classPtr != null; }
			F_RETURN	Call (F_DECL_PARAMS_WITH_REF)	const override	{ return ((*_classPtr).*_func)( F_FUNC_ARGS ); }
			void		CloneTo (ptr_t &p)				const override	{ p.Set( *this ); }
			TypeId_t	TypeId ()						const override	{ return TypeIdOf( *this ); }
			usize		Size ()							const override	{ return sizeof( this ); }
		};
	
		//
		// Delegate Implementation
		//
		template <typename C, F_TEMPLATE_PARAMS>
		struct TMemberConstDelegateImpl F_TEMPLATE_OVERRIDE :
			AUXDEF_UNITE_RAW( TDelegateInterface, F_UID )< typename RawPointer< C >::type F_TEMPLATE_TYPES_VAR_WITH_COMMA >
		{
		public:
			typedef typename RawPointer<C>::type								class_t;
			typedef F_RETURN (class_t:: * PFunction_t)( F_DECL_PARAMS ) const;
			typedef TMemberConstDelegateImpl< C F_TEMPLATE_TYPES_WITH_COMMA >	Self;
			typedef typename RawPointer< C >::ptr_t								class_ptr_t;
			typedef TDelegateInterface< F_RETURN (F_DECL_PARAMS) >				Interface;
			typedef typename Interface::ptr_t									ptr_t;
		
		private:
			PFunction_t		_func;
			class_ptr_t		_classPtr;
		
		public:
			TMemberConstDelegateImpl (const class_ptr_t &ptr, PFunction_t func): _classPtr(ptr), _func(func) {}
			TMemberConstDelegateImpl (class_ptr_t &&ptr, PFunction_t func): _classPtr( RVREF(ptr) ), _func(func) {}
		
			bool		IsValid ()						const override	{ return _func != null and _classPtr != null; }
			F_RETURN	Call (F_DECL_PARAMS_WITH_REF)	const override	{ return ((*_classPtr).*_func)( F_FUNC_ARGS ); }
			void		CloneTo (ptr_t &p)				const override	{ p.Set( *this ); }
			TypeId_t	TypeId ()						const override	{ return TypeIdOf( *this ); }
			usize		Size ()							const override	{ return sizeof( this ); }
		};
	}

	namespace DelegateBuilder
	{
		template <typename C, typename CP, F_TEMPLATE_PARAMS>
		inline void Create (OUT Delegate < F_RETURN (F_DECL_PARAMS) > &del, const CP &ptr, F_RETURN (C:: *func) (F_DECL_PARAMS))
		{
			typedef _types_hidden_::TMemberDelegateImpl< CP F_TEMPLATE_TYPES_WITH_COMMA >	del_impl_t;
			del._func.Set( RVREF( del_impl_t( ptr, func ) ) );
		}

		template <typename C, typename CP, F_TEMPLATE_PARAMS>
		inline void Create (OUT Delegate < F_RETURN (F_DECL_PARAMS) > &del, const CP &ptr, F_RETURN (C:: *func) (F_DECL_PARAMS) const)
		{
			typedef _types_hidden_::TMemberConstDelegateImpl< CP F_TEMPLATE_TYPES_WITH_COMMA >	del_impl_t;
			del._func.Set( RVREF( del_impl_t( ptr, func ) ) );
		}

		template <typename C, typename CP, F_TEMPLATE_PARAMS>
		inline Delegate < F_RETURN (F_DECL_PARAMS) >  Create (const CP &ptr, F_RETURN (C:: *func) (F_DECL_PARAMS))
		{
			Delegate < F_RETURN (F_DECL_PARAMS) >	del;
			Create( del, ptr, func );
			return del;
		}

		template <typename C, typename CP, F_TEMPLATE_PARAMS>
		inline Delegate < F_RETURN (F_DECL_PARAMS) >  Create (const CP &ptr, F_RETURN (C:: *func) (F_DECL_PARAMS) const)
		{
			Delegate < F_RETURN (F_DECL_PARAMS) >	del;
			Create( del, ptr, func );
			return del;
		}
	}

	
#	ifndef F_FIRST_DECLARATION
#		define F_FIRST_DECLARATION
#	endif

#	undef F_DECL_PARAMS_REF_WITH_COMMA
#	undef F_TEMPLATE_TYPES_VAR_WITH_COMMA
#	undef F_TEMPLATE_TYPES_WITH_COMMA
#	undef F_FUNC_ARGS_WITH_COMMA

#	undef F_TEMPLATE_OVERRIDE
#	undef F_TEMPLATE_PARAMS
#	undef F_TEMPLATE_TYPES
#	undef F_TEMPLATE_TYPES_VAR
#	undef F_DECL_PARAMS
#	undef F_DECL_PARAMS_WITH_REF
#	undef F_MEMBER_PARAMS
#	undef F_ARGS_TO_MEMBERS
#	undef F_MEMBERS_MOVE_CTOR
#	undef F_ARG_PARAMS
#	undef F_FUNC_ARGS
#	undef F_UID
