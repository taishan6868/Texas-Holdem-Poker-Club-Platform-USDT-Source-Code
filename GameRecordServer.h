#ifndef _GameRecordServer_H_
#define _GameRecordServer_H_

#include <iostream>
#include "servant/Application.h"
#include "OuterFactoryImp.h"

//
using namespace tars;

/**
 * 游戏数据服务
 **/
class GameRecordServer : public Application
{
public:
    /**
     *
     **/
    virtual ~GameRecordServer() {};

    /**
     *
     **/
    virtual void initialize();

    /**
     *
     **/
    virtual void destroyApp();

public:
    /*
    * 配置变更，重新加载配置
    */
    bool reloadSvrConfig(const string &command, const string &params, string &result);

public:
    /**
    * 初始化外部接口对象
    **/
    int initOuterFactory();

    /**
    * 取外部接口对象
    **/
    OuterFactoryImpPtr getOuterFactoryPtr()
    {
        return _pOuter;
    }

private:
    //外部接口对象
    OuterFactoryImpPtr _pOuter;
};

////
extern GameRecordServer g_app;

////////////////////////////////////////////
#endif
