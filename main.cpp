#include <fstream>
#include <string>
#include <stack>
#include <tuple>
#include <set>
#include "utils.h"
#include "Card.h"
#include "Hand.h"
#include "Board.h"
#include "Shape.h"
#include "Value.h"

using namespace std;

vector<Shape> nonSpecificShapeFilter(const vector<pair<Position, int>>& filters, Shape& shape);
vector<Shape> shapePopulate(const Shape& shape);
vector<Shape> posPopulate(const vector<Shape>& shapes, Position pos);

void basicDealTest() {
   cout << "---------- Commencing tests ----------" << endl;
   cout << "--Card, Hand, Board Generation Test---" << endl;
   Card egCard {Suit::C, Rank::N2};
   Card egCard2 {Suit::H, Rank::N2};
   Card egCard3 {Suit::C, Rank::N3};
   set<Card> cards {egCard, egCard3};
   Hand egHand(cards);
   Board egBoard;
   cout << egCard << endl;
   cout << egCard << endl;
   cout << (egCard == egCard2) << endl;
   cout << egHand << endl;
   cout << egBoard << endl;
   cout << "------------ Test complete -----------" << endl;
   cout << "------------ File I/O Test -----------" << endl;
   ofstream of;
   string filename = "output.txt";
   of.open(filename);
   int num;
   cout << "Please key in the number of boards you wish to deal: " << endl;
   cin >> num;
   for (int i = 0; i < num; i++) {
      Board newBoard;
      of << newBoard;
   }
   of.close();
   cout << "Required boards saved to " << filename << endl; 
   cout << "------------ Test complete -----------" << endl;
}

void advDealTest() {
   Shape egShape;
   cout << egShape;
   egShape.set(Position::E, Suit::H, 7, true);
   egShape.set(Position::E, Suit::S, 5, true);
   egShape.set(Position::W, Suit::H, 3, true);
   cout << egShape;
   egShape.inc(Position::W, Suit::S);
   cout << egShape;
   cout << "----------- End of Shape Test ---------" << endl;

   Shape egShape2;
   egShape2.set(Position::E, Suit::C, 13, true);
   egShape2.set(Position::S, Suit::D, 13, true);
   cout << egShape2 << egShape2.complete() << endl;
   vector<pair<Position, int>> filters2;
   filters2.push_back(make_pair(Position::W, 3));
//   filters2.push_back(make_pair(Position::W, 5));
//   filters2.push_back(make_pair(Position::N, 1));
   vector<Shape> filteredShapes = nonSpecificShapeFilter(filters2, egShape2);
   for (int i = 0; i < filteredShapes.size(); i++) {
      cout << "----------Unpopulated Shape No. " << i + 1 << endl;
      cout << filteredShapes[i];
      vector<Shape> populatedShapes = shapePopulate(filteredShapes[i]);
      cout << "Number of populated shapes " << populatedShapes.size() << endl;
      for (int j = 0; j < populatedShapes.size(); j++) {
	 cout << "Populated Shape No. " << j + 1 << endl;
	 cout << populatedShapes[j];
      }
   }


   cout << "----------- End of Shape Population Test ---------" << endl;

   Value egValue;
   cout << egValue;
   egValue.set(Position::E, Rank::A, 2);
   cout << egValue;
   egValue.set(Position::E, Rank::Q, 2);
   egValue.set(Position::E, Rank::A, 3);
   cout << egValue;
}

vector<Shape> nonSpecificShapeFilter(const vector<pair<Position, int>>& filters, Shape& shape) {
   stack<pair<Shape, vector<pair<Position, int>>>> todo;
   vector<Shape> results;
   set<pair<Shape, vector<pair<Position, int>>>> checked;
   if (shape.complete())
      if (filters.empty())
	 results.push_back(shape);
      else
	 throw invalid_argument("Unable to fulfuil requirement");
   else
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

// Implementation not elegant. Maybe should use recursion.

vector<Shape> shapePopulate(const Shape& shape) {
   //cout << "Attempting to populate shape\n" << shape;
   vector<Shape> results = {shape};
   for (auto& pos : posList) {
      //cout << "Populating position " << pos << endl;
      //cout << "From Shape " << results.front();
      results = posPopulate(results, pos);
   }
   return results;
}
vector<Shape> posPopulate(const vector<Shape>& shapes, Position pos) {
   vector<Shape> results;
   for (auto& shape : shapes) {
      int posVac = shape.getPosVac(pos);
      const Suit C = Suit::C;
      const Suit D = Suit::D;
      const Suit H = Suit::H;
      const Suit S = Suit::S;
      map<Suit, int> sVal = {{C, 0}, {D, 0}, {H, 0}, {S, 0}};
      //cout << "C Vac " << shape.getSuitVac(C) << endl;
      //cout << "D Vac " << shape.getSuitVac(D) << endl;
      //cout << "H Vac " << shape.getSuitVac(H) << endl;
      //cout << "S Vac " << shape.getSuitVac(S) << endl;
      for (sVal[C] = 0; sVal[C] <= shape.getSuitVac(C); sVal[C]++) {
	 if (sVal.at(C) > CARDS)
	    break;
	 bool cFixed = false;
	 if (shape.checkFix(pos, C)) {
	    cFixed = true;
	    sVal[C] = shape.get(pos, C);
	    //cout << "C fixed\n";
	 }
	 for (sVal[D] = 0; sVal[D] <= shape.getSuitVac(D); sVal[D]++) {
	    if (sVal.at(C) + sVal.at(D) > CARDS)
	       break;
	    bool dFixed = false;
	    if (shape.checkFix(pos, D)) {
	       dFixed = true;
	       sVal[D] = shape.get(pos, D);
	       //cout << "D fixed\n";
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
		     //cout << "S fixed\n";
		     }
		  int sum = sVal.at(C) + sVal.at(D) + sVal.at(H) + sVal.at(S); 
		  //cout << "C = " << sVal[C] << endl;
		  //cout << "D = " << sVal[D] << endl;
		  //cout << "H = " << sVal[H] << endl;
		  //cout << "S = " << sVal[S] << endl;
		  if (sum > CARDS) {
		     //cout << "Too large. Break\n";
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
		     //cout << "New result: " << newShape;
		  }
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

int main(void) {
   try {
      advDealTest();
   } catch (exception& ex) {
      cout << "Run time exception occurred.\n";
      cout << ex.what();
   }
   return 0;
}

