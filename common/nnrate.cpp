#include <cstdlib>
#include <iterator>
#include <algorithm>
#include "nnrate.h"
#include <unordered_map>
#include <set>
#include <functional>
#include <iostream>

namespace nnlogic
{
    void calcMaxRate(vecc_t& vecCards,vecc_t& comcards,vecc_t& vecWallCards,CardRate& cardRate);
    void generate_combinations(
        const vecc_t& pool, int k, int start, vecc_t& current,
        std::function<void(const vecc_t&)> callback
    );
    E_NN_TYPE evaluate_hand(const vecc_t& cards);

    //计算最大牌型
    CardRate maxRate(vecc_t &vecCards, vecc_t &comcards, vecc_t &vecWallCards){
        CardRate maxRate;
        maxRate.type = NN_TYPE_NN_N0;
        maxRate.rate = 0;
        calcMaxRate(vecCards, comcards, vecWallCards, maxRate);
        return maxRate;
    }

    int maxType(int a, int b){
        return a > b ? a : b;
    }


    // 牌型评估函数（需进一步优化实现）
    E_NN_TYPE evaluate_hand(const vecc_t& cards) {
        // 示例逻辑：仅判断皇家同花顺和同花顺
        // 检查牌型
        // 花色表
        int suits[7] = {0};
        // 点数表
        int ranks[7] = {0};
        int _cards[7] = { 0 };
        int i=0;
        for (card_t c : cards) {
            suits[i] = nncard::getNNType(c);
            ranks[i] = nncard::getNNNum(c);
            _cards[i] = c;
            i++;
        }


        //suits 排序
        int n = sizeof(suits) / sizeof(suits[0]);
        std::sort(suits, suits + n);
        std::sort(ranks, ranks + n);



        // 检查同花顺
        for(int i=0;i<2;i++){
            bool shunzi = true;
            for(int j=0;j<5;j++){
                if(_cards[i+j] > _cards[i]+1){
                    shunzi = false;
                    break;
                }
            }
            if(shunzi){
                return NN_TYPE_NN_FLUSH;
            }
        }
        //NN_TYPE_NN_SAMEKIND 同花 1,2,3,4,5,6,7
        for (int i = 0;i < 3;i++) {
            if (suits[i] == suits[i + 4]) {
                return NN_TYPE_NN_SAMEKIND;
            }
        }
        // NN_TYPE_NN_1P,              //一对
        // NN_TYPE_NN_2P,              //两对
        
        // NN_TYPE_NN_BOMB4,           //四条
        for (int i = 0;i < 4;i++) {
            if (ranks[i] == ranks[i + 3]) {
                return NN_TYPE_NN_BOMB4;
            }
        }
        E_NN_TYPE type = NN_TYPE_NN_N0;
        // NN_TYPE_NN_BOMB3,           //三条
        //11122,12223,12333
        for (int i = 0;i < 5;i++) {
            if (ranks[i] == ranks[i + 2]) {
                type = NN_TYPE_NN_BOMB3;
                break ;
            }
        }
        //记录所有牌的个数
        int counts[15] = {0};
        for(int i=0;i<7;i++){
            counts[ranks[i]]++;
        }
        int doubleCount = 0;
        for(int i=0;i<15;i++){
            if(counts[i] > 1){
                doubleCount++;
            }
        }
        if(type == NN_TYPE_NN_BOMB3){
            if(doubleCount >= 2){
                return NN_TYPE_NN_HULU;
            }
        }
        if (type > NN_TYPE_NN_N0) {
            return type;
        }
        
        // NN_TYPE_NN_STRAIGHT,        //顺子
        if(ranks[0] - ranks[4] == 4){
            return NN_TYPE_NN_STRAIGHT;
        }
        if(doubleCount > 1){
            return NN_TYPE_NN_2P;
        }
        if(doubleCount == 1){
            return NN_TYPE_NN_1P;
        }


        // 其他牌型判断需补充（此处简化返回高牌）
        return NN_TYPE_NN_N0;
    }

    // 生成所有组合的辅助函数
    void generate_combinations(
        const vecc_t& pool, int k, int start, vecc_t& current,
        std::function<void(const vecc_t&)> callback
    ) {
        if (current.size() == k) {
            callback(current);
            return;
        }
        for (size_t i = start; i < pool.size(); ++i) {
            current.push_back(pool[i]);
            generate_combinations(pool, k, i + 1, current, callback);
            current.pop_back();
        }
    }

    // 主函数：计算最大牌型及其概率
    void calcMaxRate(
        vecc_t& vecCards,      // 手牌（2张）
        vecc_t& comcards,      // 公共牌（3-5张）
        vecc_t& vecWallCards,  // 剩余牌堆
        CardRate& cardRate
    ) {
        
        // 输入校验
        if (vecCards.size() != 2 || comcards.size() < 3 || comcards.size() > 5) {
            cardRate.type = NN_TYPE_NN_N0;
            cardRate.rate = 0.0;
            return;
        }

        // 计算需要补全的公共牌数
        int needed = 5 - comcards.size();
        if (needed < 0 || vecWallCards.size() < (size_t)needed) {
            cardRate.type = NN_TYPE_NN_N0;
            cardRate.rate = 0.0;
            return;
        }

        // 统计所有补全组合的牌型
        std::unordered_map<E_NN_TYPE, int> counts;
        int total = 0;
        vecc_t current;
        generate_combinations(vecWallCards, needed, 0, current, [&](const vecc_t& comb) {
            vecc_t all_cards = vecCards;
            all_cards.insert(all_cards.end(), comcards.begin(), comcards.end());
            all_cards.insert(all_cards.end(), comb.begin(), comb.end());
            E_NN_TYPE rank = evaluate_hand(all_cards);
            counts[rank]++;
            total++;
        });

        // 确定最大牌型
        cardRate.type = NN_TYPE_NN_N0;
        int countForMaxRate = 0;
        for (const auto& pair : counts) {
            auto rank = pair.first;
            if (rank > cardRate.type){
                cardRate.type = rank;
                countForMaxRate = pair.second;
            }
        }

        //std::cout << "check total: "<< total<<" over" << std::endl;
        // 计算概率
        cardRate.rate = (total > 0) ? (countForMaxRate * 100.0 / total) : 0.0;
    }
}