#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <vector>

enum class Suit {C, D, H ,S, NT};
enum class Rank {N2, N3, N4, N5, N6, N7, N8, N9, T, J, Q, K, A};
enum class Position {E, S, W, N};

Position operator+ (Position pos, int n);

std::ostream& operator<< (std::ostream& out, Position pos);

std::ostream& operator<< (std::ostream& out, Suit suit);

std::ostream& operator<< (std::ostream& out, Rank rank);

const std::initializer_list suitList = {Suit::C, Suit::D, Suit::H, Suit::S};
const std::initializer_list rankList = {Rank::J, Rank::Q, Rank::K, Rank::A};
const std::initializer_list posList  = {Position::E, Position::S, Position::W, Position::N};
const std::initializer_list numRankList = {Rank::N2, Rank::N3, Rank::N4, Rank::N5, Rank::N6, Rank::N7, Rank::N8, Rank::N9, Rank::T};

const int CARDS = 13;
const int HIGH_CARDS = 4;

#endif

