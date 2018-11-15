# auto generated file
cmake_minimum_required (VERSION 3.6.0)

set( Core_VERSION_MAJOR 1 )
set( Core_VERSION_MINOR 0 )

#==================================================================================================
# project: Core.Config
#==================================================================================================
set( SOURCES 
	"Config/Engine.Config.h"
	"Config/Engine.Version.h"
	"Config/Main.cpp"
	"Config/STL.Config.h" )
add_library( "Core.Config" STATIC ${SOURCES} )
source_group( "" FILES "Config/Engine.Config.h" "Config/Engine.Version.h" "Config/Main.cpp" "Config/STL.Config.h" )
set_property( TARGET "Core.Config" PROPERTY FOLDER "Core" )
target_include_directories( "Core.Config" PUBLIC "../External" )
target_include_directories( "Core.Config" PUBLIC "${EXTERNALS_PATH}" )
target_include_directories( "Core.Config" PUBLIC ".." )
# compiler
target_compile_options( "Core.Config" PRIVATE $<$<CONFIG:DebugAnalyze>: ${PROJECTS_SHARED_CXX_FLAGS_DEBUGANALYZE}> )
target_compile_definitions( "Core.Config" PRIVATE $<$<CONFIG:Debug>: ${PROJECTS_SHARED_DEFINES_DEBUG}> )
target_compile_definitions( "Core.Config" PRIVATE $<$<CONFIG:DebugAnalyze>: ${PROJECTS_SHARED_DEFINES_DEBUGANALYZE}> )
set_target_properties( "Core.Config" PROPERTIES LINK_FLAGS_RELEASE ${PROJECTS_SHARED_LINKER_FLAGS_RELEASE} )
target_compile_definitions( "Core.Config" PRIVATE $<$<CONFIG:Profile>: ${PROJECTS_SHARED_DEFINES_PROFILE}> )
target_compile_options( "Core.Config" PRIVATE $<$<CONFIG:Release>: ${PROJECTS_SHARED_CXX_FLAGS_RELEASE}> )
set_target_properties( "Core.Config" PROPERTIES LINK_FLAGS_DEBUG ${PROJECTS_SHARED_LINKER_FLAGS_DEBUG} )
set_target_properties( "Core.Config" PROPERTIES LINK_FLAGS_DEBUGANALYZE ${PROJECTS_SHARED_LINKER_FLAGS_DEBUGANALYZE} )
target_compile_options( "Core.Config" PRIVATE $<$<CONFIG:Profile>: ${PROJECTS_SHARED_CXX_FLAGS_PROFILE}> )
set_target_properties( "Core.Config" PROPERTIES LINK_FLAGS_PROFILE ${PROJECTS_SHARED_LINKER_FLAGS_PROFILE} )
target_compile_options( "Core.Config" PRIVATE $<$<CONFIG:Debug>: ${PROJECTS_SHARED_CXX_FLAGS_DEBUG}> )
target_compile_definitions( "Core.Config" PRIVATE $<$<CONFIG:Release>: ${PROJECTS_SHARED_DEFINES_RELEASE}> )



