// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'

#include "CoreTests/STL/Common.h"
#include "Debug.h"

using namespace GX_STL;
using namespace GX_STL::GXTypes;


typedef TDebugInstCounter<1>			Elem_t;
typedef TDebugInstCounter<2>			VElem_t;


static void Deque_Test1 ()
{
	Elem_t::ClearStatistic();
	VElem_t::ClearStatistic();

	Deque<Elem_t>			pool;		pool.SetChunkSize( 10 );
	std::vector<VElem_t>	vec;


	pool.Resize( 12 );
	TEST( pool.Count() == 12 );

	vec.resize( 12 );
	TEST( vec.size() == 12 );

	for (usize i = 0; i < 12; ++i) {
		vec[i].value = pool[i].value;
	}
	TEST( (pool == ArrayRef<Elem_t>( Cast<Elem_t *>(&vec[0]), vec.size() )) );
	

	pool.Resize( 36 );
	TEST( pool.Count() == 36 );

	vec.resize( 36 );
	TEST( vec.size() == 36 );

	for (usize i = 12; i < 36; ++i) {
		vec[i].value = pool[i].value;
	}
	TEST( (pool == ArrayRef<Elem_t>( Cast<Elem_t *>(&vec[0]), vec.size() )) );
	

	pool.Resize( 6 );
	TEST( pool.Count() == 6 );

	vec.resize( 6 );
	TEST( vec.size() == 6 );

	TEST( (pool == ArrayRef<Elem_t>( Cast<Elem_t *>(&vec[0]), vec.size() )) );
}


static void Deque_Test2 ()
{
	Elem_t::ClearStatistic();
	VElem_t::ClearStatistic();

	Deque<Elem_t>			pool;		pool.SetChunkSize( 20 );
	std::vector<VElem_t>	vec;
	

	pool.Resize( 18 );
	vec.resize( 18 );

	for (usize i = 0; i < 18; ++i) {
		vec[i].value = pool[i].value;
	}
	TEST( (pool == ArrayRef<Elem_t>( Cast<Elem_t *>(&vec[0]), vec.size() )) );
	TEST( pool.Capacity() == 20 );


	// without creating new chunk
	pool.PushFront( Elem_t() );
	vec.insert( vec.begin(), VElem_t() );
	TEST( (pool == ArrayRef<Elem_t>( Cast<Elem_t *>(&vec[0]), vec.size() )) );

	pool.PushBack( Elem_t() );
	vec.push_back( VElem_t() );
	TEST( (pool == ArrayRef<Elem_t>( Cast<Elem_t *>(&vec[0]), vec.size() )) );

	TEST( pool.Capacity() == 20 );
	

	// create new chunk
	pool.PushFront( Elem_t() );
	vec.insert( vec.begin(), VElem_t() );
	TEST( (pool == ArrayRef<Elem_t>( Cast<Elem_t *>(&vec[0]), vec.size() )) );
	
	pool.PushBack( Elem_t() );
	vec.push_back( VElem_t() );
	TEST( (pool == ArrayRef<Elem_t>( Cast<Elem_t *>(&vec[0]), vec.size() )) );
	
	TEST( pool.Capacity() == 60 );
}


static void Deque_Test3 ()
{
	Elem_t::ClearStatistic();
	VElem_t::ClearStatistic();

	Deque<Elem_t>			pool;		pool.SetChunkSize( 20 );
	std::vector<VElem_t>	vec;

	
	pool.Resize( 18 );
	vec.resize( 18 );

	for (usize i = 0; i < 18; ++i) {
		vec[i].value = pool[i].value;
	}
	TEST( (pool == ArrayRef<Elem_t>( Cast<Elem_t *>(&vec[0]), vec.size() )) );
	

	Array<Elem_t>			arr1;		arr1.Resize( 5 );
	std::vector<VElem_t>	vec1;		vec1.resize( 5 );
	pool.AppendBack( arr1 );
	vec.insert( vec.end(), vec1.begin(), vec1.end() );
	TEST( (pool == ArrayRef<Elem_t>( Cast<Elem_t *>(&vec[0]), vec.size() )) );
	

	Deque<Elem_t>			arr2;		arr2.SetChunkSize( 30 );	arr2.Resize( 45 );
	std::vector<VElem_t>	vec2;		vec2.resize( 45 );
	pool.AppendBack( arr2 );
	vec.insert( vec.end(), vec2.begin(), vec2.end() );
	TEST( (pool == ArrayRef<Elem_t>( Cast<Elem_t *>(&vec[0]), vec.size() )) );
}


