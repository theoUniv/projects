
#include <utility>
#include <vector>
#include <SFML/Graphics.hpp>
#include <iostream>
#include "Plateau.hpp"
#include "../Case/Case.hpp"

using namespace std;
using namespace sf;

Plateau::Plateau(int rows_arg, int cols_arg, vector<vector<int>> calque, vector<PieceConcrete> pieces, vector<PieceConcrete> buts, vector<OperateurDeplacement> operateursDeplacement, vector<OperateurRotation> operateursRotation, vector<OperateurSymetrie> operateursSymetrie)
    : rows(rows_arg),
      cols(cols_arg),
      plateau(rows_arg, vector<Case>(cols_arg, Case({0, 0}))),
      pieces(pieces),
      buts(buts),
      operateursDeplacement(operateursDeplacement),
      operateursRotation(operateursRotation),
      operateursSymetrie(operateursSymetrie)
{
    if (rows_arg <= 0 || cols_arg <= 0)
    {
        throw invalid_argument("Le nombre de lignes et de colonnes doit être supérieur à 0.");
    }

    for (int i = 0; i < rows_arg; i++)
    {
        for (int j = 0; j < cols_arg; j++)
        {
            plateau[i][j] = Case(pair<int, int>(i, j));
            if (calque[i][j] == 1)
            {
                plateau[i][j].is_real = true;
            }
        }
    }

    // On vient marquer les cases occupées.
    for (const PieceConcrete &piece : pieces)
    {
        for (const Case &coord : piece.coordinates)
        {
            int x = coord.coords.first;
            int y = coord.coords.second;

            if (x >= 0 && x < rows_arg && y >= 0 && y < cols_arg)
            {
                plateau[x][y].is_busy = true;
            }
            else
            {
                // cout << "Coordonnées invalides pour une pièce : (" << x << ", " << y << ")" << endl;
            }
        }
    }

    for (const PieceConcrete &but : buts)
    {
        for (const Case &coord : but.coordinates)
        {
            int x = coord.coords.first;
            int y = coord.coords.second;

            if (x >= 0 && x < rows_arg && y >= 0 && y < cols_arg)
            {
                plateau[x][y].is_goal = pair<bool, Color>(true, but.color);
            }
            else
            {
                // cout << "Coordonnées invalides pour un but : (" << x << ", " << y << ")" << endl;
            }
        }
    }
}
void Plateau::handleClick(Vector2i mousePos, int caseSize, int borderSize)
{
    // Vérifie si le clic est en dehors des limites du plateau
    if (mousePos.x < borderSize || mousePos.y < borderSize ||
        mousePos.x >= borderSize + cols * caseSize || mousePos.y >= borderSize + rows * caseSize)
    {
        highlighted_piece = -1;
        return;
    }

    // Position de la souris -> Indice dans la grille
    int col = (mousePos.x - borderSize) / caseSize;
    int row = (mousePos.y - borderSize) / caseSize;

    // cout << "Position " << col << "," << row << endl;

    // Vérifie si la case cliquée est valide et met à jour `highlighted_piece`
    highlighted_piece = -1;
    for (size_t i = 0; i < pieces.size(); ++i)
    {
        for (const Case &coord : pieces[i].coordinates)
        {
            if (coord.coords.first == row && coord.coords.second == col)
            {
                highlighted_piece = i;
                // cout << "Pièce sélectionnée : " << i << endl;
                break;
            }
        }
        if (highlighted_piece != -1)
            break;
    }

    // Si aucune pièce n'est trouvée, rien à faire
    if (highlighted_piece == -1)
        return;

    // Vérifie si la case sélectionnée est un opérateur de rotation
    for (const OperateurRotation &operateur : operateursRotation)
    {
        if (operateur.position.coords.first == row && operateur.position.coords.second == col)
        {
            // cout << "Un opérateur de rotation est présent sur cette case." << endl;
            rotatePiece(highlighted_piece, operateur.sens);
            return;
        }
    }

    // Vérifie si la case sélectionnée est un opérateur de symétrie
    for (const OperateurSymetrie &operateur : operateursSymetrie)
    {
        if (operateur.position.coords.first == row && operateur.position.coords.second == col)
        {
            // cout << "Un opérateur de symétrie est présent sur cette case." << endl;
            symetriePiece(highlighted_piece, operateur.axe);
            return;
        }
    }

    // Si aucune action particulière n'est nécessaire, la pièce est simplement sélectionnée
    // cout << "Aucun opérateur spécial sur cette case." << endl;
}

