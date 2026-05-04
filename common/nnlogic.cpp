#include <cstdlib>
#include <iterator>
#include <algorithm>
#include "utils/tarslog.h"
#include "common/nnlogic.h"

namespace nnlogic
{
    const short CONST_CARD_NUM = 52;
    bool isShortDeck = false;

    int nnrand(int max, int min)
    {
        std::random_device rd;
        srand(rd());
        return min + rand() % (max - min + 1);
    }

    void build(vecc_t &vSrcCards, bool isShort)
    {
        if (!vSrcCards.empty())
        {
            vSrcCards.clear();
        }

        int start_index = 2;
        if (isShort)
        {
            start_index = 6;
            vSrcCards.reserve(36);
        }
        else
        {
            vSrcCards.reserve(CONST_CARD_NUM);
        }

        //2,3,4,5,6,7,8,9,10,J,Q,K,A or 6,7,8,9,10,J,Q,K,A
        for (int j = start_index; j <= 14; j++)
        {
            vSrcCards.push_back(E_NN_CARD::NN_CARD_DIAMOND + j);
            vSrcCards.push_back(E_NN_CARD::NN_CARD_CLUB + j);
            vSrcCards.push_back(E_NN_CARD::NN_CARD_HEART + j);
            vSrcCards.push_back(E_NN_CARD::NN_CARD_SPADE + j);
        }
    }

    void setShortDeck(bool curr)
    {
        isShortDeck = curr;
    }

    bool getShortDeck()
    {
        return isShortDeck;
    }

    void deal(vecc_t &vSrcCards, vecc_t &vDstCards, int iNum)
    {
        for (int i = 0; i < iNum; i++)
        {
            vDstCards.push_back(vSrcCards.back());
            vSrcCards.pop_back();
        }
    }

    int getbombnum(const std::map<card_t, short> &mCardCount)
    {
        int max = 0;

        for (auto it = mCardCount.begin(); it != mCardCount.end(); it++)
        {
            if (it->second > max)
            {
                max = it->second;
            }
        }

        return max;
    }

    bool issamekind(const vecc_t &vSrcCards)
    {
        for (auto it = vSrcCards.begin() + 1; it != vSrcCards.end(); it++)
        {
            if (nncard::getNNType(*it) != nncard::getNNType(*vSrcCards.begin()))
            {
                return false;
            }
        }

        return true;
    }

    bool isstraight(const vecc_t &vNumCards, const std::map<card_t, short> &mCardCount, int space, bool isShort)
    {
        for (auto it = mCardCount.begin();
                it != mCardCount.end();
                it++)
        {
            if (it->second > 1)
            {
                return false;
            }
        }

        // 短牌A6789是顺子
        int start_index = 2;
        if (isShort)
        {
            start_index = 6;
        }

        vecc_t vTestCards;
        for (int i = start_index; i <= start_index + 3; i++)
        {
            vTestCards.push_back(i);
        }

        vTestCards.push_back(14);
        if (vecfind<card_t>(vTestCards, vNumCards))
        {
            return true;
        }

        for (auto it1 = vNumCards.begin(); it1 != vNumCards.end() - 1; it1++)
        {
            for (auto it2 = it1 + 1; it2 != vNumCards.end(); it2++)
            {
                if (abs(*it1 - *it2) > space)
                {
                    return false;
                }
            }
        }

        return true;
    }

    void combination(vector<vecc_t>& vecCombinationCards, vecc_t& vecCards, const int count)
    {
        vecCombinationCards.clear();
        sort(vecCards.begin(), vecCards.end());
        if(count == 0 || count >= int(vecCards.size()))
        {
            vecCombinationCards.push_back(vecCards);
            return;
        }
        if (count == 1)
        {
            for(auto item : vecCards)
            {
                vecCombinationCards.push_back(vecc_t(1, item));
            }
            return;
        }
        do
        {
            vecc_t vT;
            if(count == 0 || count >= int(vecCards.size()))
            {
                vT.insert(vT.begin(), vecCards.begin(), vecCards.end());
            }
            else
            {
                vT.insert(vT.begin(), vecCards.begin(), vecCards.begin() + count);
            }
            vecCombinationCards.push_back(vT);
        }while (next_permutation(vecCards.begin(), vecCards.end()));

        for (auto& item: vecCombinationCards)
        {
            sort(item.begin(), item.end());
        }
        sort(vecCombinationCards.begin(),vecCombinationCards.end());
        for (auto r=vecCombinationCards.begin();r<vecCombinationCards.end()-1;r++)
        {
            if (*r==*(r+1)) vecCombinationCards.erase(r--);
        }
    }

