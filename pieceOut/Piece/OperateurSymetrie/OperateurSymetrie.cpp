#include "OperateurSymetrie.hpp"
#include "PieceConcrete.hpp"
#include "OperateurDeplacement.hpp"
#include "OperateurRotation.hpp"

OperateurSymetrie::OperateurSymetrie(Piece &source, const Case &position, AxeSymetrie axe_args)
    : PieceOperateur(source, position), axe(axe_args) {}

void OperateurSymetrie::accept(const PieceOperateur &v)
{
    v.visit(*this);
}

void OperateurSymetrie::visit(PieceConcrete &p) const
{
    for (auto &case_iterator : p.coordinates)
    {
        mapPosition(case_iterator);
    }
}

void OperateurSymetrie::visit(OperateurSymetrie &x) const
{
    mapPosition(x.position);
    // x.source.accept(*this);
}

void OperateurSymetrie::visit(OperateurDeplacement &x) const
{
    mapPosition(x.position);
}

void OperateurSymetrie::visit(OperateurRotation &x) const
{
    mapPosition(x.position);
}

void OperateurSymetrie::mapPosition(Case &case_pointee) const
{
    int cx;
    int cy;

    for (auto &coordonnees_piece : source.getCoordinates())
    {
        if (coordonnees_piece.coords.first == position.coords.first &&
            coordonnees_piece.coords.second == position.coords.second)
        {
            cx = position.coords.first;
            cy = position.coords.second;
        }
    }

    // On calcule nos coordonnées relatives par rapport au centre de symétrie.
    int rel_x = case_pointee.coords.first - cx;
    int rel_y = case_pointee.coords.second - cy;

    int new_rel_x = rel_x;
    int new_rel_y = rel_y;

    switch (axe)
    {
    case HORIZONTAL:
        // Symétrie horizontale : on inverse la coordonnée y.
        new_rel_y = -rel_y;
        break;

    case VERTICAL:
        // Symétrie verticale : on inverse la coordonnée x.
        new_rel_x = -rel_x;
        break;

    default:
        cout << "Axe de symétrie invalide !" << endl;
        return;
    }

    case_pointee.coords.first = new_rel_x + cx;
    case_pointee.coords.second = new_rel_y + cy;

    //cout << "Symétrie complète effectuée autour du centre : (" << cx << ", " << cy << ")" << endl;
}
