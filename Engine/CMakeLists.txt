# auto generated file
cmake_minimum_required (VERSION 3.6.0)

set( Engine_VERSION_MAJOR 0 )
set( Engine_VERSION_MINOR 300 )

set( OPENGLES_VERSION "320" CACHE STRING "Define OpenGLES version, supported: 200, 300, 310, 320"  )
set( VULKAN_VERSION "100" CACHE STRING "Define Vulkan version, supported: 100, 110"  )
set( OPENGL_VERSION "450" CACHE STRING "Define OpenGL version, supported: 440, 450"  )
set( SOFTRENDER_VERSION "100" CACHE STRING "Define Software renderer version, 100 is supported"  )
set( OPENCL_VERSION "120" CACHE STRING "Define OpenCL version, supported: 110, 120, 200"  )
#==================================================================================================
# project: Engine.Base
#==================================================================================================
set( SOURCES 
	"Base/Tasks/AsyncTask.h"
	"Base/Tasks/TaskManager.cpp"
	"Base/Tasks/TaskManager.h"
	"Base/Tasks/TaskModule.cpp"
	"Base/DataProvider/BuiltinStorageDataProvider.cpp"
	"Base/DataProvider/DataMessages.h"
	"Base/DataProvider/DataProviderManager.cpp"
	"Base/DataProvider/DataProviderObjectsConstructor.cpp"
	"Base/DataProvider/DataProviderObjectsConstructor.h"
	"Base/DataProvider/FileDataInput.cpp"
	"Base/DataProvider/FileDataOutput.cpp"
	"Base/DataProvider/FileInputStream.cpp"
	"Base/DataProvider/FileOutputStream.cpp"
	"Base/DataProvider/InMemoryDataProvider.cpp"
	"Base/DataProvider/InternetDataProvider.cpp"
	"Base/DataProvider/LocalStorageDataProvider.cpp"
	"Base/Main/MainSystem.cpp"
	"Base/Main/MainSystem.h"
	"Base/Modules/MessageCache.h"
	"Base/Modules/MessageHandler.cpp"
	"Base/Modules/MessageHandler.h"
	"Base/Modules/MessageHelpers.h"
	"Base/Modules/Module.cpp"
	"Base/Modules/Module.h"
	"Base/Modules/Module.inl.h"
	"Base/Modules/Module.Send.inl.h"
	"Base/Modules/ModuleAsyncTasks.h"
	"Base/Modules/ModulesFactory.cpp"
	"Base/Modules/ModulesFactory.h"
	"Base/Modules/ModuleUtils.h"
	"Base/Common/BaseObject.h"
	"Base/Common/Common.h"
	"Base/Common/Defines.h"
	"Base/Common/EModuleGroup.h"
	"Base/Common/EngineSubSystems.h"
	"Base/Common/Enums.h"
	"Base/Common/IDs.h"
	"Base/Common/ThreadID.h"
	"Base/Engine.Base.h"
	"Base/Public/AsyncMessage.h"
	"Base/Public/CreateInfo.h"
	"Base/Public/DataProvider.h"
	"Base/Public/ModuleMessages.h"
	"Base/Public/ParallelThread.h"
	"Base/Public/ProfilingMessages.h"
	"Base/Public/TaskModule.h"
	"Base/Threads/ParallelThreadImpl.cpp"
	"Base/Threads/ParallelThreadImpl.h"
	"Base/Threads/ThreadManager.cpp"
	"Base/Threads/ThreadManager.h" )
add_library( "Engine.Base" STATIC ${SOURCES} )
source_group( "Tasks" FILES "Base/Tasks/AsyncTask.h" "Base/Tasks/TaskManager.cpp" "Base/Tasks/TaskManager.h" "Base/Tasks/TaskModule.cpp" )
source_group( "DataProvider" FILES "Base/DataProvider/BuiltinStorageDataProvider.cpp" "Base/DataProvider/DataMessages.h" "Base/DataProvider/DataProviderManager.cpp" "Base/DataProvider/DataProviderObjectsConstructor.cpp" "Base/DataProvider/DataProviderObjectsConstructor.h" "Base/DataProvider/FileDataInput.cpp" "Base/DataProvider/FileDataOutput.cpp" "Base/DataProvider/FileInputStream.cpp" "Base/DataProvider/FileOutputStream.cpp" "Base/DataProvider/InMemoryDataProvider.cpp" "Base/DataProvider/InternetDataProvider.cpp" "Base/DataProvider/LocalStorageDataProvider.cpp" )
source_group( "Main" FILES "Base/Main/MainSystem.cpp" "Base/Main/MainSystem.h" )
source_group( "Modules" FILES "Base/Modules/MessageCache.h" "Base/Modules/MessageHandler.cpp" "Base/Modules/MessageHandler.h" "Base/Modules/MessageHelpers.h" "Base/Modules/Module.cpp" "Base/Modules/Module.h" "Base/Modules/Module.inl.h" "Base/Modules/Module.Send.inl.h" "Base/Modules/ModuleAsyncTasks.h" "Base/Modules/ModulesFactory.cpp" "Base/Modules/ModulesFactory.h" "Base/Modules/ModuleUtils.h" )
source_group( "Common" FILES "Base/Common/BaseObject.h" "Base/Common/Common.h" "Base/Common/Defines.h" "Base/Common/EModuleGroup.h" "Base/Common/EngineSubSystems.h" "Base/Common/Enums.h" "Base/Common/IDs.h" "Base/Common/ThreadID.h" )
source_group( "" FILES "Base/Engine.Base.h" )
source_group( "Public" FILES "Base/Public/AsyncMessage.h" "Base/Public/CreateInfo.h" "Base/Public/DataProvider.h" "Base/Public/ModuleMessages.h" "Base/Public/ParallelThread.h" "Base/Public/ProfilingMessages.h" "Base/Public/TaskModule.h" )
source_group( "Threads" FILES "Base/Threads/ParallelThreadImpl.cpp" "Base/Threads/ParallelThreadImpl.h" "Base/Threads/ThreadManager.cpp" "Base/Threads/ThreadManager.h" )
set_property( TARGET "Engine.Base" PROPERTY FOLDER "Engine" )
target_include_directories( "Engine.Base" PUBLIC "C:/Projects/ModularGraphicsFramework/External" )
target_include_directories( "Engine.Base" PUBLIC "${EXTERNALS_PATH}" )
target_include_directories( "Engine.Base" PUBLIC "C:/Projects/ModularGraphicsFramework/Core/.." )
target_link_libraries( "Engine.Base" "Core.STL" )
# compiler
set_target_properties( "Engine.Base" PROPERTIES LINK_FLAGS_RELEASE ${PROJECTS_SHARED_LINKER_FLAGS_RELEASE} )
set_target_properties( "Engine.Base" PROPERTIES LINK_FLAGS_PROFILE ${PROJECTS_SHARED_LINKER_FLAGS_PROFILE} )
target_compile_definitions( "Engine.Base" PRIVATE $<$<CONFIG:Debug>: ${PROJECTS_SHARED_DEFINES_DEBUG}> )
set_target_properties( "Engine.Base" PROPERTIES LINK_FLAGS_DEBUGANALYZE ${PROJECTS_SHARED_LINKER_FLAGS_DEBUGANALYZE} )
target_compile_definitions( "Engine.Base" PRIVATE $<$<CONFIG:DebugAnalyze>: ${PROJECTS_SHARED_DEFINES_DEBUGANALYZE}> )
target_compile_options( "Engine.Base" PRIVATE $<$<CONFIG:Profile>: ${PROJECTS_SHARED_CXX_FLAGS_PROFILE}> )
target_compile_options( "Engine.Base" PRIVATE $<$<CONFIG:Release>: ${PROJECTS_SHARED_CXX_FLAGS_RELEASE}> )
target_compile_definitions( "Engine.Base" PRIVATE $<$<CONFIG:Profile>: ${PROJECTS_SHARED_DEFINES_PROFILE}> )
target_compile_definitions( "Engine.Base" PRIVATE $<$<CONFIG:Release>: ${PROJECTS_SHARED_DEFINES_RELEASE}> )
set_target_properties( "Engine.Base" PROPERTIES LINK_FLAGS_DEBUG ${PROJECTS_SHARED_LINKER_FLAGS_DEBUG} )
target_compile_options( "Engine.Base" PRIVATE $<$<CONFIG:DebugAnalyze>: ${PROJECTS_SHARED_CXX_FLAGS_DEBUGANALYZE}> )
target_compile_options( "Engine.Base" PRIVATE $<$<CONFIG:Debug>: ${PROJECTS_SHARED_CXX_FLAGS_DEBUG}> )



