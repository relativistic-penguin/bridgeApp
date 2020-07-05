#include <algorithm>
#include <chrono>
#include <stack>
#include "BaseDealer.h"

using namespace std;

typedef map<Position, map<Suit, int>> specificShapeType;
typedef vector<pair<Position, int>>   nonSpecificShapeType;
typedef map<Position, pair<int, int>> hcpType;

BaseDealer::BaseDealer(specificShapeType ssr) {
   for (auto& [pos, suitRule] : ssr)
      for (auto& [suit, val] : suitRule)
	 initShape.set(pos, suit, val, true);
}

vector<Value> BaseDealer::hcpFilter(const map<Position, pair<int, int>>& filter, const Value& value) {
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

vector<Value> BaseDealer::rowHCPfilter(const vector<Value>& values,const Position& pos, int min, int max, bool complete) {
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

vector<Shape> BaseDealer::nonSpecificShapeFilter(const vector<pair<Position, int>>& filters, Shape& shape) {
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


