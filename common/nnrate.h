#pragma once

#include <random>
#include <algorithm>

#include "nndef.h"

using namespace nndef;
using namespace nncard;
using namespace nninvalid;

namespace nnlogic
{
    CardRate maxRate(vecc_t &vecCards, vecc_t &comcards, vecc_t &vecWallCards);
    int maxType(int a, int b);
}