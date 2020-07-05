#ifndef DEALING_MACHINE_H
#define DEALING_MACHINE_H

#include <iostream>
#include "BaseDealer.h"

class DealingMachine {
   typedef map<Position, map<Suit, int>> ssrType;
   typedef vector<pair<Position, int>>   nssrType;
   typedef map<Position, pair<int, int>> hcprType;
 
   private:
      BaseDealer*    dealer;
      std::istream&  input;
      std::ostream&  output;
      void	     strategyChooser(ssrType, nssrType, hcprType);
   public:
      DealingMachine(std::istream&, std::ostream&);
      ~DealingMachine();
      void parser(); // start listening for user command
      void deal();
};

#endif
