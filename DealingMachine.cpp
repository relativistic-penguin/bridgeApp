#include <string>
#include <algorithm>
#include "DealingMachine.h"
#include "BasicDealer.h"
#include "ShapeFirstDealer.h"
#include "ValueFirstDealer.h"
#include "utils.h"

using namespace std;
typedef map<Position, map<Suit, int>> ssrType;
typedef vector<pair<Position, int>>   nssrType;
typedef map<Position, pair<int, int>> hcprType;

static const int MAX_HCP = 37;

static vector<string> splitStr(string inStr, string delimiter) {
   int pos = 0;
   vector<string> tokens;
   while ((pos = inStr.find(delimiter)) != string::npos) {
      tokens.push_back(inStr.substr(0, pos));
      inStr.erase(0, pos + delimiter.length());
   }
   tokens.push_back(inStr);
   return tokens;
}

static bool isInteger(string inStr) {
   for (const char& c : inStr)
      if (!isdigit(c))
	 return false;
   return true;
}

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
   string command;
   output << "--------------------- Constrained Dealing Program ---------------------\n";
   output << "Usage Instruction: " << endl;
   output << "Three types of commands are available: Give constraints, deal, END" << endl;
   output << "To give constraints:\nFirst give the Position (E or S or W or N), followed by ',', and then a list of constraints." << endl;
   output << "Option 1: two integer separated by comma to indicate HCP range (inclusive). e.g. 10,20 means HCP 10-20 inclusive." << endl;
   output << "Option 2: four integers or x separated by '-' to indicate non suit specific shape. e.g. 4-5-x-x means 4 & 5 cards in two suits." << endl;
   output << "Option 3: four integers or x separated by '=' to indicate suit specific shape in the order of C D H S. ";
   output << "e.g. 4=x=4=x means 4 cards in C and 4 cards in S." << endl;
   output << "These 3 options may be combined in any order separated by comma. e.g. S,10,20,4=3=x=x,3-3-x-x means S must have 10 to 20 HCP, ";
   output << "4 cards in C, 3 cards in D, and 3-3 in other two suits." << endl;
   output << "Constraints for different positions are separated by ';' e.g. S,10,20,4=3=x,x,3-3-x-x; E, 20, 40 " << endl;
   output << "White spaces in the constraint expression are ignored. e.g. S,      10,     20 is equivalent to S,10,20" << endl;
   output << "To deal a Board with the given constraint:\n Enter 'deal'" << endl;
   output << "To END (terminate the program): \n Enter 'END'" << endl << endl;
   output << "Now default to an unconstrained dealer." << endl;
   output << "Awaiting user input\n";
   getline(input, command);
   command.erase(remove(command.begin(), command.end(), ' '), command.end());
   while (command != "END") {
      try {
	 if (command == "deal") {
	    output << "Start Dealing\n";
	    deal();
	    output << "Dealing successfully completed.\n";
	    output << "Awaiting user input\n";
	    getline(input, command);
	    command.erase(remove(command.begin(), command.end(), ' '), command.end());
	    continue;
	 }
	 else if (command == "RAND" || command.empty()) {
	    ssrType ssr;
	    nssrType nssr;
	    hcprType hcpr;
	    strategyChooser(ssr, nssr, hcpr);    
	    output << "New basic random dealer successfully created. Now ready to deal.\n";
	    output << "Awaiting user input\n";
	    getline(input, command);
	    command.erase(remove(command.begin(), command.end(), ' '), command.end());
	 }
	 else {      
	    ssrType ssr;
	    nssrType nssr;
	    hcprType hcpr;
	    vector<string> filters = splitStr(command, ";");
	    for (string filter : filters) {
	       vector<string> tokens = splitStr(filter, ",");
	       Position pos;
	       if (tokens.front() == "E")
		  pos = Position::E;
	       else if (tokens.front() == "S")
		  pos = Position::S;
	       else if (tokens.front() == "W")
		  pos = Position::W;
	       else if (tokens.front() == "N")
		  pos = Position::N;
	       else	  
		  throw invalid_argument("Unrecognised Position");

	       bool ssrSet = false, nssrSet = false;
	       int min = 0 - 1;
	       int max = MAX_HCP + 1; //TODO: Change hard coded maximum HCP value
	       for (int i = 1; i < tokens.size(); i++) {
		  if (min >= 0 && max > MAX_HCP && !isInteger(tokens[i])) //min is set but max is not
		     throw invalid_argument("HCP rule not complete. Min value given but not max value.");
		  if (isInteger(tokens[i])) {
		     int tempVal = stoi(tokens[i]);
		     if (min < 0) {
			if (tempVal >= 0)
			   min = tempVal;
			else
			   min = 0;
		     }
		     else if (max > MAX_HCP) {
			if (tempVal <= MAX_HCP)
			   max = tempVal;
			else
			   max = MAX_HCP;
			if (min > 0 || max < MAX_HCP)
			   hcpr.insert(make_pair(pos, make_pair(min, max)));
		     }
		     else
			throw invalid_argument("HCPRs set more than once.");
		  }
		  else if (tokens[i].find("-") != string::npos) {
		     if (nssrSet)
			throw invalid_argument("NSSRs set more than once.");
		     int cardCount = 0;
		     vector<string> items = splitStr(tokens[i], "-");
		     if (items.size() != 4)
			throw invalid_argument("Incorrectly formatted NSSRs given. Expect 4 values or x separated by -."); 
		     for (int j = 0; j < items.size(); j++) {
			if (isInteger(items[j])) {
			   int length = stoi(items[j]);
			   cardCount += length;
			   if (cardCount <= CARDS) 
			      nssr.push_back(make_pair(pos, length));
			   else {
			      output << "requires " << cardCount << "cards" << endl;
			      throw invalid_argument("Requires too many cards in a suit.");
			   }
			}
		     }
		     nssrSet = true;
		  }
		  else if (tokens[i].find("=") != string::npos) {
		     if (ssrSet)
			throw invalid_argument("SSRs set more than once.");
		     int cardCount = 0;
		     vector<string> items = splitStr(tokens[i], "=");
		     if (items.size() != 4)
			throw invalid_argument("Incorrectly formatted SSRs given. Expect 4 values or x separated by =.");
		     vector<Suit>   suitVec = suitList;
		     for (int j = 0; j < items.size(); j++) {
			if (isInteger(items[j])) {
			   int length = stoi(items[j]);
			   cardCount += length;
			   if (cardCount <= CARDS) {
			      if (ssr.find(pos) != ssr.end()) {
				 ssr.at(pos).insert(make_pair(suitVec[j], length));
			      } else {
				 map<Suit, int> suitMap = { {suitVec[j], length} };
				 ssr.insert(make_pair(pos, suitMap));
			      }
			   } 
			   else
			      throw invalid_argument("Requires too many cards in a suit.");
			}
		     }
		     ssrSet = true;
		  }
		  else
		     throw invalid_argument("Unknown input token.");
	       }
	    }
	    output << "User command successfully parsed and produced the following rules:\n";
	    output << "SSR:" << endl;
	    for (auto& [pos, suitMap] : ssr) {
	       output << pos << ": ";
	       for (auto& [suit, length] : suitMap) {
		  output << suit << " " << length << ", ";
	       }
	       output << endl;
	    }
	    output << "NSSR:" << endl;
	    for (auto& [pos, length] : nssr) {
	       output << pos << ": " << length << endl;
	    }
	    output << "HCPR:" << endl;
	    for (auto& [pos, interval] : hcpr) {
	       output << pos << ": " << interval.first << " " << interval.second << endl;
	    }
	    output << "Now initialising Dealer\n";
	    strategyChooser(ssr, nssr, hcpr);
	    output << "New dealer successfully created. Now ready to deal.\n";
	    output << "Awaiting user input\n";
	    getline(input, command);
	    command.erase(remove(command.begin(), command.end(), ' '), command.end());
	 }
      }
      catch (invalid_argument& ex) {
	 output << ex.what() << endl;
	 output << "Awaiting new user input\n";
	 getline(input, command);
	 command.erase(remove(command.begin(), command.end(), ' '), command.end());
      }
   }
}

