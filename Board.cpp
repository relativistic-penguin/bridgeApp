#include <unordered_set>
#include <algorithm>
#include <random>
#include "Board.h"

using std::pair;

map<Position, Hand> boardBuilder(vector<int> seq, Position dealer) {
   if (seq.size() != 52)
      throw std::length_error("attempting to build board from incorrect number of cards");
   map<Position, set<Card>> board {{Position::E, {}}, {Position::S, {}}, {Position::W, {}}, {Position::N, {}}};
   std::unordered_set<int> dupChecker;
   Position player = dealer;
   for (auto num : seq) {
      if (dupChecker.find(num) != dupChecker.end()) {
	 std::string msg = "Attempting to deal card No. ";
	 msg.append(std::to_string(num)).append(" more than once.");
	 throw std::invalid_argument(msg);
      }
      else {
	 dupChecker.insert(num);
	 board.at(player).insert(cardSelector(num));
	 player = player + 1;
      }
   }
   map<Position, Hand> result;
   for (auto& [pos, cards] : board)
      result.insert(pair<Position, Hand>(pos, Hand(cards)));
   return result;
}

ostream& operator<< (ostream& out, Board board) {
   out << "----------" << endl;
   for (auto& [pos, Hand] : board.get())
      out << pos << ":" << endl << Hand << endl;
   out << "----------" << endl;
   return out;
}

Board::Board() {
   // Unconstrained random deal
   vector<int> seq;
   seq.reserve(54);
   for (int i = 0; i < 52; i++) {
      seq.push_back(i);
   }
   std::random_device rd;
   std::mt19937 rg(rd());
   std::shuffle(seq.begin(), seq.end(), rg);
   config = boardBuilder(seq, Position::E);
}

