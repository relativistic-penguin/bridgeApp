#include <fstream>
#include <string>
#include "utils.h"
#include "Card.h"
#include "Hand.h"
#include "Board.h"

using namespace std;

void test() {
   cout << "---------- Commencing tests ----------" << endl;
   try {
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
   } catch (...) {
      cout << "Something wrong" <<endl;
   }
   cout << "---------- Tests completed ----------" << endl;
}

int main(void) {
   test();
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
   return 0;
}

