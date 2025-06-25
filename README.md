# Test bench for Bonsai noise generation functions

## Overview

The perlin noise function in Bonsai is very fast.  In fact, at the time of this
writing, it is the fastest open-source implementation in the world.  

## Building on Windows

* Install [MSYS](https://www.msys2.org/#installation)
* From the `MSYS MINGW64` shell, run `pacman -S mingw-w64-x86_64-cmake clang`

`./make.sh` should now build FastNoise2 and the bonsai test bench.
The executable will be in the `bin` directory.


## Timing

On my laptop :

```
$ bin/bonsai_main.exe
   Info    - Initializing Bonsai
AvgCycles/Cell(4.358449) MinCycles/Cell(4.064439)
```

```
$ bin/fastnoise_main.cpp
AvgCycles/Cell(10.452278) MinCycles/Cell(9.677464)
```

# TODO

* side-by-side viewer of output
* 