#==================================================================================================
# project: Engine.Platforms
#==================================================================================================
set( SOURCES 
	"Platforms/Vulkan/Android/VkAndSurface.cpp"
	"Platforms/Vulkan/Android/VkAndSurface.h"
	"Platforms/Soft/SoftRendererContext.cpp"
	"Platforms/Soft/SoftRendererObjectsConstructor.h"
	"Platforms/Soft/SoftRendererThread.cpp"
	"Platforms/SDL/SDLDisplay.cpp"
	"Platforms/SDL/SDLDisplay.h"
	"Platforms/SDL/SDLKeyInput.cpp"
	"Platforms/SDL/SDLMessages.h"
	"Platforms/SDL/SDLMouseInput.cpp"
	"Platforms/SDL/SDLObjectsConstructor.h"
	"Platforms/SDL/SDLPlatform.cpp"
	"Platforms/SDL/SDLWindow.cpp"
	"Platforms/OpenGL/OpenGLContext.cpp"
	"Platforms/OpenGL/OpenGLObjectsConstructor.h"
	"Platforms/OpenGL/OpenGLThread.cpp"
	"Platforms/OpenCL/120/CL1BaseModule.cpp"
	"Platforms/OpenCL/120/CL1BaseModule.h"
	"Platforms/OpenCL/120/CL1Buffer.cpp"
	"Platforms/OpenCL/120/CL1CommandBuffer.cpp"
	"Platforms/OpenCL/120/CL1CommandBuilder.cpp"
	"Platforms/OpenCL/120/CL1CommandQueue.cpp"
	"Platforms/OpenCL/120/CL1Device.cpp"
	"Platforms/OpenCL/120/CL1Device.h"
	"Platforms/OpenCL/120/CL1Emulator.cpp"
	"Platforms/OpenCL/120/CL1Emulator.h"
	"Platforms/OpenCL/120/CL1Enums.h"
	"Platforms/OpenCL/120/CL1Image.cpp"
	"Platforms/OpenCL/120/CL1Messages.h"
	"Platforms/OpenCL/120/CL1Pipeline.cpp"
	"Platforms/OpenCL/120/CL1PipelineResourceTable.cpp"
	"Platforms/OpenCL/120/CL1ResourceCache.h"
	"Platforms/OpenCL/120/CL1Sampler.cpp"
	"Platforms/OpenCL/120/CL1SamplerCache.h"
	"Platforms/OpenCL/120/CL1SyncManager.cpp"
	"Platforms/OpenCL/120/cl2.cpp"
	"Platforms/OpenCL/120/cl2.h"
	"Platforms/Vulkan/110/Vk1BaseModule.cpp"
	"Platforms/Vulkan/110/Vk1BaseModule.h"
	"Platforms/Vulkan/110/Vk1BaseObject.h"
	"Platforms/Vulkan/110/Vk1Buffer.cpp"
	"Platforms/Vulkan/110/Vk1CommandBuffer.cpp"
	"Platforms/Vulkan/110/Vk1CommandBuilder.cpp"
	"Platforms/Vulkan/110/Vk1CommandQueue.cpp"
	"Platforms/Vulkan/110/Vk1Device.cpp"
	"Platforms/Vulkan/110/Vk1Device.h"
	"Platforms/Vulkan/110/Vk1Enums.h"
	"Platforms/Vulkan/110/Vk1Framebuffer.cpp"
	"Platforms/Vulkan/110/Vk1Image.cpp"
	"Platforms/Vulkan/110/Vk1Library.h"
	"Platforms/Vulkan/110/Vk1ManagedMemory.cpp"
	"Platforms/Vulkan/110/Vk1MemoryManager.cpp"
	"Platforms/Vulkan/110/Vk1Messages.h"
	"Platforms/Vulkan/110/Vk1Pipeline.cpp"
	"Platforms/Vulkan/110/Vk1PipelineCache.cpp"
	"Platforms/Vulkan/110/Vk1PipelineCache.h"
	"Platforms/Vulkan/110/Vk1PipelineLayout.cpp"
	"Platforms/Vulkan/110/Vk1PipelineLayout.h"
	"Platforms/Vulkan/110/Vk1PipelineResourceTable.cpp"
	"Platforms/Vulkan/110/Vk1RenderPass.cpp"
	"Platforms/Vulkan/110/Vk1RenderPassCache.h"
	"Platforms/Vulkan/110/Vk1ResourceCache.h"
	"Platforms/Vulkan/110/Vk1Sampler.cpp"
	"Platforms/Vulkan/110/Vk1SamplerCache.h"
	"Platforms/Vulkan/110/Vk1SwapchainImage.h"
	"Platforms/Vulkan/110/Vk1SyncManager.cpp"
	"Platforms/Vulkan/110/vulkan1.cpp"
	"Platforms/Vulkan/110/vulkan1.h"
	"Platforms/Vulkan/110/vulkan1_platform.cpp"
	"Platforms/Vulkan/110/vulkan1_platform.h"
	"Platforms/Vulkan/110/vulkan1_utils.h"
	"Platforms/OpenGL/300es/gles3.cpp"
	"Platforms/OpenGL/300es/gles3.h"
	"Platforms/OpenCL/OpenCLContext.cpp"
	"Platforms/OpenCL/OpenCLObjectsConstructor.h"
	"Platforms/OpenCL/OpenCLThread.cpp"
	"Platforms/Input/InputManager.cpp"
	"Platforms/Input/InputManager.h"
	"Platforms/Input/InputThread.cpp"
	"Platforms/Soft/ShaderLang/gen_Image.h"
	"Platforms/Soft/ShaderLang/gen_Texture.h"
	"Platforms/Soft/ShaderLang/SWLang.h"
	"Platforms/Soft/ShaderLang/SWLangArray.h"
	"Platforms/Soft/ShaderLang/SWLangBarrier.h"
	"Platforms/Soft/ShaderLang/SWLangBuffer.h"
	"Platforms/Soft/ShaderLang/SWLangCommon.h"
	"Platforms/Soft/ShaderLang/SWLangGLM.h"
	"Platforms/Soft/ShaderLang/SWLangImage.cpp"
	"Platforms/Soft/ShaderLang/SWLangImage.h"
	"Platforms/Soft/ShaderLang/SWLangShared.h"
	"Platforms/Soft/ShaderLang/SWLangTexture.cpp"
	"Platforms/Soft/ShaderLang/SWLangTexture.h"
	"Platforms/Soft/ShaderLang/SWLangTexture2D.h"
	"Platforms/Soft/ShaderLang/SWShaderHelper.h"
	"Platforms/Vulkan/VulkanContext.cpp"
	"Platforms/Vulkan/VulkanObjectsConstructor.h"
	"Platforms/Vulkan/VulkanThread.cpp"
	"Platforms/Soft/Impl/SWBaseModule.cpp"
	"Platforms/Soft/Impl/SWBaseModule.h"
	"Platforms/Soft/Impl/SWBuffer.cpp"
	"Platforms/Soft/Impl/SWCommandBuffer.cpp"
	"Platforms/Soft/Impl/SWCommandBuilder.cpp"
	"Platforms/Soft/Impl/SWCommandQueue.cpp"
	"Platforms/Soft/Impl/SWDevice.cpp"
	"Platforms/Soft/Impl/SWDevice.h"
	"Platforms/Soft/Impl/SWDeviceProperties.h"
	"Platforms/Soft/Impl/SWEnums.h"
	"Platforms/Soft/Impl/SWImage.cpp"
	"Platforms/Soft/Impl/SWMemory.cpp"
	"Platforms/Soft/Impl/SWMessages.h"
	"Platforms/Soft/Impl/SWPipeline.cpp"
	"Platforms/Soft/Impl/SWPipelineResourceTable.cpp"
	"Platforms/Soft/Impl/SWSampler.cpp"
	"Platforms/Soft/Impl/SWSamplerCache.h"
	"Platforms/Soft/Impl/SWShaderModel.cpp"
	"Platforms/Soft/Impl/SWShaderModel.h"
	"Platforms/Soft/Impl/SWSyncManager.cpp"
	"Platforms/Soft/Impl/SWSyncObjects.h"
	"Platforms/Windows/WinDisplay.cpp"
	"Platforms/Windows/WinDisplay.h"
	"Platforms/Windows/WinKeyInput.cpp"
	"Platforms/Windows/WinMessages.h"
	"Platforms/Windows/WinMouseInput.cpp"
	"Platforms/Windows/WinObjectsConstructor.h"
	"Platforms/Windows/WinPlatform.cpp"
	"Platforms/Windows/WinWindow.cpp"
	"Platforms/Public/GPU/Buffer.h"
	"Platforms/Public/GPU/BufferEnums.h"
	"Platforms/Public/GPU/CommandBuffer.h"
	"Platforms/Public/GPU/CommandEnums.h"
	"Platforms/Public/GPU/Context.cpp"
	"Platforms/Public/GPU/Context.h"
	"Platforms/Public/GPU/Enums.ToString.h"
	"Platforms/Public/GPU/FragmentOutputState.h"
	"Platforms/Public/GPU/Framebuffer.h"
	"Platforms/Public/GPU/IDs.h"
	"Platforms/Public/GPU/Image.cpp"
	"Platforms/Public/GPU/Image.h"
	"Platforms/Public/GPU/ImageEnums.h"
	"Platforms/Public/GPU/ImageLayer.h"
	"Platforms/Public/GPU/ImageSwizzle.h"
	"Platforms/Public/GPU/Memory.h"
	"Platforms/Public/GPU/MemoryEnums.h"
	"Platforms/Public/GPU/MipmapLevel.h"
	"Platforms/Public/GPU/MultiSamples.h"
	"Platforms/Public/GPU/ObjectEnums.h"
	"Platforms/Public/GPU/Pipeline.cpp"
	"Platforms/Public/GPU/Pipeline.h"
	"Platforms/Public/GPU/PipelineLayout.cpp"
	"Platforms/Public/GPU/PipelineLayout.h"
	"Platforms/Public/GPU/PixelFormatEnums.h"
	"Platforms/Public/GPU/RenderPass.cpp"
	"Platforms/Public/GPU/RenderPass.h"
	"Platforms/Public/GPU/RenderPassEnums.h"
	"Platforms/Public/GPU/RenderState.cpp"
	"Platforms/Public/GPU/RenderState.h"
	"Platforms/Public/GPU/RenderStateEnums.h"
	"Platforms/Public/GPU/Sampler.cpp"
	"Platforms/Public/GPU/Sampler.h"
	"Platforms/Public/GPU/SamplerEnums.h"
	"Platforms/Public/GPU/ShaderEnums.h"
	"Platforms/Public/GPU/Sync.h"
	"Platforms/Public/GPU/Thread.h"
	"Platforms/Public/GPU/VertexAttribs.h"
	"Platforms/Public/GPU/VertexDescr.h"
	"Platforms/Public/GPU/VertexEnums.h"
	"Platforms/Public/GPU/VertexInputState.cpp"
	"Platforms/Public/GPU/VertexInputState.h"
	"Platforms/Public/GPU/VR.h"
	"Platforms/Impl/Main.cpp"
	"Platforms/Impl/PipelineTemplate.cpp"
	"Platforms/Engine.Platforms.h"
	"Platforms/Soft/Windows/SwWinSurface.cpp"
	"Platforms/Soft/Windows/SwWinSurface.h"
	"Platforms/OpenGL/Windows/GLWinContext.cpp"
	"Platforms/OpenGL/Windows/GLWinContext.h"
	"Platforms/OpenGL/Windows/GLWinLibrary.cpp"
	"Platforms/OpenGL/Windows/GLWinLibrary.h"
	"Platforms/Soft/Android/SwAndSurface.cpp"
	"Platforms/Soft/Android/SwAndSurface.h"
	"Platforms/Public/Common.h"
	"Platforms/Public/Tools/AsyncCommandsEmulator.h"
	"Platforms/Public/Tools/GPUThreadHelper.cpp"
	"Platforms/Public/Tools/GPUThreadHelper.h"
	"Platforms/Public/Tools/ImageUtils.cpp"
	"Platforms/Public/Tools/ImageUtils.h"
	"Platforms/Public/Tools/ImageViewHashMap.h"
	"Platforms/Public/Tools/MemoryMapperHelper.cpp"
	"Platforms/Public/Tools/MemoryMapperHelper.h"
	"Platforms/Public/Tools/SamplerUtils.cpp"
	"Platforms/Public/Tools/SamplerUtils.h"
	"Platforms/Public/Tools/WindowHelper.cpp"
	"Platforms/Public/Tools/WindowHelper.h"
	"Platforms/OpenGL/450/gl4.cpp"
	"Platforms/OpenGL/450/gl4.h"
	"Platforms/OpenGL/450/GL4BaseModule.cpp"
	"Platforms/OpenGL/450/GL4BaseModule.h"
	"Platforms/OpenGL/450/GL4Buffer.cpp"
	"Platforms/OpenGL/450/GL4CommandBuffer.cpp"
	"Platforms/OpenGL/450/GL4CommandBuilder.cpp"
	"Platforms/OpenGL/450/GL4CommandQueue.cpp"
	"Platforms/OpenGL/450/GL4Device.cpp"
	"Platforms/OpenGL/450/GL4Device.h"
	"Platforms/OpenGL/450/GL4Emulator.h"
	"Platforms/OpenGL/450/GL4Enums.h"
	"Platforms/OpenGL/450/GL4FlippedSystemFramebuffer.h"
	"Platforms/OpenGL/450/GL4Framebuffer.cpp"
	"Platforms/OpenGL/450/GL4Image.cpp"
	"Platforms/OpenGL/450/GL4Memory.cpp"
	"Platforms/OpenGL/450/GL4Messages.h"
	"Platforms/OpenGL/450/GL4Pipeline.cpp"
	"Platforms/OpenGL/450/GL4PipelineResourceTable.cpp"
	"Platforms/OpenGL/450/GL4RenderPass.cpp"
	"Platforms/OpenGL/450/GL4ResourceCache.h"
	"Platforms/OpenGL/450/GL4Sampler.cpp"
	"Platforms/OpenGL/450/GL4SamplerCache.h"
	"Platforms/OpenGL/450/GL4SyncManager.cpp"
	"Platforms/OpenGL/450/GL4SystemFramebuffer.h"
	"Platforms/Public/OS/Display.h"
	"Platforms/Public/OS/IDs.h"
	"Platforms/Public/OS/Input.h"
	"Platforms/Public/OS/OSEnums.h"
	"Platforms/Public/OS/Platform.h"
	"Platforms/Public/OS/ScreenCoords.h"
	"Platforms/Public/OS/Window.h"
	"Platforms/Vulkan/Windows/VkWinSurface.cpp"
	"Platforms/Vulkan/Windows/VkWinSurface.h"
	"Platforms/VR/Emulator/EmulatorVRThread.cpp"
	"Platforms/VR/VRObjectsConstructor.cpp"
	"Platforms/VR/VRObjectsConstructor.h" )
