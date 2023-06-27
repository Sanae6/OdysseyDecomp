#include <al/Library/Base/String.h>
#include <al/Library/LiveActor/ActorModelFunction.h>
#include <al/Library/LiveActor/ActorResourceFunction.h>
#include <al/Library/LiveActor/ActorSensorFunction.h>
#include <al/Library/LiveActor/SubActorKeeper.h>
#include <al/Library/Obj/BreakModel.h>
#include <al/Library/Obj/CollisionObj.h>
#include <al/Library/Obj/DepthShadowModel.h>
#include <al/Library/Obj/ModelDrawParts.h>
#include <al/Library/Obj/PartsModel.h>
#include <al/Library/Obj/SilhouetteModel.h>
#include <al/Library/Obj/SimpleCircleShadowXZ.h>
#include <al/Library/Yaml/ByamlUtil.h>

namespace al {
SubActorKeeper::SubActorKeeper(al::LiveActor *rootActor) {
    mRootActor = rootActor;
}

void SubActorKeeper::registerSubActor(al::LiveActor* subActor, u32 syncType) {
    mBuffer[mCurActorCount] = new al::SubActorInfo(subActor, static_cast<SubActorSync>(syncType));
    mCurActorCount++;
}
// NON-MATCHING
void SubActorKeeper::init(al::ActorInitInfo const& initInfo,char const* suffix,int maxSubActors) {
    sead::FixedSafeString<0x80> actorInitFileName;
    int creatorCount;
    u8 *modelResourceYaml;

    if(al::isExistModelResource(mRootActor) && !al::tryGetActorInitFileName(&actorInitFileName, mRootActor, "InitSubActor", suffix)) {
        al::createFileNameBySuffix(&actorInitFileName, "InitSubActor", suffix);
    }

    if(al::isExistModelResource(mRootActor) && al::isExistModelResourceYaml(mRootActor, actorInitFileName.cstr(), nullptr)) {
        modelResourceYaml = al::getModelResourceYaml(mRootActor, actorInitFileName.cstr(), nullptr);
        al::ByamlIter modelResourceIter(modelResourceYaml);
        al::ByamlIter initInfoIter;
        if(modelResourceIter.tryGetIterByKey(&initInfoIter, "InitInfo")) {
            int addActorNum = 0;
            if(initInfoIter.tryGetIntByKey(&addActorNum, "AddActorNum"))
                maxSubActors += addActorNum;
        }
        al::ByamlIter creatorIter;
        if(modelResourceIter.tryGetIterByKey(&creatorIter, "CreatorList"))
            creatorCount = creatorIter.getSize();
        else
            creatorCount = 0;
    }else {
        modelResourceYaml = nullptr;
        creatorCount = 0;
    }

    int actorCount = maxSubActors + creatorCount;
    mMaxActorCount = actorCount;
    mBuffer = new al::SubActorInfo*[maxSubActors + creatorCount];

    if(actorCount >= 1 && mMaxActorCount >= 2)
        for (int i = 1; i < mMaxActorCount; ++i)
            mBuffer[i] = nullptr;

    if(modelResourceYaml && creatorCount > 0) {
        al::ByamlIter modelResourceIter(modelResourceYaml);
        al::ByamlIter creatorIter;
        modelResourceIter.tryGetIterByKey(&creatorIter, "CreatorList");

        // TODO: finish the logic for this, it seems like theres some heavy optimizations going on, making it tough to figure out original logic
        for (int i = 0; i < creatorCount; ++i) {
            al::ByamlIter subActorIter;
            creatorIter.tryGetIterByIndex(&subActorIter, i);

            al::SubActorInfo *actorInfo = new SubActorInfo();
            mBuffer[i] = actorInfo;

            const char* actorObjectName = al::tryGetByamlKeyStringOrNULL(subActorIter, "ObjectName");
            const char* actorModelName = al::tryGetByamlKeyStringOrNULL(subActorIter, "ModelName");
            const char* actorSuffix = al::tryGetByamlKeyStringOrNULL(subActorIter, "InitFileSuffixName");
            const char* actorClassName = al::tryGetByamlKeyStringOrNULL(subActorIter, "ClassName");
            const char* actorCategoryName = al::tryGetByamlKeyStringOrNULL(subActorIter, "CategoryName");

            bool isAlive = false;
            bool isAliveResult = subActorIter.tryGetBoolByKey(&isAlive, "IsAlive");

            bool isUseHostPlacementInfo = true;
            subActorIter.tryGetBoolByKey(&isUseHostPlacementInfo, "IsUseHostPlacementInfo");

            bool isSyncAppear = false;
            bool isGotSyncAppear = al::tryGetByamlBool(&isSyncAppear, subActorIter, "IsSyncAppear");
            if(isSyncAppear)
                actorInfo->mSyncType |= SubActorSync::cAppear;

            bool isSyncHide = false;
            bool isGotSyncHide = al::tryGetByamlBool(&isSyncHide, subActorIter, "IsSyncHide");
            if(isSyncHide)
                actorInfo->mSyncType |= SubActorSync::cHide;

            if(al::tryGetByamlKeyBoolOrFalse(subActorIter, "IsSyncAlphaMask"))
                actorInfo->mSyncType |= SubActorSync::cAlphaMask;

            if(al::tryGetByamlKeyBoolOrFalse(subActorIter, "IsSyncClipping"))
                actorInfo->mSyncType |= SubActorSync::cClipping;

            bool isCalcDepthShadowLength = true;
            al::tryGetByamlBool(&isCalcDepthShadowLength, subActorIter, "IsCalcDepthShadowLength");

            if(actorClassName && !al::isEqualString(actorClassName, "LiveActor")) {
                if(al::isEqualString(actorClassName, "PartsModel")) {
                    const char* actorFixFileSuffixName = al::tryGetByamlKeyStringOrNULL(subActorIter, "FixFileSuffixName");
                    PartsModel *partsModel = new PartsModel(actorObjectName);
                    partsModel->initPartsFixFileNoRegister(mRootActor, initInfo, actorModelName, actorSuffix, actorFixFileSuffixName);
                    actorInfo->mSubActor = partsModel;

                    actorInfo->mSyncType = (!isGotSyncAppear ? actorInfo->mSyncType | SubActorSync::cAppear : actorInfo->mSyncType)
                                           | SubActorSync::cClipping;

                    if(al::isExistModel(partsModel)) {
                        actorInfo->mSyncType = (!isGotSyncHide ? actorInfo->mSyncType | SubActorSync::cHide : actorInfo->mSyncType)
                                               | SubActorSync::cAlphaMask;
                    }
                }
                else if(al::isEqualString(actorClassName, "BreakModel")) {
                    const char *actionName = al::tryGetByamlKeyStringOrNULL(subActorIter, "ActionName");
                    const char *jointName = al::tryGetByamlKeyStringOrNULL(subActorIter, "JointName");

                    sead::Matrix34f *jointMtxPtr = jointName ? al::getJointMtxPtr(mRootActor, jointName) : nullptr;

                    if(!actionName)
                        actionName = "Break";

                    al::BreakModel *breakModel = new BreakModel(mRootActor, actorObjectName, actorModelName,
                                                                actorSuffix, jointMtxPtr, actionName);

                    al::initCreateActorNoPlacementInfo(breakModel, initInfo);
                    actorInfo->mSubActor = breakModel;
                }
                else if(al::isEqualString(actorClassName, "SilhouetteModel")) {
                    actorInfo->mSubActor = new al::SilhouetteModel(mRootActor, initInfo, actorCategoryName);
                }
                else {
                    if(al::isEqualString(actorClassName, "DepthShadowModel")) {
                        actorInfo->mSubActor = new al::DepthShadowModel(mRootActor, initInfo,
                                                                        actorCategoryName ? actorCategoryName : actorSuffix,
                                                                        isCalcDepthShadowLength);

                        continue;
                    }
                    else if(al::isEqualString(actorClassName, "InvincibleModel")) {
                        actorInfo->mSubActor = new al::ModelDrawParts("無敵モデル", mRootActor, initInfo, actorCategoryName);
                    }else {
                        if(!al::isEqualString(actorClassName, "SimpleCircleShadowXZ")) {
                            if(al::isEqualString(actorClassName, "CollisionObj")) {
                                const char* collSuffixName = al::tryGetByamlKeyStringOrNULL(subActorIter, "CollisionSuffixName");
                                const char* collName = al::tryGetByamlKeyStringOrNULL(subActorIter, "CollisionName");
                                const char* sensorName = al::tryGetByamlKeyStringOrNULL(subActorIter, "SensorName");
                                const char* fileSuffixName = al::tryGetByamlKeyStringOrNULL(subActorIter, "InitFileSuffixName");
                                const char* jointName = al::tryGetByamlKeyStringOrNULL(subActorIter, "JointName");

                                if(!collName)
                                    collName = collSuffixName;

                                const char *newSensorName = sensorName ? sensorName : collSuffixName;

                                if(fileSuffixName)
                                    collSuffixName = fileSuffixName;

                                auto *sensor = al::getHitSensor(mRootActor, newSensorName);

                                actorInfo->mSubActor = al::createCollisionObj(mRootActor, initInfo, collName, sensor, jointName, collSuffixName);
                                if(actorObjectName)
                                    actorInfo->mSubActor->setName(actorObjectName);

                                continue;
                            }

                            actorInfo->mSubActor = new al::LiveActor(actorObjectName);
                            al::initActorWithArchiveName(actorInfo->mSubActor, initInfo, actorModelName, actorSuffix);

                            continue;
                        }

                        al::SimpleCircleShadowXZ *dropShadow = new al::SimpleCircleShadowXZ(actorObjectName);
                        dropShadow->initSimpleCircleShadow(mRootActor, initInfo, actorModelName, actorSuffix);
                        actorInfo->mSubActor = dropShadow;
                    }
                }
            }else {
                actorInfo->mSubActor = new al::LiveActor(actorObjectName);

                if(isUseHostPlacementInfo) {
                    al::initActorWithArchiveName(actorInfo->mSubActor, initInfo, actorModelName, actorSuffix);
                }else {
                    al::initChildActorWithArchiveNameNoPlacementInfo(actorInfo->mSubActor, initInfo, actorModelName, actorSuffix);
                }
            }

            al::initActorModelForceCubeMap(actorInfo->mSubActor, initInfo);

            if(isAliveResult) {
                if(isAlive)
                    actorInfo->mSubActor->makeActorAlive();
                else
                    actorInfo->mSubActor->makeActorDead();
            }
            bool isShow = false;
            if(al::tryGetByamlBool(&isShow, subActorIter, "IsShow") && !isShow)
                al::hideModel(actorInfo->mSubActor);

        }
    }

}

SubActorKeeper* SubActorKeeper::create(al::LiveActor* rootActor) {
    return new SubActorKeeper(rootActor);
}
SubActorKeeper* SubActorKeeper::tryCreate(al::LiveActor* rootActor,char const* suffix, int maxSubActors) {

    sead::FixedSafeString<0x80> actorInitFileName;

    if(!al::isExistModelResource(rootActor) || !al::tryGetActorInitFileName(&actorInitFileName, rootActor, "InitSubActor", suffix))
        return nullptr;

    if(maxSubActors <= 0) {
        if(!al::isExistModelResource(rootActor))
            return nullptr;

        if(!al::isExistModelResourceYaml(rootActor, actorInitFileName.cstr(), nullptr))
            return nullptr;
    }

    return new SubActorKeeper(rootActor);
}
}