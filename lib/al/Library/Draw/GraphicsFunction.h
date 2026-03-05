#pragma once

#include <basis/seadTypes.h>
#include <common/aglGPUCommon.hpp>
#include <math/seadMatrix.h>
#include <math/seadVector.h>

#include "nn/gfx/gfx_Buffer.h"
#include "nn/gfx/gfx_Common.h"
#include "nvn/nvn.h"

// TODO: Someone should add this to nnheaders!
namespace nn::g3d {
class MaterialObj;
class ResVertex;
class ResMesh;
}  // namespace nn::g3d

// TODO: Someone should add this to sead!
namespace sead {
class GraphicsContext;
}

namespace agl {
struct ConstGPUMemVoidAddr;
}

namespace al {
class Scene;
class LiveActor;
class ModelCtrl;
class ModelShaderHolder;

f32 getDepthClearValue();

void setDepthFuncNearDraw(sead::GraphicsContext* context);
void setDepthFuncFarDraw(sead::GraphicsContext* context);
void setDepthFuncNearDraw(sead::GraphicsContext* context);
void setDepthFuncFarDraw(sead::GraphicsContext* context);
bool getAlphaTestEnable(nn::g3d::MaterialObj* material);
}  // namespace al

namespace alGraphicsFunction {
bool isEnableRain(al::Scene*);
void setFogNoiseUpdateFlag(al::Scene*, bool);
const sead::Vector3f& getDirectionalLightDir(const al::LiveActor*);
const sead::Vector3f& calcDirectionalLightDir(const al::LiveActor*);
f32 getDepthShadowFarLength(const al::LiveActor*);
void invalidateCameraBlurSystem(al::Scene*);
void validateCameraBlurSystem(al::Scene*);
void invalidateOcclusionCullingSystem(al::Scene*);
void validateOcclusionCullingSystem(al::Scene*);
bool isGraphicsQualityModeConsole(const al::LiveActor*);
bool isGraphicsQualityModeHandheld(const al::LiveActor*);
void forceGraphicsQualityModeConsole(al::Scene*);
void forceGraphicsQualityModeHandheld(al::Scene*);
void forceGraphicsQualityModeSnapShot(al::Scene*);
void unforceGraphicsQualityMode(al::Scene*);
void validateModelLodAll(al::Scene*);
void invalidateModelLodAll(al::Scene*);
void setSphereMapUpdateEveryFrame(al::Scene*, const sead::Vector3f&);
void setMaterialLightUpdateBaseAngleChanged(al::Scene*, bool);
void requestChangeShaderVariation(const al::LiveActor*, const char*, const char*, bool);
void requestChangeShaderVariation(al::ModelCtrl*, int, al::ModelShaderHolder*, const char*,
                                  const char*, bool);
void requestChangeShaderVariation(al::ModelCtrl*, al::ModelShaderHolder*, const char*, const char*,
                                  bool);
void requestChangeShaderVariationWithoutForward(const al::LiveActor*, const char*, const char*,
                                                bool);
void requestUpdateMaterialInfo(al::Scene*);
void validateGpuStressAnalyzer(al::Scene*);
void invalidateGpuStressAnalyzer(al::Scene*);
bool isValidGpuStressAnalyzer(al::Scene*);
void getMemoryPoolPtr(const nn::gfx::TBuffer<nn::gfx::ApiVariationNvn8>&);
void getMemoryPoolSize(const NVNmemoryPool*);
void calcMemoryPoolOffset(const nn::gfx::TBuffer<nn::gfx::ApiVariationNvn8>&);
void getGPUAddressMemoryPool(NVNmemoryPool*);
void getGPUAddress(const nn::gfx::TBuffer<nn::gfx::ApiVariationNvn8>&);
void getGPUAddressMemoryPool(const nn::gfx::TBuffer<nn::gfx::ApiVariationNvn8>&);
void getGPUAddress(const agl::GPUMemVoidAddr&);
void getGPUAddress(const agl::ConstGPUMemVoidAddr&);
void getGPUAddress(const NVNbuffer&);
void calcVertexBufferGPUAddress(const nn::g3d::ResVertex&, int);
void calcIndexBufferGPUAddress(nn::g3d::ResMesh&);
}  // namespace alGraphicsFunction
