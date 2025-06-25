# Test bench for Bonsai noise generation functions

## Compiling

`./make.sh` should work on Windows and produce a pair of binaries; `bonsai_main.exe` and `fastnoise_main.exe`

## Timing

On my laptop, I get :

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
