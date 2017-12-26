# Modular Graphics Framework
At first it was experimental port of my engine to another architecture, and now I make it as may primary engine.

Features:
- Many graphics APIs without recompilation.
- Low level abstraction of API.
- Run many graphics API at one time for debugging - to see different behavior.
- High level abstraction (work in progress).
- Shader compiler - one shader for all platforms.
- Procedural generation (work in progress).
- VR (work in progress).

Supported APIs:
- Vulkan 1.0 (primary)
- OpenGL 4.4
- DirectX 11 (work in progress)
- OpenCL 1.1
- Soft renderer (work in progress)
- (OpenGL ES 3.0 - somewhen)

Supported OS:
- Windows
- (Android - somewhen)

Supported compilers:
- VS 2017 (C++17)
- MinGW (with C++17 support)

Dependencies:
- LunarGLASS and other https://github.com/azhirnov/ModularGraphicsFramework-Externals
