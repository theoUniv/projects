// clear &&make clean &&make ebaucheModel &&./ ebaucheModel

#include "niveaux.hpp"

using namespace std;

static int MARGIN_LEFT = 100, MARGIN_RIGHT = 100, MARGIN_TOP = 100, MARGIN_BOTTOM = 100;
static int TILE_SIZE = 64;
static int NB_COL = 6, NB_LIGNE = 6;
static unsigned int nbPix_x = MARGIN_LEFT + MARGIN_RIGHT + (NB_COL * TILE_SIZE);
static unsigned int nbPix_y = MARGIN_TOP + MARGIN_BOTTOM + (NB_LIGNE * TILE_SIZE);

void lvl3()
{

    RectangleShape centralPane(Vector2f(NB_COL * TILE_SIZE, NB_LIGNE * TILE_SIZE));
    // On vient positionner le panneau central.
    centralPane.setPosition(MARGIN_LEFT, MARGIN_TOP);

    sf::RenderWindow window(sf::VideoMode(nbPix_x, nbPix_y), "Piece Out", sf::Style::Titlebar | sf::Style::Close);

    vector<vector<int>> array = {
        {1, 1, 1, 1, 1, 1},
        {1, 1, 1, 1, 1, 1},
        {0, 1, 1, 1, 1, 1},
        {0, 1, 1, 1, 1, 1},
        {0, 1, 1, 1, 1, 1},
        {0, 1, 1, 1, 1, 1},
    };

    ////////////////////// BUT /////////////////////
    std::vector<Case> coordinates1_goal = {Case({0, 0}), Case({1, 1}), Case({1, 0}), Case({0, 1})};
    PieceConcrete piece1_goal(coordinates1_goal, Color::Red);
    ///////////////////////////////////////////////

    std::vector<Case> coordinates1 = {Case({2, 2}), Case({1, 2}), Case({0, 2}), Case({2, 3})};
    std::vector<Case> coordinates2 = {Case({4, 1}), Case({4, 2}), Case({5, 1}), Case({5, 2})};
    std::vector<Case> coordinates3 = {Case({3, 4}), Case({4, 4}), Case({5, 4}), Case({5, 3})};
    std::vector<Case> coordinates4 = {Case({2, 5}), Case({3, 5}), Case({4, 5})};

    // PieceConcrete
    PieceConcrete piece1(coordinates1, Color::Magenta);
    PieceConcrete piece2(coordinates2, Color::Red);
    PieceConcrete piece3(coordinates3, Color::Blue);
    PieceConcrete piece4(coordinates4, Color::Cyan);

    // Créer un vecteur de PieceConcrete
    std::vector<PieceConcrete> pieces = {piece1, piece2, piece3, piece4};
    std::vector<PieceConcrete> pieces_goal = {piece1_goal};

    // Operateur deplacement
    //Magenta
    OperateurDeplacement operateur_deplacement_1 = OperateurDeplacement(piece1, piece1.getCoordinates()[2], OrientationDeplacement(NORD));
    OperateurDeplacement operateur_deplacement_2 = OperateurDeplacement(piece1, piece1.getCoordinates()[0], OrientationDeplacement(SUD));
    //Rouge
    OperateurDeplacement operateur_deplacement_3 = OperateurDeplacement(piece2, piece2.getCoordinates()[0], OrientationDeplacement(NORD));
    OperateurDeplacement operateur_deplacement_4 = OperateurDeplacement(piece2, piece2.getCoordinates()[1], OrientationDeplacement(EST));
    OperateurDeplacement operateur_deplacement_5 = OperateurDeplacement(piece2, piece2.getCoordinates()[3], OrientationDeplacement(SUD));
    OperateurDeplacement operateur_deplacement_6 = OperateurDeplacement(piece2, piece2.getCoordinates()[2], OrientationDeplacement(OUEST));
    //Blue
    OperateurDeplacement operateur_deplacement_7 = OperateurDeplacement(piece3, piece3.getCoordinates()[0], OrientationDeplacement(NORD)) ; 
    OperateurDeplacement operateur_deplacement_8 = OperateurDeplacement(piece3, piece3.getCoordinates()[2], OrientationDeplacement(SUD)) ; 
    //Cyan
    OperateurDeplacement operateur_deplacement_9 =  OperateurDeplacement(piece4, piece4.getCoordinates()[0], OrientationDeplacement(NORD)) ; 
    OperateurDeplacement operateur_deplacement_10 = OperateurDeplacement(piece4, piece4.getCoordinates()[2], OrientationDeplacement(SUD)) ; 



    // Ajout dans le vecteur
    vector<OperateurDeplacement> operatorsD;
    operatorsD.push_back(operateur_deplacement_1);
    operatorsD.push_back(operateur_deplacement_2);
    operatorsD.push_back(operateur_deplacement_3);
    operatorsD.push_back(operateur_deplacement_4);
    operatorsD.push_back(operateur_deplacement_5);
    operatorsD.push_back(operateur_deplacement_6);
    operatorsD.push_back(operateur_deplacement_7);
    operatorsD.push_back(operateur_deplacement_8);
    operatorsD.push_back(operateur_deplacement_9);
    operatorsD.push_back(operateur_deplacement_10);



    OperateurRotation operateur_rotation_1 = OperateurRotation(piece3, piece3.getCoordinates()[1], SensRotation(ANTIHORAIRE));

    std::vector<OperateurRotation> operatorsR;
    operatorsR.push_back(operateur_rotation_1);

    OperateurSymetrie operateur_symetrie = OperateurSymetrie(piece1, piece1.getCoordinates()[1], AxeSymetrie(VERTICAL));

    std::vector<OperateurSymetrie> operatorsS;
    operatorsS.push_back(operateur_symetrie);

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