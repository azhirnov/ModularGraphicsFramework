# Modular Graphics Framework
At first it was experimental port of my engine to another architecture, and now I make it as may primary engine.

Features:
- Many graphics APIs without recompilation.
- Low level abstraction of API: Image, Buffer, CommandBuffer, Pipeline and other.
- Run many graphics API at one time for debugging - to see different behavior (WIP).
- High level abstraction: Scene, RenderTechnique and other (WIP).
- Shader compiler - one shader for all platforms.
- Procedural generation (WIP).
- Virtual Reality (Emulator, Oculus - WIP).

Supported APIs:
- Vulkan 1.0 (primary)
- OpenGL 4.4
- DirectX 11 (WIP)
- OpenCL 1.1
- Software renderer (WIP)
- (OpenGL ES 3.0 - somewhen)

Supported OS:
- Windows
- (other with SDL2 - not tested)
- (Android - somewhen)

Supported compilers:
- VS 2017 (tested with 15.5.7)
- MinGW (tested with 7.2.0)

Dependencies:
- LunarGLASS and other https://github.com/azhirnov/ModularGraphicsFramework-Externals
- SDL2 (optional) https://www.libsdl.org/download-2.0.php
- glm (optional, required for Software renderer only) https://glm.g-truc.net/0.9.8/index.html