add_library( "Engine.Platforms" STATIC ${SOURCES} )
source_group( "Vulkan\\Android" FILES "Platforms/Vulkan/Android/VkAndSurface.cpp" "Platforms/Vulkan/Android/VkAndSurface.h" )
source_group( "Soft" FILES "Platforms/Soft/SoftRendererContext.cpp" "Platforms/Soft/SoftRendererObjectsConstructor.h" "Platforms/Soft/SoftRendererThread.cpp" )
source_group( "SDL" FILES "Platforms/SDL/SDLDisplay.cpp" "Platforms/SDL/SDLDisplay.h" "Platforms/SDL/SDLKeyInput.cpp" "Platforms/SDL/SDLMessages.h" "Platforms/SDL/SDLMouseInput.cpp" "Platforms/SDL/SDLObjectsConstructor.h" "Platforms/SDL/SDLPlatform.cpp" "Platforms/SDL/SDLWindow.cpp" )
source_group( "OpenGL" FILES "Platforms/OpenGL/OpenGLContext.cpp" "Platforms/OpenGL/OpenGLObjectsConstructor.h" "Platforms/OpenGL/OpenGLThread.cpp" )
source_group( "OpenCL\\120" FILES "Platforms/OpenCL/120/CL1BaseModule.cpp" "Platforms/OpenCL/120/CL1BaseModule.h" "Platforms/OpenCL/120/CL1Buffer.cpp" "Platforms/OpenCL/120/CL1CommandBuffer.cpp" "Platforms/OpenCL/120/CL1CommandBuilder.cpp" "Platforms/OpenCL/120/CL1CommandQueue.cpp" "Platforms/OpenCL/120/CL1Device.cpp" "Platforms/OpenCL/120/CL1Device.h" "Platforms/OpenCL/120/CL1Emulator.cpp" "Platforms/OpenCL/120/CL1Emulator.h" "Platforms/OpenCL/120/CL1Enums.h" "Platforms/OpenCL/120/CL1Image.cpp" "Platforms/OpenCL/120/CL1Messages.h" "Platforms/OpenCL/120/CL1Pipeline.cpp" "Platforms/OpenCL/120/CL1PipelineResourceTable.cpp" "Platforms/OpenCL/120/CL1ResourceCache.h" "Platforms/OpenCL/120/CL1Sampler.cpp" "Platforms/OpenCL/120/CL1SamplerCache.h" "Platforms/OpenCL/120/CL1SyncManager.cpp" "Platforms/OpenCL/120/cl2.cpp" "Platforms/OpenCL/120/cl2.h" )
source_group( "Vulkan\\110" FILES "Platforms/Vulkan/110/Vk1BaseModule.cpp" "Platforms/Vulkan/110/Vk1BaseModule.h" "Platforms/Vulkan/110/Vk1BaseObject.h" "Platforms/Vulkan/110/Vk1Buffer.cpp" "Platforms/Vulkan/110/Vk1CommandBuffer.cpp" "Platforms/Vulkan/110/Vk1CommandBuilder.cpp" "Platforms/Vulkan/110/Vk1CommandQueue.cpp" "Platforms/Vulkan/110/Vk1Device.cpp" "Platforms/Vulkan/110/Vk1Device.h" "Platforms/Vulkan/110/Vk1Enums.h" "Platforms/Vulkan/110/Vk1Framebuffer.cpp" "Platforms/Vulkan/110/Vk1Image.cpp" "Platforms/Vulkan/110/Vk1Library.h" "Platforms/Vulkan/110/Vk1ManagedMemory.cpp" "Platforms/Vulkan/110/Vk1MemoryManager.cpp" "Platforms/Vulkan/110/Vk1Messages.h" "Platforms/Vulkan/110/Vk1Pipeline.cpp" "Platforms/Vulkan/110/Vk1PipelineCache.cpp" "Platforms/Vulkan/110/Vk1PipelineCache.h" "Platforms/Vulkan/110/Vk1PipelineLayout.cpp" "Platforms/Vulkan/110/Vk1PipelineLayout.h" "Platforms/Vulkan/110/Vk1PipelineResourceTable.cpp" "Platforms/Vulkan/110/Vk1RenderPass.cpp" "Platforms/Vulkan/110/Vk1RenderPassCache.h" "Platforms/Vulkan/110/Vk1ResourceCache.h" "Platforms/Vulkan/110/Vk1Sampler.cpp" "Platforms/Vulkan/110/Vk1SamplerCache.h" "Platforms/Vulkan/110/Vk1SwapchainImage.h" "Platforms/Vulkan/110/Vk1SyncManager.cpp" "Platforms/Vulkan/110/vulkan1.cpp" "Platforms/Vulkan/110/vulkan1.h" "Platforms/Vulkan/110/vulkan1_platform.cpp" "Platforms/Vulkan/110/vulkan1_platform.h" "Platforms/Vulkan/110/vulkan1_utils.h" )
source_group( "OpenGL\\300es" FILES "Platforms/OpenGL/300es/gles3.cpp" "Platforms/OpenGL/300es/gles3.h" )
source_group( "OpenCL" FILES "Platforms/OpenCL/OpenCLContext.cpp" "Platforms/OpenCL/OpenCLObjectsConstructor.h" "Platforms/OpenCL/OpenCLThread.cpp" )
source_group( "Input" FILES "Platforms/Input/InputManager.cpp" "Platforms/Input/InputManager.h" "Platforms/Input/InputThread.cpp" )
source_group( "Soft\\ShaderLang" FILES "Platforms/Soft/ShaderLang/gen_Image.h" "Platforms/Soft/ShaderLang/gen_Texture.h" "Platforms/Soft/ShaderLang/SWLang.h" "Platforms/Soft/ShaderLang/SWLangArray.h" "Platforms/Soft/ShaderLang/SWLangBarrier.h" "Platforms/Soft/ShaderLang/SWLangBuffer.h" "Platforms/Soft/ShaderLang/SWLangCommon.h" "Platforms/Soft/ShaderLang/SWLangGLM.h" "Platforms/Soft/ShaderLang/SWLangImage.cpp" "Platforms/Soft/ShaderLang/SWLangImage.h" "Platforms/Soft/ShaderLang/SWLangShared.h" "Platforms/Soft/ShaderLang/SWLangTexture.cpp" "Platforms/Soft/ShaderLang/SWLangTexture.h" "Platforms/Soft/ShaderLang/SWLangTexture2D.h" "Platforms/Soft/ShaderLang/SWShaderHelper.h" )
source_group( "Vulkan" FILES "Platforms/Vulkan/VulkanContext.cpp" "Platforms/Vulkan/VulkanObjectsConstructor.h" "Platforms/Vulkan/VulkanThread.cpp" )
source_group( "Soft\\Impl" FILES "Platforms/Soft/Impl/SWBaseModule.cpp" "Platforms/Soft/Impl/SWBaseModule.h" "Platforms/Soft/Impl/SWBuffer.cpp" "Platforms/Soft/Impl/SWCommandBuffer.cpp" "Platforms/Soft/Impl/SWCommandBuilder.cpp" "Platforms/Soft/Impl/SWCommandQueue.cpp" "Platforms/Soft/Impl/SWDevice.cpp" "Platforms/Soft/Impl/SWDevice.h" "Platforms/Soft/Impl/SWDeviceProperties.h" "Platforms/Soft/Impl/SWEnums.h" "Platforms/Soft/Impl/SWImage.cpp" "Platforms/Soft/Impl/SWMemory.cpp" "Platforms/Soft/Impl/SWMessages.h" "Platforms/Soft/Impl/SWPipeline.cpp" "Platforms/Soft/Impl/SWPipelineResourceTable.cpp" "Platforms/Soft/Impl/SWSampler.cpp" "Platforms/Soft/Impl/SWSamplerCache.h" "Platforms/Soft/Impl/SWShaderModel.cpp" "Platforms/Soft/Impl/SWShaderModel.h" "Platforms/Soft/Impl/SWSyncManager.cpp" "Platforms/Soft/Impl/SWSyncObjects.h" )
source_group( "Windows" FILES "Platforms/Windows/WinDisplay.cpp" "Platforms/Windows/WinDisplay.h" "Platforms/Windows/WinKeyInput.cpp" "Platforms/Windows/WinMessages.h" "Platforms/Windows/WinMouseInput.cpp" "Platforms/Windows/WinObjectsConstructor.h" "Platforms/Windows/WinPlatform.cpp" "Platforms/Windows/WinWindow.cpp" )
source_group( "Public\\GPU" FILES "Platforms/Public/GPU/Buffer.h" "Platforms/Public/GPU/BufferEnums.h" "Platforms/Public/GPU/CommandBuffer.h" "Platforms/Public/GPU/CommandEnums.h" "Platforms/Public/GPU/Context.cpp" "Platforms/Public/GPU/Context.h" "Platforms/Public/GPU/Enums.ToString.h" "Platforms/Public/GPU/FragmentOutputState.h" "Platforms/Public/GPU/Framebuffer.h" "Platforms/Public/GPU/IDs.h" "Platforms/Public/GPU/Image.cpp" "Platforms/Public/GPU/Image.h" "Platforms/Public/GPU/ImageEnums.h" "Platforms/Public/GPU/ImageLayer.h" "Platforms/Public/GPU/ImageSwizzle.h" "Platforms/Public/GPU/Memory.h" "Platforms/Public/GPU/MemoryEnums.h" "Platforms/Public/GPU/MipmapLevel.h" "Platforms/Public/GPU/MultiSamples.h" "Platforms/Public/GPU/ObjectEnums.h" "Platforms/Public/GPU/Pipeline.cpp" "Platforms/Public/GPU/Pipeline.h" "Platforms/Public/GPU/PipelineLayout.cpp" "Platforms/Public/GPU/PipelineLayout.h" "Platforms/Public/GPU/PixelFormatEnums.h" "Platforms/Public/GPU/RenderPass.cpp" "Platforms/Public/GPU/RenderPass.h" "Platforms/Public/GPU/RenderPassEnums.h" "Platforms/Public/GPU/RenderState.cpp" "Platforms/Public/GPU/RenderState.h" "Platforms/Public/GPU/RenderStateEnums.h" "Platforms/Public/GPU/Sampler.cpp" "Platforms/Public/GPU/Sampler.h" "Platforms/Public/GPU/SamplerEnums.h" "Platforms/Public/GPU/ShaderEnums.h" "Platforms/Public/GPU/Sync.h" "Platforms/Public/GPU/Thread.h" "Platforms/Public/GPU/VertexAttribs.h" "Platforms/Public/GPU/VertexDescr.h" "Platforms/Public/GPU/VertexEnums.h" "Platforms/Public/GPU/VertexInputState.cpp" "Platforms/Public/GPU/VertexInputState.h" "Platforms/Public/GPU/VR.h" )
source_group( "Impl" FILES "Platforms/Impl/Main.cpp" "Platforms/Impl/PipelineTemplate.cpp" )
source_group( "" FILES "Platforms/Engine.Platforms.h" )
source_group( "Soft\\Windows" FILES "Platforms/Soft/Windows/SwWinSurface.cpp" "Platforms/Soft/Windows/SwWinSurface.h" )
source_group( "OpenGL\\Windows" FILES "Platforms/OpenGL/Windows/GLWinContext.cpp" "Platforms/OpenGL/Windows/GLWinContext.h" "Platforms/OpenGL/Windows/GLWinLibrary.cpp" "Platforms/OpenGL/Windows/GLWinLibrary.h" )
source_group( "Soft\\Android" FILES "Platforms/Soft/Android/SwAndSurface.cpp" "Platforms/Soft/Android/SwAndSurface.h" )
source_group( "Public" FILES "Platforms/Public/Common.h" )
source_group( "Public\\Tools" FILES "Platforms/Public/Tools/AsyncCommandsEmulator.h" "Platforms/Public/Tools/GPUThreadHelper.cpp" "Platforms/Public/Tools/GPUThreadHelper.h" "Platforms/Public/Tools/ImageUtils.cpp" "Platforms/Public/Tools/ImageUtils.h" "Platforms/Public/Tools/ImageViewHashMap.h" "Platforms/Public/Tools/MemoryMapperHelper.cpp" "Platforms/Public/Tools/MemoryMapperHelper.h" "Platforms/Public/Tools/SamplerUtils.cpp" "Platforms/Public/Tools/SamplerUtils.h" "Platforms/Public/Tools/WindowHelper.cpp" "Platforms/Public/Tools/WindowHelper.h" )
source_group( "OpenGL\\450" FILES "Platforms/OpenGL/450/gl4.cpp" "Platforms/OpenGL/450/gl4.h" "Platforms/OpenGL/450/GL4BaseModule.cpp" "Platforms/OpenGL/450/GL4BaseModule.h" "Platforms/OpenGL/450/GL4Buffer.cpp" "Platforms/OpenGL/450/GL4CommandBuffer.cpp" "Platforms/OpenGL/450/GL4CommandBuilder.cpp" "Platforms/OpenGL/450/GL4CommandQueue.cpp" "Platforms/OpenGL/450/GL4Device.cpp" "Platforms/OpenGL/450/GL4Device.h" "Platforms/OpenGL/450/GL4Emulator.h" "Platforms/OpenGL/450/GL4Enums.h" "Platforms/OpenGL/450/GL4FlippedSystemFramebuffer.h" "Platforms/OpenGL/450/GL4Framebuffer.cpp" "Platforms/OpenGL/450/GL4Image.cpp" "Platforms/OpenGL/450/GL4Memory.cpp" "Platforms/OpenGL/450/GL4Messages.h" "Platforms/OpenGL/450/GL4Pipeline.cpp" "Platforms/OpenGL/450/GL4PipelineResourceTable.cpp" "Platforms/OpenGL/450/GL4RenderPass.cpp" "Platforms/OpenGL/450/GL4ResourceCache.h" "Platforms/OpenGL/450/GL4Sampler.cpp" "Platforms/OpenGL/450/GL4SamplerCache.h" "Platforms/OpenGL/450/GL4SyncManager.cpp" "Platforms/OpenGL/450/GL4SystemFramebuffer.h" )
source_group( "Public\\OS" FILES "Platforms/Public/OS/Display.h" "Platforms/Public/OS/IDs.h" "Platforms/Public/OS/Input.h" "Platforms/Public/OS/OSEnums.h" "Platforms/Public/OS/Platform.h" "Platforms/Public/OS/ScreenCoords.h" "Platforms/Public/OS/Window.h" )
source_group( "Vulkan\\Windows" FILES "Platforms/Vulkan/Windows/VkWinSurface.cpp" "Platforms/Vulkan/Windows/VkWinSurface.h" )
source_group( "VR\\Emulator" FILES "Platforms/VR/Emulator/EmulatorVRThread.cpp" )
source_group( "VR" FILES "Platforms/VR/VRObjectsConstructor.cpp" "Platforms/VR/VRObjectsConstructor.h" )
set_property( TARGET "Engine.Platforms" PROPERTY FOLDER "Engine" )
target_include_directories( "Engine.Platforms" PUBLIC "C:/Projects/ModularGraphicsFramework/External" )
target_include_directories( "Engine.Platforms" PUBLIC "${EXTERNALS_PATH}" )
target_include_directories( "Engine.Platforms" PUBLIC "C:/Projects/ModularGraphicsFramework/Core/.." )
if (WIN32)
	target_link_libraries( "Engine.Platforms" "dxguid.lib" )
endif()
if (WIN32)
	target_link_libraries( "Engine.Platforms" "opengl32.lib" )
endif()
target_link_libraries( "Engine.Platforms" "Engine.Base" )
if ((MSVC AND WIN32))
	target_link_libraries( "Engine.Platforms" "Shcore.lib" )
endif()
if (WIN32)
	target_link_libraries( "Engine.Platforms" "Dxva2.lib" )
endif()
if (WIN32)
	target_link_libraries( "Engine.Platforms" "dxgi.lib" )
endif()
if (DEFINED OPENGLES_VERSION AND NOT (OPENGLES_VERSION EQUAL "0"))
	target_compile_definitions( "Engine.Platforms" PUBLIC "GRAPHICS_API_OPENGLES=${OPENGLES_VERSION}" )
endif()
if (DEFINED VULKAN_VERSION AND NOT (VULKAN_VERSION EQUAL "0"))
	target_compile_definitions( "Engine.Platforms" PUBLIC "GRAPHICS_API_VULKAN=${VULKAN_VERSION}" )
endif()
if (DEFINED OPENCL_VERSION AND NOT (OPENCL_VERSION EQUAL "0"))
	target_compile_definitions( "Engine.Platforms" PUBLIC "COMPUTE_API_OPENCL=${OPENCL_VERSION}" )
endif()
if (DEFINED SOFTRENDER_VERSION AND NOT (SOFTRENDER_VERSION EQUAL "0"))
	target_compile_definitions( "Engine.Platforms" PUBLIC "GRAPHICS_API_SOFT=${SOFTRENDER_VERSION}" )
endif()
if (DEFINED OPENGL_VERSION AND NOT (OPENGL_VERSION EQUAL "0"))
	target_compile_definitions( "Engine.Platforms" PUBLIC "GRAPHICS_API_OPENGL=${OPENGL_VERSION}" )
endif()
# compiler
set_target_properties( "Engine.Platforms" PROPERTIES LINK_FLAGS_RELEASE ${PROJECTS_SHARED_LINKER_FLAGS_RELEASE} )
set_target_properties( "Engine.Platforms" PROPERTIES LINK_FLAGS_PROFILE ${PROJECTS_SHARED_LINKER_FLAGS_PROFILE} )
target_compile_definitions( "Engine.Platforms" PRIVATE $<$<CONFIG:Debug>: ${PROJECTS_SHARED_DEFINES_DEBUG}> )
set_target_properties( "Engine.Platforms" PROPERTIES LINK_FLAGS_DEBUGANALYZE ${PROJECTS_SHARED_LINKER_FLAGS_DEBUGANALYZE} )
target_compile_definitions( "Engine.Platforms" PRIVATE $<$<CONFIG:DebugAnalyze>: ${PROJECTS_SHARED_DEFINES_DEBUGANALYZE}> )
target_compile_options( "Engine.Platforms" PRIVATE $<$<CONFIG:Profile>: ${PROJECTS_SHARED_CXX_FLAGS_PROFILE}> )
target_compile_options( "Engine.Platforms" PRIVATE $<$<CONFIG:Release>: ${PROJECTS_SHARED_CXX_FLAGS_RELEASE}> )
target_compile_definitions( "Engine.Platforms" PRIVATE $<$<CONFIG:Profile>: ${PROJECTS_SHARED_DEFINES_PROFILE}> )
target_compile_definitions( "Engine.Platforms" PRIVATE $<$<CONFIG:Release>: ${PROJECTS_SHARED_DEFINES_RELEASE}> )
set_target_properties( "Engine.Platforms" PROPERTIES LINK_FLAGS_DEBUG ${PROJECTS_SHARED_LINKER_FLAGS_DEBUG} )
target_compile_options( "Engine.Platforms" PRIVATE $<$<CONFIG:DebugAnalyze>: ${PROJECTS_SHARED_CXX_FLAGS_DEBUGANALYZE}> )
target_compile_options( "Engine.Platforms" PRIVATE $<$<CONFIG:Debug>: ${PROJECTS_SHARED_CXX_FLAGS_DEBUG}> )



