#!/usr/bin/env bash

# TODO(Jesse)(build): Revisit this blog and see if there are some tidbits we can use.
# http://ptspts.blogspot.com/2013/12/how-to-make-smaller-c-and-c-binaries.html


# SANITIZER="-fsanitize=undefined"
# SANITIZER="-fsanitize=address"
# SANITIZER="-fsanitize=thread"

BUILD_EVERYTHING=0

RunPoof=0

BuildExecutables=0
BuildExamples=0

BuildTests=0
BuildDebugOnlyTests=0

RunTests=0

stdlib_build_scripts='external/bonsai_stdlib/scripts'
source $stdlib_build_scripts/preamble.sh
source $stdlib_build_scripts/setup_for_cxx.sh


OPTIMIZATION_LEVEL=""
EMCC=0


# NOTE(Jesse): Can't do this cause it fucks with the paths poof outputs
# ROOT="$(pwd)"
ROOT="."
SRC="$ROOT/src"
INCLUDE="$ROOT/external"
EXAMPLES="$ROOT/examples"
TESTS="$SRC/tests"
BIN="$ROOT/bin"
BIN_TEST="$BIN/tests"
BIN_GAME_LIBS="$BIN/game_libs"

BONSAI_INTERNAL='-D BONSAI_INTERNAL=1'

EXECUTABLES_TO_BUILD="
  $SRC/game_loader.cpp
  $SRC/font/ttf.cpp
"
  # $SRC/tools/asset_packer.cpp
  # $SRC/net/server.cpp

# COMPILER="clang++-19"
COMPILER="clang++"

function BuildExecutables
{
  echo ""
  ColorizeTitle "Executables"
  for executable in $EXECUTABLES_TO_BUILD; do
    SetOutputBinaryPathBasename "$executable" "$BIN"
    echo -e "$Building $executable"
    $COMPILER                                       \
      $SANITIZER                                     \
      $OPTIMIZATION_LEVEL                            \
      $CXX_OPTIONS                                   \
      $BONSAI_INTERNAL                               \
      $PLATFORM_CXX_OPTIONS                          \
      $PLATFORM_LINKER_OPTIONS                       \
      $PLATFORM_DEFINES                              \
      $PLATFORM_INCLUDE_DIRS                         \
      -I "$ROOT"                                     \
      -I "$SRC"                                      \
      -I "$INCLUDE"                                  \
      -o "$output_basename""$PLATFORM_EXE_EXTENSION" \
      $executable &

    TrackPid "$executable" $!

  done
}


if [ ! -d "$BIN" ]; then
  mkdir "$BIN"
fi

function RunEntireBuild {

  if [ $RunPoof == 1 ]; then
    RunPoof
    [ $? -ne 0 ] && exit 1
  fi

  BuildWithClang
  [ $? -ne 0 ] && exit 1

}


function RunPoofHelper {

   # -D _M_CEE_PURE \

   # -I "C:/Program Files (x86)/Microsoft Visual Studio/2019/Community/VC/Tools/MSVC/14.28.29333/include"        \
   # -I "C:/Program Files (x86)/Microsoft Visual Studio/2019/Community/VC/Tools/MSVC/14.28.29333/atlmfc/include" \

   # -I "C:/Program Files/LLVM/lib/clang/11.0.0/include"                                                         \
   # -I "C:/Program Files (x86)/Microsoft Visual Studio/2019/Community/VC/Tools/MSVC/14.28.29333/include"        \
   # -I "C:/Program Files (x86)/Microsoft Visual Studio/2019/Community/VC/Tools/MSVC/14.28.29333/atlmfc/include" \
   # -I "C:/Program Files (x86)/Windows Kits/10/Include/10.0.18362.0/ucrt"                                       \
   # -I "C:/Program Files (x86)/Windows Kits/10/include/10.0.18362.0/shared"                                     \
   # -I "C:/Program Files (x86)/Windows Kits/10/include/10.0.18362.0/um"                                         \
   # -I "C:/Program Files (x86)/Windows Kits/10/include/10.0.18362.0/winrt"                                      \

   which poof > /dev/null 2>&1
   if [ $? -eq 0 ]; then

   cmd="poof $COLOR_FLAG -D POOF_PREPROCESSOR -D BONSAI_PREPROCESSOR -I src/ -I external/ $PLATFORM_DEFINES $BONSAI_INTERNAL -o generated $1 "

   echo "$cmd"
   $cmd

   else
     echo "poof not found, skipping."
   fi


}

