#include "Hand.h"

Hand::Hand(set<Card> cards) {
   all = cards;

   for (auto& card : all) {
      sortedHand.at(card.suit).insert(card);
   }
}

ostream& operator<< (ostream& out, Hand hand) {
   out << "*****" << endl;
   for (auto& [suit, cards] : hand.getSortedHand()) {
      out << suit << ": ";
      for (auto& card : cards) {
	 out << card.rank << " ";
      }
      if (!cards.size())
	 out << "-";
      out << endl;
   }
   out << "*****" << endl;
   return out;
}

