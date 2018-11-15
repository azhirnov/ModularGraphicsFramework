// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#pragma once

#include "Engine/Base/Common/IDs.h"
#include "Engine/Base/Common/Enums.h"
#include "Engine/Base/Public/ModuleMessages.h"
#include "Engine/Base/Public/CreateInfo.h"

namespace Engine
{
namespace Base
{

	//
	// Data Source Description
	//

	struct DataSourceDescription
	{
	// variables
		BytesU					totalSize;		// can be 0 if total size is unknown
		BytesU					available;		
		EMemoryAccess::bits		memoryFlags;
		bool					resizable	= false;

	// methods
		DataSourceDescription () {}
		
		DataSourceDescription (BytesU totalSize, BytesU available, EMemoryAccess::bits memoryFlags) :
			totalSize{totalSize}, available{available}, memoryFlags{memoryFlags} {}
	};

}	// Base


namespace CreateInfo
{

	//
	// Data Provider Manager Create Info
	//
	struct DataProviderManager
	{
	};


	//
	// Local Storage Data Provider Create Info
	//
	struct LocalStorageDataProvider
	{
	// variables
		ModulePtr					manager;
		String						baseFolder;
		Base::EMemoryAccess::bits	access;

	// methods
		LocalStorageDataProvider () {}
		LocalStorageDataProvider (StringCRef folder, Base::EMemoryAccess::bits access) : baseFolder{folder}, access{access} {}
		explicit LocalStorageDataProvider (StringCRef folder) : baseFolder{folder}, access{Base::EMemoryAccess::CpuReadWrite} {}
	};


	//
	// In Memery Data Provider Create Info
	//
	struct InMemoryDataProvider
	{
	// variables
		ModulePtr					manager;
		String						baseFolder;
		Base::EMemoryAccess::bits	access;		// must be read/write to store and load data from cache

	// methods
		InMemoryDataProvider () {}
		InMemoryDataProvider (StringCRef folder, Base::EMemoryAccess::bits access) : baseFolder{folder}, access{access} {}
		explicit InMemoryDataProvider (StringCRef folder) : baseFolder{folder}, access{Base::EMemoryAccess::CpuReadWrite} {}
	};


	//
	// Internet Data Provider Create Info
	//
	struct InternetDataProvider
	{
	// variables
		ModulePtr					manager;
		String						baseURL;
		Base::EMemoryAccess::bits	access;

	// methods
		InternetDataProvider () {}
		InternetDataProvider (StringCRef url, Base::EMemoryAccess::bits access) : baseURL{url}, access{access} {}
		explicit InternetDataProvider (StringCRef url) : baseURL{url}, access{Base::EMemoryAccess::CpuReadWrite} {}
	};


	//
	// Builtin Storage Data Provider Create Info
	//
	struct BuiltinStorageDataProvider
	{
	// types
		using LoadFunc_t		= void (*) (OUT BinaryArray &);
		using LoaderFunc_t		= LoadFunc_t (*) (StringCRef filename);;

	// variables
		ModulePtr			manager;
		LoaderFunc_t		func		= null;

	// methods
		explicit BuiltinStorageDataProvider (LoaderFunc_t func) : func{func} {}
	};


	//
	// Stream Create Info
	//
	struct InputStream
	{
	// variables
		ModulePtr		provider;
		String			uri;			// local file name or internet address or something else
		BytesU			bufferSize;		// (optional)
		
	// methods
		explicit InputStream (StringCRef uri, const ModulePtr &provider = null) : provider{provider}, uri{uri} {}
		explicit InputStream (StringCRef uri, BytesU bufSize, const ModulePtr &provider = null) : provider{provider}, uri{uri}, bufferSize{bufSize} {}
	};

	struct OutputStream
	{
	// variables
		ModulePtr		provider;
		String			uri;			// local file name or internet address or something else
		BytesU			bufferSize;		// (optional)
		
	// methods
		explicit OutputStream (StringCRef uri, const ModulePtr &provider = null) : provider{provider}, uri{uri} {}
		explicit OutputStream (StringCRef uri, BytesU bufSize, const ModulePtr &provider = null) : provider{provider}, uri{uri}, bufferSize{bufSize} {}
	};
	

	//
	// Stream Wrapper for Random Access Data Input Create Info
	//
	struct InputStreamFromDataInput
	{
	// variables
		ModulePtr		dataSource;

	// methods
		explicit InputStreamFromDataInput (const ModulePtr &ds) : dataSource{ds} {}
	};

	struct OutputStreamFromDataOutput
	{
	// variables
		ModulePtr		dataSource;

	// methods
		explicit OutputStreamFromDataOutput (const ModulePtr &ds) : dataSource{ds} {}
	};


	//
	// Random Access Data Input/Output Create Info
	//
	struct DataInput
	{
	// variables
		ModulePtr		provider;
		String			uri;			// local file name or internet address or something else
		
	// methods
		explicit DataInput (StringCRef uri, const ModulePtr &provider = null) : provider{provider}, uri{uri} {}
	};

	struct DataOutput
	{
	// variables
		ModulePtr		provider;
		String			uri;			// local file name or internet address or something else
		
	// methods
		explicit DataOutput (StringCRef uri, const ModulePtr &provider = null) : provider{provider}, uri{uri} {}
	};

}	// CreateInfo


namespace DSMsg
{
	using namespace Engine::_BaseMessages_;