#==================================================================================================
# project: Engine.Graphics
#==================================================================================================
set( SOURCES 
	"Graphics/Impl/AsyncCommandBuffer.cpp"
	"Graphics/Impl/BatchRenderer.cpp"
	"Graphics/Impl/CommandBufferManager.cpp"
	"Graphics/Impl/Font.cpp"
	"Graphics/Impl/FontManager.cpp"
	"Graphics/Impl/GraphicsBaseModule.cpp"
	"Graphics/Impl/GraphicsBaseModule.h"
	"Graphics/Impl/GraphicsObjectsConstructor.h"
	"Graphics/Impl/Main.cpp"
	"Graphics/Impl/MemoryInputStream.cpp"
	"Graphics/Impl/MemoryOutputStream.cpp"
	"Graphics/Impl/TextureLineAtlas.h"
	"Graphics/Engine.Graphics.h"
	"Graphics/Public/BatchRenderer.h"
	"Graphics/Public/Commands.h"
	"Graphics/Public/Common.h"
	"Graphics/Public/Font.h"
	"Graphics/Public/IDs.h"
	"Graphics/Public/MemStream.h"
	"Graphics/Public/Primitives.h"
	"Graphics/Public/Vertices.h" )
add_library( "Engine.Graphics" STATIC ${SOURCES} )
source_group( "Impl" FILES "Graphics/Impl/AsyncCommandBuffer.cpp" "Graphics/Impl/BatchRenderer.cpp" "Graphics/Impl/CommandBufferManager.cpp" "Graphics/Impl/Font.cpp" "Graphics/Impl/FontManager.cpp" "Graphics/Impl/GraphicsBaseModule.cpp" "Graphics/Impl/GraphicsBaseModule.h" "Graphics/Impl/GraphicsObjectsConstructor.h" "Graphics/Impl/Main.cpp" "Graphics/Impl/MemoryInputStream.cpp" "Graphics/Impl/MemoryOutputStream.cpp" "Graphics/Impl/TextureLineAtlas.h" )
source_group( "" FILES "Graphics/Engine.Graphics.h" )
source_group( "Public" FILES "Graphics/Public/BatchRenderer.h" "Graphics/Public/Commands.h" "Graphics/Public/Common.h" "Graphics/Public/Font.h" "Graphics/Public/IDs.h" "Graphics/Public/MemStream.h" "Graphics/Public/Primitives.h" "Graphics/Public/Vertices.h" )
set_property( TARGET "Engine.Graphics" PROPERTY FOLDER "Engine" )
target_include_directories( "Engine.Graphics" PUBLIC "C:/Projects/ModularGraphicsFramework/External" )
target_include_directories( "Engine.Graphics" PUBLIC "${EXTERNALS_PATH}" )
target_include_directories( "Engine.Graphics" PUBLIC "C:/Projects/ModularGraphicsFramework/Core/.." )
target_link_libraries( "Engine.Graphics" "Engine.Platforms" )
# compiler
set_target_properties( "Engine.Graphics" PROPERTIES LINK_FLAGS_RELEASE ${PROJECTS_SHARED_LINKER_FLAGS_RELEASE} )
set_target_properties( "Engine.Graphics" PROPERTIES LINK_FLAGS_PROFILE ${PROJECTS_SHARED_LINKER_FLAGS_PROFILE} )
target_compile_definitions( "Engine.Graphics" PRIVATE $<$<CONFIG:Debug>: ${PROJECTS_SHARED_DEFINES_DEBUG}> )
set_target_properties( "Engine.Graphics" PROPERTIES LINK_FLAGS_DEBUGANALYZE ${PROJECTS_SHARED_LINKER_FLAGS_DEBUGANALYZE} )
target_compile_definitions( "Engine.Graphics" PRIVATE $<$<CONFIG:DebugAnalyze>: ${PROJECTS_SHARED_DEFINES_DEBUGANALYZE}> )
target_compile_options( "Engine.Graphics" PRIVATE $<$<CONFIG:Profile>: ${PROJECTS_SHARED_CXX_FLAGS_PROFILE}> )
target_compile_options( "Engine.Graphics" PRIVATE $<$<CONFIG:Release>: ${PROJECTS_SHARED_CXX_FLAGS_RELEASE}> )
target_compile_definitions( "Engine.Graphics" PRIVATE $<$<CONFIG:Profile>: ${PROJECTS_SHARED_DEFINES_PROFILE}> )
target_compile_definitions( "Engine.Graphics" PRIVATE $<$<CONFIG:Release>: ${PROJECTS_SHARED_DEFINES_RELEASE}> )
set_target_properties( "Engine.Graphics" PROPERTIES LINK_FLAGS_DEBUG ${PROJECTS_SHARED_LINKER_FLAGS_DEBUG} )
target_compile_options( "Engine.Graphics" PRIVATE $<$<CONFIG:DebugAnalyze>: ${PROJECTS_SHARED_CXX_FLAGS_DEBUGANALYZE}> )
target_compile_options( "Engine.Graphics" PRIVATE $<$<CONFIG:Debug>: ${PROJECTS_SHARED_CXX_FLAGS_DEBUG}> )



#==================================================================================================
# project: Engine.Profilers
#==================================================================================================
set( SOURCES 
	"Profilers/Impl/FPSCounter.cpp"
	"Profilers/Impl/Main.cpp"
	"Profilers/Impl/ProfilerObjectsConstructor.h"
	"Profilers/Engine.Profilers.h"
	"Profilers/Public/Common.h"
	"Profilers/Public/GpuStatistic.h"
	"Profilers/Public/IDs.h" )
add_library( "Engine.Profilers" STATIC ${SOURCES} )
source_group( "Impl" FILES "Profilers/Impl/FPSCounter.cpp" "Profilers/Impl/Main.cpp" "Profilers/Impl/ProfilerObjectsConstructor.h" )
source_group( "" FILES "Profilers/Engine.Profilers.h" )
source_group( "Public" FILES "Profilers/Public/Common.h" "Profilers/Public/GpuStatistic.h" "Profilers/Public/IDs.h" )
set_property( TARGET "Engine.Profilers" PROPERTY FOLDER "Engine" )
target_include_directories( "Engine.Profilers" PUBLIC "C:/Projects/ModularGraphicsFramework/External" )
target_include_directories( "Engine.Profilers" PUBLIC "${EXTERNALS_PATH}" )
target_include_directories( "Engine.Profilers" PUBLIC "C:/Projects/ModularGraphicsFramework/Core/.." )
target_link_libraries( "Engine.Profilers" "Engine.Platforms" )
# compiler
set_target_properties( "Engine.Profilers" PROPERTIES LINK_FLAGS_RELEASE ${PROJECTS_SHARED_LINKER_FLAGS_RELEASE} )
set_target_properties( "Engine.Profilers" PROPERTIES LINK_FLAGS_PROFILE ${PROJECTS_SHARED_LINKER_FLAGS_PROFILE} )
target_compile_definitions( "Engine.Profilers" PRIVATE $<$<CONFIG:Debug>: ${PROJECTS_SHARED_DEFINES_DEBUG}> )
set_target_properties( "Engine.Profilers" PROPERTIES LINK_FLAGS_DEBUGANALYZE ${PROJECTS_SHARED_LINKER_FLAGS_DEBUGANALYZE} )
target_compile_definitions( "Engine.Profilers" PRIVATE $<$<CONFIG:DebugAnalyze>: ${PROJECTS_SHARED_DEFINES_DEBUGANALYZE}> )
target_compile_options( "Engine.Profilers" PRIVATE $<$<CONFIG:Profile>: ${PROJECTS_SHARED_CXX_FLAGS_PROFILE}> )
target_compile_options( "Engine.Profilers" PRIVATE $<$<CONFIG:Release>: ${PROJECTS_SHARED_CXX_FLAGS_RELEASE}> )
target_compile_definitions( "Engine.Profilers" PRIVATE $<$<CONFIG:Profile>: ${PROJECTS_SHARED_DEFINES_PROFILE}> )
target_compile_definitions( "Engine.Profilers" PRIVATE $<$<CONFIG:Release>: ${PROJECTS_SHARED_DEFINES_RELEASE}> )
set_target_properties( "Engine.Profilers" PROPERTIES LINK_FLAGS_DEBUG ${PROJECTS_SHARED_LINKER_FLAGS_DEBUG} )
target_compile_options( "Engine.Profilers" PRIVATE $<$<CONFIG:DebugAnalyze>: ${PROJECTS_SHARED_CXX_FLAGS_DEBUGANALYZE}> )
target_compile_options( "Engine.Profilers" PRIVATE $<$<CONFIG:Debug>: ${PROJECTS_SHARED_CXX_FLAGS_DEBUG}> )



#==================================================================================================
# project: Engine.ImportExport
#==================================================================================================
set( SOURCES 
	"ImportExport/Impl/GX_ImageFormat.h"
	"ImportExport/Impl/GX_ImageLoader.cpp"
	"ImportExport/Impl/GX_ImageSaver.cpp"
	"ImportExport/Impl/ImportExportObjectsConstructor.h"
	"ImportExport/Impl/Main.cpp"
	"ImportExport/Engine.ImportExport.h"
	"ImportExport/README.md"
	"ImportExport/Public/Common.h"
	"ImportExport/Public/IDs.h"
	"ImportExport/Public/Image.h" )
add_library( "Engine.ImportExport" STATIC ${SOURCES} )
source_group( "Impl" FILES "ImportExport/Impl/GX_ImageFormat.h" "ImportExport/Impl/GX_ImageLoader.cpp" "ImportExport/Impl/GX_ImageSaver.cpp" "ImportExport/Impl/ImportExportObjectsConstructor.h" "ImportExport/Impl/Main.cpp" )
source_group( "" FILES "ImportExport/Engine.ImportExport.h" "ImportExport/README.md" )
source_group( "Public" FILES "ImportExport/Public/Common.h" "ImportExport/Public/IDs.h" "ImportExport/Public/Image.h" )
set_property( TARGET "Engine.ImportExport" PROPERTY FOLDER "Engine" )
target_include_directories( "Engine.ImportExport" PUBLIC "C:/Projects/ModularGraphicsFramework/External" )
target_include_directories( "Engine.ImportExport" PUBLIC "${EXTERNALS_PATH}" )
target_include_directories( "Engine.ImportExport" PUBLIC "C:/Projects/ModularGraphicsFramework/Core/.." )
target_link_libraries( "Engine.ImportExport" "Engine.Graphics" )
# compiler
set_target_properties( "Engine.ImportExport" PROPERTIES LINK_FLAGS_RELEASE ${PROJECTS_SHARED_LINKER_FLAGS_RELEASE} )
set_target_properties( "Engine.ImportExport" PROPERTIES LINK_FLAGS_PROFILE ${PROJECTS_SHARED_LINKER_FLAGS_PROFILE} )
target_compile_definitions( "Engine.ImportExport" PRIVATE $<$<CONFIG:Debug>: ${PROJECTS_SHARED_DEFINES_DEBUG}> )
set_target_properties( "Engine.ImportExport" PROPERTIES LINK_FLAGS_DEBUGANALYZE ${PROJECTS_SHARED_LINKER_FLAGS_DEBUGANALYZE} )
target_compile_definitions( "Engine.ImportExport" PRIVATE $<$<CONFIG:DebugAnalyze>: ${PROJECTS_SHARED_DEFINES_DEBUGANALYZE}> )
target_compile_options( "Engine.ImportExport" PRIVATE $<$<CONFIG:Profile>: ${PROJECTS_SHARED_CXX_FLAGS_PROFILE}> )
target_compile_options( "Engine.ImportExport" PRIVATE $<$<CONFIG:Release>: ${PROJECTS_SHARED_CXX_FLAGS_RELEASE}> )
target_compile_definitions( "Engine.ImportExport" PRIVATE $<$<CONFIG:Profile>: ${PROJECTS_SHARED_DEFINES_PROFILE}> )
target_compile_definitions( "Engine.ImportExport" PRIVATE $<$<CONFIG:Release>: ${PROJECTS_SHARED_DEFINES_RELEASE}> )
set_target_properties( "Engine.ImportExport" PROPERTIES LINK_FLAGS_DEBUG ${PROJECTS_SHARED_LINKER_FLAGS_DEBUG} )
target_compile_options( "Engine.ImportExport" PRIVATE $<$<CONFIG:DebugAnalyze>: ${PROJECTS_SHARED_CXX_FLAGS_DEBUGANALYZE}> )
target_compile_options( "Engine.ImportExport" PRIVATE $<$<CONFIG:Debug>: ${PROJECTS_SHARED_CXX_FLAGS_DEBUG}> )



