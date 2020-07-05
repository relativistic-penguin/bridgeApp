#ifndef BOARD_H
#define BOARD_H

#include <iostream>
#include <vector>
#include "Hand.h"
#include "utils.h"

using std::ostream, std::map, std::vector;

class Board {
   private:
      map<Position, Hand> config;
   public:
      Board();
      Board(map<Position, set<Card>>);
      Board(map<Position, Hand> config) {this->config = config;}
      map<Position, Hand> get() {return config;}
      Hand get(Position pos) {return config.at(pos);}
};

map<Position, Hand> boardBuilder(vector<int> seq, Position dealer);

ostream& operator<< (ostream& out, Board board);

#endif
