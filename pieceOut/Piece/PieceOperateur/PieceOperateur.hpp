#ifndef PIECEOPERATEUR_HPP
#define PIECEOPERATEUR_HPP

#include "../Piece_abstract/Piece.hpp"
#include "../PieceConcrete/PieceConcrete.hpp" // Ajoutez cette ligne

using namespace std;

class OperateurDeplacement;
class OperateurRotation;
class OperateurSymetrie;

class PieceOperateur : public Piece
{
public:
    Piece &source;
    Case position;

    PieceOperateur(Piece &source, const Case &position)
        : source(source), position(position) {}

    const vector<Case> &getCoordinates() const override;

    void trigger(const Case &relativePos, Piece &origin) override;

    virtual void mapPosition(Case &) const = 0;

    virtual void visit(PieceConcrete &) const;
    virtual void visit(OperateurDeplacement &) const = 0;
    virtual void visit(OperateurRotation &) const = 0;
    virtual void visit(OperateurSymetrie &) const = 0;

    virtual void accept(const PieceOperateur &v) override = 0;
    void changerPosition(int x, int y);
};

#endif