#==================================================================================================
# project: Core.STL
#==================================================================================================
set( SOURCES 
	"STL/CompileTime/CompileTimeChecks.h"
	"STL/CompileTime/CompileTimeDebug.h"
	"STL/CompileTime/CompileTimeTypes.h"
	"STL/CompileTime/DeferredTemplates.h"
	"STL/CompileTime/FunctionInfo.h"
	"STL/CompileTime/Map.h"
	"STL/CompileTime/NearType.h"
	"STL/CompileTime/NewTypeInfo.h"
	"STL/CompileTime/OldTypeInfo.h"
	"STL/CompileTime/PlainOldDataType.h"
	"STL/CompileTime/Sequence.h"
	"STL/CompileTime/StaticFloat.h"
	"STL/CompileTime/StaticFractional.h"
	"STL/CompileTime/StaticInteger.h"
	"STL/CompileTime/StringToID.h"
	"STL/CompileTime/TemplateMath.h"
	"STL/CompileTime/TemplateUtils.h"
	"STL/CompileTime/TypeInfoFunc.h"
	"STL/CompileTime/TypeList.h"
	"STL/CompileTime/TypeListHelpers.h"
	"STL/CompileTime/TypeListUtils.h"
	"STL/CompileTime/TypeQualifier.h"
	"STL/CompileTime/TypeTraits.h"
	"STL/Types/Bitfield.h"
	"STL/Types/Bool.h"
	"STL/Types/DefferedType.h"
	"STL/Types/Delegate.h"
	"STL/Types/Function.h"
	"STL/Types/Limit.h"
	"STL/Types/Noncopyable.h"
	"STL/Types/Noninstancable.h"
	"STL/Types/Optional.h"
	"STL/Types/Ptr.h"
	"STL/Types/RefCountedObject.h"
	"STL/Types/RefCounter.h"
	"STL/Types/ScopeSetter.h"
	"STL/Types/SharedPointer.h"
	"STL/Types/StaticLimit.h"
	"STL/Types/StaticRefCountedObject.h"
	"STL/Types/Union.h"
	"STL/Types/UniquePtr.h"
	"STL/Types/UntypedKey.h"
	"STL/Types/VariantCRef.h"
	"STL/Types/VariantInterface.h"
	"STL/Types/VariantRef.h"
	"STL/Types/VariantRefIndexedContainer.h"
	"STL/Types/WeakPointer.h"
	"STL/Common/Compilers/CompilerClang.h"
	"STL/Common/Compilers/CompilerGCC.h"
	"STL/Common/Compilers/CompilerMSVC.h"
	"STL/OS/OSLowLevel.h"
	"STL/OS/Android/OSAndroid.h"
	"STL/Math/3D/AxisAlignedBox.h"
	"STL/Math/3D/CoordTransform3.h"
	"STL/Math/3D/Frustum.h"
	"STL/Math/3D/Line3.h"
	"STL/Math/3D/MathTypes3D.h"
	"STL/Math/3D/PerspectiveCamera.h"
	"STL/Math/3D/Plane.h"
	"STL/Math/3D/Transform.h"
	"STL/Math/3D/Triangle.h"
	"STL/Math/2D/Circle.h"
	"STL/Math/2D/Line2.h"
	"STL/Math/2D/MathTypes2D.h"
	"STL/Math/2D/OrientedRectangle.h"
	"STL/Math/2D/Rectangle.h"
	"STL/Math/Spline/Spline.h"
	"STL/Math/Algebra.h"
	"STL/Math/BinaryMath.h"
	"STL/Math/FastMath.h"
	"STL/Math/Interpolations.h"
	"STL/Math/MathConstants.h"
	"STL/Math/Mathematics.h"
	"STL/Math/MathFunc.h"
	"STL/Math/MathTypeCast.h"
	"STL/Math/MathTypes.h"
	"STL/Math/Matrix.h"
	"STL/Math/Matrix2.h"
	"STL/Math/Matrix3.h"
	"STL/Math/Matrix4.h"
	"STL/Math/MatrixCR.h"
	"STL/Math/MatrixUtils.h"
	"STL/Math/OverflowCheck.h"
	"STL/Math/Quaternion.h"
	"STL/Math/Trigonometry.h"
	"STL/Math/Vec.h"
	"STL/Math/VecI.h"
	"STL/OS/Windows/OSWindows.h"
	"STL/OS/Windows/WinFileSystem.cpp"
	"STL/OS/Windows/WinFileSystem.h"
	"STL/OS/Windows/WinHeader.h"
	"STL/OS/Windows/WinLibrary.cpp"
	"STL/OS/Windows/WinLibrary.h"
	"STL/OS/Windows/WinPlatformUtils.cpp"
	"STL/OS/Windows/WinPlatformUtils.h"
	"STL/OS/Windows/WinRandDevice.cpp"
	"STL/OS/Windows/WinRandDevice.h"
	"STL/OS/Windows/WinSyncPrimitives.cpp"
	"STL/OS/Windows/WinSyncPrimitives.h"
	"STL/OS/Windows/WinThread.cpp"
	"STL/OS/Windows/WinThread.h"
	"STL/OS/Windows/WinTimer.cpp"
	"STL/OS/Windows/WinTimer.h"
	"STL/Time/FloatTimeImpl.h"
	"STL/Time/IntTimeImpl.h"
	"STL/Time/Time.h"
	"STL/Time/TimeProfiler.h"
	"STL/Defines/AuxiliaryDefines.h"
	"STL/Defines/CtorHelpers.h"
	"STL/Defines/Defines.h"
	"STL/Defines/EnumHelpers.h"
	"STL/Defines/Errors.h"
	"STL/Defines/MemberDetector.h"
	"STL/Defines/OperatorHelpers.h"
	"STL/Defines/PublicMacro.h"
	"STL/OS/Posix/OSPosix.h"
	"STL/OS/Posix/PosixFileSystem.cpp"
	"STL/OS/Posix/PosixFileSystem.h"
	"STL/OS/Posix/PosixHeader.h"
	"STL/OS/Posix/PosixLibrary.cpp"
	"STL/OS/Posix/PosixLibrary.h"
	"STL/OS/Posix/PosixPlatformUtils.h"
	"STL/OS/Posix/PosixRandDevice.cpp"
	"STL/OS/Posix/PosixRandDevice.h"
	"STL/OS/Posix/PosixSyncPrimitives.cpp"
	"STL/OS/Posix/PosixSyncPrimitives.h"
	"STL/OS/Posix/PosixThread.cpp"
	"STL/OS/Posix/PosixThread.h"
	"STL/OS/Posix/PosixTimer.cpp"
	"STL/OS/Posix/PosixTimer.h"
	"STL/Common/AllFunc.h"
	"STL/Common/Cast.h"
	"STL/Common/Init.h"
	"STL/Common/Main.cpp"
	"STL/Common/Platforms.h"
	"STL/Common/TypeId.h"
	"STL/Common/Types.h"
	"STL/Common/UMax.h"
	"STL/Common/Uninitialized.h"
	"STL/Containers/Adaptors.h"
	"STL/Containers/AppendableAdaptor.h"
	"STL/Containers/Array.h"
	"STL/Containers/ArrayRef.h"
	"STL/Containers/CircularQueue.h"
	"STL/Containers/CopyStrategy.h"
	"STL/Containers/Deque.h"
	"STL/Containers/ErasableAdaptor.h"
	"STL/Containers/HashMap.h"
	"STL/Containers/HashSet.h"
	"STL/Containers/IndexedArray.h"
	"STL/Containers/IndexedIterator.h"
	"STL/Containers/Map.h"
	"STL/Containers/MapUtils.h"
	"STL/Containers/Pair.h"
	"STL/Containers/Queue.h"
	"STL/Containers/Set.h"
	"STL/Containers/Stack.h"
	"STL/Containers/StaticArray.h"
	"STL/Containers/StaticBitArray.h"
	"STL/Containers/String.h"
	"STL/Containers/StringRef.h"
	"STL/Containers/Tuple.h"
	"STL/Containers/UniBuffer.h"
	"STL/Compression/Compression.h"
	"STL/Compression/LZ4Compression.h"
	"STL/Compression/MiniZCompression.h"
	"STL/Math/Image/ImageUtils.h"
	"STL/Algorithms/ArrayUtils.h"
	"STL/Algorithms/Comparators.h"
	"STL/Algorithms/Enum.h"
	"STL/Algorithms/FileAddress.cpp"
	"STL/Algorithms/FileAddress.h"
	"STL/Algorithms/Hash.h"
	"STL/Algorithms/InvokeWithVariant.h"
	"STL/Algorithms/Range.h"
	"STL/Algorithms/Sorts.h"
	"STL/Algorithms/StringParser.cpp"
	"STL/Algorithms/StringParser.h"
	"STL/Algorithms/StringUtils.h"
	"STL/Algorithms/Swap.h"
	"STL/Log/ELog.h"
	"STL/Log/Logger.cpp"
	"STL/Log/Logger.h"
	"STL/Log/ToString.h"
	"STL/OS/SDL/OS_SDL.h"
	"STL/OS/SDL/SDLFileSystem.h"
	"STL/OS/SDL/SDLLibrary.cpp"
	"STL/OS/SDL/SDLLibrary.h"
	"STL/OS/SDL/SDLPlatformUtils.cpp"
	"STL/OS/SDL/SDLPlatformUtils.h"
	"STL/OS/SDL/SDLRandDevice.h"
	"STL/OS/SDL/SDLSyncPrimitives.cpp"
	"STL/OS/SDL/SDLSyncPrimitives.h"
	"STL/OS/SDL/SDLThread.cpp"
	"STL/OS/SDL/SDLThread.h"
	"STL/OS/SDL/SDLTimer.h"
	"STL/OS/STD/STDSyncPrimitives.h"
	"STL/OS/STD/STDThread.cpp"
	"STL/OS/STD/STDThread.h"
	"STL/OS/STD/STDTimer.h"
	"STL/Algorithms/Crypt/SimpleCrypt.h"
	"STL/Math/Color/Color.h"
	"STL/Math/Color/ColorFormats.h"
	"STL/Math/Color/Half.h"
	"STL/Math/Color/TR11G11B10F.h"
	"STL/Math/Color/TRGB9_E5.h"
	"STL/Dimensions/ByteAndBit.h"
	"STL/Dimensions/Percentage.h"
	"STL/Dimensions/PowerOfTwoValue.h"
	"STL/Dimensions/RadiansAndDegrees.h"
	"STL/DataBase/SimpleDB.h"
	"STL/DataBase/Utf8StringUtils.cpp"
	"STL/DataBase/Utf8StringUtils.h"
	"STL/Memory/Allocators.h"
	"STL/Memory/MemFunc.h"
	"STL/Memory/MemoryContainer.h"
	"STL/Memory/MemoryViewer.h"
	"STL/Memory/PlacementNew.h"
	"STL/CompileTime/Runtime/TypeIdList.h"
	"STL/Core.STL.h"
	"STL/Algorithms/Filters/GaussianFilter.h"
	"STL/Math/Rand/NormalDistribution.h"
	"STL/Math/Rand/Pseudorandom.h"
	"STL/Math/Rand/RandEngine.h"
	"STL/Math/Rand/Random.h"
	"STL/Math/Rand/RandomWithChance.h"
	"STL/OS/Base/BaseFileSystem.cpp"
	"STL/OS/Base/BaseFileSystem.h"
	"STL/OS/Base/Common.h"
	"STL/OS/Base/ConditionVariableEmulation.h"
	"STL/OS/Base/Date.cpp"
	"STL/OS/Base/Date.h"
	"STL/OS/Base/Endianes.h"
	"STL/OS/Base/ReadWriteSyncEmulation.h"
	"STL/OS/Base/ScopeLock.h"
	"STL/OS/Base/SemaphoreEmulator.h"
	"STL/OS/Base/SyncEventEmulation.h"
	"STL/ThreadSafe/Atomic.h"
	"STL/ThreadSafe/AtomicBitfield.h"
	"STL/ThreadSafe/AtomicCounter.h"
	"STL/ThreadSafe/AtomicFlag.h"
	"STL/ThreadSafe/MtFile.h"
	"STL/ThreadSafe/MtQueue.h"
	"STL/ThreadSafe/Singleton.h"
	"STL/Files/BaseFile.h"
	"STL/Files/CryptFile.h"
	"STL/Files/HDDFile.h"
	"STL/Files/LzmaFile.h"
	"STL/Files/MemFile.h"
	"STL/Files/SubFile.h"
	"STL/Files/ZipFile.h" )
