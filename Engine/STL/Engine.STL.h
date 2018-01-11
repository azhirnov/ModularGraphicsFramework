// Copyright (c)  Zhirnov Andrey. For more information see 'LICENSE.txt'
/*
	STL is my vision of C++ code and programming style for
	more readable, more safely code on C++.
*/

#pragma once


// Include main directives  //
#include "Common/Init.h"
#include "Common/Types.h"
#include "Common/Cast.h"


// Compile Time //
#include "CompileTime/NearType.h"
#include "CompileTime/StaticFloat.h"
#include "CompileTime/StaticFractional.h"
#include "CompileTime/StaticInteger.h"
#include "CompileTime/TemplateMath.h"
#include "CompileTime/TemplateUtils.h"
#include "CompileTime/TypeList.h"
#include "CompileTime/TypeTraits.h"
#include "CompileTime/StringToID.h"
#include "CompileTime/Sequence.h"
#include "CompileTime/Map.h"
#include "CompileTime/TypeInfoFunc.h"
//#include "CompileTime/FunctionInfo.h"


// Runtime //
#include "CompileTime/Runtime/TypeIdList.h"


// Memory //
#include "Memory/Allocators.h"
#include "Memory/MemoryContainer.h"
#include "Memory/MemoryViewer.h"
#include "Memory/MemFunc.h"
#include "Memory/MemPointer.h"
#include "Memory/PlacementNew.h"


// Containers //
#include "Containers/CopyStrategy.h"
#include "Containers/Array.h"
#include "Containers/CircularQueue.h"
#include "Containers/Stack.h"
#include "Containers/String.h"
#include "Containers/StaticArray.h"
#include "Containers/StaticBitArray.h"
#include "Containers/Queue.h"
#include "Containers/Set.h"
#include "Containers/Map.h"
#include "Containers/Pair.h"
#include "Containers/ArrayRef.h"
#include "Containers/StringRef.h"
#include "Containers/HashSet.h"
#include "Containers/HashMap.h"
#include "Containers/UniBuffer.h"
#include "Containers/IndexedArray.h"
#include "Containers/Adaptors.h"


// Algorithms/Crypt //
#include "Algorithms/Crypt/SimpleCrypt.h"


// Algorithms/Filters //
#include "Algorithms/Filters/GaussianFilter.h"


// Algorithms //
#include "Algorithms/ArrayUtils.h"
#include "Algorithms/Comparators.h"
#include "Algorithms/FileAddress.h"
#include "Algorithms/Enum.h"
#include "Algorithms/Hash.h"
#include "Algorithms/Sorts.h"
#include "Algorithms/StringParser.h"
#include "Algorithms/StringUtils.h"
#include "Algorithms/Swap.h"
#include "Algorithms/InvokeWithVariant.h"


// Types //
#include "Types/Ptr.h"
#include "Types/UniquePtr.h"
#include "Types/Function.h"
#include "Types/Delegate.h"
#include "Types/Optional.h"
#include "Types/Union.h"
#include "Types/VariantRef.h"
#include "Types/VariantCRef.h"
#include "Types/VariantInterface.h"
#include "Types/VariantRefIndexedContainer.h"
#include "Types/Noncopyable.h"
#include "Types/Noninstancable.h"
#include "Types/Limit.h"
#include "Types/StaticLimit.h"
#include "Types/RefCountedObject.h"
#include "Types/SharedPointer.h"
#include "Types/WeakPointer.h"
#include "Types/StaticRefCountedObject.h"
#include "Types/ScopeSetter.h"


// Dimensions //
#include "Dimensions/ByteAndBit.h"
#include "Dimensions/RadiansAndDegrees.h"
#include "Dimensions/PhysicsValue.h"
#include "Dimensions/PhysicsValueVec.h"
#include "Dimensions/PhysicsValueMath.h"
#include "Dimensions/Percentage.h"
#include "Dimensions/PowerOfTwoValue.h"


// ThreadSafe //
#include "ThreadSafe/MtQueue.h"
#include "ThreadSafe/Atomic.h"
#include "ThreadSafe/MtFile.h"
#include "ThreadSafe/Singleton.h"


// Math //
#include "Math/Mathematics.h"
#include "Math/MathTypes.h"
#include "Math/Vec.h"
#include "Math/Matrix.h"
#include "Math/Quaternion.h"
#include "Math/Algebra.h"
#include "Math/MathTypeCast.h"
#include "Math/Trigonometry.h"
#include "Math/BinaryMath.h"
#include "Math/Interpolations.h"


// Math/2D //
#include "Math/2D/Rectangle.h"
#include "Math/2D/Line2.h"
#include "Math/2D/Circle.h"
#include "Math/2D/OrientedRectangle.h"


// Math/3D //
#include "Math/3D/CoordTransform3.h"
#include "Math/3D/Line3.h"
#include "Math/3D/Transform.h"
#include "Math/3D/Triangle.h"
#include "Math/3D/PerspectiveCamera.h"
#include "Math/3D/AxisAlignedBox.h"
#include "Math/3D/Plane.h"
#include "Math/3D/Frustum.h"


// Math/Color //
#include "Math/Color/Color.h"
#include "Math/Color/ColorFormats.h"
#include "Math/Color/TR11G11B10F.h"
#include "Math/Color/TRGB9_E5.h"


// Math/Image //
#include "Math/Image/ImageUtils.h"


// Math/Rand //
#include "Math/Rand/Random.h"
#include "Math/Rand/RandomWithChance.h"
#include "Math/Rand/Pseudorandom.h"
#include "Math/Rand/NormalDistribution.h"


// Math/Spline //
#include "Math/Spline/Spline.h"


// OS //
#include "OS/OSLowLevel.h"


// Files //
#include "Files/BaseFile.h"
#include "Files/HDDFile.h"
#include "Files/MemFile.h"
#include "Files/SubFile.h"
#include "Files/CryptFile.h"
#include "Files/LzmaFile.h"
#include "Files/ZipFile.h"


// DataBase //
#include "DataBase/SimpleDB.h"


// Java //
#include "Java/JavaEnvironment.h"
#include "Java/JavaTypeInfo.h"
#include "Java/JavaArray.h"
#include "Java/JavaString.h"
#include "Java/JavaClass.h"
#include "Java/JavaMethod.h"
#include "Java/JavaMethodType.h"


// Log //
#include "Log/Logger.h"
#include "Log/ToString.h"


// Time //
#include "Time/Time.h"
#include "Time/TimeProfiler.h"


// ScriptBinding //
#include "ScriptBindings/DefaultBindings.h"
#include "ScriptBindings/ScriptBinder.h"
#include "ScriptBindings/ScriptTypes.h"
#include "ScriptBindings/ScriptEnum.h"
#include "ScriptBindings/ScriptEngine.h"


// Experimental //
#include "Experimental/Unroll.h"
#include "Experimental/FastMath.h"


// Debug //
#include "CompileTime/CompileTimeDebug.h"

#include "CompileTime/FunctionInfo.h"
