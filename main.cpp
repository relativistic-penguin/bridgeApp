#include <iostream>
#include <set>
#include <map>
#include <vector>
#include <random>
#include <unordered_set>
#include <algorithm>
#include <fstream>

using namespace std;

enum class Suit {C, D, H ,S, NT};
enum class Rank {N2, N3, N4, N5, N6, N7, N8, N9, T, J, Q, K, A};

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
      out << static_cast<int>(rank) + 2;
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

Card cardSelector(int i) {
   Suit suit {i / 13};
   Rank rank {i % 13};
   Card card = {suit, rank};
   return card;
}

ostream& operator<< (ostream& out, Card card) {
   out << card.suit << card.rank;
   return out;
}

class Hand {
   //TODO: define copy constructor
   private:
      set<Card>            all;
      map<Suit, set<Card>> sortedHand {{Suit::C, {}}, {Suit::D, {}}, {Suit::H, {}}, {Suit::S, {}}};
      int       length;
   public:
      Hand(set<Card>);
      set<Card>            get() {return all;}
      set<Card>            get(Suit suit) {return sortedHand.at(suit);}
      map<Suit, set<Card>> getSortedHand() {return sortedHand;}
      int                  size() {return all.size();}
      Hand&                play(Card); //TODO: implement play function
};

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

enum class Position {E, S, W, N};

Position operator+ (Position pos, int n) {
   return static_cast<Position> ((static_cast<int>(pos) + n) % 4);
}

ostream& operator<< (ostream& out, Position pos) {
   switch (pos) {
      case Position::E:
	 out << "E";
	 break;
      case Position::S:
	 out << "S";
	 break;
      case Position::W:
	 out << "W";
	 break;
      case Position::N:
	 out << "N";
	 break;
   }
   return out;
}

class Board {
   private:
      map<Position, Hand> config;
   public:
      Board();
      Board(map<Position, Hand> config) {this->config = config;}
      map<Position, Hand> get() {return config;}
      Hand get(Position pos) {return config.at(pos);}
};

map<Position, Hand> boardBuilder(vector<int> seq, Position dealer) {
   if (seq.size() != 52)
      throw length_error("attempting to build board from incorrect number of cards");
   map<Position, set<Card>> board {{Position::E, {}}, {Position::S, {}}, {Position::W, {}}, {Position::N, {}}};
   unordered_set<int> dupChecker;
   Position player = dealer;
   for (auto num : seq) {
      if (dupChecker.find(num) != dupChecker.end()) {
	 string msg = "Attempting to deal card No. ";
	 msg.append(to_string(num)).append(" more than once.");
	 throw invalid_argument(msg);
      }
      else {
	 dupChecker.insert(num);
	 board.at(player).insert(cardSelector(num));
	 player = player + 1;
      }
   }
   map<Position, Hand> result;
   for (auto& [pos, cards] : board)
      result.insert(pair<Position, Hand>(pos, Hand(cards)));
   return result;
}

ostream& operator<< (ostream& out, Board board) {
   out << "----------" << endl;
   for (auto& [pos, Hand] : board.get())
      out << pos << ":" << endl << Hand << endl;
   out << "----------" << endl;
   return out;
}

Board::Board() {
   // Unconstrained random deal
   vector<int> seq;
   seq.reserve(54);
   for (int i = 0; i < 52; i++) {
      seq.push_back(i);
   }
   random_device rd;
   mt19937 rg(rd());
   shuffle(seq.begin(), seq.end(), rg);
   config = boardBuilder(seq, Position::E);
}

void test() {
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
      cout << "Something wrong";
   }
}

int main(void) {
   test();
   ofstream of;
   of.open("output.txt");
   int num;
   cin >> num;
   for (int i = 0; i < num; i++) {
      Board newBoard;
      of << newBoard;
   }
   of.close();
   return 0;
}

