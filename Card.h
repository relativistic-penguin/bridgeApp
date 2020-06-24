#ifndef CARD_H
#define CARD_H

#include <iostream>
#include "utils.h"

// using namespace std;

struct Card {
   Suit suit;
   Rank rank;
   bool operator== (const Card& otherCard) const;
   bool operator!= (const Card& otherCard) const;
   bool operator< (const Card& otherCard) const;
   bool operator<= (const Card& otherCard) const;
   bool operator> (const Card& otherCard) const;
   bool operator>= (const Card& otherCard) const;

};

Card cardSelector(int i);

std::ostream& operator<< (std::ostream& out, Card card);

#endif
