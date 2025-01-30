// clear &&make clean &&make ebaucheModel &&./ ebaucheModel

// QUESTIONS POUR LE PROF
// SI ON RESPECTE SA STRUCTURE POUR LES PIECES? PEUX TON GARDER LA STRUCTURE DE PLATEAU QUE NOUS AVONS FAIT EN UTILISANT CORRECTEMENT DECORATOR/VISITOR ?
// EGALEMENT, EST-CE POSSIBLE DE GARDER LE SYSTEME DE CASES POUR UNE PIECE CONCRETE A LA PLACE DU VECTEUR INT,INT ?

#include "niveaux.hpp"
// -------------------------------------------------------

using namespace std;

static int MARGIN_LEFT = 200, MARGIN_RIGHT = 200, MARGIN_TOP = 200, MARGIN_BOTTOM = 200;
static int TILE_SIZE = 64;
static int NB_COL = 8, NB_LIGNE = 6;
static unsigned int nbPix_x = MARGIN_LEFT + MARGIN_RIGHT + (NB_COL * TILE_SIZE);
static unsigned int nbPix_y = MARGIN_TOP + MARGIN_BOTTOM + (NB_LIGNE * TILE_SIZE);

void lvl2()
{

    RectangleShape centralPane(Vector2f(NB_COL * TILE_SIZE, NB_LIGNE * TILE_SIZE));
    // On vient positionner le panneau central.
    centralPane.setPosition(MARGIN_LEFT, MARGIN_TOP);

    sf::RenderWindow window(sf::VideoMode(nbPix_x, nbPix_y), "Piece Out", sf::Style::Titlebar | sf::Style::Close);

   vector<vector<int>> array = {
    {0, 0, 0, 0, 1, 0}, //[0,5]
    {0, 0, 0, 0, 1, 0},
    {0, 1, 1, 1, 1, 0},
    {0, 1, 1, 1, 1, 0},
    {1, 1, 1, 1, 1, 1},
    {1, 1, 1, 1, 1, 1},
    {0, 0, 0, 0, 1, 0},
    {0, 0, 0, 0, 1, 0},//[5,7]
};
    ////////////////////// BUT /////////////////////
    std::vector<Case> coordinates1_goal = {Case({0, 4}), Case({1, 4})};
    PieceConcrete piece1_goal(coordinates1_goal, Color::Red);
    ///////////////////////////////////////////////

    std::vector<Case> coordinates1 = {Case({5, 3}), Case({5, 4}), Case({5, 5}), Case({4, 5})};
    std::vector<Case> coordinates2 = {Case({7, 4}), Case({6, 4})};
    std::vector<Case> coordinates3 = {Case({3, 2}), Case({3, 3}), Case({4, 3}), Case({3, 4})};
    std::vector<Case> coordinates4 = {Case({4, 0}), Case({4, 1}), Case({4, 2}), Case({5, 2})};

    // PieceConcrete
    PieceConcrete piece1(coordinates1, Color::Magenta);
    PieceConcrete piece2(coordinates2, Color::Red);
    PieceConcrete piece3(coordinates3, Color::Green);
    PieceConcrete piece4(coordinates4, Color::Blue);

    // CrÃ©er un vecteur de PieceConcrete
    std::vector<PieceConcrete> pieces = {piece1, piece2, piece3, piece4 };
    std::vector<PieceConcrete> pieces_goal = {piece1_goal};

    // OPERATEURS Deplacement 
    // Magenta
    OperateurDeplacement operateur_deplacement_1 = OperateurDeplacement(piece1, piece1.getCoordinates()[0], OrientationDeplacement(OUEST));
    OperateurDeplacement operateur_deplacement_2 = OperateurDeplacement(piece1, piece1.getCoordinates()[2], OrientationDeplacement(EST));
    //Bleu 
    OperateurDeplacement operateur_deplacement_3 = OperateurDeplacement(piece4, piece4.getCoordinates()[0], OrientationDeplacement(OUEST));
    OperateurDeplacement operateur_deplacement_4 = OperateurDeplacement(piece4, piece4.getCoordinates()[2], OrientationDeplacement(EST));
    //Rouge 
    OperateurDeplacement operateur_deplacement_5 = OperateurDeplacement(piece2, piece2.getCoordinates()[0], OrientationDeplacement(SUD));
    OperateurDeplacement operateur_deplacement_6 = OperateurDeplacement(piece2, piece2.getCoordinates()[1], OrientationDeplacement(NORD));


    // Ajout dans le vecteur
    vector<OperateurDeplacement> operatorsD;
    operatorsD.push_back(operateur_deplacement_1);
    operatorsD.push_back(operateur_deplacement_2);
    operatorsD.push_back(operateur_deplacement_3);
    operatorsD.push_back(operateur_deplacement_4);
    operatorsD.push_back(operateur_deplacement_5);
    operatorsD.push_back(operateur_deplacement_6);



    //OPERATEUR ROTATION 
    //Vert 
    OperateurRotation operateur_rotation_1 = OperateurRotation(piece3, piece3.getCoordinates()[1], SensRotation(ANTIHORAIRE));

    //Bleu 
    OperateurRotation operateur_rotation_2 = OperateurRotation(piece4, piece4.getCoordinates()[1], SensRotation(ANTIHORAIRE));

    std::vector<OperateurRotation> operatorsR;
    operatorsR.push_back(operateur_rotation_1);
    operatorsR.push_back(operateur_rotation_2);
    

    //OperateurSymetrie operateur_symetrie = OperateurSymetrie(piece1, piece1.getCoordinates()[1], AxeSymetrie(VERTICAL));

    std::vector<OperateurSymetrie> operatorsS;
  

    Plateau plateau = Plateau{
        NB_COL,
        NB_COL,
        array,
        pieces,
        pieces_goal,
        operatorsD,
        operatorsR,
        operatorsS};

    // cout << plateau.isMovePossible(3, OrientationDeplacement(NORD)) << endl;
    while (window.isOpen())
    {
        Event event;
        while (window.pollEvent(event))
        {
            // FERMETURE DE L'EXE
            if (event.type == Event::Closed)
                window.close();

            // FLECHES DU CLAVIER
            if (event.type == Event::KeyPressed && plateau.highlighted_piece != -1)
            {
                switch (event.key.code)
                {
                case Keyboard::Up:
                    plateau.movePiece(plateau.highlighted_piece, OrientationDeplacement(NORD));
                    break;
                case Keyboard::Down:
                    plateau.movePiece(plateau.highlighted_piece, OrientationDeplacement(SUD));
                    break;
                case Keyboard::Right:
                    plateau.movePiece(plateau.highlighted_piece, OrientationDeplacement(EST));
                    break;
                case Keyboard::Left:
                    plateau.movePiece(plateau.highlighted_piece, OrientationDeplacement(OUEST));
                    break;
                case Keyboard::A:
                    plateau.rotatePiece(plateau.highlighted_piece, SensRotation(ANTIHORAIRE));
                    break;
                case Keyboard::H: // Rotation horaire
                    plateau.rotatePiece(plateau.highlighted_piece, SensRotation(HORAIRE));
                    break;
                case Keyboard::S:
                    plateau.symetriePiece(plateau.highlighted_piece, AxeSymetrie(HORIZONTAL));
                    break;
                case Keyboard::V:
                    plateau.symetriePiece(plateau.highlighted_piece, AxeSymetrie(VERTICAL));
                    break;
                default:
                    break;
                }
            }

            // CLICK DROIT SOURIS
            if (event.type == Event::MouseButtonPressed && event.mouseButton.button == Mouse::Left)
            {
                Vector2i mousePos = Mouse::getPosition(window);
                plateau.handleClick(mousePos, TILE_SIZE, MARGIN_RIGHT);
            }
        }

        window.clear(sf::Color::Black);

        plateau.drawGrid(window, TILE_SIZE, MARGIN_RIGHT);
        window.display();
    }

}