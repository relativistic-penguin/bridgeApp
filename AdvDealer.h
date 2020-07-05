#ifndef ADV_DEALER_H
#define ADV_DEALER_H

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

using std::vector, std::map, std::pair, std::set;

class AdvDealer {
   typedef map<Position, map<Suit, int>> specificShapeType;
   typedef vector<pair<Position, int>>   nonSpecificShapeType;
   typedef map<Position, pair<int, int>> hcpType;
 
   private:
      vector<Value>        reqVal;
      vector<Shape>	   reqShapes;
      std::random_device   rd;
      std::mt19937	   rdGen {std::mt19937(rd())};

      template <typename Tp>
      void			       shuffleCards(vector<Tp>& targetList);
      ValueConfig		       randomSelect(const vector<ValueConfig>&);
      Board			       dealLowCards(const Shape&, const ValueConfig&);
      bool			       isPossible(const Shape&, const ValueConfig&);
      static vector<ValueConfig>       filterConfig(const Shape, const vector<ValueConfig>&);
      static map<Position, set<Card>>  dealHighCards(const ValueConfig&);

      static vector<Shape> nonSpecificShapeFilter(const nonSpecificShapeType& filters, Shape& shape);
      static vector<Value> hcpFilter(const hcpType& filter, const Value& value);
      static vector<Value> rowHCPfilter(const vector<Value>& values,const Position& pos, int min, int max, bool complete);

   public:
      AdvDealer(specificShapeType, nonSpecificShapeType, hcpType);
      Board                deal();
      void		   test();
};

#endif
