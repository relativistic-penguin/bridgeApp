#include <stack>
#include <random>
#include <algorithm>
#include <set>
#include <map>
#include <chrono>
#include "AdvDealer.h"
#include "ValueConfig.h"
#include "Hand.h"

//include for debug
#include "Value.h"

using namespace std;

typedef map<Position, map<Suit, int>> specificShapeType;
typedef vector<pair<Position, int>>   nonSpecificShapeType;
typedef map<Position, pair<int, int>> hcpType;

template <typename Tp>
void AdvDealer::shuffleCards(vector<Tp>& targetList) {
   shuffle(targetList.begin(), targetList.end(), rdGen);
}

vector<ValueConfig> AdvDealer::filterConfig(const Shape shape, const vector<ValueConfig>& compatibleConfigs) {
   vector<ValueConfig> results;
   for (auto& config : compatibleConfigs) {
      bool failed = false;
      for (auto& pos : posList) {
	 for (auto& suit : suitList) {
	    if (shape.checkFix(pos, suit) && shape.get(pos, suit) < config.posSuitLength(pos, suit)) {
	       failed = true;
	       break;
	    }
	 }
	 if (failed)
	    break;
      }
      if (!failed)
	 results.push_back(config);
   }
   return results;
}
/*
void AdvDealer::test() {
   Value egValue;
   egValue.set(Position::E, Rank::A, 4, true);
   egValue.set(Position::E, Rank::K, 4, true);
   egValue.set(Position::E, Rank::Q, 4, true);
   egValue.set(Position::E, Rank::J, 1, true);
   
   egValue.set(Position::S, Rank::J, 3, true);

   cout << egValue;

   vector<ValueConfig> possibleConfigs = egValue.compatibleConfig();
   int i = 0;
   for (auto& conf : possibleConfigs) {
      cout << "Possible Config No. " << ++i << endl;
      cout << conf;
   }


}
*/

ValueConfig AdvDealer::randomSelect(const vector<ValueConfig>& valCons) {
   uniform_int_distribution<int> dist(0, valCons.size() - 1);
   return valCons[dist(rdGen)];
}

map<Position, set<Card>> AdvDealer::dealHighCards(const ValueConfig& valCon) {
   map<Position, set<Card>> results;
   for (auto& pos : posList) {
      set<Card> holding;
      for (auto& rank : rankList) {
	 for (auto& suit : suitList) {
	    if (valCon.get(pos, rank, suit)) {
	       Card newCard = {suit, rank};
	       holding.insert(newCard);
	    }
	 }
      }
      results.insert(make_pair(pos, holding));
   }
   return results;
}

bool AdvDealer::isPossible(const Shape& shape, const ValueConfig& valCon) {
   map<Suit, int> usedNum;
   for (auto suit : suitList) {
      usedNum.insert(make_pair(suit, 0));
   }
   for (auto pos : posList) {
      for (auto suit : suitList) {
	 if (shape.checkFix(pos, suit)) {
	    //cout << pos << " " << suit << "is fixed.\n";
	    //cout << "Initially used " << usedNum.at(suit) << " cards in this suit.\n";
	    int reqNum = shape.get(pos, suit) - valCon.posSuitLength(pos, suit);
	    usedNum.at(suit) += reqNum;
	    //cout << "Finally used " << usedNum.at(suit) << " cards in this suit.\n";
	    if (usedNum.at(suit) > numRankList.size())
	       return false;
	 }
      }
   }
   return true; 
}

