#include "controller.hpp"
#include "view.hpp"
#include <iostream>

#include <SFML/Graphics.hpp>

using namespace std;
using namespace sf;

int main()
{
	corners_view test(800, 800);

	test.loop();

	return 0;
}