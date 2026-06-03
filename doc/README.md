# SolveIt

SolveIt is a legacy MFC/OpenGL desktop application with a substantial numerical backend in Fortran and legacy CVODE/CVOde support. This repository has been migrated to a modern Visual Studio 2022 x64 build that targets Unicode and C++23 while preserving the existing application structure and behavior.

## Current Status

- `SolveIt.vcxproj` is the primary modern build entry point.
- `Debug|x64` and `Release|x64` are the validated target configurations.
- Expected application outputs:
  - `x64/Debug/SolveIt.exe`
  - `x64/Release/SolveIt.exe`
- The Intel oneAPI Fortran static libraries are built by `fortran/build_top_oneapi.ps1` and placed in `SolveIt.Lib/lib`.
- The legacy CVODE/CVOde C sources are compiled into `x64/<Configuration>/cvode_legacy.lib` by `SolveIt.Lib/lib/CVOde/build_clang_cl_lib.ps1` during the project pre-link step.
- The x64 build also generates `SolveIt.Lib/lib/dformt_x64.lib` from `SolveIt.Lib/lib/dformt_x64_stub.c` via `SolveIt.Lib/lib/build_dformt_x64_stub.ps1`.
- `SolveIt.Lib/lib/CVOde/CVOdeShim.cpp` preserves the historical `CVODE::...` API that the application expects.

## Prerequisites

- Visual Studio 2022 with MFC/ATL desktop C++ tooling
- Visual Studio LLVM tools (`clang-cl`) for the legacy CVOde static library build
- Intel oneAPI Fortran compiler (`ifx`)
- `vcpkg` packages for the x64 build, including:
  - `eigen3:x64-windows`
  - `spectra:x64-windows`
  - C:\Users\nsh\Documents\repos\vcpkg
- MiKTeX or another LaTeX distribution only if you want to regenerate `README.pdf`

## Build Workflow

Build the Fortran libraries first:

```powershell
powershell -ExecutionPolicy Bypass -File .\fortran\build_top_oneapi.ps1 -Configuration Debug
powershell -ExecutionPolicy Bypass -File .\fortran\build_top_oneapi.ps1 -Configuration Release
```

Then build the application project:

```powershell
MSBuild.exe .\SolveIt.vcxproj /p:Configuration=Debug /p:Platform=x64 /m /nologo
MSBuild.exe .\SolveIt.vcxproj /p:Configuration=Release /p:Platform=x64 /m /nologo
```

Notes:

- The x64 project automatically rebuilds `cvode_legacy.lib` during pre-link.
- The x64 project automatically regenerates `dformt_x64.lib` during pre-link. The original `SolveIt.Lib/lib/dformt.LIB` is left in place for the legacy Win32 configuration only.
- The validated modern target is x64. The older Win32 configuration is kept for reference, not as the primary migration target.

## Migration Notes

- Unicode compatibility is handled at API boundaries rather than by a global string rewrite. The shared helpers live in `StdAfx.h`.
- The legacy CVODE/CVOde wrapper headers were adjusted so their public C-facing types have stable C++ linkage.
- High-value x64 warning cleanup has already been applied in several concentrated hotspots, including pointer-to-`long` payload storage in the rigid-body, tree, and plotting paths.
- The legacy CVOde build still uses separate compilation with `clang-cl` in `gnu89` mode to avoid destabilizing the main C++ build.

## Known Limitations

- The project still emits legacy warnings, mainly in these categories:
  - deprecated CRT calls such as `strcpy`, `strtok`, `freopen`, and `_swprintf`
  - deprecated `strstream` usage in older parser, stack-machine, and plotting code
  - `size_t` to OpenGL or Win32 integer conversions in older rendering and UI code
  - some remaining narrowing conversions in message-passing and UI helpers
- The original `dformt.LIB` is a legacy x86 Digital/Compaq Visual Fortran archive. Current x64 builds do not consume symbols from it, so x64 now links against the generated `dformt_x64.lib` placeholder instead.

## Repository Artifacts

- `README.tex` is the LaTeX source for the PDF version of this document.
- `README.pdf` is generated from `README.tex` with `pdflatex`.