add_library( "Core.STL" STATIC ${SOURCES} )
source_group( "CompileTime" FILES "STL/CompileTime/CompileTimeChecks.h" "STL/CompileTime/CompileTimeDebug.h" "STL/CompileTime/CompileTimeTypes.h" "STL/CompileTime/DeferredTemplates.h" "STL/CompileTime/FunctionInfo.h" "STL/CompileTime/Map.h" "STL/CompileTime/NearType.h" "STL/CompileTime/NewTypeInfo.h" "STL/CompileTime/OldTypeInfo.h" "STL/CompileTime/PlainOldDataType.h" "STL/CompileTime/Sequence.h" "STL/CompileTime/StaticFloat.h" "STL/CompileTime/StaticFractional.h" "STL/CompileTime/StaticInteger.h" "STL/CompileTime/StringToID.h" "STL/CompileTime/TemplateMath.h" "STL/CompileTime/TemplateUtils.h" "STL/CompileTime/TypeInfoFunc.h" "STL/CompileTime/TypeList.h" "STL/CompileTime/TypeListHelpers.h" "STL/CompileTime/TypeListUtils.h" "STL/CompileTime/TypeQualifier.h" "STL/CompileTime/TypeTraits.h" )
source_group( "Types" FILES "STL/Types/Bitfield.h" "STL/Types/Bool.h" "STL/Types/DefferedType.h" "STL/Types/Delegate.h" "STL/Types/Function.h" "STL/Types/Limit.h" "STL/Types/Noncopyable.h" "STL/Types/Noninstancable.h" "STL/Types/Optional.h" "STL/Types/Ptr.h" "STL/Types/RefCountedObject.h" "STL/Types/RefCounter.h" "STL/Types/ScopeSetter.h" "STL/Types/SharedPointer.h" "STL/Types/StaticLimit.h" "STL/Types/StaticRefCountedObject.h" "STL/Types/Union.h" "STL/Types/UniquePtr.h" "STL/Types/UntypedKey.h" "STL/Types/VariantCRef.h" "STL/Types/VariantInterface.h" "STL/Types/VariantRef.h" "STL/Types/VariantRefIndexedContainer.h" "STL/Types/WeakPointer.h" )
source_group( "Common\\Compilers" FILES "STL/Common/Compilers/CompilerClang.h" "STL/Common/Compilers/CompilerGCC.h" "STL/Common/Compilers/CompilerMSVC.h" )
source_group( "OS" FILES "STL/OS/OSLowLevel.h" )
source_group( "OS\\Android" FILES "STL/OS/Android/OSAndroid.h" )
source_group( "Math\\3D" FILES "STL/Math/3D/AxisAlignedBox.h" "STL/Math/3D/CoordTransform3.h" "STL/Math/3D/Frustum.h" "STL/Math/3D/Line3.h" "STL/Math/3D/MathTypes3D.h" "STL/Math/3D/PerspectiveCamera.h" "STL/Math/3D/Plane.h" "STL/Math/3D/Transform.h" "STL/Math/3D/Triangle.h" )
source_group( "Math\\2D" FILES "STL/Math/2D/Circle.h" "STL/Math/2D/Line2.h" "STL/Math/2D/MathTypes2D.h" "STL/Math/2D/OrientedRectangle.h" "STL/Math/2D/Rectangle.h" )
source_group( "Math\\Spline" FILES "STL/Math/Spline/Spline.h" )
source_group( "Math" FILES "STL/Math/Algebra.h" "STL/Math/BinaryMath.h" "STL/Math/FastMath.h" "STL/Math/Interpolations.h" "STL/Math/MathConstants.h" "STL/Math/Mathematics.h" "STL/Math/MathFunc.h" "STL/Math/MathTypeCast.h" "STL/Math/MathTypes.h" "STL/Math/Matrix.h" "STL/Math/Matrix2.h" "STL/Math/Matrix3.h" "STL/Math/Matrix4.h" "STL/Math/MatrixCR.h" "STL/Math/MatrixUtils.h" "STL/Math/OverflowCheck.h" "STL/Math/Quaternion.h" "STL/Math/Trigonometry.h" "STL/Math/Vec.h" "STL/Math/VecI.h" )
source_group( "OS\\Windows" FILES "STL/OS/Windows/OSWindows.h" "STL/OS/Windows/WinFileSystem.cpp" "STL/OS/Windows/WinFileSystem.h" "STL/OS/Windows/WinHeader.h" "STL/OS/Windows/WinLibrary.cpp" "STL/OS/Windows/WinLibrary.h" "STL/OS/Windows/WinPlatformUtils.cpp" "STL/OS/Windows/WinPlatformUtils.h" "STL/OS/Windows/WinRandDevice.cpp" "STL/OS/Windows/WinRandDevice.h" "STL/OS/Windows/WinSyncPrimitives.cpp" "STL/OS/Windows/WinSyncPrimitives.h" "STL/OS/Windows/WinThread.cpp" "STL/OS/Windows/WinThread.h" "STL/OS/Windows/WinTimer.cpp" "STL/OS/Windows/WinTimer.h" )
source_group( "Time" FILES "STL/Time/FloatTimeImpl.h" "STL/Time/IntTimeImpl.h" "STL/Time/Time.h" "STL/Time/TimeProfiler.h" )
source_group( "Defines" FILES "STL/Defines/AuxiliaryDefines.h" "STL/Defines/CtorHelpers.h" "STL/Defines/Defines.h" "STL/Defines/EnumHelpers.h" "STL/Defines/Errors.h" "STL/Defines/MemberDetector.h" "STL/Defines/OperatorHelpers.h" "STL/Defines/PublicMacro.h" )
source_group( "OS\\Posix" FILES "STL/OS/Posix/OSPosix.h" "STL/OS/Posix/PosixFileSystem.cpp" "STL/OS/Posix/PosixFileSystem.h" "STL/OS/Posix/PosixHeader.h" "STL/OS/Posix/PosixLibrary.cpp" "STL/OS/Posix/PosixLibrary.h" "STL/OS/Posix/PosixPlatformUtils.h" "STL/OS/Posix/PosixRandDevice.cpp" "STL/OS/Posix/PosixRandDevice.h" "STL/OS/Posix/PosixSyncPrimitives.cpp" "STL/OS/Posix/PosixSyncPrimitives.h" "STL/OS/Posix/PosixThread.cpp" "STL/OS/Posix/PosixThread.h" "STL/OS/Posix/PosixTimer.cpp" "STL/OS/Posix/PosixTimer.h" )
source_group( "Common" FILES "STL/Common/AllFunc.h" "STL/Common/Cast.h" "STL/Common/Init.h" "STL/Common/Main.cpp" "STL/Common/Platforms.h" "STL/Common/TypeId.h" "STL/Common/Types.h" "STL/Common/UMax.h" "STL/Common/Uninitialized.h" )
source_group( "Containers" FILES "STL/Containers/Adaptors.h" "STL/Containers/AppendableAdaptor.h" "STL/Containers/Array.h" "STL/Containers/ArrayRef.h" "STL/Containers/CircularQueue.h" "STL/Containers/CopyStrategy.h" "STL/Containers/Deque.h" "STL/Containers/ErasableAdaptor.h" "STL/Containers/HashMap.h" "STL/Containers/HashSet.h" "STL/Containers/IndexedArray.h" "STL/Containers/IndexedIterator.h" "STL/Containers/Map.h" "STL/Containers/MapUtils.h" "STL/Containers/Pair.h" "STL/Containers/Queue.h" "STL/Containers/Set.h" "STL/Containers/Stack.h" "STL/Containers/StaticArray.h" "STL/Containers/StaticBitArray.h" "STL/Containers/String.h" "STL/Containers/StringRef.h" "STL/Containers/Tuple.h" "STL/Containers/UniBuffer.h" )
source_group( "Compression" FILES "STL/Compression/Compression.h" "STL/Compression/LZ4Compression.h" "STL/Compression/MiniZCompression.h" )
source_group( "Math\\Image" FILES "STL/Math/Image/ImageUtils.h" )
source_group( "Algorithms" FILES "STL/Algorithms/ArrayUtils.h" "STL/Algorithms/Comparators.h" "STL/Algorithms/Enum.h" "STL/Algorithms/FileAddress.cpp" "STL/Algorithms/FileAddress.h" "STL/Algorithms/Hash.h" "STL/Algorithms/InvokeWithVariant.h" "STL/Algorithms/Range.h" "STL/Algorithms/Sorts.h" "STL/Algorithms/StringParser.cpp" "STL/Algorithms/StringParser.h" "STL/Algorithms/StringUtils.h" "STL/Algorithms/Swap.h" )
source_group( "Log" FILES "STL/Log/ELog.h" "STL/Log/Logger.cpp" "STL/Log/Logger.h" "STL/Log/ToString.h" )
source_group( "OS\\SDL" FILES "STL/OS/SDL/OS_SDL.h" "STL/OS/SDL/SDLFileSystem.h" "STL/OS/SDL/SDLLibrary.cpp" "STL/OS/SDL/SDLLibrary.h" "STL/OS/SDL/SDLPlatformUtils.cpp" "STL/OS/SDL/SDLPlatformUtils.h" "STL/OS/SDL/SDLRandDevice.h" "STL/OS/SDL/SDLSyncPrimitives.cpp" "STL/OS/SDL/SDLSyncPrimitives.h" "STL/OS/SDL/SDLThread.cpp" "STL/OS/SDL/SDLThread.h" "STL/OS/SDL/SDLTimer.h" )
source_group( "OS\\STD" FILES "STL/OS/STD/STDSyncPrimitives.h" "STL/OS/STD/STDThread.cpp" "STL/OS/STD/STDThread.h" "STL/OS/STD/STDTimer.h" )
source_group( "Algorithms\\Crypt" FILES "STL/Algorithms/Crypt/SimpleCrypt.h" )
source_group( "Math\\Color" FILES "STL/Math/Color/Color.h" "STL/Math/Color/ColorFormats.h" "STL/Math/Color/Half.h" "STL/Math/Color/TR11G11B10F.h" "STL/Math/Color/TRGB9_E5.h" )
source_group( "Dimensions" FILES "STL/Dimensions/ByteAndBit.h" "STL/Dimensions/Percentage.h" "STL/Dimensions/PowerOfTwoValue.h" "STL/Dimensions/RadiansAndDegrees.h" )
source_group( "DataBase" FILES "STL/DataBase/SimpleDB.h" "STL/DataBase/Utf8StringUtils.cpp" "STL/DataBase/Utf8StringUtils.h" )
source_group( "Memory" FILES "STL/Memory/Allocators.h" "STL/Memory/MemFunc.h" "STL/Memory/MemoryContainer.h" "STL/Memory/MemoryViewer.h" "STL/Memory/PlacementNew.h" )
source_group( "CompileTime\\Runtime" FILES "STL/CompileTime/Runtime/TypeIdList.h" )
source_group( "" FILES "STL/Core.STL.h" )
source_group( "Algorithms\\Filters" FILES "STL/Algorithms/Filters/GaussianFilter.h" )
source_group( "Math\\Rand" FILES "STL/Math/Rand/NormalDistribution.h" "STL/Math/Rand/Pseudorandom.h" "STL/Math/Rand/RandEngine.h" "STL/Math/Rand/Random.h" "STL/Math/Rand/RandomWithChance.h" )
source_group( "OS\\Base" FILES "STL/OS/Base/BaseFileSystem.cpp" "STL/OS/Base/BaseFileSystem.h" "STL/OS/Base/Common.h" "STL/OS/Base/ConditionVariableEmulation.h" "STL/OS/Base/Date.cpp" "STL/OS/Base/Date.h" "STL/OS/Base/Endianes.h" "STL/OS/Base/ReadWriteSyncEmulation.h" "STL/OS/Base/ScopeLock.h" "STL/OS/Base/SemaphoreEmulator.h" "STL/OS/Base/SyncEventEmulation.h" )
source_group( "ThreadSafe" FILES "STL/ThreadSafe/Atomic.h" "STL/ThreadSafe/AtomicBitfield.h" "STL/ThreadSafe/AtomicCounter.h" "STL/ThreadSafe/AtomicFlag.h" "STL/ThreadSafe/MtFile.h" "STL/ThreadSafe/MtQueue.h" "STL/ThreadSafe/Singleton.h" )
source_group( "Files" FILES "STL/Files/BaseFile.h" "STL/Files/CryptFile.h" "STL/Files/HDDFile.h" "STL/Files/LzmaFile.h" "STL/Files/MemFile.h" "STL/Files/SubFile.h" "STL/Files/ZipFile.h" )
set_property( TARGET "Core.STL" PROPERTY FOLDER "Core" )
target_include_directories( "Core.STL" PUBLIC "../External" )
target_include_directories( "Core.STL" PUBLIC "${EXTERNALS_PATH}" )
target_include_directories( "Core.STL" PUBLIC ".." )
if (ENABLE_MINIZ)
	target_link_libraries( "Core.STL" "MiniZ" )
