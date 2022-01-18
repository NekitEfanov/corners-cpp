#include "controller.hpp"
#include "view.hpp"
#include <iostream>

#include <SFML/Graphics.hpp>

using namespace std;
using namespace sf;

int main()
{
	corners_view test(1200, 800);

	test.loop();

	//RenderWindow window(VideoMode(1200, 800), "Corners");


 //   Image board_img;
 //   board_img.loadFromFile("board_img.jpg");

 //   Texture border_texture;
 //   border_texture.loadFromImage(board_img);

 //   Sprite border_sprite;
 //   border_sprite.setTexture(border_texture);
 //   border_sprite.setPosition({ 0, 0 });
 //   border_sprite.setScale({ 0.5, 0.5 });

 //   Image black_figure_img;
 //   black_figure_img.loadFromFile("black_figure.png");


 //   window.setIcon(black_figure_img.getSize().x,
 //       black_figure_img.getSize().y,
 //       black_figure_img.getPixelsPtr());

 //   Texture black_figure_texture;
 //   black_figure_texture.loadFromImage(black_figure_img);

 //   Sprite black_figure_sprite;
 //   black_figure_sprite.setTexture(black_figure_texture);
 //   black_figure_sprite.setPosition({ 60, 60 });
 //   black_figure_sprite.setScale({ 0.4, 0.4 });


 //   while (window.isOpen())
 //   {
 //       window.draw(border_sprite);
 //       window.draw(black_figure_sprite);

 //       window.display();

 //       Event event;
 //       while (window.pollEvent(event))
 //           if (event.type == Event::Closed)
 //               window.close();
 //   }

	return 0;
}
