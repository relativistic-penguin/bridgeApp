#ifndef HAND_H
#define HAND_H

#include <iostream>
#include <set>
#include <map>
#include "Card.h"

using std::set, std::map, std::ostream, std::endl;

class Hand {
   private:
      set<Card>	           all;
      map<Suit, set<Card>> sortedHand {{Suit::C, {}}, {Suit::D, {}}, {Suit::H, {}}, {Suit::S, {}}};
      int		   length;
   public:
      Hand(set<Card>);
      set<Card>            get() {return all;}
      set<Card>            get(Suit suit) {return sortedHand.at(suit);}
      map<Suit, set<Card>> getSortedHand() {return sortedHand;}
      int                  size() {return all.size();}
      Hand&                play(Card); //TODO: implement play function
};

ostream& operator<< (std::ostream& out, Hand hand);

#endif
