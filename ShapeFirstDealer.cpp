#include <stack>
#include <random>
#include <algorithm>
#include <set>
#include <map>
#include <chrono>
#include "ShapeFirstDealer.h"
#include "ValueAllocator.h"
#include "Hand.h"

using namespace std;

typedef map<Position, map<Suit, int>> specificShapeType;
typedef vector<pair<Position, int>>   nonSpecificShapeType;
typedef map<Position, pair<int, int>> hcpType;
typedef map<Position, map<Rank, vector<Suit>>> allocType;

static map<Suit, vector<Card>> numDeckConstructor() {
   map<Suit, vector<Card>> results;
   for (auto& suit : suitList) {
      vector<Card> suitDeck;
      for (auto& rank : numRankList) {
	 Card newCard {suit, rank};
	 suitDeck.push_back(newCard);
      }
      results.insert(make_pair(suit, suitDeck));
   }
   return results;
}
map<Suit, vector<Card>> numDeck = numDeckConstructor();

ShapeFirstDealer::ShapeFirstDealer(specificShapeType spShapeRules, nonSpecificShapeType nonSpShapeRules, hcpType valueRules) : BaseDealer(spShapeRules){
   nonSpecificShapeRules    = nonSpShapeRules;
   hcpRules                 = valueRules;
   ready = false;
   getReady();
}

Board ShapeFirstDealer::dealFromAlloc(allocType alloc, Shape shape) {
   map<Position, set<Card>> newConfig;
   for (auto& pos : posList) {
      set<Card> newSet;
      for (auto& rank : rankList) {
	 for (auto& suit : alloc.at(pos).at(rank)) {
	    Card newCard {suit, rank};
	    newSet.insert(newCard);
	    shape.forceSet(pos, suit, shape.get(pos,suit) - 1);
	 }
      }
      newConfig.insert(make_pair(pos, newSet));
   }
   map<Suit, int> dealCounter;
   for (auto& suit : suitList) {
      shuffleCards(numDeck.at(suit));
      dealCounter.insert(make_pair(suit, 0));
   }
   for (auto& pos : posList) {
      for (auto& suit : suitList) {
	 int requiredNum = shape.get(pos, suit) + dealCounter.at(suit);
	 for (; dealCounter.at(suit) < requiredNum; dealCounter.at(suit)++) {
	    newConfig.at(pos).insert(numDeck.at(suit).at(dealCounter.at(suit)));
	 }
      }
   }
   map<Position, Hand> newHandConfig;
   for (auto& [pos, cardSet] : newConfig) {
      Hand newHand(cardSet);
      newHandConfig.insert(make_pair(pos, newHand));
   }
   Board newBoard(newHandConfig);
   return newBoard;
}

Board ShapeFirstDealer::deal() {
   Board egBoard;
   shuffleCards(possibleValues);
   shuffleCards(unpopulatedShapes);
   for (auto& chosenValue : possibleValues) {
      for (auto& chosenUnpopShape : unpopulatedShapes) {
	 vector<Shape>& chosenShapeRange = possibleShapes.at(chosenUnpopShape);
	 shuffleCards(chosenShapeRange);
	 for (auto& chosenShape : chosenShapeRange) {
	    ValueAllocator valAlloc(chosenValue, chosenShape);
	    auto possibleAllocations = valAlloc.getAllocation(true);
	    if (possibleAllocations.size()) {
	       shuffleCards(possibleAllocations);
	       Board newBoard = dealFromAlloc(possibleAllocations.front(), valAlloc.getShape());
	       return newBoard;
	    }
	 }
      }
   }
   cout << "Warning: Not Possible" << endl;
   return egBoard;
}

void ShapeFirstDealer::setSpecificShapeRules(specificShapeType rule) {
   for (auto& [pos, suitRule] : rule) {
      for (auto& [suit, val] : suitRule) {
	 initShape.set(pos, suit, val, true);
      }
   }
   ready = false;
}

void ShapeFirstDealer::setNonSpecificShapeRules(nonSpecificShapeType rule) {
   nonSpecificShapeRules = rule;
   ready = false;
}

void ShapeFirstDealer::setHCPrules(hcpType rule) {
   hcpRules = rule;
   ready = false;
}