endif()
if (ENABLE_SDL)
	target_link_libraries( "Core.STL" "SDL2" )
endif()
if (UNIX)
	target_link_libraries( "Core.STL" "dl;pthread" )
endif()
# compiler
target_compile_options( "Core.STL" PRIVATE $<$<CONFIG:DebugAnalyze>: ${PROJECTS_SHARED_CXX_FLAGS_DEBUGANALYZE}> )
target_compile_definitions( "Core.STL" PRIVATE $<$<CONFIG:Debug>: ${PROJECTS_SHARED_DEFINES_DEBUG}> )
target_compile_definitions( "Core.STL" PRIVATE $<$<CONFIG:DebugAnalyze>: ${PROJECTS_SHARED_DEFINES_DEBUGANALYZE}> )
set_target_properties( "Core.STL" PROPERTIES LINK_FLAGS_RELEASE ${PROJECTS_SHARED_LINKER_FLAGS_RELEASE} )
target_compile_definitions( "Core.STL" PRIVATE $<$<CONFIG:Profile>: ${PROJECTS_SHARED_DEFINES_PROFILE}> )
target_compile_options( "Core.STL" PRIVATE $<$<CONFIG:Release>: ${PROJECTS_SHARED_CXX_FLAGS_RELEASE}> )
set_target_properties( "Core.STL" PROPERTIES LINK_FLAGS_DEBUG ${PROJECTS_SHARED_LINKER_FLAGS_DEBUG} )
set_target_properties( "Core.STL" PROPERTIES LINK_FLAGS_DEBUGANALYZE ${PROJECTS_SHARED_LINKER_FLAGS_DEBUGANALYZE} )
target_compile_options( "Core.STL" PRIVATE $<$<CONFIG:Profile>: ${PROJECTS_SHARED_CXX_FLAGS_PROFILE}> )
set_target_properties( "Core.STL" PROPERTIES LINK_FLAGS_PROFILE ${PROJECTS_SHARED_LINKER_FLAGS_PROFILE} )
target_compile_options( "Core.STL" PRIVATE $<$<CONFIG:Debug>: ${PROJECTS_SHARED_CXX_FLAGS_DEBUG}> )
target_compile_definitions( "Core.STL" PRIVATE $<$<CONFIG:Release>: ${PROJECTS_SHARED_DEFINES_RELEASE}> )



