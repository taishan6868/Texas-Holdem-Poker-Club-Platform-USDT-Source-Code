#pragma once

#include "Comm/ITableGame.h"
#include "common/macros.h"
#include "common/nndef.h"

using namespace nndef;

namespace game
{
    namespace config
    {

        struct ProlongCfg{
            int iProlongTime;
            int iProlongPrice;
        };

        class GameConfig
        {
            DISALLOW_COPY_AND_ASSIGN(GameConfig)

        private:
            //
            explicit GameConfig();

        public:
            //
            static GameConfig *Create()
            {
                return new GameConfig();
            }

            ~GameConfig();

        public:
            //
            bool loadConfigFile(const char *strPath);
            //
            void printConfigFile(GameInitParam const &sInitParams);
            //
            void loadRoomConfig(const void *p);
            //
            void printRoomConfig();

        public:
            //
            int getNNRatioByNNType(E_NN_TYPE nntype);
            //
            int getSysProfitLimit(int iPlayerNum);
            int getInsureOdds(int num)
            {
                if (bShortDeck)
                {
                    auto it = mInsureShortOdds.find(num);
                    if(it != mInsureShortOdds.end())
                    {
                        return it->second;
                    }
                }
                else
                {
                    auto it = mInsureCommOdds.find(num);
                    if(it != mInsureCommOdds.end())
                    {
                        return it->second;
                    }
                }
                return 0;
            }
            //
            inline int getReadyTime()
            {
                return nReadyTime;
            }
            //
            inline int getCardTime()
            {
                return nCardTime ;
            }
            //
            inline int getBetTime()
            {
                return nBetTime;
            }
            //
            inline void setBetTime(long value)
            {
                nBetTime = value;
            }
            inline void setInitBetTime(int room_type, int index)
            {
                auto it = mBetTime.find(room_type);
                if(it != mBetTime.end())
                {
                    if(it->second.size() > (unsigned int)index && index >= 0)
                    {
                        nBetTime = it->second[index];
                    }
                }
            }
            inline void setRobotAllAction(const RoomSo::RobotAllAction *pRobotAction)
            {
                if(pRobotAction != nullptr)
                {
                    robotAction = *pRobotAction;
                }
            }
            inline RoomSo::RobotAllAction &getRobotAllAction()
            {
                return robotAction;
            }
            //
            inline int getOpenTime()
            {
                return nOpenTime;
            }
            //
            inline int getDelayTime()
            {
                return nDelayTime;
            }
            inline int getInsureTime()
            {
                return nInsureTime;
            }
            inline int getPaomaAiRatio()
            {
                return nPaomaAiRatio;
            }

            inline string getHoldSeatGameList()
            {
                return nHoldSeatGameList;
            }
            inline int getHoldSeatTime()
            {
                return nHoldSeatTime;
            }

