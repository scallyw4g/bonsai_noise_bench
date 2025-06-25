#include <vector>
#include <FastNoise/FastNoise.h>

#define min(a, b) a < b ? a : b

int main()
{
  auto Noise = FastNoise::New<FastNoise::Perlin>();


  auto OutputDim = 128;
  auto OutputVol = OutputDim*OutputDim*OutputDim;
  std::vector<float> noiseOutput(OutputVol);

  int IterCount = 250;
  auto Min = 0xFFFFFFFFFFFFFFFF;

  auto Elapsed = 0llu;
  for (int i = 0; i < IterCount; ++i)
  {
    auto Start = __rdtsc();
    Noise->GenUniformGrid3D(noiseOutput.data(), 0, 0, 0, OutputDim, OutputDim, OutputDim, 0.2f, 1337);
    auto End = __rdtsc();


    auto This = End-Start;
    Min = min(Min, This);

    Elapsed += This;
  }

  auto AvgCyclesPerCell = float(Elapsed)/float(OutputVol*IterCount);
  auto MinCyclesPerCell = float(Min)/float(OutputVol);

  printf("AvgCycles/Cell(%f) MinCycles/Cell(%f) \n", AvgCyclesPerCell, MinCyclesPerCell);

}