#==================================================================================================
# project: Core.Java
#==================================================================================================
set( SOURCES 
	"Java/Java.cpp"
	"Java/JavaArray.h"
	"Java/JavaClass.h"
	"Java/JavaEnvironment.h"
	"Java/JavaMethod.h"
	"Java/JavaMethodType.h"
	"Java/JavaString.h"
	"Java/JavaTypeInfo.h" )
add_library( "Core.Java" STATIC ${SOURCES} )
source_group( "" FILES "Java/Java.cpp" "Java/JavaArray.h" "Java/JavaClass.h" "Java/JavaEnvironment.h" "Java/JavaMethod.h" "Java/JavaMethodType.h" "Java/JavaString.h" "Java/JavaTypeInfo.h" )
set_property( TARGET "Core.Java" PROPERTY FOLDER "Core" )
target_include_directories( "Core.Java" PUBLIC "../External" )
target_include_directories( "Core.Java" PUBLIC "${EXTERNALS_PATH}" )
target_include_directories( "Core.Java" PUBLIC ".." )
target_link_libraries( "Core.Java" "Core.STL" )
# compiler
target_compile_options( "Core.Java" PRIVATE $<$<CONFIG:DebugAnalyze>: ${PROJECTS_SHARED_CXX_FLAGS_DEBUGANALYZE}> )
target_compile_definitions( "Core.Java" PRIVATE $<$<CONFIG:Debug>: ${PROJECTS_SHARED_DEFINES_DEBUG}> )
target_compile_definitions( "Core.Java" PRIVATE $<$<CONFIG:DebugAnalyze>: ${PROJECTS_SHARED_DEFINES_DEBUGANALYZE}> )
set_target_properties( "Core.Java" PROPERTIES LINK_FLAGS_RELEASE ${PROJECTS_SHARED_LINKER_FLAGS_RELEASE} )
target_compile_definitions( "Core.Java" PRIVATE $<$<CONFIG:Profile>: ${PROJECTS_SHARED_DEFINES_PROFILE}> )
target_compile_options( "Core.Java" PRIVATE $<$<CONFIG:Release>: ${PROJECTS_SHARED_CXX_FLAGS_RELEASE}> )
set_target_properties( "Core.Java" PROPERTIES LINK_FLAGS_DEBUG ${PROJECTS_SHARED_LINKER_FLAGS_DEBUG} )
set_target_properties( "Core.Java" PROPERTIES LINK_FLAGS_DEBUGANALYZE ${PROJECTS_SHARED_LINKER_FLAGS_DEBUGANALYZE} )
target_compile_options( "Core.Java" PRIVATE $<$<CONFIG:Profile>: ${PROJECTS_SHARED_CXX_FLAGS_PROFILE}> )
set_target_properties( "Core.Java" PROPERTIES LINK_FLAGS_PROFILE ${PROJECTS_SHARED_LINKER_FLAGS_PROFILE} )
target_compile_options( "Core.Java" PRIVATE $<$<CONFIG:Debug>: ${PROJECTS_SHARED_CXX_FLAGS_DEBUG}> )
target_compile_definitions( "Core.Java" PRIVATE $<$<CONFIG:Release>: ${PROJECTS_SHARED_DEFINES_RELEASE}> )



#==================================================================================================
# project: CoreTests.STL
#==================================================================================================
set( SOURCES 
	"../CoreTests/STL/Common.h"
	"../CoreTests/STL/Debug.h"
	"../CoreTests/STL/Main.cpp"
	"../CoreTests/STL/Test_Algorithms_InvokeWithVariant.cpp"
	"../CoreTests/STL/Test_Algorithms_Range.cpp"
	"../CoreTests/STL/Test_CompileTime_MainType.cpp"
	"../CoreTests/STL/Test_CompileTime_Map.cpp"
	"../CoreTests/STL/Test_CompileTime_Sequence.cpp"
	"../CoreTests/STL/Test_CompileTime_StaticFloat.cpp"
	"../CoreTests/STL/Test_CompileTime_StringToID.cpp"
	"../CoreTests/STL/Test_CompileTime_TemplateMath.cpp"
	"../CoreTests/STL/Test_CompileTime_TypeInfo.cpp"
	"../CoreTests/STL/Test_CompileTime_TypeList.cpp"
	"../CoreTests/STL/Test_CompileTime_TypeQualifier.cpp"
	"../CoreTests/STL/Test_CompileTime_TypeTraits.cpp"
	"../CoreTests/STL/Test_Containers_Adaptors.cpp"
	"../CoreTests/STL/Test_Containers_Array.cpp"
	"../CoreTests/STL/Test_Containers_CircularQueue.cpp"
	"../CoreTests/STL/Test_Containers_Deque.cpp"
	"../CoreTests/STL/Test_Containers_HashSet.cpp"
	"../CoreTests/STL/Test_Containers_IndexedArray.cpp"
	"../CoreTests/STL/Test_Containers_List.cpp"
	"../CoreTests/STL/Test_Containers_Map.cpp"
	"../CoreTests/STL/Test_Containers_Queue.cpp"
	"../CoreTests/STL/Test_Containers_Set.cpp"
	"../CoreTests/STL/Test_Containers_String.cpp"
	"../CoreTests/STL/Test_Containers_Tuple.cpp"
	"../CoreTests/STL/Test_Math_Abs.cpp"
	"../CoreTests/STL/Test_Math_Bit.cpp"
	"../CoreTests/STL/Test_Math_Clamp_Wrap.cpp"
	"../CoreTests/STL/Test_Math_Color.cpp"
	"../CoreTests/STL/Test_Math_ColorFormat.cpp"
	"../CoreTests/STL/Test_Math_Factorial.cpp"
	"../CoreTests/STL/Test_Math_FloorCeilTruncRoundFract.cpp"
	"../CoreTests/STL/Test_Math_Frustum.cpp"
	"../CoreTests/STL/Test_Math_ImageUtils.cpp"
	"../CoreTests/STL/Test_Math_Matrix.cpp"
	"../CoreTests/STL/Test_Math_OverflowCheck.cpp"
	"../CoreTests/STL/Test_Math_Plane.cpp"
	"../CoreTests/STL/Test_Math_Transform.cpp"
	"../CoreTests/STL/Test_OS_Atomic.cpp"
	"../CoreTests/STL/Test_OS_Date.cpp"
	"../CoreTests/STL/Test_OS_FileSystem.cpp"
	"../CoreTests/STL/Test_Runtime_VirtualTypelist.cpp"
	"../CoreTests/STL/Test_Temp.cpp"
	"../CoreTests/STL/Test_Types_Cast.cpp"
	"../CoreTests/STL/Test_Types_FileAddress.cpp"
	"../CoreTests/STL/Test_Types_Function.cpp"
	"../CoreTests/STL/Test_Types_StringParser.cpp"
	"../CoreTests/STL/Test_Types_Time.cpp"
	"../CoreTests/STL/Test_Types_Union.cpp"
	"../CoreTests/STL/Test_Type_Optional.cpp" )
