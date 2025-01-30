#include "OperateurDeplacement.hpp"
#include "PieceConcrete.hpp"
#include "OperateurRotation.hpp"
#include "OperateurSymetrie.hpp"

OperateurDeplacement::OperateurDeplacement(Piece &source, const Case &position, OrientationDeplacement sens)
    : PieceOperateur(source, position), sens(sens) {}

void OperateurDeplacement::accept(const PieceOperateur &v)
{
    //cout << "Dans accept de OperateurDeplacement, type de visiteur : " << typeid(v).name() << endl;

    // La visite c'est l'étape ultime de l'opération de déplacement, je suis le visiteur, et PieceOpérateur accepte mon déplacement.
    v.visit(*this);
}

void OperateurDeplacement::visit(PieceConcrete &p) const
{
    //cout << "Dans visit avec type : " << typeid(*this).name() << endl;

    for (auto &case_iterator : p.coordinates)
    {
        mapPosition(case_iterator);
    }
}

void OperateurDeplacement::visit(OperateurDeplacement &x) const
{
    mapPosition(x.position);
}

void OperateurDeplacement::visit(OperateurRotation &x) const
{
    mapPosition(x.position);
    // x.source.accept(*this);
}

void OperateurDeplacement::visit(OperateurSymetrie &x) const
{
    mapPosition(x.position);
    // x.source.accept(*this);
}

void OperateurDeplacement::mapPosition(Case &pos) const
{
    switch (sens)
    {
    case NORD:
        pos.coords.first--;
        break;
    case SUD:
        pos.coords.first++;
        break;
    case EST:
        pos.coords.second++;
        break;
    case OUEST:
        pos.coords.second--;
        break;
    }
}

OrientationDeplacement OperateurDeplacement::getOrientation() const
{
    return sens;
}

const Case &OperateurDeplacement::getPosition() const
{
    return position;
}

Piece &OperateurDeplacement::getPiece() const
{
    return source;
}