#==================================================================================================
# project: Engine.Scene
#==================================================================================================
set( SOURCES 
	"Scene/Surface/OffscreenSurface.cpp"
	"Scene/Surface/VideoRecorderSurface.cpp"
	"Scene/Surface/VRSurface.cpp"
	"Scene/Surface/WindowSurface.cpp"
	"Scene/Renderer/SceneRendererMainThread.cpp"
	"Scene/Camera/FreeCamera.cpp"
	"Scene/Camera/FreeVRCamera.cpp"
	"Scene/Impl/BaseSceneModule.cpp"
	"Scene/Impl/BaseSceneModule.h"
	"Scene/Impl/Main.cpp"
	"Scene/Impl/SceneMainThread.cpp"
	"Scene/Impl/SceneManager.cpp"
	"Scene/Impl/SceneObjectConstructor.h"
	"Scene/Engine.Scene.h"
	"Scene/Physics/ScenePhysicsMainThread.cpp"
	"Scene/Public/Camera.h"
	"Scene/Public/Common.h"
	"Scene/Public/IDs.h"
	"Scene/Public/Scene.h"
	"Scene/Public/Surface.h"
	"Scene/Logic/SceneLogicMainThread.cpp" )
add_library( "Engine.Scene" STATIC ${SOURCES} )
source_group( "Surface" FILES "Scene/Surface/OffscreenSurface.cpp" "Scene/Surface/VideoRecorderSurface.cpp" "Scene/Surface/VRSurface.cpp" "Scene/Surface/WindowSurface.cpp" )
source_group( "Renderer" FILES "Scene/Renderer/SceneRendererMainThread.cpp" )
source_group( "Camera" FILES "Scene/Camera/FreeCamera.cpp" "Scene/Camera/FreeVRCamera.cpp" )
source_group( "Impl" FILES "Scene/Impl/BaseSceneModule.cpp" "Scene/Impl/BaseSceneModule.h" "Scene/Impl/Main.cpp" "Scene/Impl/SceneMainThread.cpp" "Scene/Impl/SceneManager.cpp" "Scene/Impl/SceneObjectConstructor.h" )
source_group( "" FILES "Scene/Engine.Scene.h" )
source_group( "Physics" FILES "Scene/Physics/ScenePhysicsMainThread.cpp" )
source_group( "Public" FILES "Scene/Public/Camera.h" "Scene/Public/Common.h" "Scene/Public/IDs.h" "Scene/Public/Scene.h" "Scene/Public/Surface.h" )
source_group( "Logic" FILES "Scene/Logic/SceneLogicMainThread.cpp" )
set_property( TARGET "Engine.Scene" PROPERTY FOLDER "Engine" )
target_include_directories( "Engine.Scene" PUBLIC "C:/Projects/ModularGraphicsFramework/External" )
target_include_directories( "Engine.Scene" PUBLIC "${EXTERNALS_PATH}" )
target_include_directories( "Engine.Scene" PUBLIC "C:/Projects/ModularGraphicsFramework/Core/.." )
target_link_libraries( "Engine.Scene" "Engine.Graphics" )
# compiler
set_target_properties( "Engine.Scene" PROPERTIES LINK_FLAGS_RELEASE ${PROJECTS_SHARED_LINKER_FLAGS_RELEASE} )
set_target_properties( "Engine.Scene" PROPERTIES LINK_FLAGS_PROFILE ${PROJECTS_SHARED_LINKER_FLAGS_PROFILE} )
target_compile_definitions( "Engine.Scene" PRIVATE $<$<CONFIG:Debug>: ${PROJECTS_SHARED_DEFINES_DEBUG}> )
set_target_properties( "Engine.Scene" PROPERTIES LINK_FLAGS_DEBUGANALYZE ${PROJECTS_SHARED_LINKER_FLAGS_DEBUGANALYZE} )
target_compile_definitions( "Engine.Scene" PRIVATE $<$<CONFIG:DebugAnalyze>: ${PROJECTS_SHARED_DEFINES_DEBUGANALYZE}> )
target_compile_options( "Engine.Scene" PRIVATE $<$<CONFIG:Profile>: ${PROJECTS_SHARED_CXX_FLAGS_PROFILE}> )
target_compile_options( "Engine.Scene" PRIVATE $<$<CONFIG:Release>: ${PROJECTS_SHARED_CXX_FLAGS_RELEASE}> )
target_compile_definitions( "Engine.Scene" PRIVATE $<$<CONFIG:Profile>: ${PROJECTS_SHARED_DEFINES_PROFILE}> )
target_compile_definitions( "Engine.Scene" PRIVATE $<$<CONFIG:Release>: ${PROJECTS_SHARED_DEFINES_RELEASE}> )
set_target_properties( "Engine.Scene" PROPERTIES LINK_FLAGS_DEBUG ${PROJECTS_SHARED_LINKER_FLAGS_DEBUG} )
target_compile_options( "Engine.Scene" PRIVATE $<$<CONFIG:DebugAnalyze>: ${PROJECTS_SHARED_CXX_FLAGS_DEBUGANALYZE}> )
target_compile_options( "Engine.Scene" PRIVATE $<$<CONFIG:Debug>: ${PROJECTS_SHARED_CXX_FLAGS_DEBUG}> )



#==================================================================================================
# project: Engine.PipelineCompiler
#==================================================================================================
set( SOURCES 
	"PipelineCompiler/Serializers/AngelScriptSerializer.cpp"
	"PipelineCompiler/Serializers/AngelScriptSerializer.h"
	"PipelineCompiler/Serializers/CppSerializer.cpp"
	"PipelineCompiler/Serializers/CppSerializer.h"
	"PipelineCompiler/Serializers/ISerializer.h"
	"PipelineCompiler/Common/ArraySize.h"
	"PipelineCompiler/Common/Common.h"
	"PipelineCompiler/Common/Packing.h"
	"PipelineCompiler/Common/ShaderEnums.h"
	"PipelineCompiler/Common/ToGLSL.h"
	"PipelineCompiler/Pipelines/BasePipeline.cpp"
	"PipelineCompiler/Pipelines/BasePipeline.h"
	"PipelineCompiler/Pipelines/BasePipeline_Pass1.cpp"
	"PipelineCompiler/Pipelines/BasePipeline_Pass2.cpp"
	"PipelineCompiler/Pipelines/ComputePipeline.cpp"
	"PipelineCompiler/Pipelines/ComputePipeline.h"
	"PipelineCompiler/Pipelines/GraphicsPipeline.cpp"
	"PipelineCompiler/Pipelines/GraphicsPipeline.h"
	"PipelineCompiler/Pipelines/PipelineManager.cpp"
	"PipelineCompiler/Pipelines/PipelineManager.h"
	"PipelineCompiler/README.md"
	"PipelineCompiler/Shaders/DeserializedShader.cpp"
	"PipelineCompiler/Shaders/DeserializedShader.h"
	"PipelineCompiler/Shaders/glslang_Include.h"
	"PipelineCompiler/Shaders/ShaderCompiler.cpp"
	"PipelineCompiler/Shaders/ShaderCompiler.h"
	"PipelineCompiler/Shaders/ShaderCompiler_CLCompiler.cpp"
	"PipelineCompiler/Shaders/ShaderCompiler_CLTranslator.cpp"
	"PipelineCompiler/Shaders/ShaderCompiler_CPPTranslator.cpp"
	"PipelineCompiler/Shaders/ShaderCompiler_GLCompiler.cpp"
	"PipelineCompiler/Shaders/ShaderCompiler_GLParser.cpp"
	"PipelineCompiler/Shaders/ShaderCompiler_GLSLTranslator.cpp"
	"PipelineCompiler/Shaders/ShaderCompiler_HLSLCompiler.cpp"
	"PipelineCompiler/Shaders/ShaderCompiler_HLSLTranslator.cpp"
	"PipelineCompiler/Shaders/ShaderCompiler_Includer.cpp"
	"PipelineCompiler/Shaders/ShaderCompiler_Includer.h"
	"PipelineCompiler/Shaders/ShaderCompiler_NameValidator.cpp"
	"PipelineCompiler/Shaders/ShaderCompiler_NameValidator.h"
	"PipelineCompiler/Shaders/ShaderCompiler_ReplaceTypes.cpp"
	"PipelineCompiler/Shaders/ShaderCompiler_Translator.cpp"
	"PipelineCompiler/Shaders/ShaderCompiler_Translator.h"
	"PipelineCompiler/Shaders/ShaderCompiler_Utils.h"
	"PipelineCompiler/Shaders/ShaderCompiler_VkCompiler.cpp"
	"PipelineCompiler/cl/cl_source_vfs.cpp"
	"PipelineCompiler/cl/cl_source_vfs.h"
	"PipelineCompiler/cl/funcs.cpp"
	"PipelineCompiler/cl/matrix.cpp"
	"PipelineCompiler/cl/vload.cpp"
	"PipelineCompiler/hlsl/hlsl_source_vfs.cpp"
	"PipelineCompiler/hlsl/hlsl_source_vfs.h"
	"PipelineCompiler/hlsl/vload.cpp"
	"PipelineCompiler/glsl/AABBox.cpp"
	"PipelineCompiler/glsl/Billow.cpp"
	"PipelineCompiler/glsl/BitMath.cpp"
	"PipelineCompiler/glsl/Blur.cpp"
	"PipelineCompiler/glsl/Cellular.cpp"
	"PipelineCompiler/glsl/Cmp.cpp"
	"PipelineCompiler/glsl/Color.cpp"
	"PipelineCompiler/glsl/Colors.cpp"
	"PipelineCompiler/glsl/DefaultSDF.cpp"
	"PipelineCompiler/glsl/Defines.cpp"
	"PipelineCompiler/glsl/DHash.cpp"
	"PipelineCompiler/glsl/Emitters.cpp"
	"PipelineCompiler/glsl/FBM.cpp"
	"PipelineCompiler/glsl/Gabor.cpp"
	"PipelineCompiler/glsl/GlobalIndex.cpp"
	"PipelineCompiler/glsl/glsl_source_vfs.cpp"
	"PipelineCompiler/glsl/glsl_source_vfs.h"
	"PipelineCompiler/glsl/Gravity.cpp"
	"PipelineCompiler/glsl/Hash.cpp"
	"PipelineCompiler/glsl/Hash2.cpp"
	"PipelineCompiler/glsl/IHash.cpp"
	"PipelineCompiler/glsl/Line2.cpp"
	"PipelineCompiler/glsl/Line3.cpp"
	"PipelineCompiler/glsl/Magnetism.cpp"
	"PipelineCompiler/glsl/Math.cpp"
	"PipelineCompiler/glsl/MathDef.cpp"
	"PipelineCompiler/glsl/Matrix.cpp"
	"PipelineCompiler/glsl/Perlin.cpp"
	"PipelineCompiler/glsl/Plane.cpp"
	"PipelineCompiler/glsl/Quaternion.cpp"
	"PipelineCompiler/glsl/Ray.cpp"
	"PipelineCompiler/glsl/Rect.cpp"
	"PipelineCompiler/glsl/Simplex.cpp"
	"PipelineCompiler/glsl/Turbulence.cpp"
	"PipelineCompiler/glsl/TypeInfo.cpp"
	"PipelineCompiler/glsl/Utils.cpp"
	"PipelineCompiler/glsl/Voronoi.cpp"
	"PipelineCompiler/glsl/VoronoiLines.cpp"
	"PipelineCompiler/glsl/VoronoiNoise.cpp"
	"PipelineCompiler/glsl/_NoiseUtils.cpp" )
