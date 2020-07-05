#include "DealingMachine.h"
#include "BasicDealer.h"

using namespace std;
typedef map<Position, map<Suit, int>> ssrType;
typedef vector<pair<Position, int>>   nssrType;
typedef map<Position, pair<int, int>> hcprType;
 
DealingMachine::DealingMachine(istream& in, ostream& out) : input {in}, output {out} {
   dealer = new BasicDealer();
}

void DealingMachine::parser() {
   //Listen to user input
   //
   //If user gives instruction to deal, call deal()
   //
   //If user gives new specification, parse specs into rules and pass to strategyChooser()
   //
   //If user gives END, finish the function and exit
}

void DealingMachine::deal() {
   //Call dealer.deal() and pass output to output stream
}

void strategyChooser(ssrType ssr, nssrType nssr, hcprType hcpr) {
   //Choose VFD or SFD or BD to be instantiated as the dealer based on required rules.
}

DealingMachine::~DealingMachine() {
   delete dealer;
}
