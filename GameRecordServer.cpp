#include "GameRecordServer.h"
#include "GameRecordServantImp.h"

//
using namespace std;

/////
GameRecordServer g_app;

/////////////////////////////////////////////////////////////////
void GameRecordServer::initialize()
{
    //initialize application here:
    //...

    //注册服务接口
    addServant<GameRecordServantImp>(ServerConfig::Application + "." + ServerConfig::ServerName + ".GameRecordServantObj");

    //初始化外部接口对象
    initOuterFactory();

    // 注册动态加载命令
    TARS_ADD_ADMIN_CMD_NORMAL("reload", GameRecordServer::reloadSvrConfig);
}

/////////////////////////////////////////////////////////////////
void GameRecordServer::destroyApp()
{
    //destroy application here:
    //...
}

/*
* 配置变更，重新加载配置
*/
bool GameRecordServer::reloadSvrConfig(const string &command, const string &params, string &result)
{
    try
    {
        //加载配置
        getOuterFactoryPtr()->load();

        result = "reload server config success.";
        LOG_DEBUG << "reloadSvrConfig: " << result << endl;
        return true;
    }
    catch (TC_Exception const &e)
    {
        result = string("catch tc exception: ") + e.what();
    }
    catch (std::exception const &e)
    {
        result = string("catch std exception: ") + e.what();
    }
    catch (...)
    {
        result = "catch unknown exception.";
    }

    result += "\n fail, please check it.";
    LOG_DEBUG << "reloadSvrConfig: " << result << endl;
    return true;
}

/**
* 初始化外部接口对象
**/
int GameRecordServer::initOuterFactory()
{
    _pOuter = new OuterFactoryImp();
    return 0;
}

/////////////////////////////////////////////////////////////////
int main(int argc, char *argv[])
{
    try
    {
        g_app.main(argc, argv);
        g_app.waitForShutdown();
    }
    catch (std::exception &e)
    {
        cerr << "std::exception:" << e.what() << std::endl;
    }
    catch (...)
    {
        cerr << "unknown exception." << std::endl;
    }

    return -1;
}
/////////////////////////////////////////////////////////////////
