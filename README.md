# bridgeApp

## Compilation

Compile using `g++ -std=c++17 *.cpp` to suppress warnings about new code features used.

## Basic Usage

There are three types of permitted constraints that can be used in the dealing machine.

Specific Shape Rules (SSRs) specify a player at a position must exactly has some number of cards in a specific suit. (e.g. Player at E position must have 3 Club suit cards)

Non Specific Shape Rules (NSSRs) specify a player at a position must exactly has some number of cards in any of the suit. (e.g. Player at S position must have a suit in which he has 5 cards)

HCP Rules (HCPRs) specify a player at a position must have a HCP value in some range. (e.g. Player at N position must have 10 - 20 HCP inclusive)

SSRs are represented by a `map<Position, map<Suit, int>>` data type, so it maps a Position (which represents a player) and a Suit to a int, which represents the length of the Suit of that Position.

NSSRs are represented by a `vector<pair<Position, int>>` data type, where each pair specifies one rule of a player (e.g. Player E must have a length-4 suit) and the vector represents a collection of them.

HCPRs are represented by a `map<Position, pair<int, int>>` data type, which maps a Position to a pair of integer, which represents the minimum and maximum HCP (inclusive) allowed for the player.

To deal a Board:

1. Initialise a `AdvDealer` class object with these filter rules.

2. Call `deal()` method of the instantiated `AdvDealer` object.

3. `deal()` returns a `Board` object which can be printed directly with `cout`.

Usage examples can be found in `newAdvDealerTest1` and `newAdvDealerTest2` function in the `main.cpp` file.
