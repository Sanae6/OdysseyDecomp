#include <al/Library/Base/String.h>
#include <al/Library/Execute/ExecuteDirector.h>
#include <al/Library/Execute/ExecuteTablesImpl.h>
#include <al/Library/Layout/LayoutActor.h>
#include <al/Library/LiveActor/LiveActor.h>

namespace al {

ExecuteDirector::ExecuteDirector(int count) {
    mRequestCount = count;
}

ExecuteDirector::~ExecuteDirector() {}

void ExecuteDirector::init(al::ExecuteSystemInitInfo const&initInfo) {
    mUpdateTableCount = UpdateTableSize;
    mUpdateTables = new ExecuteTableHolderUpdate*[UpdateTableSize];

    for (int i = 0; i < mUpdateTableCount; ++i) {
        mUpdateTables[i] = new ExecuteTableHolderUpdate();
        mUpdateTables[i]->init(UpdateTable[i].mName, initInfo, UpdateTable[i].mExecuteOrders, UpdateTable[i].mExecuteOrderCount);
    }

    mDrawTableCount = DrawTableSize;
    mDrawTables = new ExecuteTableHolderDraw*[DrawTableSize];

    for (int i = 0; i < mDrawTableCount; ++i) {
        mDrawTables[i] = new ExecuteTableHolderDraw();
        mDrawTables[i]->init(DrawTable[i].mName, initInfo, DrawTable[i].mExecuteOrders, DrawTable[i].mExecuteOrderCount);
    }

    mRequestKeeper = new ExecuteRequestKeeper(mRequestCount);
}

void ExecuteDirector::registerActorUpdate(al::LiveActor* actor, char const* tableName) {
    if(!actor->getExecuteInfo()) {
        actor->initExecuteInfo(new ActorExecuteInfo(mRequestKeeper));
    }
    if(mUpdateTableCount >= 1) {
        for (int i = 0; i < mUpdateTableCount; ++i) {
            mUpdateTables[i]->tryRegisterActor(actor, tableName);
        }
    }
}

void ExecuteDirector::registerActorDraw(al::LiveActor* actor, char const* tableName) {
    if(!actor->getExecuteInfo()) {
        actor->initExecuteInfo(new ActorExecuteInfo(mRequestKeeper));
    }
    if(mDrawTableCount >= 1) {
        for (int i = 0; i < mDrawTableCount; ++i) {
            mDrawTables[i]->tryRegisterActor(actor, tableName);
            mDrawTables[i]->tryRegisterActorModel(actor, tableName);
        }
    }
}

void ExecuteDirector::registerActorModelDraw(al::LiveActor* actor, char const* tableName) {
    if(!actor->getExecuteInfo()) {
        actor->initExecuteInfo(new ActorExecuteInfo(mRequestKeeper));
    }
    if(mDrawTableCount >= 1) {
        for (int i = 0; i < mDrawTableCount; ++i) {
            mDrawTables[i]->tryRegisterActorModel(actor, tableName);
        }
    }
}

void ExecuteDirector::registerLayoutUpdate(al::LayoutActor* lyt, char const* tableName) {
    if(!lyt->getExecuteInfo()) {
        lyt->initExecuteInfo(new LayoutExecuteInfo());
    }
    if(mUpdateTableCount >= 1) {
        for (int i = 0; i < mUpdateTableCount; ++i) {
            mUpdateTables[i]->tryRegisterLayout(lyt, tableName);
        }
    }
}

void ExecuteDirector::registerLayoutDraw(al::LayoutActor* lyt, char const* tableName) {
    if(!lyt->getExecuteInfo()) {
        lyt->initExecuteInfo(new LayoutExecuteInfo());
    }
    if(mDrawTableCount >= 1) {
        for (int i = 0; i < mDrawTableCount; ++i) {
            mDrawTables[i]->tryRegisterLayout(lyt, tableName);
        }
    }
}

void ExecuteDirector::registerUser(al::IUseExecutor* user, char const* tableName) {
    if(mUpdateTableCount >= 1) {
        for (int i = 0; i < mUpdateTableCount; ++i) {
            mUpdateTables[i]->tryRegisterUser(user, tableName);
        }
    }
    if(mDrawTableCount >= 1) {
        for (int i = 0; i < mDrawTableCount; ++i) {
            mDrawTables[i]->tryRegisterUser(user, tableName);
        }
    }
}

void ExecuteDirector::registerFunctor(al::FunctorBase const& functor, char const* tableName) {
    if(mUpdateTableCount >= 1) {
        for (int i = 0; i < mUpdateTableCount; ++i) {
            mUpdateTables[i]->tryRegisterFunctor(functor, tableName);
        }
    }
    if(mDrawTableCount >= 1) {
        for (int i = 0; i < mDrawTableCount; ++i) {
            mDrawTables[i]->tryRegisterFunctor(functor, tableName);
        }
    }
}

void ExecuteDirector::registerFunctorDraw(al::FunctorBase const& functor, char const* tableName) {
    if(mDrawTableCount >= 1) {
        for (int i = 0; i < mDrawTableCount; ++i) {
            mDrawTables[i]->tryRegisterFunctor(functor, tableName);
        }
    }
}

void ExecuteDirector::createExecutorListTable() {
    if(mUpdateTableCount >= 1) {
        for (int i = 0; i < mUpdateTableCount; ++i) {
            mUpdateTables[i]->createExecutorListTable();
        }
    }
    if(mDrawTableCount >= 1) {
        for (int i = 0; i < mDrawTableCount; ++i) {
            mDrawTables[i]->createExecutorListTable();
        }
    }
}

void ExecuteDirector::execute(char const* tableName) const {

    if(!tableName) {
        mRequestKeeper->executeRequestActorMovementAllOn();
        mRequestKeeper->executeRequestActorDrawAllOn();

        mUpdateTables[0]->execute();

        mRequestKeeper->executeRequestActorMovementAllOff();
        mRequestKeeper->executeRequestActorDrawAllOff();
    }else {

        bool isActorEnabled;

        if(al::isEqualString(tableName, mUpdateTables[0]->getName())) {
            mRequestKeeper->executeRequestActorMovementAllOn();
            mRequestKeeper->executeRequestActorDrawAllOn();
            isActorEnabled = true;
        }else {
            isActorEnabled = false;
        }

        if(mUpdateTableCount >= 1) {
            for (int i = 0; i < mUpdateTableCount; ++i) {
                if(al::isEqualString(tableName, mUpdateTables[i]->getName())) {

                    mUpdateTables[i]->execute();

                    if(isActorEnabled) {
                        mRequestKeeper->executeRequestActorMovementAllOff();
                        mRequestKeeper->executeRequestActorDrawAllOff();
                    }

                    return;
                }
            }
        }

        if(isActorEnabled) {
            mRequestKeeper->executeRequestActorMovementAllOff();
            mRequestKeeper->executeRequestActorDrawAllOff();
        }
    }
}

void ExecuteDirector::executeList(char const* tableName, char const* listName) const {
    mRequestKeeper->executeRequestActorMovementAllOn();
    mRequestKeeper->executeRequestActorDrawAllOn();
    if(tableName) {
        if(mUpdateTableCount >= 1) {
            for (int i = 0; i < mUpdateTableCount; ++i) {
                if(al::isEqualString(tableName, mUpdateTables[i]->getName())) {
                    mUpdateTables[i]->executeList(listName);
                }
            }
        }
    }else {
        mUpdateTables[0]->executeList(listName);
    }
    mRequestKeeper->executeRequestActorMovementAllOff();
    mRequestKeeper->executeRequestActorDrawAllOff();
}

void ExecuteDirector::draw(char const* tableName) const {
    if(tableName) {
        if(mDrawTableCount >= 1) {
            for (int i = 0; i < mDrawTableCount; ++i) {
                if(al::isEqualString(tableName, mDrawTables[i]->getName())) {
                    mDrawTables[i]->execute();
                    return;
                }
            }
        }
    }else {
        mDrawTables[0]->execute();
    }
}

void ExecuteDirector::drawList(char const* tableName, char const* listName) const {
    if(mDrawTableCount >= 1) {
        for (int i = 0; i < mDrawTableCount; ++i) {
            if(al::isEqualString(tableName, mDrawTables[i]->getName())) {
                mDrawTables[i]->executeList(listName);
                return;
            }
        }
    }
}

bool ExecuteDirector::isActiveDraw(char const* tableName) const {
    if(tableName) {
        if(mDrawTableCount >= 1) {
            for (int i = 0; i < mDrawTableCount; ++i) {
                if(al::isEqualString(tableName, mDrawTables[i]->getName())) {
                    return mDrawTables[i]->isActive();
                }
            }
        }
    }else {
        return mDrawTables[0]->isActive();
    }
}

}