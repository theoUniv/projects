#ifndef OPERATEURSYMETRIE_HPP
#define OPERATEURSYMETRIE_HPP

#include "../PieceOperateur/PieceOperateur.hpp"

enum AxeSymetrie
{
    HORIZONTAL,
    VERTICAL
};

class OperateurSymetrie : public PieceOperateur
{
public:
    AxeSymetrie axe;

    OperateurSymetrie(Piece &source, const Case &position, AxeSymetrie axe_args);
    virtual ~OperateurSymetrie() = default;

    void accept(const PieceOperateur &v) override;
    void visit(PieceConcrete &p) const override;
    void visit(OperateurDeplacement &x) const override;
    void visit(OperateurRotation &x) const override;
    void visit(OperateurSymetrie &x) const override;
    void mapPosition(Case &pos) const override;
};

#endif
