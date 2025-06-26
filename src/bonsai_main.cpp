#include <FastNoise/FastNoise.h>

#define PLATFORM_WINDOW_IMPLEMENTATIONS 1
#define BONSAI_DEBUG_SYSTEM_API 1

#include "external/bonsai_stdlib/bonsai_stdlib.h"
#include "external/bonsai_stdlib/bonsai_stdlib.cpp"

global_variable const u32 xyOutputDim = 512;
global_variable const u32  zOutputDim = 16;
global_variable u32 OutputVol = Square(xyOutputDim)*zOutputDim;
global_variable const u32 IterCount = 32ull;
/* global_variable u32 IterCount = 1ull; */


link_internal f32 *
DoBonsaiBenchmark(memory_arena *Memory)
{
  v3i ODim = V3i(s32(xyOutputDim), s32(xyOutputDim), s32(zOutputDim));
  f32 *NoiseOutput = AllocateAligned(f32, Memory, OutputVol, 32);
  auto Params = AllocatePerlinParams(ODim, Memory);

  auto Best  = 0xFFFFFFFFFFFFFFFF;
  auto Worst = 0ull;
  auto Elapsed = 0llu;
  for (s32 i = 0; i < IterCount; ++i)
  {
    v3i Offset = V3i(i*xyOutputDim, i*xyOutputDim, i*zOutputDim);
    auto Start = __rdtsc();
    PerlinNoise( NoiseOutput, V3(20.f), 1.0f, ODim, Offset, 1, &Params);
    auto End = __rdtsc();


    auto This = End-Start;
    Best  = Min(Best , This);
    Worst = Max(Worst, This);

    Elapsed += This;
    PushHistogramDataPoint(This);
  }


  auto AvgCyclesPerCell = float(Elapsed)/float(OutputVol*IterCount);
  auto BestCyclesPerCell = float(Best)/float(OutputVol);
  auto WorstCyclesPerCell = float(Worst)/float(OutputVol);

  printf("Bonsai    Cycles/Cell Avg(%.2f) Best(%.2f) Worst(%.2f)\n",
      r64(AvgCyclesPerCell),
      r64(BestCyclesPerCell),
      r64(WorstCyclesPerCell));


#if 1
  f32 *Result = AllocateAligned(f32, Memory, OutputVol, 32);
  auto Params2 = AllocatePerlinParams(ODim, Memory);
  PerlinNoise( Result, V3(20.f), 1.0f, ODim, V3i(0), 1, &Params2);
  return Result;
#else
  return NoiseOutput;
#endif
}

link_internal std::vector<float>
DoFastNoiseBenchmark()
{
  auto Noise = FastNoise::New<FastNoise::Perlin>();
  std::vector<float> NoiseOutput(OutputVol);

  auto Best = 0xFFFFFFFFFFFFFFFF;
  auto Worst = 0ull;
  auto Elapsed = 0llu;
  for (auto i = 0ull; i < IterCount; ++i)
  {
    auto Start = __rdtsc();
    Noise->GenUniformGrid3D(NoiseOutput.data(), 0, 0, 0, s32(xyOutputDim), s32(xyOutputDim), s32(zOutputDim), 0.04f, 1337);
    auto End = __rdtsc();


    auto This = End-Start;
    Best = Min(Best, This);
    Worst = Max(Worst, This);

    Elapsed += This;
    /* MAIN_THREAD_ADVANCE_DEBUG_SYSTEM(Plat->dt); */
    PushHistogramDataPoint(This);
  }

  auto AvgCyclesPerCell = float(Elapsed)/float(OutputVol*IterCount);
  auto BestCyclesPerCell = float(Best)/float(OutputVol);
  auto WorstCyclesPerCell = float(Worst)/float(OutputVol);

  printf("FastNoise Cycles/Cell Avg(%.2f) Best(%.2f) Worst(%.2f)\n",
      r64(AvgCyclesPerCell),
      r64(BestCyclesPerCell),
      r64(WorstCyclesPerCell));

  return NoiseOutput;
}

int main()
{
  memory_arena Memory = {};
  bonsai_stdlib Stdlib = {};
  UNPACK_STDLIB(&Stdlib);

  InitializeBonsaiStdlib(BonsaiInit_InitDebugSystem, 0, &Stdlib, &Memory, 0);

  auto BonsaiData = DoBonsaiBenchmark(&Memory);
  auto FNData     = DoFastNoiseBenchmark();

  OpenAndInitializeWindow();
  PlatformMakeRenderContextCurrent(Os);
  Ensure( InitializeOpenglFunctions() );

  texture BonsaiTextures[zOutputDim];
  RangeIterator(z, zOutputDim)
  {
    auto Data = BonsaiData+(xyOutputDim*xyOutputDim*z);
    BonsaiTextures[z] = MakeTexture_SingleChannel( V2i(s32(xyOutputDim)), Data, CSz(""), False );
  }

  texture FNTextures[zOutputDim];
  RangeIterator(z, zOutputDim)
  {
    FNTextures[z] = MakeTexture_SingleChannel( V2i(s32(xyOutputDim)), FNData.data()+(xyOutputDim*xyOutputDim*z), CSz(""), False );
  }


  renderer_2d Ui = {};
  InitRenderer2D(&Ui, Plat, &Memory);

  f32 ClearC =  0.4f;
  GetGL()->ClearColor(ClearC, ClearC, ClearC, 1.f);

  window_layout Window = WindowLayout("NoiseTextures");
  while (Os->ContinueRunning)
  {
    BonsaiFrameBegin(&Stdlib, &Ui);

    PushWindowStart(&Ui, &Window);
    RangeIterator(i, zOutputDim)
    {
      u32 Start = StartColumn(&Ui);
        PushTexturedQuad(&Ui, &BonsaiTextures[i], V2(s32(xyOutputDim)), zDepth_Text);
      EndColumn(&Ui, Start);
      if ( ((i+1) % 4) == 0)
      {
        PushNewRow(&Ui);
      }
    }

    PushNewRow(&Ui);
    PushNewRow(&Ui);
    PushNewRow(&Ui);
    PushNewRow(&Ui);
    PushNewRow(&Ui);
    PushNewRow(&Ui);

    RangeIterator(i, zOutputDim)
    {
      u32 Start = StartColumn(&Ui);
        PushTexturedQuad(&Ui, &FNTextures[i], V2(s32(xyOutputDim)), zDepth_Text);
      EndColumn(&Ui, Start);
      if ( ((i+1) % 4) == 0)
      {
        PushNewRow(&Ui);
      }
    }
    PushWindowEnd(&Ui, &Window);

    BonsaiFrameEnd(&Stdlib, &Ui);

    if (Stdlib.Plat.Input.Escape.Clicked) { Os->ContinueRunning = False; }
  }
}