static void Deque_Test4 ()
{
	Elem_t::ClearStatistic();
	VElem_t::ClearStatistic();

	Deque<Elem_t>			pool;		pool.SetChunkSize( 20 );
	std::vector<VElem_t>	vec;

	
	pool.Resize( 18 );
	vec.resize( 18 );

	for (usize i = 0; i < 18; ++i) {
		vec[i].value = pool[i].value;
	}
	TEST( (pool == ArrayRef<Elem_t>( Cast<Elem_t *>(&vec[0]), vec.size() )) );
	

	Array<Elem_t>			arr1;		arr1.Resize( 5 );
	std::vector<VElem_t>	vec1;		vec1.resize( 5 );
	pool.AppendFront( arr1 );
	vec.insert( vec.begin(), vec1.begin(), vec1.end() );
	TEST( (pool == ArrayRef<Elem_t>( Cast<Elem_t *>(&vec[0]), vec.size() )) );
	

	Deque<Elem_t>			arr2;		arr2.SetChunkSize( 30 );	arr2.Resize( 45 );
	std::vector<VElem_t>	vec2;		vec2.resize( 45 );
	pool.AppendFront( arr2 );
	vec.insert( vec.begin(), vec2.begin(), vec2.end() );
	TEST( (pool == ArrayRef<Elem_t>( Cast<Elem_t *>(&vec[0]), vec.size() )) );
}


static void Deque_Test5 ()
{
	Elem_t::ClearStatistic();
	VElem_t::ClearStatistic();

	Deque<Elem_t>			pool;		pool.SetChunkSize( 20 );
	std::vector<VElem_t>	vec;
	
	pool.Resize( 58 );
	vec.resize( 58 );

	auto iter = vec.begin();

	for (auto& item : pool)
	{
		TEST( item.value == iter->value );
		++iter;
	}
	TEST( iter == vec.end() );


	pool.EraseFromFront( 2 );
	TEST( pool.Count() == 56 );

	vec.erase( vec.begin(), vec.begin()+2 );
	TEST( vec.size() == 56 );

	TEST( (pool == ArrayRef<Elem_t>( Cast<Elem_t *>(&vec[0]), vec.size() )) );
	

	pool.EraseFromBack( 2 );
	TEST( pool.Count() == 54 );

	vec.erase( vec.end()-2, vec.end() );
	TEST( vec.size() == 54 );

	TEST( (pool == ArrayRef<Elem_t>( Cast<Elem_t *>(&vec[0]), vec.size() )) );


	pool.Erase( 33, 2 );
	TEST( pool.Count() == 52 );

	vec.erase( vec.begin()+33, vec.begin()+33+2 );
	TEST( vec.size() == 52 );

	TEST( (pool == ArrayRef<Elem_t>( Cast<Elem_t *>(&vec[0]), vec.size() )) );
}


extern void Test_Containers_Pool ()
{
	Deque_Test1();
	TEST( Elem_t::CheckStatistic() );
	TEST( VElem_t::CheckStatistic() );
	
	Deque_Test2();
	TEST( Elem_t::CheckStatistic() );
	TEST( VElem_t::CheckStatistic() );
	
	Deque_Test3();
	TEST( Elem_t::CheckStatistic() );
	TEST( VElem_t::CheckStatistic() );
	
	Deque_Test4();
	TEST( Elem_t::CheckStatistic() );
	TEST( VElem_t::CheckStatistic() );
	
	Deque_Test5();
	TEST( Elem_t::CheckStatistic() );
	TEST( VElem_t::CheckStatistic() );
}
