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
#include "Dealer.h"

using namespace std;

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
/*
   Shape egShape2;
   egShape2.set(Position::E, Suit::C, 13, true);
   egShape2.set(Position::S, Suit::D, 13, true);
   cout << egShape2 << egShape2.complete() << endl;
   vector<pair<Position, int>> filters2;
//   filters2.push_back(make_pair(Position::W, 3));
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
*/
   Value egValue;
   cout << egValue;
   egValue.set(Position::E, Rank::A, 2);
   cout << egValue;
   egValue.set(Position::E, Rank::Q, 2);
   egValue.set(Position::E, Rank::A, 3);
   cout << egValue;
   cout << "----------- End of basic Value test --------------" << endl;
  
   map<Suit, int> egEfilter = { {Suit::C, 13} }, egSfilter = { {Suit::D, 13} }; 
//   map<Position, map<Suit, int>> egSpecificShapeFilter = { {Position::E, egEfilter}, {Position::S, egSfilter} };
   map<Position, map<Suit, int>> egSpecificShapeFilter = { {Position::E, egEfilter} };
   vector<pair<Position, int>>   egNonSpecificShapeFilter = { {Position::W, 10} };
   map<Position, pair<int, int>> egHCPfilter = { {Position::E, {20, 40}}, {Position::S, {18, 40}} };
   Dealer egDealer(egSpecificShapeFilter, egNonSpecificShapeFilter, egHCPfilter );
   egDealer.test(cout, true, false);

/*
   Value egValue2;
   egValue2.set(Position::E, Rank::A, 4, true);
   egValue2.set(Position::S, Rank::K, 4, true);
   map<Position, pair<int, int>> egValFilter = {{Position::W, {12, 40}}};
   vector<Value> resultValues = hcpFilter(egValFilter, egValue2);
   for (int i = 0; i < resultValues.size(); i++) {
      cout << "Value Result No. " << i + 1 << endl;
      cout << resultValues[i];
   }
*/
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

