#include <fstream>
#include <string>
#include <stack>
#include <tuple>
#include <set>
#include <chrono>
#include "utils.h"
#include "Card.h"
#include "Hand.h"
#include "Board.h"
#include "Shape.h"
#include "Value.h"
#include "Dealer.h"
#include "ValueAllocator.h"
#include "ValueConfig.h"
#include "AdvDealer.h"

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
   cout << "----------- End of Value Population test ----------" << endl;
}

void newAdvDealerTest1(ostream& out) {
   map<Suit, int> egEfilter = { {Suit::C, 13} }, egSfilter = { {Suit::D, 13} }, egWfilter = { {Suit::H, 13} }, egNfilter = { {Suit::S, 13} }; 
   map<Position, map<Suit, int>> egSpecificShapeFilter	    = { {Position::E, egEfilter}, {Position::S, egSfilter}, {Position::W, egWfilter}, {Position::N, egNfilter} };
   vector<pair<Position, int>>	 egNonSpecificShapeFilter;
   map<Position, pair<int, int>> egHcpFilter		    = { {Position::E, {10, 10}}, {Position::S, {10, 10}}, {Position::W, {10, 10}}, {Position::N, {10, 10}} };
   AdvDealer egAdvDealer(egSpecificShapeFilter, egNonSpecificShapeFilter, egHcpFilter);
   auto startTime = chrono::high_resolution_clock::now();
   Board egBoard = egAdvDealer.deal();
   auto endTime = chrono::high_resolution_clock::now();
   cout << egBoard;
   chrono::duration<double> elapsedTime = endTime - startTime;
   out << "Dealing used: " << elapsedTime.count() << " s\n";
}

void newAdvDealerTest2(ostream& out) {
   map<Suit, int> egEfilter = { {Suit::C, 13} }, egSfilter = { {Suit::D, 13} }, egWfilter = { {Suit::H, 13} }, egNfilter = { {Suit::S, 13} }; 
   map<Position, map<Suit, int>> egSpecificShapeFilter;
   vector<pair<Position, int>>	 egNonSpecificShapeFilter;
   map<Position, pair<int, int>> egHcpFilter;
   AdvDealer egAdvDealer(egSpecificShapeFilter, egNonSpecificShapeFilter, egHcpFilter);
   auto startTime = chrono::high_resolution_clock::now();
   Board egBoard = egAdvDealer.deal();
   auto endTime = chrono::high_resolution_clock::now();
   cout << egBoard;
   chrono::duration<double> elapsedTime = endTime - startTime;
   out << "Dealing used: " << elapsedTime.count() << " s\n";
}

void valueAllocationTest1(ostream& out) {
   Value egValue;
   Shape egShape;
   egShape.set(Position::E, Suit::C, 13, true);
   egShape.set(Position::S, Suit::D, 13, true);
   egShape.set(Position::W, Suit::H, 13, true);
   egShape.set(Position::N, Suit::S, 13, true);
   out << "Using shape: " << endl;
   out << egShape;
   egValue.set(Position::E, Rank::A, 1, true);
   egValue.set(Position::E, Rank::K, 1, true);
   egValue.set(Position::E, Rank::Q, 1, true);
   egValue.set(Position::E, Rank::J, 1, true);

   egValue.set(Position::S, Rank::A, 1, true);
   egValue.set(Position::S, Rank::K, 1, true);
   egValue.set(Position::S, Rank::Q, 1, true);
   egValue.set(Position::S, Rank::J, 1, true);

   egValue.set(Position::W, Rank::A, 1, true);
   egValue.set(Position::W, Rank::K, 1, true);
   egValue.set(Position::W, Rank::Q, 1, true);
   egValue.set(Position::W, Rank::J, 1, true);

   egValue.set(Position::N, Rank::A, 1, true);
   egValue.set(Position::N, Rank::K, 1, true);
   egValue.set(Position::N, Rank::Q, 1, true);
   egValue.set(Position::N, Rank::J, 1, true);
   out << "Using value: " << endl;
   out << egValue;

   ValueAllocator egValueAllocator(egValue, egShape);
   typedef map<Position, map<Rank, vector<Suit>>> allocType;
   vector<allocType> allocations = egValueAllocator.getAllocation(true);
   for (int i = 0; i < allocations.size(); i++) {
      out << "=========== Allocation No. " << i << endl;
   printAlloc(allocations[i], out);
   }
}

