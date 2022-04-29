#include "deviceCode.h"

using namespace owl;

extern "C" __constant__ LaunchParams optixLaunchParams;

OPTIX_RAYGEN_PROGRAM(raygen)()
{
    auto& lp = optixLaunchParams;
    const vec2i launchIdx = owl::getLaunchIndex();

    // just fill with sth. we can print later..
    lp.CoordsBuffer[launchIdx.y*512+launchIdx.x] = owl::vec3f(launchIdx.x,launchIdx.y,0.f);
}
