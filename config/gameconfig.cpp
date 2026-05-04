#include <unistd.h>
#include "config/gameconfig.h"
#include "utils/tarslog.h"
#include "util/tc_config.h"
#include "util/tc_md5.h"
#include "log/logcomm.h"

namespace game
{
    namespace config
    {
        
        GameConfig::GameConfig()
        {
        }

        GameConfig::~GameConfig()
        {

        }

        ProlongCfg GameConfig::getExtraProlongPrice(int id)
        {
            auto it = mProlongConfigs.find(id);
            if (it != mProlongConfigs.end())
            {
                return it->second;
            }
            ProlongCfg cfg;
            cfg.iProlongTime = 0;
            cfg.iProlongPrice = 0;
            return cfg;
        }

        bool GameConfig::loadConfigFile(const char *strPath)
        {
            DLOG_TRACE("GameConfig loadConfigFile: " << strPath << endl);
            if (access(strPath, F_OK) == -1)
            {
                return false;
            }

            tars::TC_Config tc_config;
            tc_config.parseFile(strPath);

            bOpenSneak = false;

            const vector<string> &vecDomain = tc_config.getDomainVector("/Main/Game");
            for (auto it = vecDomain.begin(); it != vecDomain.end(); ++it)
            {
                if (*it == "GameConfig")
                {
                    string strOperateNode = "/Main/Game/" + *it;

                    nBetTime = 8;
                    nReadyTime = TC_Common::strto<tars::Int32>(tc_config.get(strOperateNode + "<nReadyTime>", "5"));
                    nCardTime = TC_Common::strto<tars::Int32>(tc_config.get(strOperateNode + "<nCardTime>", "5"));
                    //nBetTime = TC_Common::strto<tars::Int32>(tc_config.get(strOperateNode + "<nBetTime>", "5"));
                    std::vector<std::string> &&vecBetTime = tars::TC_Common::sepstr<std::string>(tc_config.get(strOperateNode + "<nBetTime>", ""), ",");
                    for (auto it = vecBetTime.begin(); it != vecBetTime.end(); ++it)
                    {
                        std::vector<int> &&vecBetTimeType = tars::TC_Common::sepstr<int>(*it, ":");
                        auto itt = mBetTime.find(vecBetTimeType[0]);
                        if (itt == mBetTime.end())
                            mBetTime[vecBetTimeType[0]] = std::vector<int>({vecBetTimeType[1]});
                        else
                            itt->second.push_back(vecBetTimeType[1]);
                    }
                    

                    nOpenTime = TC_Common::strto<tars::Int32>(tc_config.get(strOperateNode + "<nOpenTime>", "10"));
                    nDelayTime = TC_Common::strto<tars::Int32>(tc_config.get(strOperateNode + "<nDelayTime>", "1"));
                    nRoomTimeout = TC_Common::strto<tars::Int32>(tc_config.get(strOperateNode + "<nRoomTimeout>", "180"));
                    nEndTime = TC_Common::strto<tars::Int32>(tc_config.get(strOperateNode + "<nEndTime>", "5"));
                    nMaxBet = TC_Common::strto<tars::Int32>(tc_config.get(strOperateNode + "<nMaxBet>", "2"));
                    nMaxBanker = TC_Common::strto<tars::Int32>(tc_config.get(strOperateNode + "<nMaxBanker>", "4"));
                    nMaxRound = TC_Common::strto<tars::Int32>(tc_config.get(strOperateNode + "<nMaxRound>", "4"));
                    nMinBegin = TC_Common::strto<tars::Int32>(tc_config.get(strOperateNode + "<nMinBegin>", "2"));
                    nAiModel = TC_Common::strto<tars::Int32>(tc_config.get(strOperateNode + "<nAiModel>", "0"));
                    nWinMaxRatio = TC_Common::strto<tars::Int32>(tc_config.get(strOperateNode + "<nWinMaxRatio>", "0"));
                    nBankerPoint = TC_Common::strto<tars::Int32>(tc_config.get(strOperateNode + "<nBankerPoint>", "0"));
                    nNotReady = TC_Common::strto<tars::Int32>(tc_config.get(strOperateNode + "<nNotReady>", "3"));
                    nRewardInit = TC_Common::strto<tars::Int32>(tc_config.get(strOperateNode + "<nRewardInit>", "0"));
                    // nSysInit = TC_Common::strto<tars::Int32>(tc_config.get(strOperateNode + "<nSysInit>", "0"));
                    nSysProfit = TC_Common::strto<tars::Float>(tc_config.get(strOperateNode + "<nSysProfit>", "5"));

                    mProfitLimit.clear();
                    std::vector<std::string> &&vecProfitLimit = tars::TC_Common::sepstr<std::string>(tc_config.get(strOperateNode + "<nSysProfitLimit>", ""), ",");
                    for (unsigned int i = 1; i <= vecProfitLimit.size(); i++)
                    {
                        std::vector<long> &&vecProfitLimitItem = tars::TC_Common::sepstr<long>(vecProfitLimit[i-1], ":");
                        if(vecProfitLimitItem.size() != 2)
                        {
                            continue;
                        }
                        vecProfitLimitItem[0] *= 1000;
                        vecProfitLimitItem[1] *= 1000;
                        mProfitLimit.insert(std::make_pair(i, vecProfitLimitItem));
                    }

                    nSysRobotProfit = TC_Common::strto<tars::Float>(tc_config.get(strOperateNode + "<nSysRobotProfit>", "5"));
                    vGuessNum = tars::TC_Common::sepstr<int>(tc_config.get(strOperateNode + "<sGuessNum>", ""), ",");
                    std::vector<int> &&vecUserID = tars::TC_Common::sepstr<int>(tc_config.get(strOperateNode + "<sUserID>", ""), ",");
                    vUserID.insert(vUserID.begin(), vecUserID.begin(), vecUserID.end());

                    std::vector<std::string> &&vecGuessMap = tars::TC_Common::sepstr<std::string>(tc_config.get(strOperateNode + "<sGuessMap>", ""), ",");
                    for (auto it = vecGuessMap.begin(); it != vecGuessMap.end(); ++it)
                    {
                        std::vector<int> &&vecGuessType = tars::TC_Common::sepstr<int>(*it, ":");
                        mGuessType.insert(std::make_pair(vecGuessType.at(0), vecGuessType.at(1)));
                    }

                    nVersion = TC_Common::strto<tars::Int32>(tc_config.get(strOperateNode + "<nVersion>", "0"));
                    if (nVersion == 0)
                        g_isDebug = true;
                    else
                        g_isDebug = false;

                    nInsureTime = TC_Common::strto<tars::Int32>(tc_config.get(strOperateNode + "<nInsureTime>", "30"));
                    DLOG_TRACE("nInsureTime");
                    mInsureCommOdds.clear();
                    DLOG_TRACE(tc_config.get(strOperateNode + "<nInsureCommOdds>", ""));
                    std::vector<std::string> &&vecInsureCommOdds = tars::TC_Common::sepstr<std::string>(tc_config.get(strOperateNode + "<nInsureCommOdds>", ""), ",");
                    for (unsigned int i = 0; i < vecInsureCommOdds.size(); i++)
                    {
                        std::vector<int> &&vecInsureOddsItem = tars::TC_Common::sepstr<int>(vecInsureCommOdds[i], ":");
                        if(vecInsureOddsItem.size() != 2)
                        {
                            continue;
                        }
                        mInsureCommOdds.insert(std::make_pair(vecInsureOddsItem[0], vecInsureOddsItem[1]));
                    }

                    // 延迟配置
                    // std::vector<int> mProLongCfg;
                    mProlongConfigs.clear();
                    std::vector<std::string> &&vecPriceOdds = tars::TC_Common::sepstr<std::string>(tc_config.get(strOperateNode + "<nProlongPrices>", "15:1"), ",");
                    for (unsigned int i = 0; i < vecPriceOdds.size(); i++)
                    {
                        std::vector<int> &&item = tars::TC_Common::sepstr<int>(vecPriceOdds[i], ":");
                        if(item.size() != 2)
                        {
                            continue;
                        }
                        mProlongConfigs.insert(std::make_pair(i, ProlongCfg{item[0], item[1]}));
                        DLOG_TRACE("prolong price: " << i <<" -> "<< item[0]);
                    }

                    mInsureShortOdds.clear();
                    std::vector<std::string> &&vecInsureShortOdds = tars::TC_Common::sepstr<std::string>(tc_config.get(strOperateNode + "<nInsureShortOdds>", ""), ",");
                    for (unsigned int i = 0; i < vecInsureShortOdds.size(); i++)
                    {
                        std::vector<int> &&vecInsureOddsItem = tars::TC_Common::sepstr<int>(vecInsureShortOdds[i], ":");
                        if(vecInsureOddsItem.size() != 2)
                        {
                            continue;
                        }
                        mInsureShortOdds.insert(std::make_pair(vecInsureOddsItem[0], vecInsureOddsItem[1]));
                    }


                    iBasePauseTime = TC_Common::strto<tars::Int32>(tc_config.get(strOperateNode + "<nBasePauseTime>", "600"));
                    mmExtraPauseTime.clear();
                    std::vector<std::string> &&vecPauseTime = tars::TC_Common::sepstr<std::string>(tc_config.get(strOperateNode + "<nExtraPauseTime>", ""), ",");
                    for (unsigned int i = 1; i <= vecPauseTime.size(); i++)
                    {
                        std::vector<int> &&vecPauseTimeItem = tars::TC_Common::sepstr<int>(vecPauseTime[i-1], ":");
                        if(vecPauseTimeItem.size() != 2)
                        {
                            continue;
                        }
                        mmExtraPauseTime.insert(std::make_pair(vecPauseTimeItem[0], vecPauseTimeItem[1]));
                    }

                    nPaomaAiRatio = TC_Common::strto<tars::Int32>(tc_config.get(strOperateNode + "<nPaomaAiRatio>", "20"));

                    nHoldSeatGameList = tc_config.get(strOperateNode + "<nHoldSeatGameList>", "");
                    nHoldSeatTime = TC_Common::strto<tars::Int32>(tc_config.get(strOperateNode + "<nHoldSeatTime>", "50"));

                    FDLOG_GAME_CONFIG_INFO << "nHoldSeatGameList: "<< nHoldSeatGameList << ", nHoldSeatTime: "<< nHoldSeatTime << endl;
                }
                else
                {
                    DLOG_TRACE("resolve unkown node, domain : " << *it);
                }
            }

            return true;
        }

