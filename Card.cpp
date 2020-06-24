#include "Card.h"

bool Card::operator== (const Card& otherCard) const{
   return suit == otherCard.suit && rank == otherCard.rank;
}

bool Card::operator!= (const Card& otherCard) const{
   return suit != otherCard.suit || rank != otherCard.rank;
}

bool Card::operator< (const Card& otherCard) const{
   return suit < otherCard.suit || (suit == otherCard.suit && rank < otherCard.rank);
}

bool Card::operator<= (const Card& otherCard) const{
   return suit < otherCard.suit || (suit == otherCard.suit && rank <= otherCard.rank);
}

bool Card::operator> (const Card& otherCard) const{
   return suit > otherCard.suit || (suit == otherCard.suit && rank > otherCard.rank);
}

bool Card::operator>= (const Card& otherCard) const{
   return suit > otherCard.suit || (suit == otherCard.suit && rank >= otherCard.rank);
}

Card cardSelector(int i) {
   Suit suit {i / 13};
   Rank rank {i % 13};
   Card card = {suit, rank};
   return card;
}

std::ostream& operator<< (std::ostream& out, Card card) {
   out << card.suit << card.rank;
   return out;
}

