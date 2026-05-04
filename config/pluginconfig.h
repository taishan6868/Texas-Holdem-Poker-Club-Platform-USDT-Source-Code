#pragma once

#include "Comm/ITableGame.h"

#include "common/macros.h"
#include "common/nndef.h"
#include "singleton.h"

using namespace nndef;

namespace game
{
    namespace config
    {
        class PluginConfig
        {
            DISALLOW_COPY_AND_ASSIGN(PluginConfig)

        private:
            //
            explicit PluginConfig();

        public:
            //
            static PluginConfig *Create()
            {
                return new PluginConfig();
            }

        public:
            //
            bool loadPluginConfig(const char *strPath);
            //
            void printPluginConfig();
            //
            void triggerPluginConfig();

        public:
            inline bool isPluginEnable()
            {
                return bPluginEnable;
            }
            inline int getControlCount()
            {
                return nControlCount;
            }
            inline int getProbability()
            {
                return nProbability;
            }
            inline std::vector<int> &getVecUserID()
            {
                return vUserID;
            }

        protected:
            //
            bool bPluginEnable;
            //
            int nControlCount;
            //
            int nProbability;
            //
            std::vector<int> vUserID;
            //
            std::string md5cfg;
        };
    };
};