    E_NN_TYPE getnn_omh(const vecc_t &vSrcCards, const vecc_t &vComCards, vecc_t &vMaxCards)
    {
        std::ostringstream os;
        os<< "--> cards: ";
        for(auto card : vSrcCards)
        {
            os<< getNNName(card) << " ";
        }
        os<< " | ";
        for(auto card : vComCards)
        {
            os<< getNNName(card) << " ";
        }

        vector<vecc_t> vecHdCards;
        vecc_t vTSrcCards;
        vTSrcCards.insert(vTSrcCards.begin(), vSrcCards.begin(), vSrcCards.end());
        combination(vecHdCards, vTSrcCards, 2);

        //
        vector<vecc_t> vecCommCards;
        vecc_t vTComCards;
        vTComCards.insert(vTComCards.begin(), vComCards.begin(), vComCards.end());
        combination(vecCommCards, vTComCards, 3);

        //最大牌型
        vecc_t maxcards;
        E_NN_TYPE max = nil_nntype;
        for(auto hdcard : vecHdCards)
        {
            for(auto commcard : vecCommCards)
            {
                E_NN_TYPE cur_type = getnn_comm(hdcard, commcard, maxcards);
                if (cur_type > max || (cur_type == max && compare(maxcards, vMaxCards, cur_type) == 1))
                {
                    max = cur_type;
                    vMaxCards = maxcards;
                }
            }
        }

        os<< "==> best cards: ";
        for(auto card : vMaxCards)
        {
            os<< getNNName(card) << " ";
        }
        os << endl;
        LOG_DEBUG(os.str());

        return max;
    }

    E_NN_TYPE getnn(const vecc_t &vSrcCards, const vecc_t &vComCards, vecc_t &vMaxCards, int iGameType)
    {
        if (!vMaxCards.empty())
        {
            vMaxCards.clear();
        }
        if(iGameType == 7)
        {
            return getnn_omh(vSrcCards, vComCards, vMaxCards);
        }
        else
        {
            return getnn_comm(vSrcCards, vComCards, vMaxCards, iGameType);
        }
        return nil_nntype;
    }

    string getNNName(card_t card)
    {
        std::ostringstream os;
        card_t card_face = getNNNum(card);
        if(card_face > 10)
        {
            if(card_face == 11)
            {
                os << "J";
            }
            if(card_face == 12)
            {
                os << "Q";
            }
            if(card_face == 13)
            {
                os << "K";
            }
            if(card_face == 14)
            {
                os << "A";
            }
        }
        else
        {
            os << card_face;
        }

        card_t card_type = getNNType(card);
        if(card_type == 0)
        {
            os << "♦";
        }
        if(card_type == 16)
        {
            os << "♣";
        }
        if(card_type == 32)
        {
            os << "♥";
        }
        if(card_type == 48)
        {
            os << "♠";
        }
        return os.str();
    }