function RunPoof
{
  echo -e ""
  echo -e "$Delimeter"
  echo -e ""

  ColorizeTitle "Poofing"

  # [ -d src/generated ] && rm -Rf src/generated
  # [ -d generated ] && rm -Rf generated

  # RunPoofHelper examples/ui_test/game.cpp && echo -e "$Success poofed examples/ui_test/game.cpp" &
  # TrackPid "" $!

  # RunPoofHelper src/game_loader.cpp && echo -e "$Success poofed src/game_loader.cpp" &
  # TrackPid "" $!

  # RunPoofHelper examples/turn_based/game.cpp && echo -e "$Success poofed examples/turn_based/game.cpp" &
  # TrackPid "" $!

  RunPoofHelper examples/terrain_gen/game.cpp && echo -e "$Success poofed examples/terrain_gen/game.cpp" &
  TrackPid "" $!

  # RunPoofHelper examples/the_wanderer/game.cpp && echo -e "$Success poofed examples/the_wanderer/game.cpp" &
  # TrackPid "" $!

  # RunPoofHelper examples/tools/voxel_synthesis_rule_baker/game.cpp && echo -e "$Success poofed examples/tools/voxel_synthesis_rule_baker/game.cpp" &
  # TrackPid "" $!

  # RunPoofHelper src/tools/asset_packer.cpp && echo -e "$Success poofed src/tools/asset_packer.cpp" &
  # TrackPid "" $!

  WaitForTrackedPids
  sync

  [ -d tmp ] && rmdir tmp
}

BuildAll() {

  BuildExamples=1
  BuildExecutables=1
  BuildTests=1

  for ex in $BUNDLED_EXAMPLES; do
    EXAMPLES_TO_BUILD="$EXAMPLES_TO_BUILD $ex"
  done
}

if [ $# -eq 0 ]; then
  OPTIMIZATION_LEVEL="-O2"
  BuildAll
fi

BundleRelease=0
while (( "$#" )); do
  CliArg=$1
  echo $CliArg

  case $CliArg in

    "BuildAll")
      BuildAll
    ;;

    "BuildExecutables")
      BuildExecutables=1
    ;;

    "BuildDebugSystem")
      echo "BuildDebugSystem has been deprecated and will be removed in a future version."
    ;;

    "BuildBundledExamples")
      BuildExamples=1
      for ex in $BUNDLED_EXAMPLES; do
        EXAMPLES_TO_BUILD="$EXAMPLES_TO_BUILD $ex"
      done
    ;;

    "BuildTests")
      BuildTests=1
    ;;

    "BuildDebugOnlyTests")
      BuildDebugOnlyTests=1
    ;;

    "RunTests")
      RunTests=1
    ;;

    "RunPoof")
      RunPoof=1
    ;;

    "BuildWithEMCC")
      BuildWithEMCC=1
    ;;

    "BuildSingleExample")
      BuildExamples=1
      EXAMPLES_TO_BUILD="$EXAMPLES_TO_BUILD $2"
      shift
    ;;

    "BundleRelease")
      BundleRelease=1
      OPTIMIZATION_LEVEL="-O2"
      BONSAI_INTERNAL="-D BONSAI_INTERNAL=0"
      # BuildAll
    ;;

    "-Od")
      OPTIMIZATION_LEVEL="-Od"
    ;;

    "-O0")
      OPTIMIZATION_LEVEL="-O0"
    ;;

    "-O1")
      OPTIMIZATION_LEVEL="-O1"
    ;;

    "-O2")
      OPTIMIZATION_LEVEL="-O2"
    ;;

    "-O3")
      OPTIMIZATION_LEVEL="-O3"
    ;;


    *)
      echo "Unrecognized Build Option ($CliArg), exiting." && exit 1
    ;;
  esac

  shift
done

time RunEntireBuild