// Renvoie la couleur d'une case, en prennant en considération si y'a une pièce dessus.
Color Plateau::getColorOfCase(int x, int y)
{
    if (x < 0 || x >= rows || y < 0 || y >= cols)
    {
        throw out_of_range("Les indices x et y doivent être dans les limites du plateau.");
    }
    for (size_t i = 0; i < pieces.size(); i++)
    {
        for (size_t j = 0; j < pieces[i].coordinates.size(); j++)
        {

            int x2 = pieces[i].coordinates[j].coords.first;
            int y2 = pieces[i].coordinates[j].coords.second;

            if (x == x2 && y == y2)
            {
                return pieces[i].color;
            }
        }
    }
    return Color::White;
}

sf::Texture textureNord, textureSud, textureEst, textureOuest, textureRotationH, textureRotationA, textureSymetrieV, textureSymetrieH;
void Plateau::drawGrid(RenderWindow &window, int caseSize, int borderSize)
{
    if (!textureNord.loadFromFile("images/aup.png") ||
        !textureSud.loadFromFile("images/adown.png") ||
        !textureEst.loadFromFile("images/aright.png") ||
        !textureOuest.loadFromFile("images/aleft.png") ||
        !textureRotationH.loadFromFile("images/rotate_right.png") ||
        !textureRotationA.loadFromFile("images/rotate_left.png") ||
        !textureSymetrieV.loadFromFile("images/flip_v.png") ||
        !textureSymetrieH.loadFromFile("images/flip_h.png"))
    {
        std::cerr << "Erreur de chargement des images." << std::endl;
        return;
    }

    for (size_t i = 0; i < pieces.size(); i++)
    {
        for (size_t j = 0; j < pieces[i].coordinates.size(); j++)
        {
            int x = pieces[i].coordinates[j].coords.first;
            int y = pieces[i].coordinates[j].coords.second;
            if (plateau[x][y].is_real == true)
            {
                plateau[x][y].is_busy = true;
            }
        }
    }

    // On dessine notre plateau et nos pièces.
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            RectangleShape rectangle(Vector2f(caseSize, caseSize));
            rectangle.setPosition(borderSize + j * caseSize, borderSize + i * caseSize);
            rectangle.setOutlineThickness(2);
            rectangle.setOutlineColor(Color::Black);

            rectangle.setFillColor(Color::Black);

            // Cases "réelles".
            if (plateau[i][j].is_real)
            {
                rectangle.setFillColor(Color::White);
            }

            // Cases "objectifs".
            if (plateau[i][j].is_goal.first)
            {
                sf::Color color = plateau[i][j].is_goal.second;
                color.a = 100; // Opacité.
                rectangle.setFillColor(color);
            }

            // Cases "occupées" par des pièces.
            if (plateau[i][j].is_busy)
            {
                rectangle.setFillColor(getColorOfCase(i, j));
            }

            // Case "surlignée".
            if (highlighted_piece != -1)
            {
                for (const auto &coord : pieces[highlighted_piece].coordinates)
                {
                    if (coord.coords.first == i && coord.coords.second == j)
                    {
                        rectangle.setFillColor(Color::Yellow);
                    }
                }
            }

            window.draw(rectangle);
        }
    }

    // Facteur de mise à l'échelle pour les flèches.
    float scaleFactor = 0.6f;
    float offsetFactor = 0.3f;

    for (const OperateurDeplacement &op : operateursDeplacement)
    {
        pair<int, int> position = op.position.coords;
        int x = position.first;
        int y = position.second;

        if (x >= 0 && x < rows && y >= 0 && y < cols)
        {
            float centerX = borderSize + y * caseSize + caseSize / 2.0f;
            float centerY = borderSize + x * caseSize + caseSize / 2.0f;

            sf::Sprite arrow;

            if (op.sens == OrientationDeplacement::NORD)
            {
                arrow.setTexture(textureNord);
                arrow.setOrigin(textureNord.getSize().x / 2.0f, textureNord.getSize().y / 2.0f);
                arrow.setPosition(centerX, centerY - caseSize * offsetFactor);
            }
            else if (op.sens == OrientationDeplacement::SUD)
            {
                arrow.setTexture(textureSud);
                arrow.setOrigin(textureSud.getSize().x / 2.0f, textureSud.getSize().y / 2.0f);
                arrow.setPosition(centerX, centerY + caseSize * offsetFactor);
            }
            else if (op.sens == OrientationDeplacement::EST)
            {
                arrow.setTexture(textureEst);
                arrow.setOrigin(textureEst.getSize().x / 2.0f, textureEst.getSize().y / 2.0f);
                arrow.setPosition(centerX + caseSize * offsetFactor, centerY);
            }
            else if (op.sens == OrientationDeplacement::OUEST)
            {
                arrow.setTexture(textureOuest);
                arrow.setOrigin(textureOuest.getSize().x / 2.0f, textureOuest.getSize().y / 2.0f);
                arrow.setPosition(centerX - caseSize * offsetFactor, centerY);
            }

            arrow.setScale(scaleFactor, scaleFactor);

            window.draw(arrow);
        }
    }

    for (const OperateurRotation &op : operateursRotation)
    {
        pair<int, int> position = op.position.coords;
        int x = position.first;
        int y = position.second;

        if (x >= 0 && x < rows && y >= 0 && y < cols)
        {
            // Calculer le centre de la cellule
            float centerX = borderSize + y * caseSize + caseSize / 2.0f;
            float centerY = borderSize + x * caseSize + caseSize / 2.0f;

            sf::Sprite symbol;

            if (op.sens == SensRotation::ANTIHORAIRE)
            {
                symbol.setTexture(textureRotationA);
            }
            else
            {
                symbol.setTexture(textureRotationH);
            }

            symbol.setOrigin(symbol.getTexture()->getSize().x / 2.0f, symbol.getTexture()->getSize().y / 2.0f);

            symbol.setPosition(centerX, centerY);

            symbol.setScale(scaleFactor - 0.3f, scaleFactor - 0.3f);

            window.draw(symbol);
        }
    }

    for (const OperateurSymetrie &op : operateursSymetrie)
    {
        pair<int, int> position = op.position.coords;
        int x = position.first;
        int y = position.second;

        if (x >= 0 && x < rows && y >= 0 && y < cols)
        {
            float centerX = borderSize + y * caseSize + caseSize / 2.0f;
            float centerY = borderSize + x * caseSize + caseSize / 2.0f;

            sf::Sprite symbol;

            if (op.axe == AxeSymetrie::HORIZONTAL)
            {
                symbol.setTexture(textureSymetrieH);
            }
            else
            {
                symbol.setTexture(textureSymetrieV);
            }

            symbol.setOrigin(symbol.getTexture()->getSize().x / 2.0f, symbol.getTexture()->getSize().y / 2.0f);

            symbol.setPosition(centerX, centerY);

            symbol.setScale(scaleFactor - 0.3f, scaleFactor - 0.3f);

            window.draw(symbol);
        }
    }
}