        void GameConfig::printConfigFile(GameInitParam const &sInitParams)
        {
            FDLOG_GAME_CONFIG_INFO << "strRoomTableId: " << sInitParams.strRoomTableId << ", nReadyTime: " << nReadyTime << ", nCardTime: " << nCardTime << endl;
            std::ostringstream os;
            for(auto item : mBetTime)
            {
                os << "roomtype:" << item.first << "{ ";
                for(auto sub : item.second)
                {
                    os << sub << ",";
                }
                os << "}, " << endl;
            }
            os << "profit limit :" << endl;
            for(auto item : mProfitLimit)
            {
                if(item.second.size() != 2)
                {
                    continue;
                }
                os << item.first << ":"<< item.second[0]<< "-"<< item.second[1]<< endl;
            }
            FDLOG_GAME_CONFIG_INFO << os.str() << endl;
        }

        void GameConfig::loadRoomConfig(const void *p)
        {
            RoomSo::TGAME_GameConfig const &roomconfig = *static_cast<RoomSo::TGAME_GameConfig const *>(p);

            eRoomType = roomconfig.eGameType;
            iInitPoint = roomconfig.iInitPoint; //初始金币
            iBasePoint = roomconfig.iBasePoint; //金币倍率
            iProfit = roomconfig.iProfit;
            iServiceFee = roomconfig.iServiceFee;
            iMaxSeatNum = roomconfig.iMaxSeatNum;
            iAiGameRoundLimit = roomconfig.iAiGameRoundLimit;
            nBaseRatio = iBasePoint;
            nSysInit = roomconfig.iRecycLine;

            vNNType.clear();
            bEnterLimit = false;
            bRubCard = false;
            bBetLimit = false;
            bAddBetLimit = false;
            bRaiseBet = false;
            bEnterLimit = false;
            bKing = false;
            bAddBet = false;
            bDoubleBet = false;
            iNNRatioType = 0;
            mNNType = mDefaultNNType;

            __TRY__

            /*
            using namespace systemconfig;
            ListGamePlayOptionResp Tcfg = Decoding<ListGamePlayOptionResp>(roomconfig.vecGameConf);
            for (auto& sCfg : Tcfg.gamePlayOptionList)
            {
                DLOG_TRACE("sCfg.optionCode = " << sCfg.optionCode);
                DLOG_TRACE("sCfg.optionValue = " << sCfg.optionValue);

                std::vector<std::string>&& vecOptionCode = tars::TC_Common::sepstr<std::string>(sCfg.optionCode, ":");
                if (vecOptionCode.at(0) == "FrontBet")
                {
                    iFrontBet = TC_Common::strto<tars::Int32>(sCfg.optionValue);
                }
                else if (vecOptionCode.at(0) == "Blind")
                {
                    iBlindNum = TC_Common::strto<tars::Int32>(sCfg.optionValue);
                }
                else if (vecOptionCode.at(0) == "MinTake")
                {
                    iMinTake = TC_Common::strto<tars::Int32>(sCfg.optionValue);
                }
                else
                {
                    DLOG_TRACE("sCfg is validate, optionCode = " << sCfg.optionValue << ", optionValue = " << sCfg.optionValue);
                }
            }
            */

            __CATCH__

            //iFrontBet = iFrontBet * iBlindNum;
            iFrontBet = 0;
            iBlindNum = 100;
            iMinTake = 10000;
            iBlindLevel = 0;
        }

        void GameConfig::printRoomConfig()
        {
            FDLOG_GAME_CONFIG_INFO << "iBlindNum: " << iBlindNum << ", iMinTake: " << iMinTake << ", iServiceFee:" << iServiceFee << ", iProfit:" << iProfit <<", nSysInit: "<< nSysInit<< ", nHoldSeatGameList: "<< nHoldSeatGameList << ", nHoldSeatTime: "<< nHoldSeatTime << endl;
        }

        int GameConfig::getNNRatioByNNType(E_NN_TYPE nntype)
        {
            std::map<int, int>::iterator it = mNNType.find(nntype);
            if (it != mNNType.end())
            {
                return it->second;
            }

            return 1;
        }
        int GameConfig::getSysProfitLimit(int iPlayerNum)
        {
            DLOG_TRACE("iPlayerNum : " << iPlayerNum <<", iBlindLevel: "<< iBlindLevel << ", mProfitLimit size: "<< mProfitLimit.size());
            auto it = mProfitLimit.find(iBlindLevel);
            if(it != mProfitLimit.end())
            {
                if(it->second.size() != 2)
                {
                    return 0;
                }
                return iPlayerNum <= 4 ? it->second[0] : it->second[1];
            }
            return 0;
        }
    };
};
