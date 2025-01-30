#include "PieceOperateur.hpp"
#include "OperateurDeplacement.hpp"
#include "OperateurRotation.hpp"
#include "OperateurSymetrie.hpp"

using namespace std;

void PieceOperateur::trigger(const Case &relativePos, Piece &origin)
{
    // cout << "trigger PieceOperateur" << endl;
    // cout << "trigger PieceOperateur avec type : " << typeid(origin).name() << endl;

    // Cette méthode fait appel à la méthode de la classe source (la pièce concrète).
    source.accept(*this);
}

void PieceOperateur::visit(PieceConcrete &p) const
{
    for (auto &x : p.coordinates)
        mapPosition(x);
}

void PieceOperateur::changerPosition(int x, int y)
{
    position.coords.first += x;
    position.coords.second += y;
}

const vector<Case> &PieceOperateur::getCoordinates() const
{
    return source.getCoordinates();
}