bool Plateau::isGameFinished()
{
    for (PieceConcrete &but : buts)
    {
        for (PieceConcrete &piece : pieces)
        {
            if (IsPieceInGoal(piece, but) == false)
            {
                return false;
            }
        }
    }

    return true;
}

bool Plateau::IsPieceInGoal(PieceConcrete p1, PieceConcrete p2)
{
    if (p1.color == p2.color)
    {
        for (Case &case_piece : p1.coordinates)
        {
            if (std::find(p2.coordinates.begin(), p2.coordinates.end(), case_piece) == p2.coordinates.end())
            {
                return false; // Si une case n'est pas trouvée, retourner false.
            }
        }
    }
    return true;
}

bool Plateau::isMovePossible(int pieceIndex, OrientationDeplacement direction)
{

    if (pieceIndex < 0 || pieceIndex >= pieces.size())
    {
        throw out_of_range("Index de pièce invalide.");
    }

    // Verification si la pièce existe.
    if (pieceIndex < 0 || static_cast<size_t>(pieceIndex) >= pieces.size())
    {
        // cout << "Verification impossible de la pièce: indice invalide" << endl;
        return false;
    }

    // Definition des décalages.
    int dx = 0, dy = 0;
    switch (direction)
    {
    case NORD:
        dx = -1;
        break;
    case SUD:
        dx = 1;
        break;
    case EST:
        dy = 1;
        break;
    case OUEST:
        dy = -1;
        break;
    }

    // Toutes les cases de la pièces peuvent se déplacer ?
    for (const Case &coord : pieces[pieceIndex].coordinates)
    {
        int newX = coord.coords.first + dx;
        int newY = coord.coords.second + dy;

        // On check les limites du plateau de jeu.
        if (newX < 0 || newX >= rows || newY < 0 || newY >= cols)
        {
            cout << "Déplacement impossible (limites plateau) : " << newX << ":" << newY << endl;
            return false;
        }

        if (!plateau[newX][newY].is_real)
        {
            cout << "Déplacement impossible : La case n'existe pas." << endl;
            return false;
        }

        if (plateau[newX][newY].is_busy && getColorOfCase(newX, newY) != pieces[pieceIndex].color)
        {
            cout << "Déplacement impossible : La case est déjà occupée par une pièce d'une autre couleur." << endl;
            return false;
        }

        // cout << "Déplacement possible : " << newX << ":" << newY << endl;
    }

    return true;
}

