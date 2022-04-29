#include <iostream>
#include <ostream>
#include <vector>
#include <cuda_runtime.h>
#include <owl/owl.h>
#include "deviceCode.h"

extern "C" char deviceCode_ptx[];

OWLVarDecl rayGenVars[]
= {
    { nullptr /* sentinel to mark end of list */ }
};

OWLVarDecl launchParamsVars[]
= {
    { "CoordsBuffer", OWL_BUFPTR, OWL_OFFSETOF(LaunchParams,CoordsBuffer) },
    { nullptr /* sentinel to mark end of list */ }
};

int main(int argc, char** argv) {

    OWLContext owl = owlContextCreate(nullptr,1);
    OWLModule module = owlModuleCreate(owl,deviceCode_ptx);
    OWLRayGen raygen = owlRayGenCreate(owl,module,"raygen",
                                                  sizeof(RayGen),rayGenVars,-1);
    OWLParams lp = owlParamsCreate(owl,sizeof(LaunchParams),launchParamsVars,-1);

    OWLBuffer CoordsBuffer = owlDeviceBufferCreate(owl,OWL_FLOAT3,512*512,NULL);

    owlParamsSetBuffer(lp,"CoordsBuffer",CoordsBuffer);

    owlBuildPrograms(owl);
    owlBuildPipeline(owl);
    owlBuildSBT(owl);

    owlLaunch2D(raygen,512,512,lp);

    std::vector<owl::vec3f> HostCoords(512*512);
    cudaMemcpy(HostCoords.data(),owlBufferGetPointer(CoordsBuffer,0),HostCoords.size()*sizeof(owl::vec3f),
               cudaMemcpyDeviceToHost);

    for (size_t i=0; i<HostCoords.size(); ++i) {
        std::cout << HostCoords[i] << '\n';
    }
}

