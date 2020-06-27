#ifndef UTILS_H
#define UTILS_H

#include <iostream>

enum class Suit {C, D, H ,S, NT};
enum class Rank {N2, N3, N4, N5, N6, N7, N8, N9, T, J, Q, K, A};
enum class Position {E, S, W, N};

Position operator+ (Position pos, int n);

std::ostream& operator<< (std::ostream& out, Position pos);

std::ostream& operator<< (std::ostream& out, Suit suit);

std::ostream& operator<< (std::ostream& out, Rank rank);

#endif

