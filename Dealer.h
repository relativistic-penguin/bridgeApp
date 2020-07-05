#ifndef DEALER_H
#define DEALER_H

#include <map>
#include <iostream>
#include <vector>
#include "utils.h"
#include "Board.h"
#include "Shape.h"
#include "Value.h"

class Dealer {
   typedef map<Position, map<Suit, int>> specificShapeType;
   typedef vector<pair<Position, int>>   nonSpecificShapeType;
   typedef map<Position, pair<int, int>> hcpType;
   private:
      specificShapeType          specificShapeRules;
      nonSpecificShapeType       nonSpecificShapeRules;
      hcpType                    hcpRules;
      vector<Shape>              unpopulatedShapes;
      map<Shape, vector<Shape>>  possibleShapes;
      vector<Value>              possibleValues;
      bool                       ready;

      vector<Shape>              nonSpecificShapeFilter(const vector<pair<Position, int>>& filters, Shape& shape);
      vector<Shape>              shapePopulate(const Shape& shape);
      vector<Shape>              posPopulate(const vector<Shape>& shapes, Position pos);

      vector<Shape>              compatibleUnpopStates(Value);
      vector<Shape>              compatibleFinalStates(Value);

   public:
      Dealer();
      Dealer(specificShapeType, nonSpecificShapeType, hcpType);
//      Dealer(istream& in);

      void  setSpecificShapeRules(specificShapeType);
      void  setNonSpecificShapeRules(nonSpecificShapeType);
      void  setHCPrules(hcpType);
      void  getReady();
      bool  isReady() {return ready;} 
      Board deal();
      vector<Value>              hcpFilter(const map<Position, pair<int, int>>& filter, const Value& value);
      vector<Value>	         rowHCPfilter(const vector<Value>& values, const Position& pos, int min, int max, bool complete);


      void  test(ostream&, bool showShape = false, bool showValue = false);
};

template <typename Tp>
void shuffleCards(vector<Tp>& targetList); 


#endif