	//
	// Get Data Source Description	// TODO: rename
	//
	struct GetDataSourceDescription : _MsgBase_
	{
		Out< Base::DataSourceDescription >		result;
	};


	//
	// Release Data
	//
	struct ReleaseData : _MsgBase_
	{
	};


	//
	// (Sequential) Read from CPU Visible Memory
	//
	struct ReadFromStream : _MsgBase_
	{
	// variables
		BytesU						offset;
		Editable< BinArrayRef >		writableBuffer;	// preallocated memory, 'result' may contains all or part of this buffer
		Out_opt< BinArrayCRef >		result;

	// methods
		ReadFromStream () {}
		ReadFromStream (BytesU off, BinArrayRef buf) : offset{off}, writableBuffer{buf} {}
		explicit ReadFromStream (BinArrayRef buf) : writableBuffer{buf} {}
	};


	//
	// (Sequential) Write to CPU Visible Memory
	//
	struct WriteToStream : _MsgBase_
	{
	// variables
		BinArrayCRef			data;
		Out_opt< BytesU >		wasWritten;

	// methods
		WriteToStream () {}
		explicit WriteToStream (BinArrayCRef data) : data(data) {}
		template <typename T>			explicit WriteToStream (ArrayCRef<T> arr) : data(BinArrayCRef::From(arr)) {}
		template <typename B, usize I>	explicit WriteToStream (const B (&arr)[I]) : data(BinArrayCRef::From(arr)) {}
	};


	//
	// (Random Access) Read from CPU Visible Memory
	//
	struct ReadMemRange : _MsgBase_
	{
	// variables
		BytesU						position;
		Editable< BinArrayRef >		writableBuffer;	// preallocated memory, 'result' may contains all or part of this buffer
		Out_opt< BinArrayCRef >		result;

	// methods
		ReadMemRange () {}
		ReadMemRange (BytesU pos, BinArrayRef buf) : position{pos}, writableBuffer{buf} {}
		explicit ReadMemRange (BinArrayRef buf) : writableBuffer{buf} {}
	};
	

	//
	// (Random Access) Write to CPU Visible Memory
	//
	struct WriteMemRange : _MsgBase_
	{
	// variables
		BytesU						position;
		BinArrayCRef				data;
		Out_opt< BytesU >			wasWritten;

	// methods
		WriteMemRange () {}
		explicit WriteMemRange (BytesU pos, BinArrayCRef data) : position{pos}, data(data) {}
		template <typename T>			explicit WriteMemRange (BytesU pos, ArrayCRef<T> arr) : position{pos}, data(BinArrayCRef::From(arr)) {}
		template <typename B, usize I>	explicit WriteMemRange (BytesU pos, const B (&arr)[I]) : position{pos}, data(BinArrayCRef::From(arr)) {}
	};


	//
	// (Random Access) Allocate memory for writing and return pointer
	//
	struct MapDataSource : _MsgBase_
	{
	// variables
		BytesU					position;
		BytesU					size;			// must be defined for resizable data source
		Out< BinArrayRef >		result;
		
	// methods
		MapDataSource (BytesU pos, BytesU size) : position{pos}, size{size} {}
	};


	//
	// Add / Remove On Data Modified Listener
	//
	struct AddOnDataModifiedListener : _MsgBase_
	{
	// types
		using Callback_t	= Delegate< void (StringCRef fname) >;
		
	// variables
		Callback_t		callback;
		String			filename;
		
	// methods
		template <typename Class>
		AddOnDataModifiedListener (Class *mod, void (Class::*cb) (StringCRef), StringCRef fname) :
			callback{DelegateBuilder( WeakPointerType<Class>(mod), cb )}, filename{fname} {}

		AddOnDataModifiedListener (void (*cb) (StringCRef), StringCRef fname) :
			callback{DelegateBuilder( cb )}, filename{fname} {}
	};

	// not needed if 'callback' delegate created from weak pointer
	struct RemoveOnDataModifiedListener : _MsgBase_
	{
	// variables
		ModuleWPtr		module;
		
	// methods
		explicit RemoveOnDataModifiedListener (const ModuleWPtr &mod) : module{mod} {}
	};


	//
	// Is URI Exists
	//
	struct IsUriExists : _MsgBase_
	{
	// variables
		StringCRef		uri;
		Out< bool >		result;

	// methods
		explicit IsUriExists (StringCRef uri) : uri{uri} {}
	};


	//
	// Create Data Input Module
	//
	struct CreateDataInputModule : _MsgBase_
	{
	// variables
		StringCRef			uri;
		Out< ModulePtr >	result;

	// methods
		explicit CreateDataInputModule (StringCRef uri) : uri{uri} {}
	};


	//
	// Create Data Output Module
	//
	struct CreateDataOutputModule : _MsgBase_
	{
	// variables
		StringCRef			uri;
		Out< ModulePtr >	result;

	// methods
		explicit CreateDataOutputModule (StringCRef uri) : uri{uri} {}
	};


	//
	// Get Data Provider For URI
	//
	struct GetDataProviderForURI : _MsgBase_
	{
	// variables
		StringCRef			uri;
		Out< ModulePtr >	result;
		
	// methods
		explicit GetDataProviderForURI (StringCRef uri) : uri{uri} {}
	};


}	// DSMsg
}	// Engine