void DealingMachine::deal() {
   //Call dealer.deal() and pass output to output stream
   Board newBoard = dealer->deal();
   output << newBoard;
}

void DealingMachine::strategyChooser(ssrType ssr, nssrType nssr, hcprType hcpr) {
   //Choose VFD or SFD or BD to be instantiated as the dealer based on required rules.
   delete dealer;
   int ssrCount = 0;
   for (auto& [pos, suitMap] : ssr) {
      for (auto& [suit, length] : suitMap) {
	 ssrCount++;
      }
   }
   int nssrCount = nssr.size();
   int hcprCount;
   for (auto& [pos, range] : hcpr) {
      if (range.first> 0 || range.second < MAX_HCP)
	 hcprCount++;
   }
   if (ssrCount * 4 + nssrCount > 11) {
      output << "Strict Shape Rules. Use Shape First Dealer. Initialising Dealer may take up to 3 minutes.\n";
      dealer = new ShapeFirstDealer(ssr, nssr, hcpr);
   }
   else if (ssrCount == 0 && nssrCount == 0 && hcprCount == 0) {
      output << "No Rules given. Use Basic Dealer\n";
      dealer = new BasicDealer();
   }
   else {
      output << "Loose Shape Rules. Use Value First Dealer. Initialising Dealer may take up to 3 minutes.\n";
      dealer = new ValueFirstDealer(ssr, nssr, hcpr);
   }
   
}

DealingMachine::~DealingMachine() {
   delete dealer;
}