bool Plateau::isRotationPossible(int pieceIndex, SensRotation sens)
{

    if (pieceIndex < 0 || pieceIndex >= pieces.size())
    {
        throw out_of_range("Index de pièce invalide.");
    }
    // Vérification si la pièce existe.
    if (pieceIndex < 0 || static_cast<size_t>(pieceIndex) >= pieces.size())
    {
        // cout << "Vérification impossible de la pièce : indice invalide" << endl;
        return false;
    }

    // Récupération de la pièce.
    Piece &piece = pieces[pieceIndex];
    if (typeid(piece) != typeid(PieceConcrete))
    {
        // cout << "Erreur : La pièce n'est pas de type PieceConcrete !" << endl;
        return false;
    }
    PieceConcrete &concretePiece = dynamic_cast<PieceConcrete &>(piece);

    Case center = concretePiece.coordinates[0];

    for (Case &case_one : concretePiece.coordinates)
    {
        for (OperateurRotation &operateur_rotation : operateursRotation)
        {
            if (case_one.coords.first == operateur_rotation.position.coords.first &&
                case_one.coords.second == operateur_rotation.position.coords.second)
            {
                center.coords.first = case_one.coords.first;
                center.coords.second = case_one.coords.second;
            }
        }
    }

    // cout << "Centre de rotation : " << center.coords.first << ":" << center.coords.second << endl;

    for (const Case &coord : concretePiece.coordinates)
    {
        int rel_x = coord.coords.first - center.coords.first;
        int rel_y = coord.coords.second - center.coords.second;

        int new_rel_x, new_rel_y;
        switch (sens)
        {
        case HORAIRE:
            new_rel_x = rel_y;
            new_rel_y = -rel_x;
            break;
        case ANTIHORAIRE:
            new_rel_x = -rel_y;
            new_rel_y = rel_x;
            break;
        default:
            cout << "Sens de rotation invalide !" << endl;
            return false;
        }

        int newX = new_rel_x + center.coords.first;
        int newY = new_rel_y + center.coords.second;

        if (newX < 0 || newX >= rows || newY < 0 || newY >= cols)
        {
            cout << "Rotation impossible (limites plateau) : " << newX << ":" << newY << endl;
            return false;
        }

        if (!plateau[newX][newY].is_real)
        {
            cout << "Rotation impossible : case non réelle à " << newX << ":" << newY << endl;
            return false;
        }
        else if (plateau[newX][newY].is_busy && getColorOfCase(newX, newY) != concretePiece.color && !plateau[newX][newY].is_goal.first)
        {
            cout << "Rotation impossible : case occupée par une autre pièce à " << newX << ":" << newY << endl;
            return false;
        }
        else if (plateau[newX][newY].is_busy && getColorOfCase(newX, newY) == concretePiece.color)
        {
            // cout << "Rotation impossible : case occupée par une pièce de la même couleur à " << newX << ":" << newY << endl;
        }
        else
        {
            // cout << "Rotation possible pour la case : " << newX << ":" << newY << endl;
        }
    }

    // cout << "Rotation possible pour la pièce " << pieceIndex << " dans le sens " << (sens == HORAIRE ? "HORAIRE" : "ANTIHORAIRE") << endl;
    return true;
}

