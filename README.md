# GameboyCore Retro

[RetroArch](http://www.retroarch.com/) is a frontend for emulators and game engines. It allows anyone to develop a `core` which can be loaded by the frontend and use to emulate a particular platform.

[GameboyCore](https://github.com/nnarain/gameboycore) is a Gameboy emulator library that I have been working on. Its self-contained designed made it very easy to deploy as a `libretro` core.

Build
-----

```bash
set GAMEBOYCORE_ROOT=/path/to/gameboycore/root

cd /path/to/repo
mkdir build && cd build
cmake .. -DLIBRETRO_COMMON_INCLUDE_DIR
```

Usage
-----

Windows:

```bash
retroarch -L /path/to/gameboycore_libretro.dll /path/to/rom
```

Linux:

```bash
retroarch -L /path/to/gameboycore_libretro.so /path/to/rom
```