void ShapeFirstDealer::getReady() {
   cout << "Getting ready!" << endl;
   if (!ready) {
      cout << "Not ready yet. now getting ready..." << endl;
      cout << "Init shape is" << endl;
      cout << initShape;
      unpopulatedShapes = nonSpecificShapeFilter(nonSpecificShapeRules, initShape);
      cout << "Non specific shape rules are: " << endl;
      for (auto& [pos, val] : nonSpecificShapeRules) {
	 cout << pos << ": " << val << endl;
      }
      cout << "==> unpopulated shape is: " << endl;
      for (auto& shape : unpopulatedShapes)
	 cout << shape;
      int count = 0;
      for (auto unpopulatedShape : unpopulatedShapes) {
	 //cout << "Run No. " << ++count << endl;
	 vector<Shape> populatedShapes = shapePopulate(unpopulatedShape);
	 //cout << "Populated shapes: " << endl;
	 //for (auto populatedShape : populatedShapes) {
	 //   cout << populatedShape;
	 //}
	 cout << "Finished looking for populated shapes for this unpopulated shape." << endl;
	 possibleShapes.insert( pair<Shape, vector<Shape>>(unpopulatedShape, populatedShapes) );
	 //cout << "inserted" << endl;
      }
      cout << "I am over the loop " << endl;
      Value initValue;
      possibleValues = hcpFilter(hcpRules, initValue);
      ready = true;
   }
   cout << "Now ready!" << endl;
}

void ShapeFirstDealer::test(ostream& out, bool showShape, bool showValue) {
   if (!ready)
      getReady();
   if (showShape) {
      int unPopShapeCounter = 0;
      int popedShapeCounter = 0;
      for (auto& [unpopShape, popedShapes] : possibleShapes) {
	 out << "Unpopulated Shape No. " << ++unPopShapeCounter << endl;
	 out << unpopShape;
	 int localPopedShapeCounter = 0;
	 for (auto& popedShape : popedShapes) {
	    out << "Populated Shape No. " << ++localPopedShapeCounter << " out of " << ++popedShapeCounter << endl;
	    out << popedShape;
	 }
      }
   }
   if (showValue) {
      int valueCounter = 0;
      for (auto& value : possibleValues) {
	 out << "Possible Value No. " << ++valueCounter << endl;
	 out << value;
      }
   }
   out << "INFO: Dealing Starts" << endl;
   auto startTime = chrono::high_resolution_clock::now();
   Board egBoard = deal();
   /*
   cout << "INFO: We have " << unpopulatedShapes.size() << " unpopulated Shapes" << endl;
   cout << unpopulatedShapes.front();
   cout << possibleShapes.at(unpopulatedShapes.front()).front() << endl;
   Value targetVal;
   targetVal.set(Position::E, Rank::A, 1, true);
   targetVal.set(Position::E, Rank::K, 1, true);
   targetVal.set(Position::E, Rank::Q, 1, true);
   targetVal.set(Position::E, Rank::J, 1, true);

   targetVal.set(Position::S, Rank::A, 1, true);
   targetVal.set(Position::S, Rank::K, 1, true);
   targetVal.set(Position::S, Rank::Q, 1, true);
   targetVal.set(Position::S, Rank::J, 1, true);

   targetVal.set(Position::W, Rank::A, 1, true);
   targetVal.set(Position::W, Rank::K, 1, true);
   targetVal.set(Position::W, Rank::Q, 1, true);
   targetVal.set(Position::W, Rank::J, 1, true);

   targetVal.set(Position::N, Rank::A, 1, true);
   targetVal.set(Position::N, Rank::K, 1, true);
   targetVal.set(Position::N, Rank::Q, 1, true);
   targetVal.set(Position::N, Rank::J, 1, true);
 
   int i = 0;
   bool found = false;
   for (const auto& val : possibleValues) {
      if (targetVal.isEqual(val)) {
	 cout << "Found target: " << endl;
	 cout << val;
	 cout << "At No. " << ++i;
	 found = true;
	 break;
      } else
	 i++;
   }
   if (!found)
      cout << "WARNING: NOT FOUND" << endl;
   out << "I am here" << endl;
   Shape chosenShape;
   chosenShape.set(Position::E, Suit::C, 13);
   chosenShape.set(Position::S, Suit::D, 13);
   chosenShape.set(Position::W, Suit::H, 13);
   chosenShape.set(Position::N, Suit::S, 13);
   out << "Chosen Shape is " << chosenShape;
   out << "Chosen Value is " << targetVal;
   ValueAllocator valAlloc(targetVal, chosenShape);
   Board egBoard;
   auto possibleAllocations = valAlloc.getAllocation(true);
   if (possibleAllocations.size()) {
      shuffleCards(possibleAllocations);
      egBoard = dealFromAlloc(possibleAllocations.front(), valAlloc.getShape());
   } else {
      out << "WARNING: No valid allocation found. " << endl;
   }
   */
   auto endTime = chrono::high_resolution_clock::now();
   chrono::duration<double> elapsedTime = endTime - startTime;
   out << "-------Dealing took " << elapsedTime.count() << "s\n";
   out << egBoard;
}