bool Plateau::isSymetriePossible(int pieceIndex, AxeSymetrie axe)
{

    if (pieceIndex < 0 || pieceIndex >= pieces.size())
    {
        throw out_of_range("Index de pièce invalide.");
    }

    if (pieceIndex < 0 || static_cast<size_t>(pieceIndex) >= pieces.size())
    {
        cout << "Vérification impossible de la pièce: indice invalide" << endl;
        return false;
    }

    Piece &piece = pieces[pieceIndex];
    PieceConcrete &concretePiece = dynamic_cast<PieceConcrete &>(piece);

    Case center = concretePiece.coordinates[0];

    for (Case &case_one : concretePiece.coordinates)
    {
        for (OperateurSymetrie &operateur_symetrie : operateursSymetrie)
        {
            if (case_one.coords.first == operateur_symetrie.position.coords.first &&
                case_one.coords.second == operateur_symetrie.position.coords.second)
            {
                center.coords.first = case_one.coords.first;
                center.coords.second = case_one.coords.second;
            }
        }
    }

    for (const Case &coord : concretePiece.coordinates)
    {
        int rel_x = coord.coords.first - center.coords.first;
        int rel_y = coord.coords.second - center.coords.second;

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
            return false;
        }

        int newX = new_rel_x + center.coords.first;
        int newY = new_rel_y + center.coords.second;

        if (newX < 0 || newX >= rows || newY < 0 || newY >= cols)
        {
            cout << "Symétrie impossible (limites plateau) : " << newX << ":" << newY << endl;
            return false;
        }

        if (!plateau[newX][newY].is_real ||
            (plateau[newX][newY].is_busy && getColorOfCase(newX, newY) != concretePiece.color && !plateau[newX][newY].is_goal.first))
        {
            cout << "Symétrie impossible (pièce -> case occupée) : " << newX << ":" << newY << endl;
            return false;
        }

        // cout << "Symétrie possible pour la case : " << newX << ":" << newY << endl;
    }

    return true;
}

void Plateau::displayBusyCases() const
{
    // cout << "Cases occupées (is_busy) :" << endl;
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            if (plateau[i][j].is_busy)
            {
                // cout << "Case occupée : (" << i << ", " << j << ")" << endl;
            }
        }
    }
}

OperateurDeplacement *Plateau::isOperateurDeplacementValid(int pieceIndex, OrientationDeplacement direction)
{
    for (OperateurDeplacement &operateur : operateursDeplacement)
    {
        if (operateur.getOrientation() == direction)
        {
            const std::vector<Case> &pieceCoordinates = pieces[pieceIndex].getCoordinates();
            const Case &operateurPosition = operateur.getPosition();

            for (const Case &coord : pieceCoordinates)
            {
                if (coord.coords == operateurPosition.coords)
                {
                    // cout << "opérateur trouvé" << endl;
                    return &operateur;
                }
            }
        }
    }

    return nullptr; // Aucun opérateur valide trouvé.
}

OperateurRotation *Plateau::isOperateurRotationValid(int pieceIndex, SensRotation sens)
{
    for (OperateurRotation &operateur : operateursRotation)
    {
        if (operateur.sens == sens)
        {
            const std::vector<Case> &pieceCoordinates = pieces[pieceIndex].getCoordinates();
            const Case &operateurPosition = operateur.position;

            for (const Case &coord : pieceCoordinates)
            {
                if (coord.coords == operateurPosition.coords)
                {
                    // cout << "opérateur trouvé en " << coord.coords.first << "," << coord.coords.second << endl;
                    return &operateur;
                }
            }
        }
    }

    return nullptr; // Aucun opérateur valide trouvé.
}

OperateurSymetrie *Plateau::isOperateurSymetrieValid(int pieceIndex, AxeSymetrie axe)
{
    for (OperateurSymetrie &operateur : operateursSymetrie)
    {
        if (operateur.axe == axe)
        {
            const std::vector<Case> &pieceCoordinates = pieces[pieceIndex].getCoordinates();
            const Case &operateurPosition = operateur.position;

            for (const Case &coord : pieceCoordinates)
            {
                if (coord.coords == operateurPosition.coords)
                {
                    // cout << "opérateur trouvé" << endl;
                    return &operateur;
                }
            }
        }
    }

    return nullptr; // Aucun opérateur valide trouvé.
}

