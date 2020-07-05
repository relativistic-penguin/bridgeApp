#ifndef BASE_DEALER_H
#define BASE_DEALER_H

#include <map>
#include <vector>
#include <random>
#include <algorithm>
#include "utils.h"
#include "Shape.h"
#include "Value.h"
#include "Board.h"

using std::map, std::vector, std::pair;

class BaseDealer {
   typedef map<Position, map<Suit, int>> specificShapeType;
   typedef vector<pair<Position, int>>   nonSpecificShapeType;
   typedef map<Position, pair<int, int>> hcpType;

   protected:
      std::random_device   rd;
      std::mt19937	   rdGen {std::mt19937(rd())};
      Shape		   initShape;

      template <typename Tp>
      void shuffleCards(vector<Tp>& targetList) {std::shuffle(targetList.begin(), targetList.end(), rdGen);}
 
      static vector<Shape> nonSpecificShapeFilter(const nonSpecificShapeType& filters, Shape& shape);
      static vector<Value> hcpFilter(const hcpType& filter, const Value& value);
      static vector<Value> rowHCPfilter(const vector<Value>& values,const Position& pos, int min, int max, bool complete);

   public:
      BaseDealer(specificShapeType);
      virtual Board deal() = 0;
      virtual ~BaseDealer() {};
     
};

#endif
