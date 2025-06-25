#include <FastNoise/FastNoise.h>

#define PLATFORM_WINDOW_IMPLEMENTATIONS 1
#define BONSAI_DEBUG_SYSTEM_API 1

#include "external/bonsai_stdlib/bonsai_stdlib.h"
#include "external/bonsai_stdlib/bonsai_stdlib.cpp"

global_variable u32 OutputDim = 128;
global_variable u32 OutputVol = OutputDim*OutputDim*OutputDim;
global_variable u32 IterCount = 100ull;


link_internal void
DoBonsaiBenchmark(memory_arena *Memory)
{
  v3i ODim = V3i(s32(OutputDim));
  f32 *noiseOutput = Allocate(f32, Memory, OutputVol);
  auto Params = AllocatePerlinParams(ODim, Memory);

  auto Best  = 0xFFFFFFFFFFFFFFFF;
  auto Worst = 0ull;
  auto Elapsed = 0llu;
  for (u32 i = 0; i < IterCount; ++i)
  {
    auto Start = __rdtsc();
    PerlinNoise( noiseOutput, V3(1.f), ODim, V3i(0), 1, &Params);
    auto End = __rdtsc();


    auto This = End-Start;
    Best  = Min(Best , This);
    Worst = Max(Worst, This);

    Elapsed += This;
  }

  auto AvgCyclesPerCell = float(Elapsed)/float(OutputVol*IterCount);
  auto BestCyclesPerCell = float(Best)/float(OutputVol);
  auto WorstCyclesPerCell = float(Worst)/float(OutputVol);

  printf("Bonsai    Cycles/Cell Avg(%.2f) Best(%.2f) Worst(%f.2)\n",
      r64(AvgCyclesPerCell),
      r64(BestCyclesPerCell),
      r64(WorstCyclesPerCell));

}

link_internal void
DoFastNoiseBenchmark()
{
  auto Noise = FastNoise::New<FastNoise::Perlin>();
  std::vector<float> noiseOutput(OutputVol);

  auto Best = 0xFFFFFFFFFFFFFFFF;
  auto Worst = 0ull;
  auto Elapsed = 0llu;
  for (auto i = 0ull; i < IterCount; ++i)
  {
    auto Start = __rdtsc();
    Noise->GenUniformGrid3D(noiseOutput.data(), 0, 0, 0, s32(OutputDim), s32(OutputDim), s32(OutputDim), 0.2f, 1337);
    auto End = __rdtsc();


    auto This = End-Start;
    Best = Min(Best, This);
    Worst = Max(Worst, This);

    Elapsed += This;
  }

  auto AvgCyclesPerCell = float(Elapsed)/float(OutputVol*IterCount);
  auto BestCyclesPerCell = float(Best)/float(OutputVol);
  auto WorstCyclesPerCell = float(Worst)/float(OutputVol);

  printf("FastNoise Cycles/Cell Avg(%.2f) Best(%.2f) Worst(%.2f)\n",
      r64(AvgCyclesPerCell),
      r64(BestCyclesPerCell),
      r64(WorstCyclesPerCell));

}

int main()
{
  memory_arena Memory = {};
  bonsai_stdlib Stdlib = {};
  UNPACK_STDLIB(&Stdlib);

  InitializeBonsaiStdlib(BonsaiInit_InitDebugSystem, 0, &Stdlib, &Memory, 0);

  DoBonsaiBenchmark(&Memory);
  auto Noise = FastNoise::New<FastNoise::Perlin>();
  DoFastNoiseBenchmark();

  OpenAndInitializeWindow();
  PlatformMakeRenderContextCurrent(Os);
  Ensure( InitializeOpenglFunctions() );

  renderer_2d Ui = {};
  InitRenderer2D(&Ui, Plat, &Memory);

  Init_Global_QuadVertexBuffer();

  f32 ClearC =  0.4f;
  GetGL()->ClearColor(ClearC, ClearC, ClearC, 1.f);

  while (1)
  {
    BonsaiFrameBegin(&Stdlib, &Ui);

    if (Plat->Input.LMB.Pressed)
    {
      Info("dt(%.2f) (%d)", r64(Plat->dt), Plat->Input.LMB.Pressed); //Plat->Input.F1.Clicked, Plat->Input.F2.Clicked);
    }

    BonsaiFrameEnd(&Stdlib, &Ui);

    if (Stdlib.Plat.Input.Escape.Clicked) return 0;
  }
}
