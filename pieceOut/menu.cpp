#include <SFML/Graphics.hpp>
#include <iostream>
#include "niveaux.hpp"

int main()
{
    sf::RenderWindow window(sf::VideoMode(900, 700), "Piece Out", sf::Style::Titlebar | sf::Style::Close);

    sf::Font font;
    if (!font.loadFromFile("./fonts/sewer.TTF"))
    {
        std::cerr << "Erreur de chargement de la police!" << std::endl;
        return -1;
    }

    sf::Text title("Piece Out", font, 50);
    title.setPosition((window.getSize().x - title.getLocalBounds().width) / 2, 50);

    sf::Text level1("Level 1", font, 30);
    sf::Text level2("Level 2", font, 30);
    sf::Text level3("Level 3", font, 30);
    sf::Text quit("Quitter", font, 30);

    float SpacerV = 50;

    level1.setPosition((window.getSize().x - level1.getLocalBounds().width) / 2, 150);
    level2.setPosition((window.getSize().x - level2.getLocalBounds().width) / 2, level1.getPosition().y + level1.getLocalBounds().height + SpacerV);
    level3.setPosition((window.getSize().x - level3.getLocalBounds().width) / 2, level2.getPosition().y + level2.getLocalBounds().height + SpacerV);
    quit.setPosition((window.getSize().x - quit.getLocalBounds().width) / 2, level3.getPosition().y + level3.getLocalBounds().height + SpacerV);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
            {
                window.close();
            }

            if (event.type == sf::Event::MouseButtonPressed)
            {
                if (event.mouseButton.button == sf::Mouse::Left)
                {
                    sf::Vector2i mousePos = sf::Mouse::getPosition(window);

                    if (level1.getGlobalBounds().contains(mousePos.x, mousePos.y))
                    {
                        std::cout << "Niveau 1, bon jeu !" << std::endl;
                        lvl1();
                    }
                    else if (level2.getGlobalBounds().contains(mousePos.x, mousePos.y))
                    {
                        std::cout << "Niveau 2, bon jeu !" << std::endl;
                        lvl2();
                    }
                    else if (level3.getGlobalBounds().contains(mousePos.x, mousePos.y))
                    {
                        std::cout << "Niveau 3, bon jeu !" << std::endl;
                        lvl3();
                    }
                    else if (quit.getGlobalBounds().contains(mousePos.x, mousePos.y))
                    {
                        std::cout << "Au revoir, à bientôt !" << std::endl;
                        std::cout << "Piece Out by Aymane Ichou et Théo Lemaire." << std::endl;
                        window.close();
                    }
                }
            }
        }

        window.clear(sf::Color(135, 206, 235));
        window.draw(title);
        window.draw(level1);
        window.draw(level2);
        window.draw(level3);
        window.draw(quit);
        window.display();
    }

    return 0;
}
