#include <iostream>
#include <set>
#include <map>

using namespace std;

enum Suit {C, D, H ,S, NT};
enum Rank {N2, N3, N4, N5, N6, N7, N8, N9, T, J, Q, K, A};

ostream& operator<< (ostream& out, Suit suit) {
   switch (suit) {
      case Suit::C:
	 out << 'C';
	 break;
      case Suit::D:
	 out << 'D';
	 break;
      case Suit::H:
	 out << 'H';
	 break;
      case Suit::S:
	 out << 'S';
	 break;
   }
   return out;
}

ostream& operator<< (ostream& out, Rank rank) {
   if (rank < Rank::T)
      out << rank + 2;
   else {
      switch (rank) {
	 case Rank::T:
	    out << 'T';
	    break;
	 case Rank::J:
	    out << 'J';
	    break;
	 case Rank::Q:
	    out << 'Q';
	    break;
	 case Rank::K:
	    out << 'K';
	    break;
	 case Rank::A:
	    out << 'A';
	    break;
      }
   }
   return out;
}


struct Card {
   Suit suit;
   Rank rank;
   bool operator== (const Card& otherCard) const{
      return suit == otherCard.suit && rank == otherCard.rank;
   }
   bool operator!= (const Card& otherCard) const{
      return suit != otherCard.suit || rank != otherCard.rank;
   }
   bool operator< (const Card& otherCard) const{
      return suit < otherCard.suit || (suit == otherCard.suit && rank < otherCard.rank);
   }
   bool operator<= (const Card& otherCard) const{
      return suit < otherCard.suit || (suit == otherCard.suit && rank <= otherCard.rank);
   }
   bool operator> (const Card& otherCard) const{
      return suit > otherCard.suit || (suit == otherCard.suit && rank > otherCard.rank);
   }
   bool operator>= (const Card& otherCard) const{
      return suit > otherCard.suit || (suit == otherCard.suit && rank >= otherCard.rank);
   }

};

ostream& operator<< (ostream& out, Card card) {
   out << card.suit << card.rank;
   return out;
}

class Hand {
   private:
      set<Card> all;
      map<Suit, set<Card>> sortedHand {{Suit::C, {}}, {Suit::D, {}}, {Suit::H, {}}, {Suit::S, {}}};
      int       length;
   public:
      Hand(set<Card>);
      set<Card> get() {return all;}
      set<Card> get(Suit suit) {return sortedHand.at(suit);}
      map<Suit, set<Card>> getSortedHand() {return sortedHand;}
      int       size() {return all.size();}
      Hand&     play(Card);
};

Hand::Hand(set<Card> cards) {
   all = cards;

   for (auto& card : all) {
      sortedHand.at(card.suit).insert(card);
   }
}

ostream& operator<< (ostream& out, Hand hand) {
   cout << "*****" << endl;
   for (auto& [suit, cards] : hand.getSortedHand()) {
      cout << suit << ": ";
      for (auto& card : cards) {
	 out << card.rank << " ";
      }
      if (!cards.size())
	 cout << "-";
      out << endl;
   }
   cout << "*****" << endl;
   return out;
}

int main(void) {
   Card egCard {Suit::C, Rank::N2};
   Card egCard2 {Suit::H, Rank::N2};
   Card egCard3 {Suit::C, Rank::N3};
   set<Card> cards {egCard, egCard3};
   Hand egHand (cards);
   cout << egCard << endl;
   cout << egCard << endl;
   cout << (egCard == egCard2) << endl;
   cout << egHand;
   return 0;
}

