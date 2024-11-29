# 🕹️ Play Man
An awesome retro console emulator written in c++.  
Made with ❤️ by [@rvan-mee](https://github.com/rvan-mee) and [@K1ngmar](https://github.com/K1ngmar)

## ⚙️ Requirements
- Cmake version >= 3.22.1
- A compiler with >= c++17 support

## 🔨 How to install
```bash
# clone the respository
$ git clone --recurse-submodules git@github.com:rvan-mee/play-man.git

# create build folder
$ mkdir build
$ cd build

# generate build files
$ cmake ..

# build the project
$ make

# play some awesome emulated games
$ ./play-man
```

## Tools used

### Graphics, input and audio library
For our graphics, input and audio we use [Simple DirectMedia Layer](https://www.libsdl.org/). We chose this library because it grants us low level access to the graphicshardware, audio and input devices. It also supports OpenGl and Direct3D.

## 🗺️ Emulation Roadmap

### 🎯 Supported compilation targets
- [ ] pc
	- [ ] linux
	- [ ] windows
	- [ ] mac
- [ ] Raspberry pi
- [ ] Webassembly
- [ ] Micro controller? (hopefully some day)

### 💻 Devices to emulate
- [ ] Gameboy
	- [ ] Gameboy
	- [ ] Gameboy Color
	- [ ] Gameboy Advanced
- [ ] Gamecube
- [ ] Ds
- [ ] N64
- [ ] NES
- [ ] SNES

## Sources
- [gameboy programming manual](https://ia803208.us.archive.org/9/items/GameBoyProgManVer1.1/GameBoyProgManVer1.1.pdf)
- [gameboy development community](https://gbdev.io/)
- [gameboy technical refference](https://gekkio.fi/files/gb-docs/gbctr.pdf)