add_library( "Engine.PipelineCompiler" STATIC ${SOURCES} )
source_group( "Serializers" FILES "PipelineCompiler/Serializers/AngelScriptSerializer.cpp" "PipelineCompiler/Serializers/AngelScriptSerializer.h" "PipelineCompiler/Serializers/CppSerializer.cpp" "PipelineCompiler/Serializers/CppSerializer.h" "PipelineCompiler/Serializers/ISerializer.h" )
source_group( "Common" FILES "PipelineCompiler/Common/ArraySize.h" "PipelineCompiler/Common/Common.h" "PipelineCompiler/Common/Packing.h" "PipelineCompiler/Common/ShaderEnums.h" "PipelineCompiler/Common/ToGLSL.h" )
source_group( "Pipelines" FILES "PipelineCompiler/Pipelines/BasePipeline.cpp" "PipelineCompiler/Pipelines/BasePipeline.h" "PipelineCompiler/Pipelines/BasePipeline_Pass1.cpp" "PipelineCompiler/Pipelines/BasePipeline_Pass2.cpp" "PipelineCompiler/Pipelines/ComputePipeline.cpp" "PipelineCompiler/Pipelines/ComputePipeline.h" "PipelineCompiler/Pipelines/GraphicsPipeline.cpp" "PipelineCompiler/Pipelines/GraphicsPipeline.h" "PipelineCompiler/Pipelines/PipelineManager.cpp" "PipelineCompiler/Pipelines/PipelineManager.h" )
source_group( "" FILES "PipelineCompiler/README.md" )
source_group( "Shaders" FILES "PipelineCompiler/Shaders/DeserializedShader.cpp" "PipelineCompiler/Shaders/DeserializedShader.h" "PipelineCompiler/Shaders/glslang_Include.h" "PipelineCompiler/Shaders/ShaderCompiler.cpp" "PipelineCompiler/Shaders/ShaderCompiler.h" "PipelineCompiler/Shaders/ShaderCompiler_CLCompiler.cpp" "PipelineCompiler/Shaders/ShaderCompiler_CLTranslator.cpp" "PipelineCompiler/Shaders/ShaderCompiler_CPPTranslator.cpp" "PipelineCompiler/Shaders/ShaderCompiler_GLCompiler.cpp" "PipelineCompiler/Shaders/ShaderCompiler_GLParser.cpp" "PipelineCompiler/Shaders/ShaderCompiler_GLSLTranslator.cpp" "PipelineCompiler/Shaders/ShaderCompiler_HLSLCompiler.cpp" "PipelineCompiler/Shaders/ShaderCompiler_HLSLTranslator.cpp" "PipelineCompiler/Shaders/ShaderCompiler_Includer.cpp" "PipelineCompiler/Shaders/ShaderCompiler_Includer.h" "PipelineCompiler/Shaders/ShaderCompiler_NameValidator.cpp" "PipelineCompiler/Shaders/ShaderCompiler_NameValidator.h" "PipelineCompiler/Shaders/ShaderCompiler_ReplaceTypes.cpp" "PipelineCompiler/Shaders/ShaderCompiler_Translator.cpp" "PipelineCompiler/Shaders/ShaderCompiler_Translator.h" "PipelineCompiler/Shaders/ShaderCompiler_Utils.h" "PipelineCompiler/Shaders/ShaderCompiler_VkCompiler.cpp" )
source_group( "cl" FILES "PipelineCompiler/cl/cl_source_vfs.cpp" "PipelineCompiler/cl/cl_source_vfs.h" "PipelineCompiler/cl/funcs.cpp" "PipelineCompiler/cl/matrix.cpp" "PipelineCompiler/cl/vload.cpp" )
source_group( "hlsl" FILES "PipelineCompiler/hlsl/hlsl_source_vfs.cpp" "PipelineCompiler/hlsl/hlsl_source_vfs.h" "PipelineCompiler/hlsl/vload.cpp" )
source_group( "glsl" FILES "PipelineCompiler/glsl/AABBox.cpp" "PipelineCompiler/glsl/Billow.cpp" "PipelineCompiler/glsl/BitMath.cpp" "PipelineCompiler/glsl/Blur.cpp" "PipelineCompiler/glsl/Cellular.cpp" "PipelineCompiler/glsl/Cmp.cpp" "PipelineCompiler/glsl/Color.cpp" "PipelineCompiler/glsl/Colors.cpp" "PipelineCompiler/glsl/DefaultSDF.cpp" "PipelineCompiler/glsl/Defines.cpp" "PipelineCompiler/glsl/DHash.cpp" "PipelineCompiler/glsl/Emitters.cpp" "PipelineCompiler/glsl/FBM.cpp" "PipelineCompiler/glsl/Gabor.cpp" "PipelineCompiler/glsl/GlobalIndex.cpp" "PipelineCompiler/glsl/glsl_source_vfs.cpp" "PipelineCompiler/glsl/glsl_source_vfs.h" "PipelineCompiler/glsl/Gravity.cpp" "PipelineCompiler/glsl/Hash.cpp" "PipelineCompiler/glsl/Hash2.cpp" "PipelineCompiler/glsl/IHash.cpp" "PipelineCompiler/glsl/Line2.cpp" "PipelineCompiler/glsl/Line3.cpp" "PipelineCompiler/glsl/Magnetism.cpp" "PipelineCompiler/glsl/Math.cpp" "PipelineCompiler/glsl/MathDef.cpp" "PipelineCompiler/glsl/Matrix.cpp" "PipelineCompiler/glsl/Perlin.cpp" "PipelineCompiler/glsl/Plane.cpp" "PipelineCompiler/glsl/Quaternion.cpp" "PipelineCompiler/glsl/Ray.cpp" "PipelineCompiler/glsl/Rect.cpp" "PipelineCompiler/glsl/Simplex.cpp" "PipelineCompiler/glsl/Turbulence.cpp" "PipelineCompiler/glsl/TypeInfo.cpp" "PipelineCompiler/glsl/Utils.cpp" "PipelineCompiler/glsl/Voronoi.cpp" "PipelineCompiler/glsl/VoronoiLines.cpp" "PipelineCompiler/glsl/VoronoiNoise.cpp" "PipelineCompiler/glsl/_NoiseUtils.cpp" )
set_property( TARGET "Engine.PipelineCompiler" PROPERTY FOLDER "EngineTools" )
target_include_directories( "Engine.PipelineCompiler" PUBLIC "C:/Projects/ModularGraphicsFramework/External" )
target_include_directories( "Engine.PipelineCompiler" PUBLIC "${EXTERNALS_PATH}" )
target_include_directories( "Engine.PipelineCompiler" PUBLIC "C:/Projects/ModularGraphicsFramework/Core/.." )
target_include_directories( "Engine.PipelineCompiler" PRIVATE "${EXTERNALS_PATH}/glslang" )
target_link_libraries( "Engine.PipelineCompiler" "Core.STL" )
if (D3DCOMPILER_LIBRARY)
	target_link_libraries( "Engine.PipelineCompiler" "${D3DCOMPILER_LIBRARY}" )
endif()
if (ENABLE_GLSLANG)
	target_link_libraries( "Engine.PipelineCompiler" "glsl" )
endif()
# compiler
set_target_properties( "Engine.PipelineCompiler" PROPERTIES LINK_FLAGS_RELEASE ${PROJECTS_SHARED_LINKER_FLAGS_RELEASE} )
set_target_properties( "Engine.PipelineCompiler" PROPERTIES LINK_FLAGS_PROFILE ${PROJECTS_SHARED_LINKER_FLAGS_PROFILE} )
target_compile_definitions( "Engine.PipelineCompiler" PRIVATE $<$<CONFIG:Debug>: ${PROJECTS_SHARED_DEFINES_DEBUG}> )
set_target_properties( "Engine.PipelineCompiler" PROPERTIES LINK_FLAGS_DEBUGANALYZE ${PROJECTS_SHARED_LINKER_FLAGS_DEBUGANALYZE} )
target_compile_definitions( "Engine.PipelineCompiler" PRIVATE $<$<CONFIG:DebugAnalyze>: ${PROJECTS_SHARED_DEFINES_DEBUGANALYZE}> )
target_compile_options( "Engine.PipelineCompiler" PRIVATE $<$<CONFIG:Profile>: ${PROJECTS_SHARED_CXX_FLAGS_PROFILE}> )
target_compile_options( "Engine.PipelineCompiler" PRIVATE $<$<CONFIG:Release>: ${PROJECTS_SHARED_CXX_FLAGS_RELEASE}> )
target_compile_definitions( "Engine.PipelineCompiler" PRIVATE $<$<CONFIG:Profile>: ${PROJECTS_SHARED_DEFINES_PROFILE}> )
target_compile_definitions( "Engine.PipelineCompiler" PRIVATE $<$<CONFIG:Release>: ${PROJECTS_SHARED_DEFINES_RELEASE}> )
set_target_properties( "Engine.PipelineCompiler" PROPERTIES LINK_FLAGS_DEBUG ${PROJECTS_SHARED_LINKER_FLAGS_DEBUG} )
target_compile_options( "Engine.PipelineCompiler" PRIVATE $<$<CONFIG:DebugAnalyze>: ${PROJECTS_SHARED_CXX_FLAGS_DEBUGANALYZE}> )
target_compile_options( "Engine.PipelineCompiler" PRIVATE $<$<CONFIG:Debug>: ${PROJECTS_SHARED_CXX_FLAGS_DEBUG}> )



#==================================================================================================
# project: Engine.ResourcePacker
#==================================================================================================
set( SOURCES 
	"ResourcePacker/Images/DevILConverter.cpp"
	"ResourcePacker/Images/ImageConverter.cpp"
	"ResourcePacker/Images/ImageConverter.h"
	"ResourcePacker/FilePacker/BinaryFilePacker.cpp"
	"ResourcePacker/FilePacker/BinaryFilePacker.h"
	"ResourcePacker/FilePacker/CppFileSystemPacker.cpp"
	"ResourcePacker/FilePacker/CppFileSystemPacker.h"
	"ResourcePacker/FilePacker/IFileSystemPacker.h"
	"ResourcePacker/Pipelines/PipelineConverter.cpp"
	"ResourcePacker/Pipelines/PipelineConverter.h"
	"ResourcePacker/Pipelines/ScriptComputePipeline.cpp"
	"ResourcePacker/Pipelines/ScriptComputePipeline.h"
	"ResourcePacker/Pipelines/ScriptGraphicsPipeline.cpp"
	"ResourcePacker/Pipelines/ScriptGraphicsPipeline.h"
	"ResourcePacker/Pipelines/ScriptPipeline.cpp"
	"ResourcePacker/Pipelines/ScriptPipeline.h"
	"ResourcePacker/Main.cpp"
	"ResourcePacker/Packer/Common.h"
	"ResourcePacker/Packer/ResourcePacker.cpp"
	"ResourcePacker/Packer/ResourcePacker.h"
	"ResourcePacker/Packer/ScriptHelper.cpp" )
if (DEFINED ANDROID)
	add_library( "Engine.ResourcePacker" SHARED ${SOURCES} )
else()
	add_executable( "Engine.ResourcePacker" ${SOURCES} )
endif()
source_group( "Images" FILES "ResourcePacker/Images/DevILConverter.cpp" "ResourcePacker/Images/ImageConverter.cpp" "ResourcePacker/Images/ImageConverter.h" )
source_group( "FilePacker" FILES "ResourcePacker/FilePacker/BinaryFilePacker.cpp" "ResourcePacker/FilePacker/BinaryFilePacker.h" "ResourcePacker/FilePacker/CppFileSystemPacker.cpp" "ResourcePacker/FilePacker/CppFileSystemPacker.h" "ResourcePacker/FilePacker/IFileSystemPacker.h" )
source_group( "Pipelines" FILES "ResourcePacker/Pipelines/PipelineConverter.cpp" "ResourcePacker/Pipelines/PipelineConverter.h" "ResourcePacker/Pipelines/ScriptComputePipeline.cpp" "ResourcePacker/Pipelines/ScriptComputePipeline.h" "ResourcePacker/Pipelines/ScriptGraphicsPipeline.cpp" "ResourcePacker/Pipelines/ScriptGraphicsPipeline.h" "ResourcePacker/Pipelines/ScriptPipeline.cpp" "ResourcePacker/Pipelines/ScriptPipeline.h" )
source_group( "" FILES "ResourcePacker/Main.cpp" )
source_group( "Packer" FILES "ResourcePacker/Packer/Common.h" "ResourcePacker/Packer/ResourcePacker.cpp" "ResourcePacker/Packer/ResourcePacker.h" "ResourcePacker/Packer/ScriptHelper.cpp" )
set_property( TARGET "Engine.ResourcePacker" PROPERTY FOLDER "EngineTools" )
target_include_directories( "Engine.ResourcePacker" PUBLIC "C:/Projects/ModularGraphicsFramework/External" )
target_include_directories( "Engine.ResourcePacker" PUBLIC "${EXTERNALS_PATH}" )
target_include_directories( "Engine.ResourcePacker" PUBLIC "C:/Projects/ModularGraphicsFramework/Core/.." )
if (ENABLE_FREEIMAGE)
	target_link_libraries( "Engine.ResourcePacker" "FreeImage" )
endif()
if (ENABLE_LODEPNG)
	target_link_libraries( "Engine.ResourcePacker" "lodepng" )
endif()
if (ENABLE_DEVIL)
	target_link_libraries( "Engine.ResourcePacker" "DevIL" )
endif()
target_link_libraries( "Engine.ResourcePacker" "Engine.PipelineCompiler" )
target_link_libraries( "Engine.ResourcePacker" "Core.Script" )
# compiler
set_target_properties( "Engine.ResourcePacker" PROPERTIES LINK_FLAGS_RELEASE ${PROJECTS_SHARED_LINKER_FLAGS_RELEASE} )
set_target_properties( "Engine.ResourcePacker" PROPERTIES LINK_FLAGS_PROFILE ${PROJECTS_SHARED_LINKER_FLAGS_PROFILE} )
target_compile_definitions( "Engine.ResourcePacker" PRIVATE $<$<CONFIG:Debug>: ${PROJECTS_SHARED_DEFINES_DEBUG}> )
set_target_properties( "Engine.ResourcePacker" PROPERTIES LINK_FLAGS_DEBUGANALYZE ${PROJECTS_SHARED_LINKER_FLAGS_DEBUGANALYZE} )
target_compile_definitions( "Engine.ResourcePacker" PRIVATE $<$<CONFIG:DebugAnalyze>: ${PROJECTS_SHARED_DEFINES_DEBUGANALYZE}> )
target_compile_options( "Engine.ResourcePacker" PRIVATE $<$<CONFIG:Profile>: ${PROJECTS_SHARED_CXX_FLAGS_PROFILE}> )
target_compile_options( "Engine.ResourcePacker" PRIVATE $<$<CONFIG:Release>: ${PROJECTS_SHARED_CXX_FLAGS_RELEASE}> )
target_compile_definitions( "Engine.ResourcePacker" PRIVATE $<$<CONFIG:Profile>: ${PROJECTS_SHARED_DEFINES_PROFILE}> )
target_compile_definitions( "Engine.ResourcePacker" PRIVATE $<$<CONFIG:Release>: ${PROJECTS_SHARED_DEFINES_RELEASE}> )
set_target_properties( "Engine.ResourcePacker" PROPERTIES LINK_FLAGS_DEBUG ${PROJECTS_SHARED_LINKER_FLAGS_DEBUG} )
target_compile_options( "Engine.ResourcePacker" PRIVATE $<$<CONFIG:DebugAnalyze>: ${PROJECTS_SHARED_CXX_FLAGS_DEBUGANALYZE}> )
target_compile_options( "Engine.ResourcePacker" PRIVATE $<$<CONFIG:Debug>: ${PROJECTS_SHARED_CXX_FLAGS_DEBUG}> )

#-----------------------------------
if ( CONFIGURATION_DEPENDENT_PATH )
	set( RESOURCE_PACKER_EXE "${MAIN_BINARY_DIR}/$<CONFIG>/Engine.ResourcePacker${CMAKE_EXECUTABLE_SUFFIX}" CACHE INTERNAL "" FORCE )
else ()
	set( RESOURCE_PACKER_EXE "${MAIN_BINARY_DIR}/Engine.ResourcePacker${CMAKE_EXECUTABLE_SUFFIX}" CACHE INTERNAL "" FORCE )
endif ()

#-----------------------------------