void valueAllocationTest2(ostream& out) {
   Value egValue;
   Shape egShape;
   egShape.set(Position::E, Suit::C, 13, true);
   egShape.set(Position::S, Suit::D, 13, true);
   egShape.set(Position::W, Suit::H, 10, true);
   egShape.set(Position::W, Suit::S, 3, true);
   egShape.set(Position::N, Suit::S, 10, true);
   egShape.set(Position::N, Suit::H, 3, true);
   out << "Using shape: " << endl;
   out << egShape;
   egValue.set(Position::E, Rank::A, 1, true);
   egValue.set(Position::E, Rank::K, 1, true);
   egValue.set(Position::E, Rank::Q, 1, true);
   egValue.set(Position::E, Rank::J, 1, true);

   egValue.set(Position::S, Rank::A, 1, true);
   egValue.set(Position::S, Rank::K, 1, true);
   egValue.set(Position::S, Rank::Q, 1, true);
   egValue.set(Position::S, Rank::J, 1, true);

   egValue.set(Position::W, Rank::A, 2, true);
   egValue.set(Position::W, Rank::K, 1, true);
   egValue.set(Position::W, Rank::Q, 1, true);
   egValue.set(Position::W, Rank::J, 1, true);

   egValue.set(Position::N, Rank::A, 0, true);
   egValue.set(Position::N, Rank::K, 1, true);
   egValue.set(Position::N, Rank::Q, 1, true);
   egValue.set(Position::N, Rank::J, 1, true);
   out << "Using value: " << endl;
   out << egValue;

   ValueAllocator egValueAllocator(egValue, egShape);
   typedef map<Position, map<Rank, vector<Suit>>> allocType;
   vector<allocType> allocations = egValueAllocator.getAllocation(true);
   for (int i = 0; i < allocations.size(); i++) {
      out << "=========== Allocation No. " << i << endl;
   printAlloc(allocations[i], out);
   }
}

void dealerTest(ostream& out) {
   map<Suit, int> egEfilter = { {Suit::C, 13} }, egSfilter = { {Suit::D, 13} }, egWfilter ={ {Suit::H, 13} }, egNfilter = { {Suit::S, 13} }; 
   map<Position, map<Suit, int>> egSpecificShapeFilter = { {Position::E, egEfilter},  {Position::S, egSfilter}, {Position::W, egWfilter}, {Position::N, egNfilter} };
   vector<pair<Position, int>>   egNonSpecificShapeFilter;
   map<Position, pair<int, int>> egHCPfilter;
   Dealer egDealer(egSpecificShapeFilter, egNonSpecificShapeFilter, egHCPfilter );
   egDealer.test(out, true, false);
  
}

void dealerTest2(ostream& out) {
   //map<Suit, int> egEfilter = { {Suit::C, 13} }, egSfilter = { {Suit::D, 13} }, egWfilter ={ {Suit::H, 13} }, egNfilter = { {Suit::S, 13} }; 
   map<Position, map<Suit, int>> egSpecificShapeFilter;
   vector<pair<Position, int>>   egNonSpecificShapeFilter = { {Position::E, 4},  };
   map<Position, pair<int, int>> egHCPfilter = { {Position::E, {37, 37}} };
   Dealer egDealer(egSpecificShapeFilter, egNonSpecificShapeFilter, egHCPfilter );
   egDealer.test(out, false, true);
  
}

void valueConfigTest(ostream& out) {
   ValueConfig egValCon;
   out << egValCon;
   out << "Set E K C" << endl;
   egValCon.set(Position::E, Rank::K, Suit::C);
   out << "CK is available: " << egValCon.cardAvailable(Suit::C, Rank::K) << endl;
   out << "Pos E value: " << egValCon.posVal(Position::E) << endl;
   out << "Suit C available rank: ";
   for (auto&  rank : egValCon.suitAvaiRank(Suit::C))
      out << rank << " ";
   out << endl;
   out << "Rank K avaialble suit: ";
   for (auto& suit : egValCon.rankAvaiSuit(Rank::K))
      out << suit << " ";
   out << endl;
   out << egValCon;
   out << "Unset E K C" << endl;
   egValCon.reset(Position::E, Rank::K, Suit::C);
   out << egValCon;
   out << "CK is available: " << egValCon.cardAvailable(Suit::C, Rank::K) << endl;
   out << "Complete: " << egValCon.complete() << endl;
   int i = 0;
   for (auto& suit : suitList) {
      for (auto& rank : rankList) {
	 if (++i <= CARDS) {
	    egValCon.set(Position::E, rank, suit);
	 }
	 else {
	    egValCon.set(Position::W, rank, suit);
	 }
      }
   }
   out << egValCon;
   out << "Complete: " << egValCon.complete() << endl;
   out << "reset S A\n";
   egValCon.reset(Rank::A, Suit::S);
   out << egValCon;
   try {
      egValCon.set(Position::N, Rank::K, Suit::H);
      out << "ERROR: exception not caught." << endl;
   } catch (invalid_argument& ex) {
      out << "Caught exception as expected. Message is " << ex.what() << endl;
      out << "Test successfully completed!" << endl;
   }
}

