#ifndef _PLATEAU
#define _PLATEAU

#include <utility>
#include <vector>
#include <SFML/Graphics.hpp>
#include <iostream>
#include "../Case/Case.hpp"
#include "../Piece/Piece_abstract/Piece.hpp"
#include "../Piece/PieceConcrete/PieceConcrete.hpp"
#include "../Piece/OperateurDeplacement/OperateurDeplacement.hpp"
#include "../Piece/OperateurRotation/OperateurRotation.hpp"
#include "../Piece/OperateurSymetrie/OperateurSymetrie.hpp"
using namespace std;
using namespace sf;

class Plateau
{
public:
    int highlighted_piece = -1; // -1 signifie qu'aucune pièce n'est seléctionnée.
    void handleClick(Vector2i mousePos, int caseSize, int borderSize);

    const int rows, cols;
    vector<vector<Case>> plateau;
    vector<PieceConcrete> pieces;
    vector<PieceConcrete> buts;

    vector<OperateurDeplacement> operateursDeplacement;
    vector<OperateurRotation> operateursRotation;
    vector<OperateurSymetrie> operateursSymetrie;

    Plateau(int rows, int cols, vector<vector<int>> calque, vector<PieceConcrete> pieces, vector<PieceConcrete> buts, vector<OperateurDeplacement> operateursDeplacement, vector<OperateurRotation> operateursRotation, vector<OperateurSymetrie> operateursSymetrie);
    Color getColorOfCase(int x, int y);
    void drawGrid(RenderWindow &window, int caseSize, int borderSize);
    void displayBusyCases() const;

    bool isMovePossible(int pieceIndex, OrientationDeplacement direction);
    bool isRotationPossible(int pieceIndex, SensRotation sens);
    bool isSymetriePossible(int pieceIndex, AxeSymetrie axe);

    void movePiece(int pieceIndex, OrientationDeplacement sens);
    void rotatePiece(int pieceIndex, SensRotation sens);
    void symetriePiece(int pieceIndex, AxeSymetrie);

    bool isGameFinished();
    bool IsPieceInGoal(PieceConcrete p1, PieceConcrete p2);

    OperateurDeplacement *isOperateurDeplacementValid(int pieceIndex, OrientationDeplacement direction);
    OperateurRotation *isOperateurRotationValid(int pieceIndex, SensRotation sens);
    OperateurSymetrie *isOperateurSymetrieValid(int pieceIndex, AxeSymetrie axe);

    void ApplyTransformationToOperateur(PieceConcrete &concretePiece, PieceOperateur *operateur_a_appliquer);

    void move_operator(PieceConcrete &concretePiece, OperateurDeplacement *operateur_type);
    void rotate_operator(PieceConcrete &concretePiece, OperateurRotation *operateur_type);
    void symetrie_operator(PieceConcrete &concretePiece, OperateurSymetrie *operateur_type);
};

#endif