void Plateau::movePiece(int pieceIndex, OrientationDeplacement direction)
{

    if (!isMovePossible(pieceIndex, direction))
    {
        // std::cout << "Déplacement impossible pour la pièce " << pieceIndex << std::endl;
        return;
    }

    // Vérification de la validité de l'opérateur
    OperateurDeplacement *operateur2 = isOperateurDeplacementValid(pieceIndex, direction);
    if (operateur2 == nullptr)
    {
        std::cout << "Aucun opérateur valide trouvé pour la pièce " << pieceIndex << " dans la direction donnée." << std::endl;
        return;
    }

    // On cible la pièce que l'on veut déplacer.
    Piece &piece = pieces[pieceIndex];
    PieceConcrete &concretePiece = dynamic_cast<PieceConcrete &>(piece);
    // OperateurDeplacement operateur(concretePiece, operateur2->position, operateur2->sens);
    Piece *p = new OperateurDeplacement{concretePiece, operateur2->position, operateur2->sens};

    // Libérer les anciennes cases sur le plateau
    for (Case &coord : concretePiece.coordinates)
    {
        plateau[coord.coords.first][coord.coords.second].is_busy = false;
    }

    ApplyTransformationToOperateur(concretePiece, operateur2);
    // Appliquer le déplacement avec l'opérateur validé
    // concretePiece.trigger(concretePiece.coordinates[0], operateur);
    p->trigger(concretePiece.coordinates[0]);
    // Occuper les nouvelles cases sur le plateau
    for (Case &coord : concretePiece.coordinates)
    {
        plateau[coord.coords.first][coord.coords.second].is_busy = true;
    }

    // Vérification si le jeu est terminé après le déplacement
    if (isGameFinished())
    {
        std::cout << "Partie terminée !!" << std::endl;
    }
}

void Plateau::rotatePiece(int pieceIndex, SensRotation sens)
{
    if (!isRotationPossible(pieceIndex, sens))
    {
        // std::cout << "Rotation impossible pour la pièce " << pieceIndex << std::endl;
        return;
    }

    OperateurRotation *operateurRotation = isOperateurRotationValid(pieceIndex, sens);
    if (operateurRotation == nullptr)
    {
        std::cout << "Aucun opérateur valide trouvé pour la pièce " << pieceIndex << " dans le sens donné." << std::endl;
        return;
    }

    Piece &piece = pieces[pieceIndex];
    PieceConcrete &concretePiece = dynamic_cast<PieceConcrete &>(piece);
    Piece *p = new OperateurRotation{concretePiece, operateurRotation->position, operateurRotation->sens};

    for (Case &coord : concretePiece.coordinates)
    {
        plateau[coord.coords.first][coord.coords.second].is_busy = false;
    }

    ApplyTransformationToOperateur(concretePiece, operateurRotation);

    // OperateurRotation operateur(concretePiece, Case({cx, cy}), sens);
    p->trigger(concretePiece.coordinates[0]);
    for (Case &coord : concretePiece.coordinates)
    {
        plateau[coord.coords.first][coord.coords.second].is_busy = true;
    }

    if (isGameFinished())
    {
        std::cout << "Partie terminée !!" << std::endl;
    }
}