    E_NN_TYPE getnn_comm(const vecc_t &vSrcCards, const vecc_t &vComCards, vecc_t &vMaxCards, int iGameType)
    {
        if (!vMaxCards.empty())
        {
            vMaxCards.clear();
        }

        //最大牌型
        E_NN_TYPE max = nil_nntype;

        vecc_t vTestCards;
        vTestCards.insert(vTestCards.begin(), vSrcCards.begin(), vSrcCards.end());
        vTestCards.insert(vTestCards.begin(), vComCards.begin(), vComCards.end());

        //手牌加公牌
        if (vTestCards.size() < 5 || vTestCards.size() > 7)
        {
            return max;
        }

        if (vTestCards.size() == 5)
        {
            vecc_t temp = vTestCards;
            E_NN_TYPE nntype = getnn(temp, iGameType);
            vMaxCards = temp;
            max = nntype;
        }

        if (vTestCards.size() == 6)
        {
            for (vecc_t::iterator it1 = vTestCards.begin();
                    it1 != vTestCards.end();
                    it1++)
            {
                vecc_t temp = vTestCards;
                deletecard(temp, *it1);

                //
                E_NN_TYPE nntype = getnn(temp, iGameType);
                if (nntype > max || (nntype == max && compare(temp, vMaxCards, nntype, iGameType == 3) == 1))
                {
                    vMaxCards = temp;
                    max = nntype;
                }
            }
        }

        //
        if (vTestCards.size() == 7)
        {
            for (vecc_t::iterator it1 = vTestCards.begin();
                    it1 != vTestCards.end() - 1;
                    it1++)
            {
                //
                for (vecc_t::iterator it2 = it1 + 1;
                        it2 != vTestCards.end();
                        it2++)
                {
                    vecc_t temp = vTestCards;
                    deletecard(temp, *it1);
                    deletecard(temp, *it2);

                    //
                    E_NN_TYPE nntype = getnn(temp, iGameType);
                    if (nntype > max || (nntype == max && compare(temp, vMaxCards, nntype, iGameType == 3) == 1))
                    {
                        vMaxCards = temp;
                        max = nntype;
                    }
                }
            }
        }
        return max;
    }

    int compare(const vecc_t &vSrcCards1, const vecc_t &vSrcCards2, E_NN_TYPE nntype, bool isShort)
    {
        vecc_t vNumCards1 = nncard2cardnum(vSrcCards1);
        vecc_t vNumCards2 = nncard2cardnum(vSrcCards2);

        if (nntype == E_NN_TYPE::NN_TYPE_NN_STRAIGHT
                || nntype == E_NN_TYPE::NN_TYPE_NN_FLUSH)
        {
            int start_index = 2;
            if (isShort)
            {
                start_index = 6;
            }

            vecc_t vTestCards;
            for (int i = start_index; i <= start_index + 3; i++)
            {
                vTestCards.push_back(i);
            }

            vTestCards.push_back(14);
            if (vecfind<card_t>(vNumCards1, vNumCards2)
                    && vecfind<card_t>(vNumCards2, vNumCards1))
            {
                return 0;
            }

            if (vecfind<card_t>(vTestCards, vNumCards1))
            {
                return -1;
            }

            if (vecfind<card_t>(vTestCards, vNumCards2))
            {
                return 1;
            }
        }

        std::map<card_t, short> mCardCount1 = nncard2cardcount(vNumCards1);
        std::map<card_t, short> mCardCount2 = nncard2cardcount(vNumCards2);
        std::sort(vNumCards1.begin(), vNumCards1.end(), [&mCardCount1](card_t c1, card_t c2)->bool
        {
            if (mCardCount1[c1] > mCardCount1[c2])
            {
                return true;
            }
            else if (mCardCount1[c1] < mCardCount1[c2])
            {
                return false;
            }
            else
            {
                return c1 > c2;
            }
        });

        std::sort(vNumCards2.begin(), vNumCards2.end(), [&mCardCount2](card_t c1, card_t c2)->bool
        {
            if (mCardCount2[c1] > mCardCount2[c2])
            {
                return true;
            }
            else if (mCardCount2[c1] < mCardCount2[c2])
            {
                return false;
            }
            else
            {
                return c1 > c2;
            }
        });

        for (int i = 0; i < static_cast<int>(vNumCards1.size()); i++)
        {
            if (*(vNumCards1.begin() + i) > *(vNumCards2.begin() + i))
            {
                return 1;
            }

            if (*(vNumCards1.begin() + i) < * (vNumCards2.begin() + i))
            {
                return -1;
            }
        }

        return 0;
    }

    E_NN_TYPE getnn2(const vecc_t &vSrcCards)
    {
        if (vSrcCards.size() != 2)
        {
            return E_NN_TYPE::NN_TYPE_NN_N0;
        }

        if (getNNNum(vSrcCards[0]) == getNNNum(vSrcCards[1]))
        {
            return E_NN_TYPE::NN_TYPE_NN_1P;
        }
        else
        {
            return E_NN_TYPE::NN_TYPE_NN_N0;
        }
    }

