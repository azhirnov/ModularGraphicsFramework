// Copyright ©  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Platforms/OpenGL/Impl/GL4BaseModule.h"
#include "Engine/Platforms/Shared/GPU/Sampler.h"

#if defined( GRAPHICS_API_OPENGL )

namespace Engine
{
namespace PlatformGL
{
	
	//
	// OpenGL Sampler
	//

	class GL4Sampler final : public GL4BaseModule
	{
	// types
	private:
		using SupportedMessages_t	= GL4BaseModule::SupportedMessages_t::Erase< MessageListFrom<
											ModuleMsg::Delete
										> >
										::Append< MessageListFrom<
											GpuMsg::GetSamplerDescriptor,
											GpuMsg::GetGLSamplerID
										> >;

		using SupportedEvents_t		= GL4BaseModule::SupportedEvents_t;


	// constants
	private:
		static const Runtime::VirtualTypeList	_msgTypes;
		static const Runtime::VirtualTypeList	_eventTypes;


	// variables
	private:
		SamplerDescriptor	_descr;

		gl::GLuint			_samplerId;


	// methods
	public:
		GL4Sampler (const GlobalSystemsRef gs, const CreateInfo::GpuSampler &ci);
		~GL4Sampler ();


	//protected:
		SamplerDescriptor const&	GetDescriptor ()	const	{ return _descr; }
		gl::GLuint					GetSamplerID ()		const	{ return _samplerId; }


	// message handlers
	private:
		bool _Compose (const  Message< ModuleMsg::Compose > &);
		bool _Delete (const Message< ModuleMsg::Delete > &);
		bool _GetGLSamplerID (const Message< GpuMsg::GetGLSamplerID > &);
		bool _GetSamplerDescriptor (const Message< GpuMsg::GetSamplerDescriptor > &);

	private:
		bool _IsCreated () const;
		bool _CreateSampler ();
		void _DestroySampler ();
		
		void _DestroyResources () override;
	};



	//
	// OpenGL Sampler Cache
	//

	class GL4SamplerCache final
	{
	// types
	private:
		SHARED_POINTER( GL4Sampler );

		struct SearchableSampler
		{
			GL4SamplerPtr	samp;

			explicit SearchableSampler (const GL4SamplerPtr &s) : samp(s) {}

			bool operator == (const SearchableSampler &right) const	{ return samp->GetDescriptor() == right.samp->GetDescriptor(); }
			bool operator >  (const SearchableSampler &right) const	{ return samp->GetDescriptor() >  right.samp->GetDescriptor(); }
			bool operator <  (const SearchableSampler &right) const	{ return samp->GetDescriptor() <  right.samp->GetDescriptor(); }
		};

		struct SamplerSearch
		{
			SamplerDescriptor	descr;

			explicit SamplerSearch (const SamplerDescriptor &s) : descr(s) {}

			bool operator == (const SearchableSampler &right) const	{ return descr == right.samp->GetDescriptor(); }
			bool operator >  (const SearchableSampler &right) const	{ return descr >  right.samp->GetDescriptor(); }
			bool operator <  (const SearchableSampler &right) const	{ return descr <  right.samp->GetDescriptor(); }
		};

		using Samplers_t	= Set< SearchableSampler >;


	// variables
	private:
		Samplers_t				_samplers;
		const GLSystemsRef		_glSystems;

		uint					_maxAnisotropy;
		bool					_initialized;


	// methods
	public:
		explicit GL4SamplerCache (GLSystemsRef glSys);

		GL4SamplerPtr	Create (const GlobalSystemsRef gs, const CreateInfo::GpuSampler &ci);

		GLSystemsRef	GLSystems ()			const	{ return _glSystems; }
		Ptr<GL4Device>	GetDevice ()			const	{ return _glSystems->Get< GL4Device >(); }

		// device features
		bool	SupportAnisotropyFiltering ()	const	{ return true; }
		uint	GetMaxAnisotropyLevel ()		const	{ return _maxAnisotropy; }

	private:
		void _Initialize ();
	};


}	// PlatformGL
}	// Engine

#endif	// GRAPHICS_API_OPENGL
