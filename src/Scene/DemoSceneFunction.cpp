#include "Scene/DemoSceneFunction.h"

#include "Library/Demo/DemoActorHolder.h"
#include "Library/Play/Layout/WipeHolder.h"
#include "Library/Scene/SceneUtil.h"

#include "Scene/DemoSceneStateSkipDemo.h"
#include "Scene/StageScene.h"

namespace DemoSceneFunction {
static void updateKitWithDemoActorHolderCommon(al::Scene* scene, const char* param_2,
                                               const char* param_3, const char* param_4,
                                               const sead::Vector3f* param_5) {
    al::updateKitListPrev(scene);
    al::updateKitList(scene, "カメラ");
    al::updateKitList(scene, "クリッピング");
    al::updateKitList(scene, "空");
    al::updateKitList(scene, "波");
    al::updateKitList(scene, "遠景");
    al::updateKitList(scene, "エフェクトオブジェ");
    al::updateKitList(scene, "デモオブジェクト");
    al::updateDemoActorForPauseEffect(scene);
    al::updateKitList(scene, "NPCイベントディレクター");
    al::updateKitList(scene, "通知レイアウト更新");
    al::updateKitList(scene, "シャドウマスク");
    al::updateKitList(scene, "グラフィックス要求者");
    al::updateKitList(scene, "２Ｄ");
    al::updateKitList(scene, "２Ｄ（ポーズ無視）");

    if (param_2 && param_3 && param_4) {
        if (!param_5)
            param_5 = &sead::Vector3f::zero;
        rs::requestGraphicsPresetAndCubeMap(scene, param_2, param_3, param_4, *param_5);
    }

    al::updateKitListPostDemoWithPauseNormalEffect(scene);
}

void updateKitWithDemoActorHolder(al::Scene* scene, al::DemoActorHolder* demoActorHolder,
                                  const char* param_3, const char* param_4, const char* param_5,
                                  const sead::Vector3f* param_6) {
    demoActorHolder->updateGraphics();
    updateKitWithDemoActorHolderCommon(scene, param_3, param_4, param_5, param_6);
    al::updateKitListPostOnNerveEnd(scene);
}

void updateKitWithDemoActorHolderSequence(al::Scene* scene, al::DemoActorHolder* demoActorHolder,
                                          const char* param_3, const char* param_4,
                                          const char* param_5, const sead::Vector3f* param_6) {
    demoActorHolder->updateSequence();
    updateKitWithDemoActorHolderCommon(scene, param_3, param_4, param_5, param_6);
    al::updateKitListPostOnNerveEnd(scene);
}

void updateKitWithDemoActorHolderSequenceDemoChangeWorldScene(
    al::Scene* scene, al::DemoActorHolder* demoActorHolder, const char* param_3,
    const char* param_4, const char* param_5, const sead::Vector3f* param_6) {
    demoActorHolder->updateGraphics();
    updateKitWithDemoActorHolderCommon(scene, param_3, param_4, param_5, param_6);
    if (demoActorHolder->isEndSequence())
        demoActorHolder->kill();
    al::updateKitListPostOnNerveEnd(scene);
}

void tryStartDemoEndFade(al::WipeHolder* wipeHolder, const char* wipeName,
                         DemoSceneStateSkipDemo* stateSkipDemo,
                         const al::DemoActorHolder* demoActorHolder) {
    if (!wipeName || !stateSkipDemo)
        return;
    if (!wipeHolder || !demoActorHolder)
        return;

    s32 closeTime = wipeHolder->getCloseTimeByInfo(wipeName);
    auto currentFrame = demoActorHolder->getCurrentDemoFrame();
    auto maxFrame = demoActorHolder->getCurrentDemoFrameMax();
    if (currentFrame == maxFrame - closeTime) {
        wipeHolder->startCloseByInfo(wipeName);
        stateSkipDemo->notifyStartDemoEndFade();
    }
}
}  // namespace DemoSceneFunction
