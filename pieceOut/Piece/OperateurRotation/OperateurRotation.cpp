#include "OperateurRotation.hpp"
#include "PieceConcrete.hpp"
#include "OperateurDeplacement.hpp"
#include "OperateurSymetrie.hpp"

OperateurRotation::OperateurRotation(Piece &source, const Case &position, SensRotation sens)
    : PieceOperateur(source, position), sens(sens) {}

void OperateurRotation::accept(const PieceOperateur &v)
{
    v.visit(*this);
}

void OperateurRotation::visit(PieceConcrete &p) const
{
    for (auto &case_iterator : p.coordinates)
    {
        mapPosition(case_iterator);
    }
}

void OperateurRotation::visit(OperateurRotation &o) const
{
    mapPosition(o.position);
    // o.source.accept(*this);
}

void OperateurRotation::visit(OperateurDeplacement &x) const
{
    mapPosition(x.position);
}

void OperateurRotation::visit(OperateurSymetrie &x) const
{
    mapPosition(x.position);
}

void OperateurRotation::mapPosition(Case &case_pointee) const
{

    // 1. On recupere le centre de rotation.
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

    // On vient récupérer les coordonnées relatives par rapport au centre.
    int rel_x = case_pointee.coords.first - cx;
    int rel_y = case_pointee.coords.second - cy;

    int new_rel_x, new_rel_y;

    switch (sens)
    {
    case HORAIRE:
        // Rotation horaire : (x, y) -> (y, -x).
        new_rel_x = rel_y;
        new_rel_y = -rel_x;
        break;

    case ANTIHORAIRE:
        // Rotation antihoraire : (x, y) -> (-y, x).
        new_rel_x = -rel_y;
        new_rel_y = rel_x;
        break;

    default:
        cout << "Sens de rotation invalide !" << endl;
        return;
    }

    // On met enfin à jour nos coordonnées de la pièce source.
    case_pointee.coords.first = new_rel_x + cx;
    case_pointee.coords.second = new_rel_y + cy;

    // cout << "Rotation complète effectuée autour du centre : (" << cx << ", " << cy << ")" << endl;
}