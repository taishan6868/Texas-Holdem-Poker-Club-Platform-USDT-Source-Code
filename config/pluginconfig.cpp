#include <unistd.h>
#include "config/pluginconfig.h"
#include "utils/tarslog.h"
#include "util/tc_config.h"
#include "util/tc_md5.h"
#include "log/logcomm.h"

namespace game
{
    namespace config
    {
        PluginConfig::PluginConfig() : bPluginEnable(false), nControlCount(0), nProbability(100), md5cfg("")
        {

        }

        bool PluginConfig::loadPluginConfig(const char *strPath)
        {
            if (access(strPath, F_OK) == -1)
            {
                DLOG_TRACE("plg is not exsit, plg path = " << strPath);
                return false;
            }

            tars::TC_Config tc_config;
            tc_config.parseFile(strPath);

            std::string md5 = tars::TC_MD5::md5str(tc_config.tostr());
            if (md5cfg == md5)
            {
                DLOG_TRACE("md5cfg is not change, md5cfg = " << md5cfg);
                return true;
            }

            md5cfg = md5;
            const vector<string> &vecDomain = tc_config.getDomainVector("/Main/Game");
            for (auto it = vecDomain.begin(); it != vecDomain.end(); ++it)
            {
                if (*it == "PluginConfig")
                {
                    string strOperateNode = "/Main/Game/" + *it;
                    bPluginEnable = TC_Common::strto<tars::Bool>(tc_config.get(strOperateNode + "<bPluginEnable>", "0"));
                    nControlCount = TC_Common::strto<tars::Int32>(tc_config.get(strOperateNode + "<nControlCount>", "5"));
                    nProbability = TC_Common::strto<tars::Int32>(tc_config.get(strOperateNode + "<nProbability>", "100"));
                    std::vector<int> &&vecUserID = tars::TC_Common::sepstr<int>(tc_config.get(strOperateNode + "<sUserID>", ""), ",");
                    if (not vUserID.empty())
                    {
                        vUserID.clear();
                    }

                    vUserID.insert(vUserID.begin(), vecUserID.begin(), vecUserID.end());
                }
                else
                {
                    DLOG_TRACE("resolve unkown node, domain: " << *it);
                }
            }

            return true;
        }

        void PluginConfig::printPluginConfig()
        {
            FDLOG_PLUGIN_CONFIG_INFO << "bPluginEnable: " << bPluginEnable << ", nControlCount: " << nControlCount << endl;
        }

        void PluginConfig::triggerPluginConfig()
        {
            if (nControlCount > 0)
            {
                nControlCount--;
            }
        }
    };
};