            //
            inline int getRoomTimeout()
            {
                return nRoomTimeout;
            }
            //
            inline int getEndTime()
            {
                return nEndTime;
            }
            //
            inline int getMaxBet()
            {
                return nMaxBet;
            }
            //
            inline int getMaxBanker()
            {
                return nMaxBanker;
            }
            //
            inline int getNotReady()
            {
                return nNotReady;
            }
            //
            inline RoomSo::E_Type getRoomType()
            {
                return eRoomType;
            }
            //
            inline int getPayType()
            {
                return iPayType;
            }
            //
            inline int getMinBegin()
            {
                return nMinBegin;
            }
            //
            inline int getAiModel()
            {
                return nAiModel;
            }
            //
            inline int getMaxRound()
            {
                return nMaxRound;
            }
            //
            inline int getBaseRatio()
            {
                return nBaseRatio;
            }
            //
            inline int getWinMaxRatio()
            {
                return nWinMaxRatio;
            }
            //
            inline int getBankerPoint()
            {
                return nBankerPoint;
            }
            //
            inline int getMinSit()
            {
                return nMinSit;
            }
            //
            inline int getMinLeft()
            {
                return nMinLeft;
            }
            //
            inline int getFrontBet()
            {
                return iFrontBet;
            }
            //
            inline void setFrontBet(int frontBet)
            {
                iFrontBet = frontBet;
            }
            //
            inline int getBlindNum()
            {
                return iBlindNum <= 0 ? 1 : iBlindNum;
            }
            //
            inline void setBlindNum(int blindNum)
            {
                iBlindNum = blindNum;
            }
            inline int getBlindLevel()
            {
                return iBlindLevel;
            }
            inline void setBlindLevel(int level)
            {
                iBlindLevel = level;
            }
            inline void setRaiseTime(int value)
            {
                iRaiseTime = value;
            }
            inline int getRaiseTime()
            {
                return iRaiseTime;
            }
            //
            inline int getMinTake()
            {
                return iMinTake;
            }
            //
            inline int getInitPoint()
            {
                return iInitPoint;
            }
            //
            inline int getBasePoint()
            {
                return iBasePoint;
            }
            //
            inline int getNNRatioType()
            {
                return iNNRatioType;
            }
            //
            std::map<int, int> &getMapNNType()
            {
                return mNNType;
            }
            //
            std::vector<E_NN_TYPE> &getVecNNType()
            {
                return vNNType;
            }
            //
            std::map<int, int> &getGuessTypeMap()
            {
                return mGuessType;
            }
            //
            std::vector<int> &getVecGuessNum()
            {
                return vGuessNum;
            }
            //
            inline bool isEnterLimit()
            {
                return bEnterLimit;
            }
            //
            inline bool isRubCard()
            {
                return bRubCard;
            }
            //
            inline bool isAddBetLimit()
            {
                return bAddBetLimit;
            }
            //
            inline bool isRaiseBet()
            {
                return bRaiseBet;
            }
            //
            inline bool isKing()
            {
                return bKing;
            }
            //
            inline bool isAddBet()
            {
                return bAddBet;
            }
            //
            inline bool isDoubleBet()
            {
                return bDoubleBet;
            }
            //
            std::vector<int> &getVecUserID()
            {
                return vUserID;
            }
            //
            inline int getRewardInit()
            {
                return nRewardInit;
            }
            //
            inline int getSysInit()
            {
                return nSysInit;
            }
            //
            inline float getSysProfit()
            {
                return nSysProfit;
            }
            inline void setSysProfit(float value)
            {
                nSysProfit = value;
            }
            //
            inline float getSysRobotProfit()
            {
                return nSysRobotProfit;
            }
            //
            inline long getServiceFee()
            {
                return iServiceFee;
            }
            inline int getnVersion()
            {
                return nVersion;
            }
            inline void setnVersion(int value)
            {
                nVersion = value;
            }
            inline int getAiGameRoundLimit()
            {
                return iAiGameRoundLimit;
            }

            inline int getMaxSeatByRoom()
            {
                return iMaxSeat;
            }

            inline void setMaxSeatByRoom(int value)
            {
                iMaxSeat = value;
            }

            /**new 先强行改成0达到用C币的效果**/
            inline int getDalao()
            {
                return 0;//iDalao;
            }

            inline void setDalao(int value)
            {
                iDalao = value;
            }

            inline long getCreatedId()
            {
                return lCreatedId;
            }

            inline void setCreatedId(long value)
            {
                lCreatedId = value;
            }

            inline long getClubId()
            {
                return lClubId;
            }

            inline void setClubId(long value)
            {
                lClubId = value;
            }

            inline string getClubName()
            {
                return sClubName;
            }

            inline void setClubName(string value)
            {
                sClubName = value;
            }

            inline string getRoomKey()
            {
                return sRoomKey;
            }

            inline void setRoomKey(string value)
            {
                sRoomKey = value;
            }

            inline string getRoomName()
            {
                return sRoomName;
            }

            inline void setRoomName(string value)
            {
                sRoomName = value;
            }

            inline int getSmallBlind()
            {
                return iSmallBlind;
            }

            inline void setSmallBlind(int value)
            {
                iSmallBlind = value;
            }

            inline int getBigBlind()
            {
                return iBigBlind;
            }

            inline void setBigBlind(int value)
            {
                iBigBlind = value;
            }

            inline int getAnte()
            {
                return iAnte;
            }

            inline void setAnte(int value)
            {
                iAnte = value;
            }

            inline int getOptionTime()
            {
                return iOptionTime;
            }

            inline void setOptionTime(int value)
            {
                iOptionTime = value;
            }

            inline bool getIsStart()
            {
                return bIsStart;
            }
            inline void setIsStart(bool value)
            {
                bIsStart = value;
            }

            inline void setPause(bool value)
            {
                bPause = value;
            }
            inline bool isPause()
            {
                return bPause;
            }

            inline int getAutoStartNum()
            {
                return iAutoStartNum;
            }

            inline void setAutoStartNum(int value)
            {
                iAutoStartNum = value;
            }

            inline int getMaxGold()
            {
                return iMaxGold;
            }

            inline void setMaxGold(int value)
            {
                iMaxGold = value;
            }

