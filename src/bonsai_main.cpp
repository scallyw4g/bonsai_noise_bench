#include "external/bonsai_stdlib/bonsai_stdlib.h"
#include "external/bonsai_stdlib/bonsai_stdlib.cpp"



int main()
{
  memory_arena Memory = {};
  bonsai_stdlib Stdlib = {};
  InitializeBonsaiStdlib(BonsaiInit_Default, 0, &Stdlib, &Memory, 0);

  auto OutputDim = 128;
  auto OutputVol = OutputDim*OutputDim*OutputDim;
  f32 *noiseOutput = Allocate(f32, &Memory, OutputVol);

  int IterCount = 250;
  auto Min = 0xFFFFFFFFFFFFFFFF;

  auto Elapsed = 0llu;
  auto Params = AllocatePerlinParams(V3i(OutputDim), &Memory);
  for (int i = 0; i < IterCount; ++i)
  {
    auto Start = __rdtsc();
    PerlinNoise( noiseOutput, V3(1.f), V3i(OutputDim), V3i(0), 1, &Params);
    auto End = __rdtsc();


    auto This = End-Start;
    Min = min(Min, This);

    Elapsed += This;
  }

  auto AvgCyclesPerCell = float(Elapsed)/float(OutputVol*IterCount);
  auto MinCyclesPerCell = float(Min)/float(OutputVol);

  printf("AvgCycles/Cell(%f) MinCycles/Cell(%f) \n", r64(AvgCyclesPerCell), r64(MinCyclesPerCell));

}