void Plateau::symetriePiece(int pieceIndex, AxeSymetrie axe)
{
    if (!isSymetriePossible(pieceIndex, axe))
    {
        cout << "Symetrie impossible pour la pièce " << pieceIndex << endl;
        return;
    }

    OperateurSymetrie *operateurS = isOperateurSymetrieValid(pieceIndex, axe);
    if (operateurS == nullptr)
    {
        std::cout << "Aucun opérateur valide trouvé pour la pièce " << pieceIndex << " dans l'axe :" << axe << std::endl;
        return;
    }

    Piece &piece = pieces[pieceIndex];
    PieceConcrete &concretePiece = dynamic_cast<PieceConcrete &>(piece);
    OperateurSymetrie operateur(concretePiece, operateurS->position, operateurS->axe);
    Piece *p = new OperateurSymetrie{concretePiece, operateurS->position, operateurS->axe};
    // On libère les anciennes cases sur le plateau.
    for (Case &coord : concretePiece.coordinates)
    {
        plateau[coord.coords.first][coord.coords.second].is_busy = false;
    }

    ApplyTransformationToOperateur(concretePiece, operateurS);

    // On applique le déplacement.
    // On prends la première case de la pièce comme référence.
    p->trigger(concretePiece.coordinates[0]);

    // On occupe les nouvelles cases sur le plateau.
    for (Case &coord : concretePiece.coordinates)
    {
        plateau[coord.coords.first][coord.coords.second].is_busy = true;
    }
}

void Plateau::ApplyTransformationToOperateur(PieceConcrete &concretePiece, PieceOperateur *operateur_a_appliquer)
{
    if (OperateurDeplacement *operateur_type = dynamic_cast<OperateurDeplacement *>(operateur_a_appliquer))
    {
        move_operator(concretePiece, operateur_type);
    }

    if (OperateurRotation *operateur_type = dynamic_cast<OperateurRotation *>(operateur_a_appliquer))
    {
        rotate_operator(concretePiece, operateur_type);
    }

    if (OperateurSymetrie *operateur_type = dynamic_cast<OperateurSymetrie *>(operateur_a_appliquer))
    {
        symetrie_operator(concretePiece, operateur_type);
    }
}

void Plateau::symetrie_operator(PieceConcrete &concretePiece, OperateurSymetrie *operateur_type)
{
    OperateurSymetrie op_futur(concretePiece, operateur_type->position, operateur_type->axe);

    std::vector<OperateurDeplacement *> appliedDeplacementOps;
    std::vector<OperateurRotation *> appliedRotationOps;
    std::vector<OperateurSymetrie *> appliedSymetrieOps;

    for (const Case &case_piece : concretePiece.getCoordinates())
    {
        for (OperateurDeplacement &op_deplacement : operateursDeplacement)
        {
            if (case_piece.coords == op_deplacement.position.coords)
            {
                if (std::find(appliedDeplacementOps.begin(), appliedDeplacementOps.end(), &op_deplacement) == appliedDeplacementOps.end())
                {
                    op_deplacement.accept(op_futur);
                    appliedDeplacementOps.push_back(&op_deplacement);

                    switch (op_futur.axe)
                    {
                    case HORIZONTAL:
                        if (op_deplacement.sens == EST)
                            op_deplacement.sens = OUEST;
                        else if (op_deplacement.sens == OUEST)
                            op_deplacement.sens = EST;
                        break;

                    case VERTICAL:
                        if (op_deplacement.sens == NORD)
                            op_deplacement.sens = SUD;
                        else if (op_deplacement.sens == SUD)
                            op_deplacement.sens = NORD;
                        break;

                    default:
                        break;
                    }
                }
            }
        }

        for (OperateurRotation &op_rotation : operateursRotation)
        {
            if (case_piece.coords == op_rotation.position.coords)
            {
                if (std::find(appliedRotationOps.begin(), appliedRotationOps.end(), &op_rotation) == appliedRotationOps.end())
                {
                    op_rotation.accept(op_futur);
                    appliedRotationOps.push_back(&op_rotation);
                }
            }
        }

        for (OperateurSymetrie &op_symetrie : operateursSymetrie)
        {
            if (case_piece.coords == op_symetrie.position.coords)
            {
                if (std::find(appliedSymetrieOps.begin(), appliedSymetrieOps.end(), &op_symetrie) == appliedSymetrieOps.end())
                {
                    op_symetrie.accept(op_futur);
                    appliedSymetrieOps.push_back(&op_symetrie);
                }
            }
        }
    }
}