#==================================================================================================
# project: Tests.Engine.Base
#==================================================================================================
set( SOURCES 
	"../EngineTests/Base/Graphics/GApp.cpp"
	"../EngineTests/Base/Graphics/GApp.h"
	"../EngineTests/Base/Graphics/Test.GWindow.cpp"
	"../EngineTests/Base/Pipelines/all_pipelines.h"
	"../EngineTests/Base/Pipelines/default.cpp"
	"../EngineTests/Base/Pipelines/Default.ppln"
	"../EngineTests/Base/Pipelines/default2.cpp"
	"../EngineTests/Base/Pipelines/Default2.ppln"
	"../EngineTests/Base/Pipelines/resources.as"
	"../EngineTests/Base/Pipelines/shared_types.h"
	"../EngineTests/Base/Window/Test.Window.cpp"
	"../EngineTests/Base/Common.h"
	"../EngineTests/Base/Main.cpp" )
if (DEFINED ANDROID)
	add_library( "Tests.Engine.Base" SHARED ${SOURCES} )
else()
	add_executable( "Tests.Engine.Base" ${SOURCES} )
endif()
source_group( "Graphics" FILES "../EngineTests/Base/Graphics/GApp.cpp" "../EngineTests/Base/Graphics/GApp.h" "../EngineTests/Base/Graphics/Test.GWindow.cpp" )
source_group( "Pipelines" FILES "../EngineTests/Base/Pipelines/all_pipelines.h" "../EngineTests/Base/Pipelines/default.cpp" "../EngineTests/Base/Pipelines/Default.ppln" "../EngineTests/Base/Pipelines/default2.cpp" "../EngineTests/Base/Pipelines/Default2.ppln" "../EngineTests/Base/Pipelines/resources.as" "../EngineTests/Base/Pipelines/shared_types.h" )
source_group( "Window" FILES "../EngineTests/Base/Window/Test.Window.cpp" )
source_group( "" FILES "../EngineTests/Base/Common.h" "../EngineTests/Base/Main.cpp" )
set_property( TARGET "Tests.Engine.Base" PROPERTY FOLDER "EngineTests" )
target_include_directories( "Tests.Engine.Base" PUBLIC "C:/Projects/ModularGraphicsFramework/External" )
target_include_directories( "Tests.Engine.Base" PUBLIC "${EXTERNALS_PATH}" )
target_include_directories( "Tests.Engine.Base" PUBLIC "C:/Projects/ModularGraphicsFramework/Core/.." )
target_link_libraries( "Tests.Engine.Base" "Engine.Profilers" )
target_link_libraries( "Tests.Engine.Base" "Engine.Platforms" )
# compiler
set_target_properties( "Tests.Engine.Base" PROPERTIES LINK_FLAGS_RELEASE ${PROJECTS_SHARED_LINKER_FLAGS_RELEASE} )
set_target_properties( "Tests.Engine.Base" PROPERTIES LINK_FLAGS_PROFILE ${PROJECTS_SHARED_LINKER_FLAGS_PROFILE} )
target_compile_definitions( "Tests.Engine.Base" PRIVATE $<$<CONFIG:Debug>: ${PROJECTS_SHARED_DEFINES_DEBUG}> )
set_target_properties( "Tests.Engine.Base" PROPERTIES LINK_FLAGS_DEBUGANALYZE ${PROJECTS_SHARED_LINKER_FLAGS_DEBUGANALYZE} )
target_compile_definitions( "Tests.Engine.Base" PRIVATE $<$<CONFIG:DebugAnalyze>: ${PROJECTS_SHARED_DEFINES_DEBUGANALYZE}> )
target_compile_options( "Tests.Engine.Base" PRIVATE $<$<CONFIG:Profile>: ${PROJECTS_SHARED_CXX_FLAGS_PROFILE}> )
target_compile_options( "Tests.Engine.Base" PRIVATE $<$<CONFIG:Release>: ${PROJECTS_SHARED_CXX_FLAGS_RELEASE}> )
target_compile_definitions( "Tests.Engine.Base" PRIVATE $<$<CONFIG:Profile>: ${PROJECTS_SHARED_DEFINES_PROFILE}> )
target_compile_definitions( "Tests.Engine.Base" PRIVATE $<$<CONFIG:Release>: ${PROJECTS_SHARED_DEFINES_RELEASE}> )
set_target_properties( "Tests.Engine.Base" PROPERTIES LINK_FLAGS_DEBUG ${PROJECTS_SHARED_LINKER_FLAGS_DEBUG} )
target_compile_options( "Tests.Engine.Base" PRIVATE $<$<CONFIG:DebugAnalyze>: ${PROJECTS_SHARED_CXX_FLAGS_DEBUGANALYZE}> )
target_compile_options( "Tests.Engine.Base" PRIVATE $<$<CONFIG:Debug>: ${PROJECTS_SHARED_CXX_FLAGS_DEBUG}> )



#==================================================================================================
# project: Tests.Engine.Graphics
#==================================================================================================
set( SOURCES 
	"../EngineTests/Graphics/Pipelines/all_pipelines.h"
	"../EngineTests/Graphics/Pipelines/default3.cpp"
	"../EngineTests/Graphics/Pipelines/Default3.ppln"
	"../EngineTests/Graphics/Pipelines/resources.as"
	"../EngineTests/Graphics/Pipelines/shared_types.h"
	"../EngineTests/Graphics/GApp.cpp"
	"../EngineTests/Graphics/GApp.h"
	"../EngineTests/Graphics/Main.cpp" )
if (DEFINED ANDROID)
	add_library( "Tests.Engine.Graphics" SHARED ${SOURCES} )
else()
	add_executable( "Tests.Engine.Graphics" ${SOURCES} )
endif()
source_group( "Pipelines" FILES "../EngineTests/Graphics/Pipelines/all_pipelines.h" "../EngineTests/Graphics/Pipelines/default3.cpp" "../EngineTests/Graphics/Pipelines/Default3.ppln" "../EngineTests/Graphics/Pipelines/resources.as" "../EngineTests/Graphics/Pipelines/shared_types.h" )
source_group( "" FILES "../EngineTests/Graphics/GApp.cpp" "../EngineTests/Graphics/GApp.h" "../EngineTests/Graphics/Main.cpp" )
set_property( TARGET "Tests.Engine.Graphics" PROPERTY FOLDER "EngineTests" )
target_include_directories( "Tests.Engine.Graphics" PUBLIC "C:/Projects/ModularGraphicsFramework/External" )
target_include_directories( "Tests.Engine.Graphics" PUBLIC "${EXTERNALS_PATH}" )
target_include_directories( "Tests.Engine.Graphics" PUBLIC "C:/Projects/ModularGraphicsFramework/Core/.." )
target_link_libraries( "Tests.Engine.Graphics" "Engine.Profilers" )
target_link_libraries( "Tests.Engine.Graphics" "Engine.ImportExport" )
# compiler
set_target_properties( "Tests.Engine.Graphics" PROPERTIES LINK_FLAGS_RELEASE ${PROJECTS_SHARED_LINKER_FLAGS_RELEASE} )
set_target_properties( "Tests.Engine.Graphics" PROPERTIES LINK_FLAGS_PROFILE ${PROJECTS_SHARED_LINKER_FLAGS_PROFILE} )
target_compile_definitions( "Tests.Engine.Graphics" PRIVATE $<$<CONFIG:Debug>: ${PROJECTS_SHARED_DEFINES_DEBUG}> )
set_target_properties( "Tests.Engine.Graphics" PROPERTIES LINK_FLAGS_DEBUGANALYZE ${PROJECTS_SHARED_LINKER_FLAGS_DEBUGANALYZE} )
target_compile_definitions( "Tests.Engine.Graphics" PRIVATE $<$<CONFIG:DebugAnalyze>: ${PROJECTS_SHARED_DEFINES_DEBUGANALYZE}> )
target_compile_options( "Tests.Engine.Graphics" PRIVATE $<$<CONFIG:Profile>: ${PROJECTS_SHARED_CXX_FLAGS_PROFILE}> )
target_compile_options( "Tests.Engine.Graphics" PRIVATE $<$<CONFIG:Release>: ${PROJECTS_SHARED_CXX_FLAGS_RELEASE}> )
target_compile_definitions( "Tests.Engine.Graphics" PRIVATE $<$<CONFIG:Profile>: ${PROJECTS_SHARED_DEFINES_PROFILE}> )
target_compile_definitions( "Tests.Engine.Graphics" PRIVATE $<$<CONFIG:Release>: ${PROJECTS_SHARED_DEFINES_RELEASE}> )
set_target_properties( "Tests.Engine.Graphics" PROPERTIES LINK_FLAGS_DEBUG ${PROJECTS_SHARED_LINKER_FLAGS_DEBUG} )
target_compile_options( "Tests.Engine.Graphics" PRIVATE $<$<CONFIG:DebugAnalyze>: ${PROJECTS_SHARED_CXX_FLAGS_DEBUGANALYZE}> )
target_compile_options( "Tests.Engine.Graphics" PRIVATE $<$<CONFIG:Debug>: ${PROJECTS_SHARED_CXX_FLAGS_DEBUG}> )



#==================================================================================================
# project: Tests.Engine.Platforms.GAPI
#==================================================================================================
set( SOURCES 
	"../EngineTests/Platforms.GAPI/Compute/CApp.cpp"
	"../EngineTests/Platforms.GAPI/Compute/CApp.h"
	"../EngineTests/Platforms.GAPI/Compute/CApp_BufferAlign.cpp"
	"../EngineTests/Platforms.GAPI/Compute/CApp_BufferRange.cpp"
	"../EngineTests/Platforms.GAPI/Compute/CApp_ClearBuffer.cpp"
	"../EngineTests/Platforms.GAPI/Compute/CApp_ConvertFloatImage2D.cpp"
	"../EngineTests/Platforms.GAPI/Compute/CApp_CopyBuffer.cpp"
	"../EngineTests/Platforms.GAPI/Compute/CApp_CopyBufferToImage2D.cpp"
	"../EngineTests/Platforms.GAPI/Compute/CApp_CopyImage2D.cpp"
	"../EngineTests/Platforms.GAPI/Compute/CApp_CopyImage2DToBuffer.cpp"
	"../EngineTests/Platforms.GAPI/Compute/CApp_DynamicBuffer.cpp"
	"../EngineTests/Platforms.GAPI/Compute/CApp_ExplicitMemoryObjectSharing.cpp"
	"../EngineTests/Platforms.GAPI/Compute/CApp_Image2DBilinearFilter.cpp"
	"../EngineTests/Platforms.GAPI/Compute/CApp_Image2DNearestFilter.cpp"
	"../EngineTests/Platforms.GAPI/Compute/CApp_ShaderBarrier.cpp"
	"../EngineTests/Platforms.GAPI/Compute/CApp_UpdateBuffer.cpp"
	"../EngineTests/Platforms.GAPI/Compute/Test.ComputeApi.cpp"
	"../EngineTests/Platforms.GAPI/Sharing/SApp.cpp"
	"../EngineTests/Platforms.GAPI/Sharing/SApp.h"
	"../EngineTests/Platforms.GAPI/Sharing/SApp_BufferSharing.cpp"
	"../EngineTests/Platforms.GAPI/Sharing/SApp_ImageSharing.cpp"
	"../EngineTests/Platforms.GAPI/Sharing/Test.Sharing.cpp"
	"../EngineTests/Platforms.GAPI/Compute/Pipelines/all_pipelines.h"
	"../EngineTests/Platforms.GAPI/Compute/Pipelines/bufferalign.cpp"
	"../EngineTests/Platforms.GAPI/Compute/Pipelines/BufferAlign.ppln"
	"../EngineTests/Platforms.GAPI/Compute/Pipelines/copyfloatimage2d.cpp"
	"../EngineTests/Platforms.GAPI/Compute/Pipelines/CopyFloatImage2D.ppln"
	"../EngineTests/Platforms.GAPI/Compute/Pipelines/dynamicbuffer.cpp"
	"../EngineTests/Platforms.GAPI/Compute/Pipelines/DynamicBuffer.ppln"
	"../EngineTests/Platforms.GAPI/Compute/Pipelines/image2dbilinearfilter.cpp"
	"../EngineTests/Platforms.GAPI/Compute/Pipelines/Image2DBilinearFilter.ppln"
	"../EngineTests/Platforms.GAPI/Compute/Pipelines/image2dnearestfilter.cpp"
	"../EngineTests/Platforms.GAPI/Compute/Pipelines/Image2DNearestFilter.ppln"
	"../EngineTests/Platforms.GAPI/Compute/Pipelines/shaderbarrier.cpp"
	"../EngineTests/Platforms.GAPI/Compute/Pipelines/ShaderBarrier.ppln"
	"../EngineTests/Platforms.GAPI/Compute/Pipelines/shared_types.h"
	"../EngineTests/Platforms.GAPI/Graphics/GApp.cpp"
	"../EngineTests/Platforms.GAPI/Graphics/GApp.h"
	"../EngineTests/Platforms.GAPI/Graphics/GApp_Texture2DBilinearFilter.cpp"
	"../EngineTests/Platforms.GAPI/Graphics/GApp_Texture2DNearestFilter.cpp"
	"../EngineTests/Platforms.GAPI/Graphics/Test.GraphicsApi.cpp"
	"../EngineTests/Platforms.GAPI/Graphics/Pipelines/all_pipelines.h"
	"../EngineTests/Platforms.GAPI/Graphics/Pipelines/shared_types.h"
	"../EngineTests/Platforms.GAPI/Graphics/Pipelines/texture2dbilinearfilter.cpp"
	"../EngineTests/Platforms.GAPI/Graphics/Pipelines/Texture2DBilinearFilter.ppln"
	"../EngineTests/Platforms.GAPI/Graphics/Pipelines/texture2dnearestfilter.cpp"
	"../EngineTests/Platforms.GAPI/Graphics/Pipelines/Texture2DNearestFilter.ppln"
	"../EngineTests/Platforms.GAPI/Compiler/Pipelines/all_pipelines.h"
	"../EngineTests/Platforms.GAPI/Compiler/Pipelines/atomicadd.cpp"
	"../EngineTests/Platforms.GAPI/Compiler/Pipelines/AtomicAdd.ppln"
	"../EngineTests/Platforms.GAPI/Compiler/Pipelines/findlsb.cpp"
	"../EngineTests/Platforms.GAPI/Compiler/Pipelines/FindLSB.ppln"
	"../EngineTests/Platforms.GAPI/Compiler/Pipelines/findmsb.cpp"
	"../EngineTests/Platforms.GAPI/Compiler/Pipelines/FindMSB.ppln"
	"../EngineTests/Platforms.GAPI/Compiler/Pipelines/globaltolocal.cpp"
	"../EngineTests/Platforms.GAPI/Compiler/Pipelines/GlobalToLocal.ppln"
	"../EngineTests/Platforms.GAPI/Compiler/Pipelines/include.cpp"
	"../EngineTests/Platforms.GAPI/Compiler/Pipelines/Include.ppln"
	"../EngineTests/Platforms.GAPI/Compiler/Pipelines/inlineall.cpp"
	"../EngineTests/Platforms.GAPI/Compiler/Pipelines/InlineAll.ppln"
	"../EngineTests/Platforms.GAPI/Compiler/Pipelines/shared_types.h"
	"../EngineTests/Platforms.GAPI/Compiler/Pipelines/unnamedbuffer.cpp"
	"../EngineTests/Platforms.GAPI/Compiler/Pipelines/UnnamedBuffer.ppln"
	"../EngineTests/Platforms.GAPI/Compiler/Pipelines/vecswizzle.cpp"
	"../EngineTests/Platforms.GAPI/Compiler/Pipelines/VecSwizzle.ppln"
	"../EngineTests/Platforms.GAPI/Common.h"
	"../EngineTests/Platforms.GAPI/Main.cpp"
	"../EngineTests/Platforms.GAPI/resources.as"
	"../EngineTests/Platforms.GAPI/MultiGPU/Test.MultiGPU.cpp"
	"../EngineTests/Platforms.GAPI/Compiler/PApp.cpp"
	"../EngineTests/Platforms.GAPI/Compiler/PApp.h"
	"../EngineTests/Platforms.GAPI/Compiler/PApp_AtomicAdd.cpp"
	"../EngineTests/Platforms.GAPI/Compiler/PApp_FindLSB.cpp"
	"../EngineTests/Platforms.GAPI/Compiler/PApp_FindMSB.cpp"
	"../EngineTests/Platforms.GAPI/Compiler/PApp_GlobalToLocal.cpp"
	"../EngineTests/Platforms.GAPI/Compiler/PApp_Include.cpp"
	"../EngineTests/Platforms.GAPI/Compiler/PApp_InlineAll.cpp"
	"../EngineTests/Platforms.GAPI/Compiler/PApp_UnnamedBuffer.cpp"
	"../EngineTests/Platforms.GAPI/Compiler/PApp_VecSwizzle.cpp"
	"../EngineTests/Platforms.GAPI/Compiler/Test.PipelineCompiler.cpp" )
