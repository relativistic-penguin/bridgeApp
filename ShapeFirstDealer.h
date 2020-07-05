#ifndef SHAPE_FIRST_DEALER_H
#define SHAPE_FIRST_DEALER_H

#include <map>
#include <iostream>
#include <vector>
#include <random>
#include "utils.h"
#include "Board.h"
#include "Shape.h"
#include "Value.h"
#include "BaseDealer.h"

class ShapeFirstDealer : public BaseDealer {
   typedef map<Position, map<Suit, int>> specificShapeType;
   typedef vector<pair<Position, int>>   nonSpecificShapeType;
   typedef map<Position, pair<int, int>> hcpType;
   typedef map<Position, map<Rank, vector<Suit>>> allocType;
   private:
      nonSpecificShapeType       nonSpecificShapeRules;
      hcpType                    hcpRules;
      vector<Shape>              unpopulatedShapes;
      map<Shape, vector<Shape>>  possibleShapes;
      vector<Value>              possibleValues;
      bool                       ready;
      
      Board			 dealFromAlloc(allocType alloc, Shape shape);
      vector<Shape>              shapePopulate(const Shape& shape);
      vector<Shape>              posPopulate(const vector<Shape>& shapes, Position pos);

   public:
      ShapeFirstDealer(specificShapeType, nonSpecificShapeType, hcpType);
      void  setSpecificShapeRules(specificShapeType);
      void  setNonSpecificShapeRules(nonSpecificShapeType);
      void  setHCPrules(hcpType);
      void  getReady();
      bool  isReady() {return ready;} 
      Board deal();

      void  test(ostream&, bool showShape = false, bool showValue = false);
};


#endif