Board AdvDealer::dealLowCards(const Shape& shape,const ValueConfig& valCon) {
   // first deal high cards according to Value Config
   map<Position, set<Card>> results = dealHighCards(valCon);
   cout << "Dealt high cards as follows:\n";
   for (auto& pos : posList) {
      cout << pos << ": ";
      for (auto& card : results.at(pos)) {
	 cout << card << " ";
      }
      cout << endl;
   }
   // low cards sorted by suit
   map<Suit, pair<vector<Rank>, int>> lowCards;
   vector<Rank> lowRankSeq = numRankList;
   for (auto suit : suitList) {
      shuffleCards(lowRankSeq);
      lowCards.insert(make_pair(suit, make_pair(lowRankSeq, 0)));
   }
   // assign low cards to fulfil all shape requirements.
   for (auto pos : posList) {
      for (auto suit : suitList) {
	 if (shape.checkFix(pos, suit)) {
	    auto& [ranks, initIndex] = lowCards.at(suit);
	    int reqIndex = shape.get(pos, suit) - valCon.posSuitLength(pos, suit) + initIndex;
	    for (; initIndex < reqIndex; initIndex++) {
	       Card newCard = {suit, ranks[initIndex]};
	       results.at(pos).insert(newCard);
	    }
	 }
      }
   }
   // assign remaining low cards randomly but avoid violating shape requirements.
   set<Card> remainCards;
   for (auto suit : suitList) {
      auto [ranks, initIndex] = lowCards.at(suit);
      for (int i = initIndex; i < ranks.size(); i++) {
	 Card newCard = {suit, ranks[i]};
	 remainCards.insert(newCard);
      }
   }
   for (auto pos : posList) {
      set<Suit> allowedSuitList;
      for (auto suit : suitList) {
	 if (!shape.checkFix(pos, suit))
	    allowedSuitList.insert(suit);
      }
      vector<Card> allowedCardList;
      for (auto& card : remainCards) {
	 if (allowedSuitList.find(card.suit) != allowedSuitList.end())
	    allowedCardList.push_back(card);
      }
      shuffleCards(allowedCardList);
      int reqNum = CARDS - results.at(pos).size();
      for (int i = 0; i < reqNum; i++) {
	 results.at(pos).insert(allowedCardList[i]);
	 remainCards.erase(allowedCardList[i]);
      }
   }
   return Board(results);
}

AdvDealer::AdvDealer(specificShapeType speRules, nonSpecificShapeType blurRules, hcpType valRules) {
   Shape initShape;
   for (auto& [pos, suitRule] : speRules)
      for (auto& [suit, val] : suitRule)
	 initShape.set(pos, suit, val, true);
   cout << "Specific Filter Applied:\n" << initShape;
   reqShapes = nonSpecificShapeFilter(blurRules, initShape);
   cout << "Non-specific Filter Applied:\n";
   int counter = 0;
   for (auto& shape : reqShapes) {
      cout << "Shape No. " << ++counter << endl << shape;
   }
   Value initValue;
   reqVal = hcpFilter(valRules, initValue);
   cout << "Value filter applied. It has " << reqVal.size() << " possible Values." << endl;
}

void AdvDealer::test() {
   Value egTargetValue;
   egTargetValue.set(Position::E, Rank::J, 1);
   egTargetValue.set(Position::E, Rank::Q, 1);
   egTargetValue.set(Position::E, Rank::K, 1);
   egTargetValue.set(Position::E, Rank::A, 1);

   egTargetValue.set(Position::S, Rank::J, 1);
   egTargetValue.set(Position::S, Rank::Q, 1);
   egTargetValue.set(Position::S, Rank::K, 1);
   egTargetValue.set(Position::S, Rank::A, 1);

   egTargetValue.set(Position::W, Rank::J, 1);
   egTargetValue.set(Position::W, Rank::Q, 1);
   egTargetValue.set(Position::W, Rank::K, 1);
   egTargetValue.set(Position::W, Rank::A, 1);

   egTargetValue.set(Position::N, Rank::J, 1);
   egTargetValue.set(Position::N, Rank::Q, 1);
   egTargetValue.set(Position::N, Rank::K, 1);
   egTargetValue.set(Position::N, Rank::A, 1);

   cout << "Using known required shape:\n" << egTargetValue;

   Shape egShape = reqShapes[0];
   cout << "Using shape:\n" << egShape;

   vector<ValueConfig> valCons = egTargetValue.compatibleConfig();
   cout << "Found this many compatible configs: " << valCons.size() << endl;
   int count = 0;
   for (auto valCon : valCons) {
      ++count;
      if (count % 1000 == 0) {
	 cout << "Checked config No. " << count << endl;
      }
      bool possible = isPossible(egShape, valCon);
      //cout << "Possible: " << possible << endl;
      if (possible) {
	 cout << "This is No. " << count << endl;
	 cout << valCon;
	 Board newBoard = dealLowCards(egShape, valCon);
	 cout << newBoard;
	 break;
      }
   }
}