void Plateau::move_operator(PieceConcrete &concretePiece, OperateurDeplacement *operateur_type)
{
    OperateurDeplacement op_futur(concretePiece, operateur_type->position, operateur_type->sens);

    std::vector<OperateurDeplacement *> appliedDeplacementOps;
    std::vector<OperateurRotation *> appliedRotationOps;
    std::vector<OperateurSymetrie *> appliedSymetrieOps;

    for (const Case &case_piece : concretePiece.getCoordinates())
    {
        for (OperateurDeplacement &op_deplacement : operateursDeplacement)
        {
            if (case_piece.coords == op_deplacement.position.coords)
            {
                if (std::find(appliedDeplacementOps.begin(), appliedDeplacementOps.end(), &op_deplacement) == appliedDeplacementOps.end())
                {
                    op_deplacement.accept(op_futur);
                    appliedDeplacementOps.push_back(&op_deplacement);
                }
            }
        }

        for (OperateurRotation &op_rotation : operateursRotation)
        {
            if (case_piece.coords == op_rotation.position.coords)
            {
                if (std::find(appliedRotationOps.begin(), appliedRotationOps.end(), &op_rotation) == appliedRotationOps.end())
                {
                    op_rotation.accept(op_futur);
                    appliedRotationOps.push_back(&op_rotation);
                }
            }
        }

        for (OperateurSymetrie &op_symetrie : operateursSymetrie)
        {
            if (case_piece.coords == op_symetrie.position.coords)
            {
                if (std::find(appliedSymetrieOps.begin(), appliedSymetrieOps.end(), &op_symetrie) == appliedSymetrieOps.end())
                {
                    op_symetrie.accept(op_futur);
                    appliedSymetrieOps.push_back(&op_symetrie);
                }
            }
        }
    }
}

void Plateau::rotate_operator(PieceConcrete &concretePiece, OperateurRotation *operateur_type)
{
    OperateurRotation op_futur(concretePiece, operateur_type->position, operateur_type->sens);

    std::vector<OperateurDeplacement *> appliedDeplacementOps;
    std::vector<OperateurRotation *> appliedRotationOps;
    std::vector<OperateurSymetrie *> appliedSymetrieOps;

    for (const Case &case_piece : concretePiece.getCoordinates())
    {
        for (OperateurDeplacement &op_deplacement : operateursDeplacement)
        {
            if (case_piece.coords == op_deplacement.position.coords)
            {
                if (std::find(appliedDeplacementOps.begin(), appliedDeplacementOps.end(), &op_deplacement) == appliedDeplacementOps.end())
                {
                    op_deplacement.accept(op_futur);
                    appliedDeplacementOps.push_back(&op_deplacement);

                    switch (op_futur.sens)
                    {
                    case HORAIRE:
                        if (op_deplacement.sens == NORD)
                            op_deplacement.sens = EST;
                        else if (op_deplacement.sens == EST)
                            op_deplacement.sens = SUD;
                        else if (op_deplacement.sens == SUD)
                            op_deplacement.sens = OUEST;
                        else if (op_deplacement.sens == OUEST)
                            op_deplacement.sens = NORD;
                        break;

                    case ANTIHORAIRE:
                        if (op_deplacement.sens == NORD)
                            op_deplacement.sens = OUEST;
                        else if (op_deplacement.sens == EST)
                            op_deplacement.sens = NORD;
                        else if (op_deplacement.sens == SUD)
                            op_deplacement.sens = EST;
                        else if (op_deplacement.sens == OUEST)
                            op_deplacement.sens = SUD;
                        break;

                    default:
                        break;
                    }
                }
            }
        }

        for (OperateurRotation &op_rotation : operateursRotation)
        {
            if (case_piece.coords == op_rotation.position.coords)
            {
                if (std::find(appliedRotationOps.begin(), appliedRotationOps.end(), &op_rotation) == appliedRotationOps.end())
                {
                    op_rotation.accept(op_futur);
                    appliedRotationOps.push_back(&op_rotation);
                }
            }
        }

        for (OperateurSymetrie &op_symetrie : operateursSymetrie)
        {
            if (case_piece.coords == op_symetrie.position.coords)
            {
                if (std::find(appliedSymetrieOps.begin(), appliedSymetrieOps.end(), &op_symetrie) == appliedSymetrieOps.end())
                {
                    op_symetrie.accept(op_futur);
                    appliedSymetrieOps.push_back(&op_symetrie);

                    if (op_symetrie.axe == VERTICAL)
                        op_symetrie.axe = HORIZONTAL;
                    else if (op_symetrie.axe == HORIZONTAL)
                        op_symetrie.axe = VERTICAL;
                }
            }
        }
    }
}