#include "Piece.hpp"

// Fichier cpp fait par principe, mais inutile car Piece.hpp est une classe abstraite.

void Piece::trigger(const Case &coord) { trigger(coord, *this); }