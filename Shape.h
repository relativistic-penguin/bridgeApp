#ifndef SHAPE_H
#define SHAPE_H

#include <map>
#include <iostream>
#include "utils.h"

using std::map, std::pair, std::ostream;

class Shape {
   private:
      map<Position, map<Suit, int>>  rep1;
      map<Suit, map<Position, int>>  rep2;
      map<Position, map<Suit, bool>> fixed;
      map<Position, int>             posVac;
      map<Suit, int>                 suitVac;
      bool                           completed = false;
   public:
      Shape();
      void               set(Position, Suit, int, bool fix = false);
      int                get(Position pos, Suit suit) const {return rep1.at(pos).at(suit);}
      map<Position, int> get(Suit suit) const {return rep2.at(suit);}
      map<Suit, int>     get(Position pos) const {return rep1.at(pos);}
      map<Position, int> getPosVac() const {return posVac;}
      int                getPosVac(Position pos) const {return posVac.at(pos);}
      map<Suit, int>     getSuitVac() const {return suitVac;}
      int                getSuitVac(Suit suit) const {return suitVac.at(suit);}
      bool               complete();
      void               inc(Position pos, Suit suit) {set(pos, suit, get(pos, suit) + 1);}
      bool               checkFix(Position pos, Suit suit) const {return fixed.at(pos).at(suit);}
      bool               operator== (const Shape&) const;
};

ostream& operator<< (ostream&, Shape);

#endif

