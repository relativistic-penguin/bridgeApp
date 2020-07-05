#ifndef BASIC_DEALER_H
#define BASIC_DEALER_H

#include <map>
#include "utils.h"
#include "BaseDealer.h"
#include "Board.h"

static std::map<Position, std::map<Suit, int>> emptyFilter;

class BasicDealer : public BaseDealer {
   public:
      BasicDealer() : BaseDealer(emptyFilter) {}
      Board deal() {return Board();} //TODO: Should use base class provided random function.
};

#endif