if (DEFINED ANDROID)
	add_library( "Tests.Engine.Platforms.GAPI" SHARED ${SOURCES} )
else()
	add_executable( "Tests.Engine.Platforms.GAPI" ${SOURCES} )
endif()
source_group( "Compute" FILES "../EngineTests/Platforms.GAPI/Compute/CApp.cpp" "../EngineTests/Platforms.GAPI/Compute/CApp.h" "../EngineTests/Platforms.GAPI/Compute/CApp_BufferAlign.cpp" "../EngineTests/Platforms.GAPI/Compute/CApp_BufferRange.cpp" "../EngineTests/Platforms.GAPI/Compute/CApp_ClearBuffer.cpp" "../EngineTests/Platforms.GAPI/Compute/CApp_ConvertFloatImage2D.cpp" "../EngineTests/Platforms.GAPI/Compute/CApp_CopyBuffer.cpp" "../EngineTests/Platforms.GAPI/Compute/CApp_CopyBufferToImage2D.cpp" "../EngineTests/Platforms.GAPI/Compute/CApp_CopyImage2D.cpp" "../EngineTests/Platforms.GAPI/Compute/CApp_CopyImage2DToBuffer.cpp" "../EngineTests/Platforms.GAPI/Compute/CApp_DynamicBuffer.cpp" "../EngineTests/Platforms.GAPI/Compute/CApp_ExplicitMemoryObjectSharing.cpp" "../EngineTests/Platforms.GAPI/Compute/CApp_Image2DBilinearFilter.cpp" "../EngineTests/Platforms.GAPI/Compute/CApp_Image2DNearestFilter.cpp" "../EngineTests/Platforms.GAPI/Compute/CApp_ShaderBarrier.cpp" "../EngineTests/Platforms.GAPI/Compute/CApp_UpdateBuffer.cpp" "../EngineTests/Platforms.GAPI/Compute/Test.ComputeApi.cpp" )
source_group( "Sharing" FILES "../EngineTests/Platforms.GAPI/Sharing/SApp.cpp" "../EngineTests/Platforms.GAPI/Sharing/SApp.h" "../EngineTests/Platforms.GAPI/Sharing/SApp_BufferSharing.cpp" "../EngineTests/Platforms.GAPI/Sharing/SApp_ImageSharing.cpp" "../EngineTests/Platforms.GAPI/Sharing/Test.Sharing.cpp" )
source_group( "Compute\\Pipelines" FILES "../EngineTests/Platforms.GAPI/Compute/Pipelines/all_pipelines.h" "../EngineTests/Platforms.GAPI/Compute/Pipelines/bufferalign.cpp" "../EngineTests/Platforms.GAPI/Compute/Pipelines/BufferAlign.ppln" "../EngineTests/Platforms.GAPI/Compute/Pipelines/copyfloatimage2d.cpp" "../EngineTests/Platforms.GAPI/Compute/Pipelines/CopyFloatImage2D.ppln" "../EngineTests/Platforms.GAPI/Compute/Pipelines/dynamicbuffer.cpp" "../EngineTests/Platforms.GAPI/Compute/Pipelines/DynamicBuffer.ppln" "../EngineTests/Platforms.GAPI/Compute/Pipelines/image2dbilinearfilter.cpp" "../EngineTests/Platforms.GAPI/Compute/Pipelines/Image2DBilinearFilter.ppln" "../EngineTests/Platforms.GAPI/Compute/Pipelines/image2dnearestfilter.cpp" "../EngineTests/Platforms.GAPI/Compute/Pipelines/Image2DNearestFilter.ppln" "../EngineTests/Platforms.GAPI/Compute/Pipelines/shaderbarrier.cpp" "../EngineTests/Platforms.GAPI/Compute/Pipelines/ShaderBarrier.ppln" "../EngineTests/Platforms.GAPI/Compute/Pipelines/shared_types.h" )
source_group( "Graphics" FILES "../EngineTests/Platforms.GAPI/Graphics/GApp.cpp" "../EngineTests/Platforms.GAPI/Graphics/GApp.h" "../EngineTests/Platforms.GAPI/Graphics/GApp_Texture2DBilinearFilter.cpp" "../EngineTests/Platforms.GAPI/Graphics/GApp_Texture2DNearestFilter.cpp" "../EngineTests/Platforms.GAPI/Graphics/Test.GraphicsApi.cpp" )
source_group( "Graphics\\Pipelines" FILES "../EngineTests/Platforms.GAPI/Graphics/Pipelines/all_pipelines.h" "../EngineTests/Platforms.GAPI/Graphics/Pipelines/shared_types.h" "../EngineTests/Platforms.GAPI/Graphics/Pipelines/texture2dbilinearfilter.cpp" "../EngineTests/Platforms.GAPI/Graphics/Pipelines/Texture2DBilinearFilter.ppln" "../EngineTests/Platforms.GAPI/Graphics/Pipelines/texture2dnearestfilter.cpp" "../EngineTests/Platforms.GAPI/Graphics/Pipelines/Texture2DNearestFilter.ppln" )
source_group( "Compiler\\Pipelines" FILES "../EngineTests/Platforms.GAPI/Compiler/Pipelines/all_pipelines.h" "../EngineTests/Platforms.GAPI/Compiler/Pipelines/atomicadd.cpp" "../EngineTests/Platforms.GAPI/Compiler/Pipelines/AtomicAdd.ppln" "../EngineTests/Platforms.GAPI/Compiler/Pipelines/findlsb.cpp" "../EngineTests/Platforms.GAPI/Compiler/Pipelines/FindLSB.ppln" "../EngineTests/Platforms.GAPI/Compiler/Pipelines/findmsb.cpp" "../EngineTests/Platforms.GAPI/Compiler/Pipelines/FindMSB.ppln" "../EngineTests/Platforms.GAPI/Compiler/Pipelines/globaltolocal.cpp" "../EngineTests/Platforms.GAPI/Compiler/Pipelines/GlobalToLocal.ppln" "../EngineTests/Platforms.GAPI/Compiler/Pipelines/include.cpp" "../EngineTests/Platforms.GAPI/Compiler/Pipelines/Include.ppln" "../EngineTests/Platforms.GAPI/Compiler/Pipelines/inlineall.cpp" "../EngineTests/Platforms.GAPI/Compiler/Pipelines/InlineAll.ppln" "../EngineTests/Platforms.GAPI/Compiler/Pipelines/shared_types.h" "../EngineTests/Platforms.GAPI/Compiler/Pipelines/unnamedbuffer.cpp" "../EngineTests/Platforms.GAPI/Compiler/Pipelines/UnnamedBuffer.ppln" "../EngineTests/Platforms.GAPI/Compiler/Pipelines/vecswizzle.cpp" "../EngineTests/Platforms.GAPI/Compiler/Pipelines/VecSwizzle.ppln" )
source_group( "" FILES "../EngineTests/Platforms.GAPI/Common.h" "../EngineTests/Platforms.GAPI/Main.cpp" "../EngineTests/Platforms.GAPI/resources.as" )
source_group( "MultiGPU" FILES "../EngineTests/Platforms.GAPI/MultiGPU/Test.MultiGPU.cpp" )
source_group( "Compiler" FILES "../EngineTests/Platforms.GAPI/Compiler/PApp.cpp" "../EngineTests/Platforms.GAPI/Compiler/PApp.h" "../EngineTests/Platforms.GAPI/Compiler/PApp_AtomicAdd.cpp" "../EngineTests/Platforms.GAPI/Compiler/PApp_FindLSB.cpp" "../EngineTests/Platforms.GAPI/Compiler/PApp_FindMSB.cpp" "../EngineTests/Platforms.GAPI/Compiler/PApp_GlobalToLocal.cpp" "../EngineTests/Platforms.GAPI/Compiler/PApp_Include.cpp" "../EngineTests/Platforms.GAPI/Compiler/PApp_InlineAll.cpp" "../EngineTests/Platforms.GAPI/Compiler/PApp_UnnamedBuffer.cpp" "../EngineTests/Platforms.GAPI/Compiler/PApp_VecSwizzle.cpp" "../EngineTests/Platforms.GAPI/Compiler/Test.PipelineCompiler.cpp" )
set_property( TARGET "Tests.Engine.Platforms.GAPI" PROPERTY FOLDER "EngineTests" )
target_include_directories( "Tests.Engine.Platforms.GAPI" PUBLIC "C:/Projects/ModularGraphicsFramework/External" )
target_include_directories( "Tests.Engine.Platforms.GAPI" PUBLIC "${EXTERNALS_PATH}" )
target_include_directories( "Tests.Engine.Platforms.GAPI" PUBLIC "C:/Projects/ModularGraphicsFramework/Core/.." )
target_link_libraries( "Tests.Engine.Platforms.GAPI" "Engine.Platforms" )
# compiler
set_target_properties( "Tests.Engine.Platforms.GAPI" PROPERTIES LINK_FLAGS_RELEASE ${PROJECTS_SHARED_LINKER_FLAGS_RELEASE} )
set_target_properties( "Tests.Engine.Platforms.GAPI" PROPERTIES LINK_FLAGS_PROFILE ${PROJECTS_SHARED_LINKER_FLAGS_PROFILE} )
target_compile_definitions( "Tests.Engine.Platforms.GAPI" PRIVATE $<$<CONFIG:Debug>: ${PROJECTS_SHARED_DEFINES_DEBUG}> )
set_target_properties( "Tests.Engine.Platforms.GAPI" PROPERTIES LINK_FLAGS_DEBUGANALYZE ${PROJECTS_SHARED_LINKER_FLAGS_DEBUGANALYZE} )
target_compile_definitions( "Tests.Engine.Platforms.GAPI" PRIVATE $<$<CONFIG:DebugAnalyze>: ${PROJECTS_SHARED_DEFINES_DEBUGANALYZE}> )
target_compile_options( "Tests.Engine.Platforms.GAPI" PRIVATE $<$<CONFIG:Profile>: ${PROJECTS_SHARED_CXX_FLAGS_PROFILE}> )
target_compile_options( "Tests.Engine.Platforms.GAPI" PRIVATE $<$<CONFIG:Release>: ${PROJECTS_SHARED_CXX_FLAGS_RELEASE}> )
target_compile_definitions( "Tests.Engine.Platforms.GAPI" PRIVATE $<$<CONFIG:Profile>: ${PROJECTS_SHARED_DEFINES_PROFILE}> )
target_compile_definitions( "Tests.Engine.Platforms.GAPI" PRIVATE $<$<CONFIG:Release>: ${PROJECTS_SHARED_DEFINES_RELEASE}> )
set_target_properties( "Tests.Engine.Platforms.GAPI" PROPERTIES LINK_FLAGS_DEBUG ${PROJECTS_SHARED_LINKER_FLAGS_DEBUG} )
target_compile_options( "Tests.Engine.Platforms.GAPI" PRIVATE $<$<CONFIG:DebugAnalyze>: ${PROJECTS_SHARED_CXX_FLAGS_DEBUGANALYZE}> )
target_compile_options( "Tests.Engine.Platforms.GAPI" PRIVATE $<$<CONFIG:Debug>: ${PROJECTS_SHARED_CXX_FLAGS_DEBUG}> )