// Implementation not elegant. Maybe should use recursion.
vector<Shape> ShapeFirstDealer::shapePopulate(const Shape& shape) {
//   cout << "Attempting to populate shape\n" << shape;
   vector<Shape> results = {shape};
   for (auto& pos : posList) {
      cout << "Populating position " << pos << endl;
      auto startTime = chrono::high_resolution_clock::now();
      results = posPopulate(results, pos);
      auto endTime = chrono::high_resolution_clock::now();
      chrono::duration<double> elapsedTime = endTime - startTime;
      cout << "Results now have " << results.size() << "Shapes\n";
      cout << "This step took " << elapsedTime.count() << "s\n";
   }
   return results;
}
vector<Shape> ShapeFirstDealer::posPopulate(const vector<Shape>& shapes, Position pos) {
   vector<Shape> results;
   for (auto& shape : shapes) {
      int posVac = shape.getPosVac(pos);
      const Suit C = Suit::C;
      const Suit D = Suit::D;
      const Suit H = Suit::H;
      const Suit S = Suit::S;
      map<Suit, int> sVal = {{C, 0}, {D, 0}, {H, 0}, {S, 0}};
//      cout << "C Vac " << shape.getSuitVac(C) << endl;
//      cout << "D Vac " << shape.getSuitVac(D) << endl;
//      cout << "H Vac " << shape.getSuitVac(H) << endl;
//      cout << "S Vac " << shape.getSuitVac(S) << endl;
      for (sVal[C] = 0; sVal[C] <= shape.getSuitVac(C); sVal[C]++) {
	 if (sVal.at(C) > CARDS)
	    break;
	 bool cFixed = false;
	 if (shape.checkFix(pos, C)) {
	    cFixed = true;
	    sVal[C] = shape.get(pos, C);
//	    cout << "C fixed\n";
	 }
	 for (sVal[D] = 0; sVal[D] <= shape.getSuitVac(D); sVal[D]++) {
	    if (sVal.at(C) + sVal.at(D) > CARDS)
	       break;
	    bool dFixed = false;
	    if (shape.checkFix(pos, D)) {
	       dFixed = true;
	       sVal[D] = shape.get(pos, D);
//	       cout << "D fixed\n";
	    }
	    for (sVal[H] = 0; sVal[H] <= shape.getSuitVac(H); sVal[H]++) {
	       if (sVal.at(C) + sVal.at(D) + sVal.at(H) > CARDS)
		  break;
	       bool hFixed = false;
	       if (shape.checkFix(pos, H)) {
		  hFixed = true;
		  sVal[H] = shape.get(pos, H);
		  //cout << "H fixed\n";
	       }
	       for (sVal[S] = 0; sVal[S] <= shape.getSuitVac(S); sVal[S]++) {
		  bool sFixed = false;
		  if (shape.checkFix(pos, S)) {
		     sFixed = true;
		     sVal[S] = shape.get(pos, S);
//		     cout << "S fixed\n";
		     }
		  int sum = sVal.at(C) + sVal.at(D) + sVal.at(H) + sVal.at(S); 
//		  cout << "C = " << sVal[C] << endl;
//		  cout << "D = " << sVal[D] << endl;
//		  cout << "H = " << sVal[H] << endl;
//		  cout << "S = " << sVal[S] << endl;
		  if (sum > CARDS) {
//		     cout << "Too large. Break\n";
		     break;
		  }
		  else if (sum == CARDS) {
		     Shape newShape = shape;
		     if (!cFixed)
			newShape.set(pos, C, sVal.at(C));
		     if (!dFixed)
			newShape.set(pos, D, sVal.at(D));
		     if (!hFixed)
			newShape.set(pos, H, sVal.at(H));
		     if (!sFixed)
			newShape.set(pos, S, sVal.at(S));
		     results.push_back(newShape);
//		     cout << "New result: " << newShape;
		  }
//		  else {
//		     cout << "Too small. Next\n";
//		  }
		  if (sFixed)
		     break;
	       }
	       if (hFixed)
		  break;
	    }
	    if (dFixed)
	       break;
	 }
	 if (cFixed)
	    break; 
      }
   }
   return results;
}


