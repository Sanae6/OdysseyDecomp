
#include "Library/LiveActor/LiveActor.h"
#include "Library/Sequence/Sequence.h"
#include "System/GameDataHolderAccessor.h"

class FriendsProfileDataSetter;
class FriendsProfileDataHolder;
class FriendsProfileDownloader;
class TimeBalloonPlayBalloonDataHolder;
class TimeBalloonDataAccessor;
class TimeBalloonDataAccessorExecutor;
class TimeBalloonNoticeSequenceData;
class BalloonFindDataAccessor;
class BalloonFindMyBalloonServerHolder;
class BalloonFindSearchBalloonServerHolder;

class TimeBalloon {
public:
    class BalloonData {};

    class BalloonDataServer {};
};

class TimeBalloonSequenceInfo {
public:
    TimeBalloonSequenceInfo(GameDataHolderAccessor, const al::SequenceInitInfo&);
    ~TimeBalloonSequenceInfo();
    void init();
    void copy(const TimeBalloonSequenceInfo*);
    void setAccessor(al::LiveActor*);
    void addHioNode();
    void updateBalloonFind();
    void resetSequenceId();
    bool isUseCoinSequence() const;
    void getFindBalloon();
    void getFindBalloonOwnerAchievementDataId();
    void setFindBalloonDataId(u64);
    void isEnableKidsMode() const;
    void isSetSequenceInfo() const;
    void isGetSequenceInfo() const;
    void createProfileDataSetter(FriendsProfileDataSetter**, const al::IUseSceneObjHolder*);
    void isSayBreakAlerady(al::LiveActor*) const;
    void onSayBreak(al::LiveActor*);
    void offSayBreak(al::LiveActor*);
    void getMyBalloonHolder() const;
    void getMyBalloon() const;
    void getSearchBalloonHolder() const;
    void getSearchBalloon() const;
    void getMyAchievementHolder() const;
    void onTutorial();
    void offTutorial();
    void isTutorial() const;
    void onKidsModeReturnNormalPlay();
    void getFindBalloonCoinNum() const;
    void getFindBalloonRetryCoinNum() const;
    void getSuccessNum() const;
    void getEventEntryId() const;
    void setEventEntryId(unsigned int);

private:
    int _8;
    int _C;
    int _10;
    int _14;
    TimeBalloon::BalloonData* mBalloonData;
    void* _20;
    void* _28;
    void* _30;
    int _34;
    FriendsProfileDataHolder* mFriendsProfileDataHolder;
    FriendsProfileDownloader* mFriendsProfileDownloader;
    TimeBalloonPlayBalloonDataHolder* mPlayBalloonDataHolder;
    TimeBalloonDataAccessor* mDataAccessor;
    TimeBalloonDataAccessorExecutor* mDataAccessorExecutor;
    TimeBalloonNoticeSequenceData* mNoticeSequenceData;
    int _70;
    void* _78;
    BalloonFindDataAccessor* mFindDataAccessor;
    BalloonFindMyBalloonServerHolder* mFindMyBalloonServerHolder;
    BalloonFindSearchBalloonServerHolder* mFindSearchBalloonServerHolder;
};
