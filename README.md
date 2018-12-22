# GameboyCore Retro

[![Build status](https://ci.appveyor.com/api/projects/status/mmovpvpvfs04tqv3?svg=true)](https://ci.appveyor.com/project/nnarain/gameboycore-retro)
[![GitHub release](https://img.shields.io/github/release/nnarain/gameboycore-retro.svg)](https://github.com/nnarain/gameboycore-retro/releases)

[RetroArch](http://www.retroarch.com/) is a frontend for emulators and game engines. It allows anyone to develop a `core` which can be loaded by the frontend and use to emulate a particular platform.

[GameboyCore](https://github.com/nnarain/gameboycore) is a Gameboy emulator library that I have been working on. Its self-contained designed made it very easy to deploy as a `libretro` core.

Build
-----

```bash
git clone https://github.com/nnarain/gameboycore-retro
git submodule update --init --recursive

mkdir build && cd build
cmake ..
cmake --build . --config Release
```

Installation
------------

**Installer (Windows)**

Download the latest [release](https://github.com/nnarain/gameboycore-retro/releases) installer. Run the installer and point the install directory to your RetroArch installation.

**Manual (RetroArch)**

* Build the libretro core by following the above instructions
* Copy gameboycore_libretro shared library (`.dll` for Windows, `.so` for Linux) to the `<retroarch root>/cores`
* Copy `gameboycore-retro/config/gameboycore_libretro.info` to `<retroarch root>/info`


Commandline Usage
-----------------

Windows:

```bash
retroarch -L /path/to/gameboycore_libretro.dll /path/to/rom
```

Linux:

```bash
retroarch -L /path/to/gameboycore_libretro.so /path/to/rom
```