            inline int getMinGold()
            {
                return iMinGold;
            }

            inline void setMinGold(int value)
            {
                iMinGold = value;
            }

            inline int getMaxSeatNum()
            {
                return iMaxSeatNum;
            }

            inline void setMaxSeatNum(int value)
            {
                iMaxSeatNum = value;
            }

            inline bool getInsure()
            {
                return bInsure;
            }

            inline void setInsure(bool value)
            {
                bInsure = value;
            }

            inline int getGameType()
            {
                return iGameType;
            }

            inline void setGameType(int value)
            {
                iGameType = value;
            }

            inline int getProfit()
            {
                return iProfit;
            }

            inline void setProfit(int value)
            {
                iProFit = value;
            }

            inline int getGameTime()
            {
                return iGameTime;
            }

            inline void setGameTime(int value)
            {
                iGameTime = value;
            }
            
            inline void setCreateTime(long value)
            {
                lCreateTime = value;
            }

            inline long getCreateTime()
            {
                return lCreateTime;
            }

            inline void setBlind(bool value)
            {
                bBlind = value;
            }

            inline bool getBlind()
            {
                return bBlind;
            }

            inline void setContral(bool value)
            {
                bContral = value;
            }

            inline bool getContral()
            {
                return bContral;
            }

            inline void setBetLimit(bool value)
            {
                bBetLimit = value;
            }

            inline bool isBetLimit()
            {
                return bBetLimit;
            }

            inline void setHdCount(int value)
            {
                iHdCount = value;
            }

            inline int getHdCount()
            {
                return iHdCount;
            }

            inline void setOpenSneak(bool value)
            {
                bOpenSneak = value;
            }

            inline bool isOpenSneak()
            {
                return bOpenSneak;
            }

            inline void setMimi(bool value)
            {
                bMimi = value;
            }

            inline bool isMimi()
            {
                return bMimi;
            }

            inline void setShow(bool value)
            {
                bShow = value;
            }

            inline bool isShow()
            {
                return bShow;
            }

            inline void setMinBuyIn(int value)
            {
                iMinBuyIn = value;
            }

            inline int getMinBuyIn()
            {
                return iMinBuyIn;
            }

            inline void setFeeLimit(int value)
            {
                iFeeLimit = value;
            }

            inline int getFeeLimit()
            {
                return iFeeLimit;
            }

            inline void setFeeDynamic(bool value)
            {
                bFeeDynamic = value;
            }

            inline bool isFeeDynamic()
            {
                return bFeeDynamic;
            }

            inline bool getShortDeck()
            {
                return bShortDeck;
            }
            inline void setShortDeck(bool value)
            {
                bShortDeck = value;
            }
            

            long timestamp()
            {
                /* struct timeval tv;
                 gettimeofday(&tv, NULL);
                 return (tv.tv_sec*1000 + tv.tv_usec/1000); */
                return 0;
            }

            int getBasePauseTime()
            {
                return iBasePauseTime;
            }

            int getExtraPauseTimeByID(int id)
            {
                auto it = mmExtraPauseTime.find(id);
                if(it == mmExtraPauseTime.end())
                {
                    return 0;
                }
                return it->second;
            }

            // 获取延迟时间,价格
            ProlongCfg getExtraProlongPrice(int id);

            inline void setTempFlag(int value)
            {
                if (value == 0)
                {
                    iTempFlag = 0;
                }
                else
                {
                    iTempFlag |= value;
                }
            }

            inline int getTempFlag()
            {
                return iTempFlag;
            }

            inline void setTempBlind(bool value)
            {
                bTempBlind = value;
            }

            inline bool getTempBlind()
            {
                return bTempBlind;
            }

            inline void setTempMaxGold(int value)
            {
                iTempMaxGold = value;
            }

            inline int getTempMaxGold()
            {
                return iTempMaxGold;
            }

            inline void setTempActTime(int value)
            {
                iTempActTime = value;
            }

            inline int getTempActTime()
            {
                return iTempActTime;
            }

            inline void setTempContral(bool value)
            {
                bTempContral = value;
            }

            inline bool getTempContral()
            {
                return bTempContral;
            }

            inline void setTempMimi(bool value)
            {
                bTempMimi = value;
            }

            inline bool getTempMimi()
            {
                return bTempMimi;
            }

            inline void setTempMinGold(int value)
            {
                iTempMinGold = value;
            }

            inline int getTempMinGold()
            {
                return iTempMinGold;
            }

