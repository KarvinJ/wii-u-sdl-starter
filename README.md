# WII U SDL STARTER

(Based off ChuckieEggNX)

A Very simple Nintendo WiiU SDL STARTER.

# Building

Requires devkitPro with the Nintendo WiiU toolchain and SDL2 for WiiU.

## Building instructions

I recomend a Linux Debian like operational system for better tools compatibility:
* Install [devkitpro](https://devkitpro.org/wiki/Getting_Started#Unix-like_platforms)
* On a terminal install needed libraries:  
  `pacman -Syu wiiu-sdl2 wiiu-sdl2_image wiiu-sdl2_mixer wiiu-sdl2_ttf`
* Clone this repo:  
  `git clone https://github.com/ArkhamKnightXD/wii-u-sdl-starter.git`

* `cd wii-u-sdl-starter`
* `make`