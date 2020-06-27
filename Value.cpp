#include "Value.h"

Value::Value() {
   for (auto& rank : rankList) {
      map<Position, int> lis;
      for (auto& pos : posList)
	 lis.insert(pair<Position, int>(pos, 0));
      rep2.insert(pair<Rank, map<Position, int>>(rank, lis));

      rankVac.insert(pair<Rank, int>(rank, VAL_CARDS));
   }
   for (auto& pos : posList) {
      map<Rank, int> lis;
      map<Rank, bool> fixedList;
      for (auto& rank : rankList) {
	 lis.insert(pair<Rank, int>(rank, 0));
	 fixedList.insert(pair<Rank, bool>(rank, false));
      }
      rep1.insert(pair<Position, map<Rank, int>>(pos, lis));
      fixed.insert(pair<Position, map<Rank, bool>>(pos, fixedList));
      posVal.insert(pair<Position, int>(pos, 0));
   }
}

void Value::set(Position pos, Rank rank, int val, bool fix) {
   if (fixed.at(pos).at(rank))
      throw std::invalid_argument("Attempting to change fixed value value.");
   int newPosVal = posVal.at(pos) + (val - rep1.at(pos).at(rank)) * valueHCP.at(rank);
   int newRankVac = rankVac.at(rank) + rep1.at(pos).at(rank) - val;
   if (newRankVac < 0)
      throw std::invalid_argument("Invalid rank length in conflict with existing setting");
   posVal.at(pos) = newPosVal;
   rankVac.at(rank) = newRankVac;
   rep1.at(pos).at(rank) = val;
   rep2.at(rank).at(pos) = val;
   if (fix)
      fixed.at(pos).at(rank) = true;
}

bool Value::complete() {
   if (completed) 
      return true;
   for (auto& rank : rankList)
      if (rankVac.at(rank) > 0) 
	 return false;
   completed = true;
   return true;
}

ostream& operator<< (ostream& out, Value value) {
   using std::endl;
   out << "   ";
   for (auto& rank : rankList) {
      out << rank << " ";
   }
   out << "| T\n";
   for (auto& pos : posList) {
      out << " " << pos << " ";
      for (auto& rank : rankList) {
	 out << value.get(pos, rank) << " ";
      }
      out << "| " << value.getPosVal().at(pos) << endl;
   }
   out << "----------\n T ";
   for (auto& rank : rankList)
      out << VAL_CARDS - value.getRankVac().at(rank) << " ";
   out << endl;
   return out;
}

