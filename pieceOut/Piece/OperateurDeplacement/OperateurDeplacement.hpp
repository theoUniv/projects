#ifndef OPERATEURDEPLACEMENT_HPP
#define OPERATEURDEPLACEMENT_HPP

#include "../PieceOperateur/PieceOperateur.hpp"

enum OrientationDeplacement
{
    NORD,
    SUD,
    EST,
    OUEST
};

class OperateurDeplacement : public PieceOperateur
{
public:
    OrientationDeplacement sens;

    OperateurDeplacement(Piece &source, const Case &position, OrientationDeplacement sens);
    virtual ~OperateurDeplacement() = default;

    Piece &getPiece() const;

    void accept(const PieceOperateur &v) override;
    void visit(PieceConcrete &p) const override;
    void visit(OperateurDeplacement &x) const override;
    void visit(OperateurRotation &x) const override;
    void visit(OperateurSymetrie &x) const override;
    void mapPosition(Case &pos) const override;
    OrientationDeplacement getOrientation() const;
    const Case &getPosition() const;
};

#endif