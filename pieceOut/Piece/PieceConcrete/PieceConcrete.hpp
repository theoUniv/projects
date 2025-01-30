#ifndef PIECECONCRETE_HPP
#define PIECECONCRETE_HPP

#include "../Piece_abstract/Piece.hpp"
#include <vector>
#include <utility> // Pour pair
#include "../../Case/Case.hpp"

using namespace std;

class PieceConcrete : public Piece
{
public:
    vector<Case> coordinates;
    Color color;

    PieceConcrete(const vector<Case> &coords, Color pieceColor);
    virtual ~PieceConcrete() = default;

    const vector<Case> &getCoordinates() const override;
    void trigger(const Case &coord, Piece &origin) override;
    void accept(const PieceOperateur &v) override;

    bool operator==(const Piece &other) const;
    bool operator==(const PieceConcrete &other) const;
    bool operator!=(const PieceConcrete &other) const;
};

#endif