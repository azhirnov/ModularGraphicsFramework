// Copyright �  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/STL/Engine.STL.h"

namespace CMake
{
	using namespace GX_STL;
	using namespace GX_STL::GXTypes;



	//
	// CMake Builder
	//

	class CMakeBuilder
	{
	// types
	private:
		class BaseCMakeObj : public RefCountedObject<>
		{
		public:
			virtual bool ToString (OUT String &src) = 0;
			virtual bool ToString2 (OUT String &src) = 0;
		};

		class CMakeProject;
		class CMakeCompiler;
		class CMakeExternalProjects;
		class CMakeExternalVSProject;

		SHARED_POINTER( BaseCMakeObj );

		using Objects_t		= Array< BaseCMakeObjPtr >;
		using StringMap_t	= HashSet<Pair< String, String >>;	// value, enable if
		using StringSet_t	= HashSet< String >;
		using Self			= CMakeBuilder;


	// variables
	private:
		Objects_t		_compilers;
		Objects_t		_projects;
		Objects_t		_externalProjects;
		Objects_t		_externalVSProjects;
		String			_baseFolder;
		String			_solutionName;
		StringSet_t		_userOptions;
		GXMath::uint2	_version;

		// for all projects
		String			_projectOutputDir;
		StringMap_t		_projectIncludeDirs;
		StringMap_t		_projectLinkDirs;
		StringMap_t		_projectLinkLibs;
		StringMap_t		_projectDefines;


	// methods
	public:
		CMakeBuilder (StringCRef baseFolder, StringCRef solutionName);
		~CMakeBuilder ();

		bool Save (StringCRef filename = "CMakeLists.txt");
		void Clear ();

		CMakeProject* AddLibrary (StringCRef name, StringCRef folder = Uninitialized);
		CMakeProject* AddExecutable (StringCRef name, StringCRef folder = Uninitialized);

		CMakeCompiler*	AddMSVisualStudioCompiler ();
		CMakeCompiler*	AddGCCompiler ();
		CMakeCompiler*	AddClangCompiler ();

		//Self*	AddLibraryProjects (StringCRef folder);
		//Self*	AddExecutableProjects (StringCRef folder);

		Self*	Projects_OutputDirectory (StringCRef folder);
		Self*	Projects_AddDefinition (StringCRef def, StringCRef enableIf = Uninitialized);
		Self*	Projects_IncludeDirectory (StringCRef folder, StringCRef enableIf = Uninitialized);
		Self*	Projects_LinkDirectory (StringCRef folder, StringCRef enableIf = Uninitialized);
		Self*	Projects_LinkLibrary (StringCRef lib, StringCRef enableIf = Uninitialized);

		Self*	AddCondition (StringCRef name);

		Self*	SetVersion (uint major, uint minor);

		CMakeExternalProjects* AddExternal (StringCRef path, StringCRef enableIf = Uninitialized);
		CMakeExternalVSProject* AddVSProject (StringCRef filename, StringCRef enableIf = Uninitialized);
		Self* SearchVSProjects (StringCRef path, StringCRef projFolder = Uninitialized, StringCRef enableIf = Uninitialized);
	};



	//
	// CMake Project
	//

	class CMakeBuilder::CMakeProject final : public BaseCMakeObj
	{
		friend class CMakeBuilder;

	// types
	private:
		enum class EProjectType
		{
			Library,
			Executable,
			SharedLibrary,
		};

		using Self			= CMakeProject;
		using Groups_t		= HashMap< String, Array<String> >;	// {full folder name; files with relative paths}
		using StringMap_t	= HashSet<Pair< String, String >>;	// value, enable if
		using StringSet_t	= HashSet< String >;


	// variables
	private:
		CMakeBuilder *			_builder;
		const String			_name;
		const String			_baseFolder;
		const EProjectType		_projType;
		Groups_t				_groups;
		String					_folder;
		String					_outputDir;
		String					_enableIf;
		StringMap_t				_includeDirs;			// public
		StringMap_t				_includeDirsPrivate;	// private
		StringMap_t				_linkDirs;
		StringMap_t				_linkLibs;
		StringMap_t				_defines;
		StringMap_t				_dependencies;
		String					_source;
		String					_compilerOpt;


	// methods
	private:
		CMakeProject (CMakeBuilder *builder, EProjectType type, StringCRef name, StringCRef folder);

