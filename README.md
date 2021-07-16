# Forked from TheCherno
Most of this code was written by TheCherno. I use this fork to test things with ImGui.

!!! You need to download the the docking branch from ImGui with version < 1.84 yourself and replace the files in OpenGL-Core/vendor/imgui with the new one.

!!! You also need to change the "files" section from OpenGL-Core/vendor/imgui/premake5.lua to include "imgui_tables.cpp"

# OpenGL-Core
Work-in-progress OpenGL library that aims to provide a powerful sandbox for you to learn or experiment with OpenGL, and graphics programming in general.

## Usage

Currently only "officially" supports Windows - Mac and Linux support is coming soon.

```
git clone --recursive https://github.com/Fiixel/OpenGL
```

Run `scripts/Win-Premake.bat` and open `OpenGL-Sandbox.sln` in Visual Studio 2019. `OpenGL-Sandbox/src/SandboxLayer.cpp` contains the example OpenGL code that's running.
