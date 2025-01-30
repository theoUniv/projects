#ifndef PIECE_HPP
#define PIECE_HPP

#include <vector>
#include <utility>
#include "../../Case/Case.hpp"

using namespace std;

class PieceOperateur;

class Piece
{
public:
    virtual void trigger(const Case &coord, Piece &origin) = 0;
    void trigger(const Case &coord);
    virtual const vector<Case> &getCoordinates() const = 0;
    virtual void accept(const PieceOperateur &v) = 0;
};

#endif