if (DEFINED ANDROID)
	add_library( "CoreTests.STL" SHARED ${SOURCES} )
else()
	add_executable( "CoreTests.STL" ${SOURCES} )
endif()
source_group( "" FILES "../CoreTests/STL/Common.h" "../CoreTests/STL/Debug.h" "../CoreTests/STL/Main.cpp" "../CoreTests/STL/Test_Algorithms_InvokeWithVariant.cpp" "../CoreTests/STL/Test_Algorithms_Range.cpp" "../CoreTests/STL/Test_CompileTime_MainType.cpp" "../CoreTests/STL/Test_CompileTime_Map.cpp" "../CoreTests/STL/Test_CompileTime_Sequence.cpp" "../CoreTests/STL/Test_CompileTime_StaticFloat.cpp" "../CoreTests/STL/Test_CompileTime_StringToID.cpp" "../CoreTests/STL/Test_CompileTime_TemplateMath.cpp" "../CoreTests/STL/Test_CompileTime_TypeInfo.cpp" "../CoreTests/STL/Test_CompileTime_TypeList.cpp" "../CoreTests/STL/Test_CompileTime_TypeQualifier.cpp" "../CoreTests/STL/Test_CompileTime_TypeTraits.cpp" "../CoreTests/STL/Test_Containers_Adaptors.cpp" "../CoreTests/STL/Test_Containers_Array.cpp" "../CoreTests/STL/Test_Containers_CircularQueue.cpp" "../CoreTests/STL/Test_Containers_Deque.cpp" "../CoreTests/STL/Test_Containers_HashSet.cpp" "../CoreTests/STL/Test_Containers_IndexedArray.cpp" "../CoreTests/STL/Test_Containers_List.cpp" "../CoreTests/STL/Test_Containers_Map.cpp" "../CoreTests/STL/Test_Containers_Queue.cpp" "../CoreTests/STL/Test_Containers_Set.cpp" "../CoreTests/STL/Test_Containers_String.cpp" "../CoreTests/STL/Test_Containers_Tuple.cpp" "../CoreTests/STL/Test_Math_Abs.cpp" "../CoreTests/STL/Test_Math_Bit.cpp" "../CoreTests/STL/Test_Math_Clamp_Wrap.cpp" "../CoreTests/STL/Test_Math_Color.cpp" "../CoreTests/STL/Test_Math_ColorFormat.cpp" "../CoreTests/STL/Test_Math_Factorial.cpp" "../CoreTests/STL/Test_Math_FloorCeilTruncRoundFract.cpp" "../CoreTests/STL/Test_Math_Frustum.cpp" "../CoreTests/STL/Test_Math_ImageUtils.cpp" "../CoreTests/STL/Test_Math_Matrix.cpp" "../CoreTests/STL/Test_Math_OverflowCheck.cpp" "../CoreTests/STL/Test_Math_Plane.cpp" "../CoreTests/STL/Test_Math_Transform.cpp" "../CoreTests/STL/Test_OS_Atomic.cpp" "../CoreTests/STL/Test_OS_Date.cpp" "../CoreTests/STL/Test_OS_FileSystem.cpp" "../CoreTests/STL/Test_Runtime_VirtualTypelist.cpp" "../CoreTests/STL/Test_Temp.cpp" "../CoreTests/STL/Test_Types_Cast.cpp" "../CoreTests/STL/Test_Types_FileAddress.cpp" "../CoreTests/STL/Test_Types_Function.cpp" "../CoreTests/STL/Test_Types_StringParser.cpp" "../CoreTests/STL/Test_Types_Time.cpp" "../CoreTests/STL/Test_Types_Union.cpp" "../CoreTests/STL/Test_Type_Optional.cpp" )
set_property( TARGET "CoreTests.STL" PROPERTY FOLDER "CoreTests" )
target_include_directories( "CoreTests.STL" PUBLIC "../External" )
target_include_directories( "CoreTests.STL" PUBLIC "${EXTERNALS_PATH}" )
target_include_directories( "CoreTests.STL" PUBLIC ".." )
target_link_libraries( "CoreTests.STL" "Core.STL" )
# compiler
target_compile_options( "CoreTests.STL" PRIVATE $<$<CONFIG:DebugAnalyze>: ${PROJECTS_SHARED_CXX_FLAGS_DEBUGANALYZE}> )
target_compile_definitions( "CoreTests.STL" PRIVATE $<$<CONFIG:Debug>: ${PROJECTS_SHARED_DEFINES_DEBUG}> )
target_compile_definitions( "CoreTests.STL" PRIVATE $<$<CONFIG:DebugAnalyze>: ${PROJECTS_SHARED_DEFINES_DEBUGANALYZE}> )
set_target_properties( "CoreTests.STL" PROPERTIES LINK_FLAGS_RELEASE ${PROJECTS_SHARED_LINKER_FLAGS_RELEASE} )
target_compile_definitions( "CoreTests.STL" PRIVATE $<$<CONFIG:Profile>: ${PROJECTS_SHARED_DEFINES_PROFILE}> )
target_compile_options( "CoreTests.STL" PRIVATE $<$<CONFIG:Release>: ${PROJECTS_SHARED_CXX_FLAGS_RELEASE}> )
set_target_properties( "CoreTests.STL" PROPERTIES LINK_FLAGS_DEBUG ${PROJECTS_SHARED_LINKER_FLAGS_DEBUG} )
set_target_properties( "CoreTests.STL" PROPERTIES LINK_FLAGS_DEBUGANALYZE ${PROJECTS_SHARED_LINKER_FLAGS_DEBUGANALYZE} )
target_compile_options( "CoreTests.STL" PRIVATE $<$<CONFIG:Profile>: ${PROJECTS_SHARED_CXX_FLAGS_PROFILE}> )
set_target_properties( "CoreTests.STL" PROPERTIES LINK_FLAGS_PROFILE ${PROJECTS_SHARED_LINKER_FLAGS_PROFILE} )
target_compile_options( "CoreTests.STL" PRIVATE $<$<CONFIG:Debug>: ${PROJECTS_SHARED_CXX_FLAGS_DEBUG}> )
target_compile_definitions( "CoreTests.STL" PRIVATE $<$<CONFIG:Release>: ${PROJECTS_SHARED_DEFINES_RELEASE}> )



#==================================================================================================
# project: Core.Physics
#==================================================================================================
set( SOURCES 
	"Physics/CompileTime/GenType.h"
	"Physics/CompileTime/PhysTypeInfo.h"
	"Physics/Common/Common.h"
	"Physics/Common/Main.cpp"
	"Physics/Algorithms/AcceleratedMotion.h"
	"Physics/Algorithms/Astrophysics.h"
	"Physics/Algorithms/Astrophysics_Orbit.h"
	"Physics/Dimensions/AstrophysicsConstants.h"
	"Physics/Dimensions/DefaultTypes.h"
	"Physics/Dimensions/PhysicsConstants.h"
	"Physics/Dimensions/PhysicsDimension.h"
	"Physics/Dimensions/PhysicsValue.h"
	"Physics/Dimensions/PhysicsValueMath.h"
	"Physics/Dimensions/PhysicsValueUtils.h"
	"Physics/Dimensions/PhysicsValueVec.h"
	"Physics/Dimensions/PhysicsValueVecI.h"
	"Physics/Core.Physics.h" )
