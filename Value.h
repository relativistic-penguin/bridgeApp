#ifndef VALUE_H
#define VALUE_H

#include <map>
#include <iostream>
#include <vector>
#include "utils.h"
#include "ValueConfig.h"

using std::map, std::pair, std::ostream, std::vector;

const int VAL_CARDS = 4;
const map<Rank, int> valueHCP = {{Rank::J, 1}, {Rank::Q, 2}, {Rank::K, 3}, {Rank::A, 4}};

class Value {
   private:
      map<Position, map<Rank, int>>  rep1;
      map<Rank, map<Position, int>>  rep2;
      map<Position, map<Rank, bool>> fixed;
      map<Position, int>             posVal;
      map<Rank, int>                 rankVac;
      bool                           completed = false;
   public:
      Value();
      void                set(Position, Rank, int, bool fix = false);
      void                forceSet(Position, Rank, int);
      int                 get(Position pos, Rank rank) const {return rep1.at(pos).at(rank);}
      map<Position, int>  get(Rank suit) const {return rep2.at(suit);}
      map<Rank, int>      get(Position pos) const {return rep1.at(pos);}
      map<Position, int>  getPosVal() const {return posVal;}
      int                 getPosVal(Position pos) const {return posVal.at(pos);}
      map<Rank, int>      getRankVac() const {return rankVac;}
      int                 getRankVac(Rank rank) const {return rankVac.at(rank);}
      bool                complete();
      bool                checkFix(Position pos, Rank rank) const {return fixed.at(pos).at(rank);}
      bool                isEqual(const Value&) const;
      vector<ValueConfig> compatibleConfig() const;
};

ostream& operator<< (ostream&, Value);

#endif