Board AdvDealer::deal() { 
   shuffleCards(reqVal);
   shuffleCards(reqShapes);
   cout << "Shuffled " << reqShapes.size() << " shapes" << endl;
   int counter = 0;
   for (auto val : reqVal) {
      //cout << "Checking Value No. " << ++counter << endl;
      //cout << "Chose the following Value:\n" << val;
      vector<ValueConfig> compatibleConfigs = val.compatibleConfig();
      //cout << "Found " << compatibleConfigs.size() << " compatible Configs with the Value" << endl;
      for (auto shape : reqShapes) {
	 //cout << "Chose the following Shape:\n" << shape;
	 vector<ValueConfig> allowedConfigs = filterConfig(shape, compatibleConfigs); //TODO: filterConfig
	 //cout << "Found " << compatibleConfigs.size() << " allowed Configs based on chosen shape" << endl;
	 if (!allowedConfigs.empty()) {
	    shuffleCards(allowedConfigs);
	    for (auto chosenConfig : allowedConfigs) {
	       //ValueConfig chosenConfig = randomSelect(allowedConfigs); TODO: randomSelecti
	       bool possible = isPossible(shape, chosenConfig);
	       if (possible) {
		  cout << "This Value is possible" << endl;
		  cout << val;
		  cout << "This Config is possible" << endl;
		  cout << "Chosen Config: \n" << chosenConfig;
		  Board newBoard = dealLowCards(shape, chosenConfig); //TODO: dealLowCards
		  cout << "Dealt low cards" << endl;
		  return newBoard;
	       }
	       /*
	       else {
		  cout << "This Config is not possible" << endl;
	       }
	       */
	    }
	 }
      }
   }
   throw invalid_argument("No possible board config with given constraint");
}

vector<Value> AdvDealer::hcpFilter(const map<Position, pair<int, int>>& filter, const Value& value) {
   vector<Value> results {value};
   int i = 0;
   for (auto& pos : posList) {
      i++;
      cout << "Filtering HCP for pos: " << pos << endl;
      auto startTime = chrono::high_resolution_clock::now();
      
      if (filter.find(pos) != filter.end())
	 results = rowHCPfilter(results, pos, filter.at(pos).first, filter.at(pos).second, pos == *(posList.end() - 1));
      else
	 results = rowHCPfilter(results, pos, 0, (valueHCP.at(Rank::J) + valueHCP.at(Rank::Q) + valueHCP.at(Rank::K) + valueHCP.at(Rank::A) ) * VAL_CARDS, pos == *(posList.end() -1));

      cout << "Now results have " << results.size() << " entries.\n";
      auto endTime = chrono::high_resolution_clock::now();
      chrono::duration<double> elapsedTime = endTime - startTime;
      cout << "This step took " << elapsedTime.count() << "s\n";
   }
   return results;
}