add_library( "Core.Physics" STATIC ${SOURCES} )
source_group( "CompileTime" FILES "Physics/CompileTime/GenType.h" "Physics/CompileTime/PhysTypeInfo.h" )
source_group( "Common" FILES "Physics/Common/Common.h" "Physics/Common/Main.cpp" )
source_group( "Algorithms" FILES "Physics/Algorithms/AcceleratedMotion.h" "Physics/Algorithms/Astrophysics.h" "Physics/Algorithms/Astrophysics_Orbit.h" )
source_group( "Dimensions" FILES "Physics/Dimensions/AstrophysicsConstants.h" "Physics/Dimensions/DefaultTypes.h" "Physics/Dimensions/PhysicsConstants.h" "Physics/Dimensions/PhysicsDimension.h" "Physics/Dimensions/PhysicsValue.h" "Physics/Dimensions/PhysicsValueMath.h" "Physics/Dimensions/PhysicsValueUtils.h" "Physics/Dimensions/PhysicsValueVec.h" "Physics/Dimensions/PhysicsValueVecI.h" )
source_group( "" FILES "Physics/Core.Physics.h" )
set_property( TARGET "Core.Physics" PROPERTY FOLDER "Core" )
target_include_directories( "Core.Physics" PUBLIC "../External" )
target_include_directories( "Core.Physics" PUBLIC "${EXTERNALS_PATH}" )
target_include_directories( "Core.Physics" PUBLIC ".." )
target_link_libraries( "Core.Physics" "Core.STL" )
# compiler
target_compile_options( "Core.Physics" PRIVATE $<$<CONFIG:DebugAnalyze>: ${PROJECTS_SHARED_CXX_FLAGS_DEBUGANALYZE}> )
target_compile_definitions( "Core.Physics" PRIVATE $<$<CONFIG:Debug>: ${PROJECTS_SHARED_DEFINES_DEBUG}> )
target_compile_definitions( "Core.Physics" PRIVATE $<$<CONFIG:DebugAnalyze>: ${PROJECTS_SHARED_DEFINES_DEBUGANALYZE}> )
set_target_properties( "Core.Physics" PROPERTIES LINK_FLAGS_RELEASE ${PROJECTS_SHARED_LINKER_FLAGS_RELEASE} )
target_compile_definitions( "Core.Physics" PRIVATE $<$<CONFIG:Profile>: ${PROJECTS_SHARED_DEFINES_PROFILE}> )
target_compile_options( "Core.Physics" PRIVATE $<$<CONFIG:Release>: ${PROJECTS_SHARED_CXX_FLAGS_RELEASE}> )
set_target_properties( "Core.Physics" PROPERTIES LINK_FLAGS_DEBUG ${PROJECTS_SHARED_LINKER_FLAGS_DEBUG} )
set_target_properties( "Core.Physics" PROPERTIES LINK_FLAGS_DEBUGANALYZE ${PROJECTS_SHARED_LINKER_FLAGS_DEBUGANALYZE} )
target_compile_options( "Core.Physics" PRIVATE $<$<CONFIG:Profile>: ${PROJECTS_SHARED_CXX_FLAGS_PROFILE}> )
set_target_properties( "Core.Physics" PROPERTIES LINK_FLAGS_PROFILE ${PROJECTS_SHARED_LINKER_FLAGS_PROFILE} )
target_compile_options( "Core.Physics" PRIVATE $<$<CONFIG:Debug>: ${PROJECTS_SHARED_CXX_FLAGS_DEBUG}> )
target_compile_definitions( "Core.Physics" PRIVATE $<$<CONFIG:Release>: ${PROJECTS_SHARED_DEFINES_RELEASE}> )



#==================================================================================================
# project: CoreTests.Physics
#==================================================================================================
set( SOURCES 
	"../CoreTests/Physics/Common.h"
	"../CoreTests/Physics/Main.cpp"
	"../CoreTests/Physics/Test_Algorithms_Orbit.cpp"
	"../CoreTests/Physics/Test_Dimensions_PhysicsValue.cpp" )
if (DEFINED ANDROID)
	add_library( "CoreTests.Physics" SHARED ${SOURCES} )
else()
	add_executable( "CoreTests.Physics" ${SOURCES} )
endif()
source_group( "" FILES "../CoreTests/Physics/Common.h" "../CoreTests/Physics/Main.cpp" "../CoreTests/Physics/Test_Algorithms_Orbit.cpp" "../CoreTests/Physics/Test_Dimensions_PhysicsValue.cpp" )
set_property( TARGET "CoreTests.Physics" PROPERTY FOLDER "CoreTests" )
target_include_directories( "CoreTests.Physics" PUBLIC "../External" )
target_include_directories( "CoreTests.Physics" PUBLIC "${EXTERNALS_PATH}" )
target_include_directories( "CoreTests.Physics" PUBLIC ".." )
target_link_libraries( "CoreTests.Physics" "Core.Physics" )
# compiler
target_compile_options( "CoreTests.Physics" PRIVATE $<$<CONFIG:DebugAnalyze>: ${PROJECTS_SHARED_CXX_FLAGS_DEBUGANALYZE}> )
target_compile_definitions( "CoreTests.Physics" PRIVATE $<$<CONFIG:Debug>: ${PROJECTS_SHARED_DEFINES_DEBUG}> )
target_compile_definitions( "CoreTests.Physics" PRIVATE $<$<CONFIG:DebugAnalyze>: ${PROJECTS_SHARED_DEFINES_DEBUGANALYZE}> )
set_target_properties( "CoreTests.Physics" PROPERTIES LINK_FLAGS_RELEASE ${PROJECTS_SHARED_LINKER_FLAGS_RELEASE} )
target_compile_definitions( "CoreTests.Physics" PRIVATE $<$<CONFIG:Profile>: ${PROJECTS_SHARED_DEFINES_PROFILE}> )
target_compile_options( "CoreTests.Physics" PRIVATE $<$<CONFIG:Release>: ${PROJECTS_SHARED_CXX_FLAGS_RELEASE}> )
set_target_properties( "CoreTests.Physics" PROPERTIES LINK_FLAGS_DEBUG ${PROJECTS_SHARED_LINKER_FLAGS_DEBUG} )
set_target_properties( "CoreTests.Physics" PROPERTIES LINK_FLAGS_DEBUGANALYZE ${PROJECTS_SHARED_LINKER_FLAGS_DEBUGANALYZE} )
target_compile_options( "CoreTests.Physics" PRIVATE $<$<CONFIG:Profile>: ${PROJECTS_SHARED_CXX_FLAGS_PROFILE}> )
set_target_properties( "CoreTests.Physics" PROPERTIES LINK_FLAGS_PROFILE ${PROJECTS_SHARED_LINKER_FLAGS_PROFILE} )
target_compile_options( "CoreTests.Physics" PRIVATE $<$<CONFIG:Debug>: ${PROJECTS_SHARED_CXX_FLAGS_DEBUG}> )
target_compile_definitions( "CoreTests.Physics" PRIVATE $<$<CONFIG:Release>: ${PROJECTS_SHARED_DEFINES_RELEASE}> )



#==================================================================================================
# project: Core.Script
#==================================================================================================
set( SOURCES 
	"Script/Bindings/BindScalarMath.cpp"
	"Script/Bindings/BindString.cpp"
	"Script/Bindings/BindVectorMath.cpp"
	"Script/Bindings/DefaultBindings.h"
	"Script/Core.Script.h"
	"Script/Impl/ClassBinder.h"
	"Script/Impl/EnumBinder.h"
	"Script/Impl/ScriptBinder.cpp"
	"Script/Impl/ScriptEngine.cpp"
	"Script/Impl/ScriptEngine.h"
	"Script/Impl/ScriptEngine.inl.h"
	"Script/Impl/ScriptModule.cpp"
	"Script/Impl/ScriptModule.h"
	"Script/Impl/ScriptTypes.h" )
