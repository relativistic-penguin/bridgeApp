#ifndef VALUE_ALLOCATOR_H
#define VALUE_ALLOCATOR_H

#include <iostream>
#include <vector>
#include "utils.h"
#include "Value.h"
#include "Shape.h"

using std::vector;

class ValueAllocator {   
   friend ostream& operator<<(ostream&, ValueAllocator);
   typedef map<Position, map<Rank, vector<Suit>>> allocType;
   private:
      allocType		    alloc;
      Value		    reqVal;
      Shape		    reqShape;
      bool		    ready;
      bool                  usable;

//      void                  getReady();
      vector<Suit>          get(Position pos, Rank rank) {return alloc.at(pos).at(rank);}
      void                  add(Position pos, Rank rank, Suit suit) {alloc[pos][rank].push_back(suit);}
      void                  set(Position pos, Rank rank, vector<Suit> suits) {alloc[pos][rank] = suits;} 
      ValueAllocator(Value, Shape, allocType);
      bool		 isAvailable(Rank, Suit);
   public:
      ValueAllocator(Value, Shape);
//      bool               isReady();
//      bool	         isUsable();
//      bool               calculate();
      Value		   getVal() {return  reqVal;}
      Shape		   getShape() {return reqShape;}
      vector<allocType>	   getAllocation(bool);
      allocType		   getAlloc() {return alloc;}
};

void printAlloc(map<Position, map<Rank, vector<Suit>>> alloc, ostream& out);

#endif
