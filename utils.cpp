#include "utils.h"

Position operator+ (Position pos, int n) {
   return static_cast<Position> ((static_cast<int>(pos) + n) % 4);
}

std::ostream& operator<< (std::ostream& out, Position pos) {
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

std::ostream& operator<< (std::ostream& out, Suit suit) {
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

std::ostream& operator<< (std::ostream& out, Rank rank) {
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

