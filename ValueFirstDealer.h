#ifndef VALUE_FIRST_DEALER_H
#define VALUE_FIRST_DEALER_H

#include <vector>
#include <map>
#include <set>
#include <random>
#include <iostream>
#include "utils.h"
#include "ValueConfig.h"
#include "Value.h"
#include "Board.h"
#include "Shape.h"
#include "BaseDealer.h"

using std::vector, std::map, std::pair, std::set;

class ValueFirstDealer : public BaseDealer {
   typedef map<Position, map<Suit, int>> specificShapeType;
   typedef vector<pair<Position, int>>   nonSpecificShapeType;
   typedef map<Position, pair<int, int>> hcpType;
 
   private:
      vector<Value>        reqVal;
      vector<Shape>	   reqShapes;

      Board			       dealLowCards(const Shape&, const ValueConfig&);
      bool			       isPossible(const Shape&, const ValueConfig&);
      static vector<ValueConfig>       filterConfig(const Shape, const vector<ValueConfig>&);
      static map<Position, set<Card>>  dealHighCards(const ValueConfig&);

   public:
      ValueFirstDealer(specificShapeType, nonSpecificShapeType, hcpType);
      Board                deal();
      void		   test();
};

#endif
