#ifndef _CASE
#define _CASE

#include <utility> // pour pair
#include <vector>
#include <SFML/Graphics.hpp>
#include <iostream>

using namespace std;
using namespace sf;

class Case
{
public:
    pair<int, int> coords;
    bool is_busy;    // La case est occupée par une pièce ?
    bool is_colored; // Fait partie des cases goal.
    bool is_real;    // La case fait partie du plateau de jeu ?
    pair<bool, Color> is_goal;

    Case(pair<int, int> coordinates);

    ~Case();

    Case(const Case &other);

    bool operator==(const Case &other) const;
};

#endif