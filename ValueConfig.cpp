#include "ValueConfig.h"
#include "utils.h"
#include "Value.h"

using namespace std;

vector<Position> posVec  = posList;
vector<Rank>     rankVec = rankList;
vector<Suit>     suitVec = suitList;

void ValueConfig::checkValidity() const{
   map<Position, int> cardCount;
   for (auto& pos : posList)
      cardCount.insert(make_pair(pos, 0));
   for (auto& rank : rankList) {
      for (auto& suit : suitList) {
	 bool used = false;
	 for (auto& pos : posList) {
	    if (valTable.at(pos).at(rank).at(suit)) {
	       if (used)
		  throw invalid_argument("Invalid Value Config: attempted to use the same card twice.");
	       else {
		  used = true;
		  if (++cardCount.at(pos) > CARDS) {
		     throw invalid_argument("Invalid Value Config: A Position has more key cards than allowed.");
		  }
	       }
	    }
	 }
      }
   }
}

ValueConfig::ValueConfig(const map<Position, map<Rank, map<Suit, bool>>> valMap) {
   valTable = valMap;
   for (auto& pos : posList) {
      if (valTable.find(pos) == valTable.end()) {
	 map<Rank, map<Suit, bool>> mapA;
	 for (auto& rank : rankList) {
	    map<Suit, bool> mapB;
	    for (auto& suit : suitList) {
	       mapB.insert(make_pair(suit, false));
	    }
	    mapA.insert(make_pair(rank, mapB));
	 }
	 valTable.insert(make_pair(pos, mapA));
      }
      else {
	 for (auto& rank : rankList) {
	    if (valTable.at(pos).find(rank) == valTable.at(pos).end()) {
	       map<Suit, bool> mapB;
	       for (auto& suit : suitList) {
		  mapB.insert(make_pair(suit, false));	  
	       }
	       valTable.at(pos).insert(make_pair(rank, mapB));
	    }
	    else {
	       for (auto& suit : suitList) {
		  if (valTable.at(pos).at(rank).find(suit)  == valTable.at(pos).at(rank).end()) {
		     valTable.at(pos).at(rank).insert(make_pair(suit, false));
		  }
	       }
	    }
	 }
      }
   }
   checkValidity();
}

ValueConfig::ValueConfig() {
   for (auto& pos : posList) {
      map<Rank, map<Suit, bool>> mapA;
      for (auto& rank : rankList) {
	 map<Suit, bool> mapB;
	 for (auto& suit : suitList) {
	    mapB.insert(make_pair(suit, false));
	 }
	 mapA.insert(make_pair(rank, mapB));
      }
      valTable.insert(make_pair(pos, mapA));
   }
}

bool ValueConfig::get(Position pos, Rank rank, Suit suit) const{
   return valTable.at(pos).at(rank).at(suit);
}

void ValueConfig::set(Position pos, Rank rank, Suit suit) {
   valTable.at(pos).at(rank).at(suit) = true;
   checkValidity();
}

void ValueConfig::reset(Position pos, Rank rank, Suit suit) {
   if (valTable.at(pos).at(rank).at(suit))
      valTable.at(pos).at(rank).at(suit) = false;
   else
      throw invalid_argument("Invalid ValueConfig reset: protected reset on 0");
}

void ValueConfig::reset(Rank rank, Suit suit) {
   for (auto& pos : posList)
      if (valTable.at(pos).at(rank).at(suit)) {
	 valTable.at(pos).at(rank).at(suit) = false;
	 break;
      }
}

bool ValueConfig::complete() const{
   for (auto& rank : rankList) {
      for (auto& suit : suitList) {
	 bool used = false;
	 for (auto& pos : posList) {
	    if (valTable.at(pos).at(rank).at(suit)) {
	       used = true;
	       break;
	    }
	 }
	 if (!used)
	    return false;
      }
   }
   return true;
}

bool ValueConfig::cardAvailable(Suit suit, Rank rank) const{
   for (auto& pos : posList) {
      if (valTable.at(pos).at(rank).at(suit))
	 return false;
   }
   return true;
}

int ValueConfig::posVal(Position pos) const{
   int val = 0;
   for (auto& rank : rankList) {
      for (auto& suit : suitList) {
	 if (valTable.at(pos).at(rank).at(suit))
	    val += valueHCP.at(rank);
      }
   }
   return val;
}

int ValueConfig::posSuitLength(Position pos, Suit suit) const {
   int length = 0;
   for (auto& rank : rankList) {
      if (valTable.at(pos).at(rank).at(suit))
	 length++;
   }
   return length;
}

vector<Suit> ValueConfig::rankAvaiSuit(Rank rank) const{
   vector<Suit> results;
   for (auto& suit : suitList) {
      bool used = false;
      for (auto& pos : posList) {
	 if (valTable.at(pos).at(rank).at(suit)) {
	    used = true;
	    break;
	 }
      }
      if (!used)
	 results.push_back(suit);
   }
   return results;
}

vector<Rank> ValueConfig::suitAvaiRank(Suit suit) const{
   vector<Rank> results;
   for (auto& rank : rankList) {
      bool used = false;
      for (auto& pos : posList) {
	 if (valTable.at(pos).at(rank).at(suit)) {
	    used = true;
	    break;
	 }
      }
      if (!used)
	 results.push_back(rank);
   }
   return results;
}

bool ValueConfig::operator< (const ValueConfig other) const {
   for (auto& pos : posList) {
      for (auto& rank : rankList) {
	 for (auto& suit : suitList) {
	    if (valTable.at(pos).at(rank).at(suit) < other.get(pos, rank, suit))
	       return true;
	    else if (valTable.at(pos).at(rank).at(suit) > other.get(pos, rank, suit))
	       return false;
	 }
      }
   }
   return false;
}

ostream& operator<< (ostream& out, ValueConfig config) {
   out << "   ";
   for (auto& rank : rankList) {
      out << rank << " ";
   }
   out << endl;
   for (auto& suit : suitList) {
      out << suit << " |";
      for (auto& rank : rankList) {
	 bool found = false;
	 Position newPos;
	 for (auto& pos : posList) {
	    if (config.get(pos, rank, suit)) {
	       found = true;
	       newPos = pos;
	       break;
	    }
	 }
	 if (found)
	    out << newPos << " ";
	 else
	    out << "- ";
      }
      out << endl;
   }
   return out;
}
