#include <stack>
#include <vector>
#include <unordered_set>
#include <iostream>
#include "ValueAllocator.h"

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

bool ValueAllocator::isAvailable(Rank rank, Suit suit) {
   for (auto& pos : posList) {
      for (auto& usedSuit : alloc.at(pos).at(rank)) {
	 if (usedSuit == suit) {
	    return false;
	 }
      }
   }
   return true;
}

ValueAllocator::ValueAllocator(Value val, Shape sha) {
   reqVal = val;
   reqShape = sha;
   //cout << "Assigned Shape" << endl;
   //cout << reqShape;
   //cout << "Assigned Value" << endl;
   //cout << reqVal;
   ready  = false;
   usable = false;
   for (auto& pos : posList) {
      map<Rank, vector<Suit>> rankMap;
      for (auto& rank : rankList) {
	 vector<Suit> suitList;
	 rankMap.insert(make_pair(rank, suitList));
      }
      alloc.insert(make_pair(pos, rankMap));
   }
}

ValueAllocator::ValueAllocator(Value val, Shape sha, map<Position, map<Rank, vector<Suit>>> al) {
   reqVal = val;
   reqShape = sha;
   alloc = al;
   ready = false;
   usable = false;
}

typedef map<Position, map<Rank, vector<Suit>>> allocType;
vector<allocType> ValueAllocator::getAllocation(bool all) {
   stack<ValueAllocator> todo;
   todo.push(*this);
   vector<allocType> results;
   int counter = 0;
   while (!todo.empty()) {
      //cout << "Run No. " << ++counter << endl;
      if (!all && !results.empty()) {
	 return results;
      }
      auto vlAllocator = todo.top();
      //cout << vlAllocator;
      todo.pop();
      Position pos;
      Rank rank;
      bool determined = false;
      bool completed = false;
      try {
      for (auto& tPos : posList) {
	 for (auto& tRank : rankList) {
	    //cout << "Attempting to get Value at " << tPos << " " << tRank << endl;
	    //cout << vlAllocator.getShape();
	    //cout << vlAllocator.getVal();
	    if (vlAllocator.getVal().get(tPos, tRank) != 0) {
	       determined = true;
	       pos = tPos;
	       rank = tRank;
	       break;
	    }
	 }
	 if (determined)
	    break;
      }
      } catch(exception& ex) {
	 cout << "Error 1;" << ex.what() << endl;
      }
      if (!determined) {
	 //Prior to push. Need to check if value allocation is compatible with Value and Shape.
	 //cout << "Pushing new results " << endl;	 
	 results.push_back(vlAllocator.getAlloc());
      }
      else {
	 try{
	 Shape vlShape = vlAllocator.getShape();
	 vector<Suit> allowedSuit;
	 for (auto& suit : suitList) {
	    if (vlShape.get(pos, suit) > 0 && vlAllocator.isAvailable(rank, suit)) {
	       allowedSuit.push_back(suit);
	    }
	 }
	 vector<vector<Suit>> highCardAllocs = select(vlAllocator.getVal().get(pos, rank), allowedSuit);
	 if (!highCardAllocs.empty()) {
	    for (auto& allocSuits : highCardAllocs) {
	       Shape newShape = vlShape;
	       for (auto& suit : allocSuits) {
		  newShape.forceSet(pos, suit, newShape.get(pos, suit) - 1);
	       }
	       Value newValue = vlAllocator.getVal();
	       newValue.forceSet(pos, rank, 0);
	       allocType newAlloc = vlAllocator.getAlloc();
	       newAlloc[pos][rank] = allocSuits;
	       ValueAllocator newValueAllocator(newValue, newShape, newAlloc);
	       todo.push(newValueAllocator);
	    }
	 }
	 } catch (exception& ex){
	    cout << "Error 2;" << ex.what() << endl;
	 }
      }
   }
   return results;
}

ostream& operator<<(ostream& out, ValueAllocator vlAlloc) {
   out << "This ValueAllocator currently has Shape:" << endl;
   out << vlAlloc.reqShape;
   out << "This ValueAllocator currently has Value:" << endl;
   out << vlAlloc.reqVal;
   out << "This ValueAllocator so far has made these assignments:" << endl;
   printAlloc(vlAlloc.alloc, out);
   return out;
}

void printAlloc(allocType alloc, ostream& out) {
   for (auto& [pos, rankAndSuits] : alloc) {
      out << pos << ": ";
      for (auto& [rank, suits] : rankAndSuits) {
	 out << rank << ": ";
	 for (auto& suit : suits) {
	    out << suit << ", ";
	 }
	 out << "|  ";
      }
      out << endl;
   }
}
