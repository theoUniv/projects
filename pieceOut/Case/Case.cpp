#include <utility>
#include <vector>
#include <SFML/Graphics.hpp>
#include <iostream>
#include "Case.hpp"

using namespace std;
using namespace sf;

Case::Case(pair<int, int> coordinates)
    : coords(coordinates), is_busy(false), is_colored(false), is_real(false), is_goal(false, Color::White)
{
    // cout << "Création de case : x:" << coords.first << " y:" << coords.second << endl;
}

Case::~Case()
{
    // cout << "Destruction de case : x:" << coords.first << " y:" << coords.second << endl;
}

Case::Case(const Case &other)
    : coords(other.coords),
      is_busy(other.is_busy),
      is_colored(other.is_colored),
      is_real(other.is_real) {}

bool Case::operator==(const Case &other) const
{
    return this->coords == other.coords;
}