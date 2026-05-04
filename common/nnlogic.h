#pragma once

#include <random>
#include <algorithm>

#include "common/nndef.h"

using namespace nndef;
using namespace nncard;
using namespace nninvalid;

namespace nnlogic
{
    extern const short CONST_CARD_NUM;
    extern bool isShortDeck;

    int nnrand(int max, int min = 0);
    void build(vecc_t &vSrcCards, bool isShort = false);
    void setShortDeck(bool curr);   // 设置是否是短牌玩法
    bool getShortDeck();            // 获取是否是短牌玩法

    template<typename T>
    void shuffle(std::vector<T> &vSrc)
    {
        std::random_device rd;
        std::mt19937 g(rd());
        std::shuffle(vSrc.begin(), vSrc.end(), g);
    }

    string getNNName(card_t card);
    void deal(vecc_t &vSrcCards, vecc_t &vDstCards, int iNum);
    int getbombnum(const std::map<card_t, short> &mCardCount);
    bool issamekind(const vecc_t &vSrcCards);
    bool isstraight(const vecc_t &vNumCards, const std::map<card_t, short> &mCardCount, int space);
    void combination(vector<vecc_t>& vecCombinationCards, vecc_t& vecCards, const int count);
    E_NN_TYPE getnn_omh(const vecc_t &vSrcCards, const vecc_t &vComCards, vecc_t &vMaxCards);
    E_NN_TYPE getnn_comm(const vecc_t &vSrcCards, const vecc_t &vComCards, vecc_t &vMaxCards, int iGameType = 0);
    E_NN_TYPE getnn(const vecc_t &vSrcCards, const vecc_t &vComCards, vecc_t &vMaxCards, int iGameType = 0);
    E_NN_TYPE getnn(const vecc_t &vSrcCards, int iGameType = 0);
    E_NN_TYPE getnn2(const vecc_t &vSrcCards);
    E_NN_GUESS getguess(const vecc_t &vSrcCards);
    int deletecard(vecc_t &vSrcCards, card_t card, bool bAll = false);
    std::map<card_t, short> nncard2cardcount(const vecc_t &vSrcCards);
    vecc_t nncard2cardnum(const vecc_t &vSrcCards);
    int compare(const vecc_t &vSrcCards1, const vecc_t &vSrcCards2, E_NN_TYPE nntype, bool isShort = false);

    void vecremove(vecc_t &vSrcCards1, vecc_t &vSrcCards2);

    template<typename T>
    bool vecfind(std::vector<T> const &vecSrc, T t)
    {
        typename std::vector<T>::const_iterator it = std::find(vecSrc.begin(), vecSrc.end(), t);
        return it != vecSrc.end();
    }

    template<typename T>
    bool vecfind(std::vector<T> const &vecSrc1, std::vector<T> const &vecSrc2)
    {
        for(typename std::vector<T>::const_iterator it = vecSrc2.begin();  it != vecSrc2.end(); it++)
        {
            if(!vecfind<T>(vecSrc1, *it))
            {
                return false;
            }
        }

        return true;
    }

    template<typename T>
    bool vecfind(std::vector<std::vector<T> > const &vecSrc1, std::vector<T> const &vecSrc2)
    {
        for (typename std::vector<std::vector<T> >::const_iterator it = vecSrc1.begin(); it != vecSrc1.end(); it++)
        {
            if(vecfind<T>(*it, vecSrc2))
            {
                return true;
            }
        }

        return false;
    }
};
