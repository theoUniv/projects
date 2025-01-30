#include "PieceConcrete.hpp"
#include "../PieceOperateur/PieceOperateur.hpp"
#include <iostream>

using namespace std;

PieceConcrete::PieceConcrete(const vector<Case> &coords, Color pieceColor)
    : coordinates(coords),
      color(pieceColor) {}

const vector<Case> &PieceConcrete::getCoordinates() const
{
    return coordinates;
}

void PieceConcrete::trigger(const Case &coord, Piece &origin)
{
    // cout << "trigger de piece concrete" << endl;
    origin.trigger(coord, *this);
}

void PieceConcrete::accept(const PieceOperateur &v)
{
    v.visit(*this); // La pièce accepte le visiteur (dans ce cas, l'opérateur de déplacement).
}

bool PieceConcrete::operator==(const Piece &other) const
{
    const PieceConcrete *otherConcrete = dynamic_cast<const PieceConcrete *>(&other);
    if (otherConcrete == nullptr)
        return false;

    return this->color == otherConcrete->color && this->coordinates == otherConcrete->coordinates;
}

bool PieceConcrete::operator==(const PieceConcrete &other) const
{
    return this->coordinates == other.coordinates && this->color == other.color;
}

bool PieceConcrete::operator!=(const PieceConcrete &other) const
{
    if (this->coordinates != other.coordinates || this->color != other.color)
    {
        return true;
    }
    else
    {
        return false;
    }
}