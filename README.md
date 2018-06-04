# Modular Graphics Framework
At first it was experimental port of my engine to another architecture, and now I make it as may primary engine.

Features:
- Event driven architecture.
- Many graphics APIs without recompilation.
- Low level abstraction of API: Image, Buffer, CommandBuffer, Pipeline and other.
- Run many graphics API at one time for debugging - to see different behavior (WIP).
- High level abstraction: Scene, RenderTechnique and other (WIP).
- Shader compiler - one shader for all platforms.
- Procedural generation (WIP).
- Virtual Reality (Emulator, Oculus - WIP).

Supported APIs:
- Vulkan 1.0 - 1.1 (primary)
- OpenGL 4.4 - 4.6
- DirectX 11 (WIP)
- OpenCL 1.1 - 2.0
- Software renderer
- OpenGL ES 3.0 - 3.2 (WIP)

Supported OS:
- Windows
- Linux (WIP)
- Android (WIP)

Supported compilers:
- VS 2017 (tested on 15.6.7)
- MinGW (tested on 7.2.0)

Optional dependencies:
- glslang, SDL2, GLM and other
