#pragma once

#include <memory>

#include "corners_model.hpp"

#include <SFML/Graphics.hpp>


#define VIEW_PIECE_OBJECT(class_name, image_path)										\
public:																					\
	class_name() {																		\
		init_static();																	\
		m_sprite.setTexture(m_texture);													\
		m_sprite.setScale({ 0.4, 0.4 });												\
	}																					\
																						\
	sf::Sprite& sprite() override {														\
		return m_sprite;																\
	}																					\
private:																				\
	sf::Sprite m_sprite;																\
																						\
	inline static std::string img_path = image_path;									\
	inline static sf::Image m_image = sf::Image();										\
	inline static sf::Texture m_texture = sf::Texture();								\
	inline static bool m_is_initialized = false;										\
																						\
	static void init_static() {															\
		if (!m_is_initialized) {														\
			m_image.loadFromFile(img_path);												\
			m_texture.loadFromImage(m_image);											\
			m_is_initialized = true;													\
		}																				\
	}																					\


class view_object {
public:
	virtual sf::Sprite& sprite() = 0;
};


class black_piece : public view_object {
	VIEW_PIECE_OBJECT(black_piece, "black_figure.png");
public:
	black_piece(int x, int y) : black_piece() {
		m_sprite.setPosition(x, y);
	}
};

class white_piece : public view_object {
	VIEW_PIECE_OBJECT(white_piece, "white_figure.png");
public:
	white_piece(int x, int y) : white_piece() {
		m_sprite.setPosition(x, y);
	}
};


class corners_view {
	using piece_holder = std::shared_ptr<view_object>;

public:
	corners_view(uint16_t width, uint16_t heigth) {
		init_static();

		m_pieces.resize(8);
		for (int8_t i = 0; i < 8; ++i)
			m_pieces[i].resize(8);

		for (const auto& piece : m_model.pieces(white))
			m_pieces[piece->m_pos.x][piece->m_pos.y] = std::make_shared<white_piece>(
				100 * piece->m_pos.x,
				100 * piece->m_pos.y
			);

		for (const auto& piece : m_model.pieces(black))
			m_pieces[piece->m_pos.x][piece->m_pos.y] = std::make_shared<black_piece>(
				100 * piece->m_pos.x,
				100 * piece->m_pos.y
			);

		m_window = std::make_shared<sf::RenderWindow>(sf::VideoMode(width, heigth), "Corners");
	}


	void loop() {
		while (m_window->isOpen())
		{
			m_window->draw(m_board_sprite);

			for (const auto& row : m_pieces)
				for (const auto& vpiece : row) {
					if (vpiece)
						m_window->draw(vpiece->sprite());
				}

			m_window->display();

			sf::Event event;
			while (m_window->pollEvent(event))
				if (event.type == sf::Event::Closed)
					m_window->close();
		}
	}

private:
	corners_model m_model;

	std::vector<std::vector<piece_holder>> m_pieces;

	std::shared_ptr<sf::RenderWindow> m_window;

	static sf::Image m_board_img;
	static sf::Texture m_board_texture;
	static sf::Sprite m_board_sprite;

	static bool m_is_initialized;

	static void init_static() {
		if (!m_is_initialized) {
			m_board_img.loadFromFile("board_img.jpg");
			m_board_texture.loadFromImage(m_board_img);

			m_board_sprite.setTexture(m_board_texture);
			m_board_sprite.setPosition({ 0, 0 });
			m_board_sprite.setScale({ 0.5, 0.5 });

			m_is_initialized = true;
		}
	}
};

sf::Image corners_view::m_board_img = sf::Image();
sf::Texture corners_view::m_board_texture = sf::Texture();
sf::Sprite corners_view::m_board_sprite = sf::Sprite();
bool corners_view::m_is_initialized = false;