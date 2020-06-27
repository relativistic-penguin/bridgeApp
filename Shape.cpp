#include "Shape.h"

Shape::Shape() {
   for (auto& suit : suitList) {
      map<Position, int> lis;
      for (auto& pos : posList)
	 lis.insert(pair<Position, int>(pos, 0));
      rep2.insert(pair<Suit, map<Position, int>>(suit, lis));

      suitVac.insert(pair<Suit, int>(suit, CARDS));
   }
   for (auto& pos : posList) {
      map<Suit, int> lis;
      map<Suit, bool> fixedList;
      for (auto& suit : suitList) {
	 lis.insert(pair<Suit, int>(suit, 0));
	 fixedList.insert(pair<Suit, bool>(suit, false));
      }
      rep1.insert(pair<Position, map<Suit, int>>(pos, lis));
      fixed.insert(pair<Position, map<Suit, bool>>(pos, fixedList));
      posVac.insert(pair<Position, int>(pos, CARDS));
   }
}

void Shape::set(Position pos, Suit suit, int val, bool fix) {
   if (fixed.at(pos).at(suit))
      throw std::invalid_argument("Attempting to change fixed shape value.");
   int newPosVac = posVac.at(pos) + rep1.at(pos).at(suit) - val;
   int newSuitVac = suitVac.at(suit) + rep1.at(pos).at(suit) - val;
   if (newPosVac < 0 || newSuitVac < 0)
      throw std::invalid_argument("Invalid suit length in conflict with existing setting");
   posVac.at(pos) = newPosVac;
   suitVac.at(suit) = newSuitVac;
   rep1.at(pos).at(suit) = val;
   rep2.at(suit).at(pos) = val;
   if (fix)
      fixed.at(pos).at(suit) = true;
}

bool Shape::complete() {
   if (completed) 
      return true;
   for (auto& pos : posList)
      if (posVac.at(pos) > 0) 
	 return false;
   completed = true;
   return true;
}

ostream& operator<< (ostream& out, Shape shape) {
   using std::endl;
   out << "   ";
   for (auto& suit : suitList) {
      out << suit << " ";
   }
   out << "| T\n";
   for (auto& pos : posList) {
      out << " " << pos << " ";
      for (auto& suit : suitList) {
	 out << shape.get(pos, suit) << " ";
      }
      out << "| " << CARDS - shape.getPosVac().at(pos) << endl;
   }
   out << "----------\n T ";
   for (auto& suit : suitList)
      out << CARDS - shape.getSuitVac().at(suit) << " ";
   out << endl;
   return out;
}

bool Shape::operator== (const Shape& other) const {
   for (auto& pos : posList)
      for (auto& suit : suitList)
	 if (rep1.at(pos).at(suit) != other.get(pos, suit) || fixed.at(pos).at(suit) != other.checkFix(pos, suit))
	    return false;
   return true;
}