		bool ToString (OUT String &src) override;
		bool ToString2 (OUT String &) override { return true; }

		void SetCompilerOptions (StringCRef str);

		bool _ValidateInclude (StringCRef path, bool withOption) const;
		bool _ValidateLinkDir (StringCRef path, bool withOption) const;

	public:
		Self*	AddFile (StringCRef filename);
		Self*	AddGroup (StringCRef groupName, ArrayCRef<StringCRef> files);
		Self*	AddFolder (StringCRef path);
		Self*	AddFoldersRecursive (StringCRef path);

		Self*	EnableIf (StringCRef condition);
		Self*	ProjFolder (StringCRef folder);
		Self*	OutputDirectory (StringCRef folder);

		Self*	AddDefinition (StringCRef def, StringCRef enableIf = Uninitialized);
		Self*	AddDefinitions (ArrayCRef<String> defs, StringCRef enableIf = Uninitialized);

		Self*	IncludeDirectory (StringCRef folder, bool isPublic = false, StringCRef enableIf = Uninitialized);
		Self*	IncludeDirectory (ArrayCRef<String> folders, bool isPublic = false, StringCRef enableIf = Uninitialized);

		Self*	LinkDirectory (StringCRef folder, StringCRef enableIf = Uninitialized);
		Self*	LinkDirectory (ArrayCRef<String> folders, StringCRef enableIf = Uninitialized);

		Self*	LinkLibrary (StringCRef lib, StringCRef enableIf = Uninitialized);
		Self*	LinkLibrary (ArrayCRef<String> libs, StringCRef enableIf = Uninitialized);
		
		Self*	LinkLibrary (CMakeProject *lib);
		Self*	LinkLibrary (ArrayCRef<CMakeProject*> libs);

		Self*	LinkLibrary (CMakeExternalVSProject *proj);
		Self*	LinkLibrary (ArrayCRef<CMakeExternalVSProject*> projs);

		Self*	SearchLibraries (StringCRef path, ArrayCRef<String> libs, StringCRef enableIf = Uninitialized);

		Self*	AddDependency (StringCRef dep, StringCRef enableIf = Uninitialized);
		Self*	AddDependency (Array<String> deps, StringCRef enableIf = Uninitialized);

		Self*	AddSource (StringCRef src);
	};



	//
	// CMake Compiler
	//

	class CMakeBuilder::CMakeCompiler final : public BaseCMakeObj
	{
		friend class CMakeBuilder;

	// types
	private:
		enum class ECompilerName
		{
			MSVisualStudio,
			Clang,
			GCC,
		};

		class Configuration;
		SHARED_POINTER( Configuration );
		
		using Self			= CMakeCompiler;
		using Configs_t		= HashMap< String, ConfigurationPtr >;
		using StringMap_t	= HashSet<Pair< String, String >>;	// value, enable if


	// variables
	private:
		CMakeBuilder *			_builder;
		Configs_t				_configurations;
		const ECompilerName		_compName;
		String					_defaultCfg;
		StringMap_t				_includeDirs;
		StringMap_t				_linkDirs;
		StringMap_t				_linkLibs;


	// methods
	private:
		CMakeCompiler (CMakeBuilder *builder, ECompilerName name);

		bool ToString (OUT String &src) override;
		bool ToString2 (OUT String &src) override;
		
	public:
		Configuration*	AddConfiguration (StringCRef name, Configuration* copyFrom = null);

		Self*	DefaultConfiguration (StringCRef name);

		Self*	IncludeDirectory (StringCRef folder, StringCRef enableIf = Uninitialized);
		Self*	IncludeDirectory (ArrayCRef<String> folders, StringCRef enableIf = Uninitialized);
		
		Self*	LinkDirectory (StringCRef folder, StringCRef enableIf = Uninitialized);
		Self*	LinkDirectory (ArrayCRef<String> folders, StringCRef enableIf = Uninitialized);

		Self*	LinkLibrary (StringCRef lib, StringCRef enableIf = Uninitialized);
		Self*	LinkLibrary (ArrayCRef<String> libs, StringCRef enableIf = Uninitialized);
	};
	


	//
	// Compiler Configuration
	//
	
	class CMakeBuilder::CMakeCompiler::Configuration final : public RefCountedObject
	{
		friend class CMakeCompiler;

