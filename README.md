# WII U SDL2 STARTER

(Based off ChuckieEggNX)

A Very simple Nintendo WiiU SDL2 STARTER.

# Building

Requires devkitPro with the Nintendo WiiU toolchain, SDL2 for WiiU and libromfs-wiiu.

## Building instructions

I built everything using Windows 10

* Install [devkitpro](https://devkitpro.org/wiki/Getting_Started#Unix-like_platforms)
* Install [libromfs-wiiu](https://github.com/yawut/libromfs-wiiu)
* On a terminal install needed libraries:
  
  `pacman -S `
  `pacman -Syu wiiu-sdl2 wiiu-sdl2_image wiiu-sdl2_mixer wiiu-sdl2_ttf`
* Clone this repo:  
  `git clone https://github.com/ArkhamKnightXD/wii-u-sdl-starter.git`

* `cd wii-u-sdl-starter`
* `make`