void valueConfigGenTest(ostream& out) {
   Value egValue;
   egValue.set(Position::E, Rank::A, 4, true);
   egValue.set(Position::E, Rank::K, 4, true);
   egValue.set(Position::E, Rank::Q, 4, true);
   egValue.set(Position::E, Rank::J, 1, true);
   
   egValue.set(Position::S, Rank::J, 3, true);

   out << egValue;

   vector<ValueConfig> possibleConfigs = egValue.compatibleConfig();
   int i = 0;
   for (auto& conf : possibleConfigs) {
      out << ++i << endl;
      out << conf;
   }
}

void valueConfigGenTest2(ostream& out) {
   Value egValue;
   egValue.set(Position::E, Rank::A, 1, true);
   egValue.set(Position::E, Rank::K, 1, true);
   egValue.set(Position::E, Rank::Q, 1, true);
   egValue.set(Position::E, Rank::J, 1, true);

   egValue.set(Position::S, Rank::A, 1, true);
   egValue.set(Position::S, Rank::K, 1, true);
   egValue.set(Position::S, Rank::Q, 1, true);
   egValue.set(Position::S, Rank::J, 1, true);

   egValue.set(Position::W, Rank::A, 1, true);
   egValue.set(Position::W, Rank::K, 1, true);
   egValue.set(Position::W, Rank::Q, 1, true);
   egValue.set(Position::W, Rank::J, 1, true);

   egValue.set(Position::N, Rank::A, 1, true);
   egValue.set(Position::N, Rank::K, 1, true);
   egValue.set(Position::N, Rank::Q, 1, true);
   egValue.set(Position::N, Rank::J, 1, true);
   out << egValue;

   auto startTime = chrono::high_resolution_clock::now();
   vector<ValueConfig> possibleConfigs = egValue.compatibleConfig();
   auto endTime = chrono::high_resolution_clock::now();
   chrono::duration<double> elapsedTime = endTime - startTime;
   cout << "Calculation took: " << elapsedTime.count() << "s\n";
   cout << "Found " << possibleConfigs.size() << " compatible configurations\n";
}


void hcpFilterDebug (ostream& out) {
   Dealer egDealer;
   //map<Position, pair<int, int>> egFilter = {{Position::E, {30, 40}}};
   map<Position, pair<int, int>> egFilter;
   Value egValue;
   vector<Value> egValues = egDealer.hcpFilter(egFilter, egValue);
   auto startTime = chrono::high_resolution_clock::now();
   shuffleCards(egValues);
   auto endTime = chrono::high_resolution_clock::now();
   chrono::duration<double> elapsedTime = endTime - startTime;
   out << "Value shuffling requires " << elapsedTime.count() << "s\n";
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
   for (const auto& val : egValues) {
      if (targetVal.isEqual(val)) {
	 cout << "Found target: " << endl;
	 cout << val;
	 cout << "At No. " << ++i;
	 break;
      } else
	 i++;
   }
   cout << "Done." << endl;
   //int counter = 0;
   //for (Value egValue : egValues) {
   //   out << "Possible Value: " << ++counter << "\n";
   //   out << egValue;
   //}
}



int main(void) {
   try {
      ofstream of("debug.txt");
      //auto coutbuf = cout.rdbuf(of.rdbuf());
      //advDealTest();
      //valueAllocationTest1(cout);
      //dealerTest2(cout);
      //hcpFilterDebug(cout);
      //valueConfigTest(cout);
      //valueConfigGenTest2(cout);
      newAdvDealerTest2(cout);
      of.close();
   } catch (exception& ex) {
      cout << "Run time exception occurred.\n";
      cout << ex.what();
   } catch (...) {
      cout << "Bad things happened.\n";
   }
   return 0;
}

