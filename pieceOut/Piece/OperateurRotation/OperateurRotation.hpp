#ifndef OPERATEURROTATION_HPP
#define OPERATEURROTATION_HPP

#include "../PieceOperateur/PieceOperateur.hpp"

enum SensRotation
{
    HORAIRE,
    ANTIHORAIRE
};

class OperateurRotation : public PieceOperateur
{
public:
    SensRotation sens;

    OperateurRotation(Piece &source, const Case &position, SensRotation sens);
    virtual ~OperateurRotation() = default;
    void accept(const PieceOperateur &v) override;
    void visit(PieceConcrete &p) const override;
    void visit(OperateurDeplacement &x) const override;
    void visit(OperateurRotation &x) const override;
    void visit(OperateurSymetrie &x) const override;
    void mapPosition(Case &pos) const override;
};

#endif