    E_NN_TYPE getnn(const vecc_t &vSrcCards, int iGameType)
    {
        if (vSrcCards.size() != 5)
        {
            return E_NN_TYPE::NN_TYPE_NN_N0;
        }

        //
        vecc_t vNumCards = nncard2cardnum(vSrcCards);
        std::map<card_t, short> mCardCount = nncard2cardcount(vNumCards);
        bool same = issamekind(vSrcCards);
        bool straight = isstraight(vNumCards, mCardCount, 4, iGameType == 3);
        if (same && straight)
        {
            std::map<card_t, short>::iterator fdA = mCardCount.find(14);
            std::map<card_t, short>::iterator fdK = mCardCount.find(13);
            if (fdA != mCardCount.end()
                    && fdK != mCardCount.end())
            {
                return E_NN_TYPE::NN_TYPE_NN_KING;
            }
            else
            {
                return E_NN_TYPE::NN_TYPE_NN_FLUSH;
            }
        }

        int bombnum = getbombnum(mCardCount);
        if (bombnum >= 4)
        {
            return E_NN_TYPE::NN_TYPE_NN_BOMB4;
        }

        if (mCardCount.size() <= 2)
        {
            return E_NN_TYPE::NN_TYPE_NN_HULU;
        }

        if (same)
        {
            return E_NN_TYPE::NN_TYPE_NN_SAMEKIND;
        }

        if (straight)
        {
            return E_NN_TYPE::NN_TYPE_NN_STRAIGHT;
        }

        if (bombnum >= 3)
        {
            return E_NN_TYPE::NN_TYPE_NN_BOMB3;
        }

        if (mCardCount.size() <= 3)
        {
            return E_NN_TYPE::NN_TYPE_NN_2P;
        }

        if (bombnum >= 2)
        {
            return E_NN_TYPE::NN_TYPE_NN_1P;
        }

        return E_NN_TYPE::NN_TYPE_NN_N0;
    }

    E_NN_GUESS getguess(const vecc_t &vSrcCards)
    {
        if (vSrcCards.size() != 2)
        {
            return nil_guess;
        }

        vecc_t vNumCards = nncard2cardnum(vSrcCards);
        std::map<card_t, short> mCardCount = nncard2cardcount(vNumCards);
        vecc_t::iterator fdA = std::find(vNumCards.begin(), vNumCards.end(), 14);
        if (fdA != vNumCards.end() && mCardCount.size() <= 1)
        {
            return E_NN_GUESS::NN_GUESS_AA;
        }

        if (mCardCount.size() <= 1)
        {
            return E_NN_GUESS::NN_GUESS_1P;
        }

        if (fdA != vNumCards.end())
        {
            return E_NN_GUESS::NN_GUESS_1A;
        }

        return nil_guess;
    }

    int deletecard(vecc_t &vSrcCards, card_t card, bool bAll)
    {
        int count = 0;

        //
        vecc_t::iterator it = vSrcCards.begin();
        while(it != vSrcCards.end())
        {
            if (card == *it)
            {
                it = vSrcCards.erase(it);

                count++;

                if (!bAll)
                {
                    break;
                }
            }
            else
            {
                ++it;
            }
        }

        return count;
    }

    std::map<card_t, short> nncard2cardcount(const vecc_t &vSrcCards)
    {
        std::map<card_t, short> mCardCount;
        for (auto it = vSrcCards.begin(); it != vSrcCards.end(); it++)
        {
            if (mCardCount.count(*it) > 0)
            {
                ++mCardCount[*it];
            }
            else
            {
                mCardCount[*it] = 1;
            }
        }

        return std::move(mCardCount);
    }

    vecc_t nncard2cardnum(const vecc_t &vSrcCards)
    {
        vecc_t vNumCards;
        for (auto it = vSrcCards.begin();
                it != vSrcCards.end();
                it++)
        {
            vNumCards.push_back(nncard::getNNNum(*it));
        }

        return std::move(vNumCards);
    }


    void vecremove(vecc_t &vSrcCards1, vecc_t &vSrcCards2)
    {
        auto it = std::remove_if (vSrcCards1.begin(), vSrcCards1.end(), [vSrcCards2](card_t card1)->bool
        {
            return vSrcCards2.end() != std::find_if (vSrcCards2.begin(), vSrcCards2.end(), [card1](card_t card2)->bool
            {
                return getNNNum(card1) == getNNNum(card2) && getNNType(card1) == getNNType(card2);
            });
        });

        vSrcCards1.erase(it, vSrcCards1.end());
    }

};
