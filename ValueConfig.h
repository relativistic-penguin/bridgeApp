#ifndef VALUE_CONFIG_H
#define VALUE_CONFIG_H

#include <iostream>
#include <vector>
#include <map>
#include "utils.h"

class ValueConfig {
   typedef std::map<Position, std::map<Rank, std::map<Suit, bool>>> configType;
   private:
       configType valTable;
       void       checkValidity() const;
   public:
      ValueConfig();
      ValueConfig(const configType);
      bool		get(Position, Rank, Suit) const;
      void		set(Position, Rank, Suit);
      void		reset(Position, Rank, Suit);
      void		reset(Rank, Suit);
      bool		complete() const;
      bool		cardAvailable (Suit, Rank) const;
      int               posVal(Position) const;
      int               posSuitLength(Position, Suit) const;
      std::vector<Suit> rankAvaiSuit(Rank) const;
      std::vector<Rank> suitAvaiRank(Suit) const;
      bool	     	operator< (const ValueConfig) const;

};

std::ostream& operator<< (std::ostream&,ValueConfig); 

#endif