add_library( "Core.Script" STATIC ${SOURCES} )
source_group( "Bindings" FILES "Script/Bindings/BindScalarMath.cpp" "Script/Bindings/BindString.cpp" "Script/Bindings/BindVectorMath.cpp" "Script/Bindings/DefaultBindings.h" )
source_group( "" FILES "Script/Core.Script.h" )
source_group( "Impl" FILES "Script/Impl/ClassBinder.h" "Script/Impl/EnumBinder.h" "Script/Impl/ScriptBinder.cpp" "Script/Impl/ScriptEngine.cpp" "Script/Impl/ScriptEngine.h" "Script/Impl/ScriptEngine.inl.h" "Script/Impl/ScriptModule.cpp" "Script/Impl/ScriptModule.h" "Script/Impl/ScriptTypes.h" )
set_property( TARGET "Core.Script" PROPERTY FOLDER "Core" )
target_include_directories( "Core.Script" PUBLIC "../External" )
target_include_directories( "Core.Script" PUBLIC "${EXTERNALS_PATH}" )
target_include_directories( "Core.Script" PUBLIC ".." )
target_link_libraries( "Core.Script" "AngelScript" )
target_link_libraries( "Core.Script" "Core.STL" )
# compiler
target_compile_options( "Core.Script" PRIVATE $<$<CONFIG:DebugAnalyze>: ${PROJECTS_SHARED_CXX_FLAGS_DEBUGANALYZE}> )
target_compile_definitions( "Core.Script" PRIVATE $<$<CONFIG:Debug>: ${PROJECTS_SHARED_DEFINES_DEBUG}> )
target_compile_definitions( "Core.Script" PRIVATE $<$<CONFIG:DebugAnalyze>: ${PROJECTS_SHARED_DEFINES_DEBUGANALYZE}> )
set_target_properties( "Core.Script" PROPERTIES LINK_FLAGS_RELEASE ${PROJECTS_SHARED_LINKER_FLAGS_RELEASE} )
target_compile_definitions( "Core.Script" PRIVATE $<$<CONFIG:Profile>: ${PROJECTS_SHARED_DEFINES_PROFILE}> )
target_compile_options( "Core.Script" PRIVATE $<$<CONFIG:Release>: ${PROJECTS_SHARED_CXX_FLAGS_RELEASE}> )
set_target_properties( "Core.Script" PROPERTIES LINK_FLAGS_DEBUG ${PROJECTS_SHARED_LINKER_FLAGS_DEBUG} )
set_target_properties( "Core.Script" PROPERTIES LINK_FLAGS_DEBUGANALYZE ${PROJECTS_SHARED_LINKER_FLAGS_DEBUGANALYZE} )
target_compile_options( "Core.Script" PRIVATE $<$<CONFIG:Profile>: ${PROJECTS_SHARED_CXX_FLAGS_PROFILE}> )
set_target_properties( "Core.Script" PROPERTIES LINK_FLAGS_PROFILE ${PROJECTS_SHARED_LINKER_FLAGS_PROFILE} )
target_compile_options( "Core.Script" PRIVATE $<$<CONFIG:Debug>: ${PROJECTS_SHARED_CXX_FLAGS_DEBUG}> )
target_compile_definitions( "Core.Script" PRIVATE $<$<CONFIG:Release>: ${PROJECTS_SHARED_DEFINES_RELEASE}> )



#==================================================================================================
# project: CoreTests.Scipt
#==================================================================================================
set( SOURCES 
	"../CoreTests/Script/Common.h"
	"../CoreTests/Script/Main.cpp"
	"../CoreTests/Script/Test_Eval.cpp"
	"../CoreTests/Script/Test_ScriptClass.cpp"
	"../CoreTests/Script/Test_ScriptScalarMath.cpp"
	"../CoreTests/Script/Test_ScriptString.cpp"
	"../CoreTests/Script/Test_ScriptVectorMath.cpp" )
if (DEFINED ANDROID)
	add_library( "CoreTests.Scipt" SHARED ${SOURCES} )
else()
	add_executable( "CoreTests.Scipt" ${SOURCES} )
endif()
source_group( "" FILES "../CoreTests/Script/Common.h" "../CoreTests/Script/Main.cpp" "../CoreTests/Script/Test_Eval.cpp" "../CoreTests/Script/Test_ScriptClass.cpp" "../CoreTests/Script/Test_ScriptScalarMath.cpp" "../CoreTests/Script/Test_ScriptString.cpp" "../CoreTests/Script/Test_ScriptVectorMath.cpp" )
set_property( TARGET "CoreTests.Scipt" PROPERTY FOLDER "CoreTests" )
target_include_directories( "CoreTests.Scipt" PUBLIC "../External" )
target_include_directories( "CoreTests.Scipt" PUBLIC "${EXTERNALS_PATH}" )
target_include_directories( "CoreTests.Scipt" PUBLIC ".." )
target_link_libraries( "CoreTests.Scipt" "Core.Script" )
# compiler
target_compile_options( "CoreTests.Scipt" PRIVATE $<$<CONFIG:DebugAnalyze>: ${PROJECTS_SHARED_CXX_FLAGS_DEBUGANALYZE}> )
target_compile_definitions( "CoreTests.Scipt" PRIVATE $<$<CONFIG:Debug>: ${PROJECTS_SHARED_DEFINES_DEBUG}> )
target_compile_definitions( "CoreTests.Scipt" PRIVATE $<$<CONFIG:DebugAnalyze>: ${PROJECTS_SHARED_DEFINES_DEBUGANALYZE}> )
set_target_properties( "CoreTests.Scipt" PROPERTIES LINK_FLAGS_RELEASE ${PROJECTS_SHARED_LINKER_FLAGS_RELEASE} )
target_compile_definitions( "CoreTests.Scipt" PRIVATE $<$<CONFIG:Profile>: ${PROJECTS_SHARED_DEFINES_PROFILE}> )
target_compile_options( "CoreTests.Scipt" PRIVATE $<$<CONFIG:Release>: ${PROJECTS_SHARED_CXX_FLAGS_RELEASE}> )
set_target_properties( "CoreTests.Scipt" PROPERTIES LINK_FLAGS_DEBUG ${PROJECTS_SHARED_LINKER_FLAGS_DEBUG} )
set_target_properties( "CoreTests.Scipt" PROPERTIES LINK_FLAGS_DEBUGANALYZE ${PROJECTS_SHARED_LINKER_FLAGS_DEBUGANALYZE} )
target_compile_options( "CoreTests.Scipt" PRIVATE $<$<CONFIG:Profile>: ${PROJECTS_SHARED_CXX_FLAGS_PROFILE}> )
set_target_properties( "CoreTests.Scipt" PROPERTIES LINK_FLAGS_PROFILE ${PROJECTS_SHARED_LINKER_FLAGS_PROFILE} )
target_compile_options( "CoreTests.Scipt" PRIVATE $<$<CONFIG:Debug>: ${PROJECTS_SHARED_CXX_FLAGS_DEBUG}> )
target_compile_definitions( "CoreTests.Scipt" PRIVATE $<$<CONFIG:Release>: ${PROJECTS_SHARED_DEFINES_RELEASE}> )



add_test( NAME "CoreTests.STL" COMMAND "CoreTests.STL" )

add_test( NAME "CoreTests.Physics" COMMAND "CoreTests.Physics" )

add_test( NAME "CoreTests.Scipt" COMMAND "CoreTests.Scipt" )

