#pragma once

#include "Library/Nerve/NerveExecutor.h"

namespace al {
class GraphicsQualityInfo;
class GraphicsSystemInfo;
class SceneCameraInfo;
class GpuPerfAreaController ;
class GraphicsParamIo;
class ParameterArray;
class ParameterIo;

class GraphicsQualityController : public NerveExecutor {
public:
    // missing

    GraphicsQualityInfo* getGraphicsQualityInfo() const { return mGraphicsQualityInfo; }

    bool isChangedGraphicsQualityMode() const { return mIsChangedGraphicsQualityMode; }

private:
    bool mIsGpuStressAnalyzerValid;
    GraphicsQualityInfo* mGraphicsQualityInfo;
    GraphicsSystemInfo* mGraphicsSystemInfo;
    SceneCameraInfo* mSceneCameraInfo;
    GpuPerfAreaController* mGpuPerfAreaController;
    void* _20[9];
    GraphicsParamIo* mGraphicsParamIo;
    ParameterArray * _88;
    ParameterIo* _90;
    ParameterArray* _98;
    s32 _a0;
    s32 _a4;

    bool mIsChangedGraphicsQualityMode;
    void* _b0[11];
};

static_assert(sizeof(GraphicsQualityController) == 0x108);

}  // namespace al