	// types
	private:
		using Self			= Configuration;
		using StringSet_t	= Array< String >;	//HashSet< String >;


	// variables
	private:
		StringSet_t		_defines;
		StringSet_t		_cFlags;
		StringSet_t		_cxxFlags;
		StringSet_t		_linkerFlags;
		
		StringSet_t		_targetCxxFlags;
		StringSet_t		_targetDefines;		// used only for CMakeProject
		StringSet_t		_targetLinkerFlags;


	// methods
	private:
		Configuration ();
		
		bool ToString (StringCRef name, OUT String &src);
		bool ToString2 (StringCRef name, OUT String &src);

		void _CopyFrom (const Configuration &other);

	public:
		Self*	AddTargetDefinitions (ArrayCRef<String> defs);
		Self*	AddTargetCxxFlags (ArrayCRef<String> flags);
		Self*	AddTargetLinkerFlags (ArrayCRef<String> flags);

		// global
		Self*	AddGlobalCFlags (ArrayCRef<String> flags);
		Self*	AddGlobalCxxFlags (ArrayCRef<String> flags);
		Self*	AddGlobalLinkerFlags (ArrayCRef<String> flags);
		Self*	AddGlobalDefinitions (ArrayCRef<String> defs);
	};


	//
	// CMake External Projects
	//

	class CMakeBuilder::CMakeExternalProjects final : public BaseCMakeObj
	{
		friend class CMakeBuilder;

	// types
	private:
		using Self	= CMakeExternalProjects;

		struct TargetInfo
		{
			String		name;
			String		path;
			String		projFolder;
		};

		enum class EKeyType {
			SubDir,
			Exe,
			Lib,
			Prop,
			Incl,
		};

		using StringMap_t	= HashMap< String, String >;
		using OptionMap_t	= HashMap< String, String >;


	// variables
	private:
		CMakeBuilder *		_builder;
		const String		_enableIf;
		const String		_baseFolder;
		String				_path;
		String				_folder;
		String				_source;
		StringMap_t			_renameProjMap;
		OptionMap_t			_options;
		bool				_overrideSrcFolders;


	// methods
	private:
		CMakeExternalProjects (CMakeBuilder *builder, StringCRef path, StringCRef enableIf);
		
		bool ToString (OUT String &src) override;
		bool ToString2 (OUT String &) override { return false; }

		static bool _ReqursiveGetTargets (EKeyType type, Array<String> dirs, OUT Array<TargetInfo> &result);
		static bool _ParseCMake (StringCRef filename, Array<String> dirs, OUT Array<TargetInfo> &result);
		static bool _ParseBlock (EKeyType blockType, StringCRef str, usize pos, Array<String> dirs, OUT Array<TargetInfo> &result);
		static bool _ParseAddProject (StringCRef line, OUT Array<TargetInfo> &result);
		static bool _ParseSetProperty (StringCRef line, OUT Array<TargetInfo> &result);

	public:
		Self*	ProjFolder (StringCRef folder, bool overrideSrc);
		Self*	Rename (StringCRef srcProjectName, StringCRef dstProjectName);
		Self*	AddBoolOption (StringCRef name, bool value = true, StringCRef info = StringCRef());
		Self*	AddStringOption (StringCRef name, StringCRef value, StringCRef info = StringCRef());
		Self*	AddPathOption (StringCRef name, StringCRef path, StringCRef info = StringCRef());
		Self*	AddSource (StringCRef src);
	};



	//
	// CMake External Visual Studio Project
	//
	
	class CMakeBuilder::CMakeExternalVSProject final : public BaseCMakeObj
	{
		friend class CMakeBuilder;

	// types
	private:
		using Self	= CMakeExternalVSProject;
	

	// variables
	private:
		CMakeBuilder *		_builder;
		const String		_enableIf;
		String				_baseFolder;
		const String		_name;
		String				_folder;


	// methods
	private:
		CMakeExternalVSProject (CMakeBuilder *builder, StringCRef filename, StringCRef enableIf);

		bool ToString (OUT String &src) override;
		bool ToString2 (OUT String &) override { return false; }
		
	public:
		Self*	ProjFolder (StringCRef folder);
		Self*	Search ();

		StringCRef	GetName () const	{ return _name; }
	};


}	// CMake


#include "MSVisualStudioConstants.h"
#include "ClangConstants.h"
#include "GccConstants.h"