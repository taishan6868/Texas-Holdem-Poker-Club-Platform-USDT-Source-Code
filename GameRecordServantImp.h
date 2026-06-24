#ifndef _GameRecordServantImp_H_
#define _GameRecordServantImp_H_

#include "servant/Application.h"
#include "GameRecordServant.h"
#include "XGameComm.pb.h"
#include "CommonCode.pb.h"
#include "CommonStruct.pb.h"
#include "GameRecord.pb.h"

//
using namespace gamerecord;

/**
 *登录服务逻辑处理接口
 *
 */
class GameRecordServantImp : public gamerecord::GameRecordServant
{
public:
    /**
     *
     */
    virtual ~GameRecordServantImp() {}

    /**
     *
     */
    virtual void initialize();

    /**
     *
     */
    virtual void destroy();

public:
    //http请求处理接口
    virtual tars::Int32 doRequest(const vector<tars::Char> &reqBuf, const map<std::string, std::string> &extraInfo, vector<tars::Char> &rspBuf, tars::TarsCurrentPtr current);
    //tcp请求处理接口
    virtual tars::Int32 onRequest(tars::Int64 lUin, const std::string &sMsgPack, const std::string &sCurServrantAddr, const JFGame::TClientParam &stClientParam, const JFGame::UserBaseInfoExt &stUserBaseInfo, tars::TarsCurrentPtr current);
    //经典场用户行为信息上报
    virtual tars::Int32 reportQSUserActInfo(const gamerecord::ReportQSUserActInfoReq &req, gamerecord::ReportQSUserActInfoRsp &resp, tars::TarsCurrentPtr current);
    //比赛场用户行为信息上报
    virtual tars::Int32 reportKOUserActInfo(const gamerecord::ReportKOUserActInfoReq &req, gamerecord::ReportKOUserActInfoRsp &resp, tars::TarsCurrentPtr current);
    //AI场用户行为信息上报
    virtual tars::Int32 reportAIUserActInfo(const gamerecord::ReportAIUserActInfoReq &req, const gamerecord::ReportAIUserCardInfoReq &req1, gamerecord::ReportAIUserActInfoRsp &resp, tars::TarsCurrentPtr current);
   //私人场用户信息上报
    virtual tars::Int32 reportPrGameInfo(const gamerecord::RoomCalStatReq &req, tars::TarsCurrentPtr current);

public:
    //查询经典场用户行为信息
    int onGetQSUserGameRecord(const XGameComm::TPackage &pkg, long uid, const std::string &sCurServrantAddr);
    //查询比赛场用户行为信息
    int onGetKOUserGameRecord(const XGameComm::TPackage &pkg, GameRecordProto::KOUserActInfoReq &req, const std::string &sCurServrantAddr);
    //查询AI场用户基础信息
    int onGetAIUserBaseGameRecord(const XGameComm::TPackage &pkg, long uid, const std::string &sCurServrantAddr);
    //查询AI场用户牌信息
    int onGetAIUserCardGameRecord(const XGameComm::TPackage &pkg, long uid, const std::string &sCurServrantAddr);
    //查询私人厂游戏记录
    int onGetPRGameRecord(const XGameComm::TPackage &pkg, GameRecordProto::PRGameInfoReq &req, const std::string &sCurServrantAddr);
    //删除私人场游戏记录
    int onDeletePRGameRecord(const XGameComm::TPackage &pkg, GameRecordProto::PRDeleteGameInfoReq &req, const std::string &sCurServrantAddr);
private:
    //发送消息到客户端
    template<typename T>
    int toClientPb(const XGameComm::TPackage &tPackage, const std::string &sCurServrantAddr, XGameProto::ActionName actionName, const T &t);
};
/////////////////////////////////////////////////////
#endif
