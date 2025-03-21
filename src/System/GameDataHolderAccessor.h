#pragma once

namespace al {
class IUseSceneObjHolder;
class SceneObjHolder;
}  // namespace al
class SaveObjInfo;
class GameDataHolder;

class GameDataHolderAccessor {
public:
    GameDataHolderAccessor(const al::IUseSceneObjHolder*);
    GameDataHolderAccessor(const al::SceneObjHolder*);
    GameDataHolderAccessor(GameDataHolder* data) : mData(data) {}

    operator GameDataHolder*() const { return mData; }

    GameDataHolder* operator->() const { return mData; }

private:
    GameDataHolder* mData;
};