vector<Value> AdvDealer::rowHCPfilter(const vector<Value>& values,const Position& pos, int min, int max, bool complete) {
   vector<Value> rowResults;
   for (auto& value : values) {
      const Rank J = Rank::J;
      const Rank Q = Rank::Q;
      const Rank K = Rank::K;
      const Rank A = Rank::A;

      if (complete) {
	 Value newValue = value;
	 int valSum = 0;
	 for (auto& rank : rankList) {
	    int rankNum = newValue.getRankVac(rank);
	    newValue.set(pos, rank, rankNum);
	    valSum += rankNum * valueHCP.at(rank);
	    if (valSum > max)
	       break;
	 }
	 if (valSum >= min && valSum <= max) {
	    rowResults.push_back(newValue);
	 }
      }
      else {
	 map<Rank, int> rVal = {{J, 0}, {Q, 0}, {K, 0}, {A, 0}};
	 for (rVal[J] = 0; rVal[J] <= value.getRankVac(J); rVal[J]++) {
	    if (rVal.at(J) * valueHCP.at(J) > max)
	       break;
	    bool jFixed = false;
	    if (value.checkFix(pos, J)) {
	       jFixed = true;
	       rVal[J] = value.get(pos, J);
	    }
	    for (rVal[Q] = 0; rVal[Q] <= value.getRankVac(Q); rVal[Q]++) {
	       if (rVal.at(J) * valueHCP.at(J) + rVal.at(Q) * valueHCP.at(Q) > max)
		  break;
	       bool qFixed = false;
	       if (value.checkFix(pos, Q)) {
		  qFixed = true;
		  rVal[Q] = value.get(pos, Q);
	       }
	       for (rVal[K] = 0; rVal[K] <= value.getRankVac(K); rVal[K]++) {
		  if (rVal.at(J) * valueHCP.at(J) + rVal.at(Q) * valueHCP.at(Q) + rVal.at(K) * valueHCP.at(K) > max)
		     break;
		  bool kFixed = false;
		  if (value.checkFix(pos, K)) {
		     kFixed = true;
		     rVal[K] = value.get(pos, K);
		     //cout << "H fixed\n";
		  }
		  for (rVal[A] = 0; rVal[A] <= value.getRankVac(A); rVal[A]++) {
		     if (rVal[A] + rVal[K] + rVal[Q] + rVal[J] > CARDS) {
			break;
		     }
		     bool aFixed = false;
		     if (value.checkFix(pos, A)) {
			aFixed = true;
			rVal[A] = value.get(pos, A);
			//cout << "S fixed\n";
		     }
		     int sum = rVal.at(J) * valueHCP.at(J) + 
			rVal.at(Q) * valueHCP.at(Q) + 
			rVal.at(K) * valueHCP.at(K) + 
			rVal.at(A) * valueHCP.at(A); 
		     //cout << "C = " << sVal[C] << endl;
		     //cout << "D = " << sVal[D] << endl;
		     //cout << "H = " << sVal[H] << endl;
		     //cout << "S = " << sVal[S] << endl;
		     if (sum > max) {
			//cout << "Too large. Break\n";
			break;
		     }
		     else if (sum >= min) {
			Value newValue = value;
			if (!jFixed)
			   newValue.set(pos, J, rVal.at(J));
			if (!qFixed)
			   newValue.set(pos, Q, rVal.at(Q));
			if (!kFixed)
			   newValue.set(pos, K, rVal.at(K));
			if (!aFixed)
			   newValue.set(pos, A, rVal.at(A));
			if (complete) {
			   //cout << "Last line find value:" << endl;
			   //cout << newValue;
			   bool filled = true;
			   for (auto& [pos, vac] : newValue.getRankVac()) {
			      if (vac) {
				 filled = false;
				 break;
			      }
			   }
			   //cout << "It is " << filled << endl;
			   if (filled) {
			      //cout << "So it is accepted." << endl;
			      rowResults.push_back(newValue);
			   }
			}
			else
			   rowResults.push_back(newValue);
			//cout << "New result: " << newShape;
		     }
		     if (aFixed)
			break;
		  }
		  if (kFixed)
		     break;
	       }
	       if (qFixed)
		  break;
	    }
	    if (jFixed)
	       break; 
	 }
      }
   }
   return rowResults;
}

vector<Shape> AdvDealer::nonSpecificShapeFilter(const vector<pair<Position, int>>& filters, Shape& shape) {
   stack<pair<Shape, vector<pair<Position, int>>>> todo;
   vector<Shape> results;
   if (filters.empty()) {
      results.push_back(shape);
      return results;
   }
   todo.push(make_pair(shape, filters));
   while (!todo.empty()) {
      auto [curShape, curFilters] = todo.top();
      todo.pop();
      auto [filterPos, filterNum] = curFilters.back();
      curFilters.pop_back();
      if (curShape.getPosVac(filterPos) >= filterNum) {
	 for (auto& suit : suitList) {
	    if (curShape.getSuitVac(suit) >= filterNum && !curShape.checkFix(filterPos, suit)) {
	       Shape newShape = curShape;
	       newShape.set(filterPos, suit, filterNum, true);
	       if (curFilters.empty()) {
		  results.push_back(newShape);
		  //cout << "New Results\n" << newShape;
	       }
	       else {
		  todo.push(make_pair(newShape, curFilters));
		  //cout << "New pushed\n" << newShape;
	       }
	    }
	 }
      }
   }
   return results;
}


