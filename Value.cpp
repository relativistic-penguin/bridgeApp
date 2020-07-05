#include <stack>
#include <tuple>
#include "Value.h"

using namespace std;

vector<vector<Suit>> select(int remain, vector<Suit> remainChoices) {
   vector<vector<Suit>> results;
   if (remain == 1) {
      for (auto& choice : remainChoices) {
	 vector<Suit> lastSet {choice};
	 results.push_back(lastSet);
	// cout << "Added " << choice << endl;
      }
   } else {
      for (int i = 0; i < remainChoices.size(); i++) {
	 auto& suit = remainChoices[i];
	 vector<Suit> newChoices;
	 for (int j = i + 1; j < remainChoices.size(); j++) {
	    auto& thisSuit = remainChoices[j];
	    newChoices.push_back(thisSuit);
	 }
	 vector<vector<Suit>> nextLevel = select(remain - 1, newChoices);
	 for (auto& suits : nextLevel) {
	    suits.push_back(suit);
	    results.push_back(suits);
	 }
      }
   }
   return results;
}


vector<ValueConfig> Value::compatibleConfig() const {
   //cout << "Attempting to assign for Value:\n";
   //cout << *this;
   vector<Position> posVector = posList;
   vector<Rank> rankVector = rankList;
   //cout << "Length of vectors " << posVector.size() << " " << rankVector.size() << endl;
   stack<tuple<int, int, ValueConfig>> todo;
   vector<ValueConfig> results;
   ValueConfig initConfig;
   //cout << initConfig;
   todo.push(make_tuple(0, 0, initConfig));
   while (!todo.empty()) {
      auto [posId, rankId, curConfig] = todo.top();
      todo.pop();
      //cout << "Current Config\n";
      //cout << curConfig;
      //cout << "posId: " << posId << "; rankId: " << rankId << endl;
      //cout << posId << ", " << rankId << endl;
      int remainNum = rep1.at(posVector[posId]).at(rankVector[rankId]);
      if (remainNum) {
	 vector<Suit> remainSuits = curConfig.rankAvaiSuit(rankVector[rankId]);
	 //cout << "ok2\n";
	 vector<vector<Suit>> chosenSuits = select(remainNum, remainSuits);
	 //cout << "ok3\n";
	 //cout << "Possible assignments:\n";
	 //for (auto& suits : chosenSuits) {
	 //   for (auto& suit : suits) {
	 //      cout << suit << " ";
	 //   }
	 //   cout << endl;
	 //}
	 for (auto selectedSuits : chosenSuits) {
	    ValueConfig newValCon = curConfig;
	    //cout << curConfig;
	    //cout << "selected vector of Suits\n";
	    //cout << newValCon;
	    //cout << "Bug here" << endl;
	    for (auto selectedSuit : selectedSuits) {
	       newValCon.set(posVector[posId], rankVector[rankId], selectedSuit);
	       //cout << "New ValueConfig";
	       //cout << newValCon;
	    }
	    if (rankId == rankVector.size() - 1) {
	       if (posId == posVector.size() - 1) {
		  results.push_back(newValCon);
	       }
	       else {
		  todo.push(make_tuple(posId + 1, 0, newValCon));
		  //cout << "Pushed new ValueConfig \n" << newValCon << "posId: " << posId + 1 << "rankId: " << 0 << endl;
	       }
	    }
	    else {
	       todo.push(make_tuple(posId, rankId + 1, newValCon));
	       //cout << "Pushed new ValueConfig \n" << newValCon << "posId: " << posId << "rankId: " << rankId + 1 << endl;
	    }
	 }
      }
      else {
	 if (rankId == rankVector.size() - 1) {
	    if (posId == posVector.size() - 1) {
	       results.push_back(curConfig);
	    }
	    else {
	       todo.push(make_tuple(posId + 1, 0, curConfig));
	       //cout << "Pushed new ValueConfig \n" << curConfig << "posId: " << posId + 1 << "rankId: " << 0 << endl;
	    }
	 }
	 else {
	    todo.push(make_tuple(posId, rankId + 1, curConfig));
	    //cout << "Pushed new ValueConfig \n" << curConfig << "posId: " << posId << "rankId: " << rankId + 1 << endl;
	 }

      }
   }
   return results;
}

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

void Value::forceSet(Position pos, Rank rank, int val) {
   fixed[pos][rank] = false;
   set(pos, rank, val, true);
   completed = false;
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

bool Value::isEqual(const Value& other) const {
   for (auto& pos : posList) {
      for (auto& rank : rankList) {
	 if (rep1.at(pos).at(rank) != other.rep1.at(pos).at(rank))
	    return false;
      }
   }
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