        protected:
            //
            int nReadyTime;
            //
            int nCardTime;
            //
            int nBetTime;
            //
            std::map<int, std::vector<int>> mBetTime;
            //
            int nOpenTime;
            //
            int nDelayTime;
            //
            int nEndTime;
            //
            int nRoomTimeout;
            //
            int nNotReady;
            //
            int nMaxBet;
            //
            int nMaxBanker;
            //
            std::map<int, int> mDefaultNNType;
            //
            std::map<int, int> mNNType;
            //
            std::map<int, int> mGuessType;
            //
            std::vector<int> vGuessNum;
            //
            std::vector<E_NN_TYPE> vNNType;
            //
            std::vector<int> vUserID;
            //
            int nRewardInit;
            //
            int nSysInit;
            //
            float nSysProfit;
            //
            map<int, std::vector<long>>mProfitLimit;
            //
            float nSysRobotProfit;

            int nVersion;

            // 是否开启保险
            bool bInsure;
            // 保险思考时间
            int nInsureTime;
            // 保险赔率表
            map<int, int> mInsureCommOdds;
            map<int, int> mInsureShortOdds;
            map<int,ProlongCfg> mProlongConfigs;

            int iBasePauseTime;
            map<int, int> mmExtraPauseTime;

            // 跑马机器人跑马或同意的概率
            int nPaomaAiRatio;

            //占座玩法列表
            string nHoldSeatGameList;
            //占座时长
            int nHoldSeatTime;
        protected:
            //最大回合数
            int nMaxRound;
            //
            int nMinBegin;
            //
            int nAiModel;
            //
            int nBaseRatio;
            //
            int nWinMaxRatio;
            //
            int nBankerPoint;
            //
            int nMinSit;
            //
            int nMinLeft;
            //
            int iInitPoint;
            //
            int iBasePoint;
            //
            int iProfit;
            //开局禁入
            bool bEnterLimit;
            //极速玩法
            bool bRubCard;
            //推注限制
            bool bAddBetLimit;
            //闲家买码
            bool bRaiseBet;
            //王癞玩法
            bool bKing;
            //显示推注
            bool bAddBet;
            //加倍玩法
            bool bDoubleBet;
            //服务费用
            long iServiceFee;
            //
            int iNNRatioType;
            //玩法类型
            int iPayType;
            //
            int iFrontBet;
            //盲目数量
            int iBlindNum;
             //盲目等级
            int iBlindLevel;
            //最小携带
            int iMinTake;
            //
            RoomSo::E_Type eRoomType;
            //
            RoomSo::RobotAllAction robotAction;

            int iAiGameRoundLimit;

            long lMaxGold;//最大带入
            int iMaxSeat; //最大座位数

            bool bShortDeck;
        protected:
            int iDalao;             // 币种(0 C币 1 B币)
            long lCreatedId;        // 创建者ID
            long lClubId;           // 俱乐部ID
            string sClubName;       // 俱乐部名称
            string sRoomKey;        // 房间号
            string sRoomName;       // 房间名称
            int iSmallBlind;        // 小肓
            int iBigBlind;          // 大肓
            int iAnte;              // 前注
            int iOptionTime;        // 操作时间
            bool bIsStart;          // 是否已开启过
            bool bPause;            // 是否暂停
            int iAutoStartNum;      // 自动开启人数
            int iMaxGold;           // 最大带入
            int iMinGold;           // 最小带入
            int iMaxSeatNum;        // 最大座位数
            int iInsure;            // 是否开启保险
            int iGameType;          // 游戏类型(1 德州 2 AOF 3 短牌)
            int iProFit;            // 抽水比率
            int iGameTime;          // 牌局时长/开赛时间
            int iRaiseTime;         // 升盲注时间
            long lCreateTime;       // 创房间时间
            bool bBlind;            // 强制肓注
            bool bContral;          // 控制玩家带入
            bool bBetLimit;         // 是否有限下注
            int iHdCount;           // 手牌数
            bool bOpenSneak;        // 开启偷偷看
            bool bMimi;             // 咪咪牌
            bool bShow;             // 亮手牌
            int iMinBuyIn;          // 最小玩牌限制
            int iFeeLimit;          // 抽水上限（BB）
            bool bFeeDynamic;       // 抽水动态变化
        protected:
            // 临时房主设置
            int iTempFlag;          // 标识 1111   8421
            bool bTempBlind;        // 强制肓注
            int iTempMaxGold;       // 最大带入
            int iTempActTime;       // 操作时间
            bool bTempContral;      // 控制玩家带入
            bool bTempMimi;         // 咪咪牌
            int iTempMinGold;       // 最小带入
        };
    };
};

