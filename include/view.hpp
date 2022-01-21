#pragma once

#include <memory>
#include <iostream>

#include "corners_model.hpp"

#include <SFML/Graphics.hpp>

class view_object {
public:
	void draw(sf::RenderWindow& window) {
		window.draw(m_sprite);
	};

	void load_sprite(const std::string& filename) {
		m_texture.loadFromFile(filename);
		m_sprite.setTexture(m_texture);

		m_size = m_texture.getSize();
	}

	sf::Vector2u shape() {
		return m_size;
	}

	sf::Vector2f pos() const {
		return m_sprite.getPosition();
	}

	void set_pos(float x, float y) {
		m_sprite.setPosition(x, y);
	}

	void set_scale(float sx, float sy) {
		m_sprite.setScale(sx, sy);
	}
	
protected:
	sf::Vector2u m_size;
	sf::Sprite m_sprite;
	sf::Texture m_texture;
};

class black_piece : public view_object {
public:
	black_piece() {
		load_sprite("black_figure.png");
	}
};

class white_piece : public view_object {
public:
	white_piece() {
		load_sprite("white_figure.png");
	}
};

class board : public view_object {
public:
	board() {
		load_sprite("board_img.jpg");
	}
};

class active_cell : public view_object {
public:
	active_cell() {
		load_sprite("mark.png");
	}

	bool active() const {
		return m_active;
	}

	void set_active(bool flag) {
		m_active = flag;
	}

private:
	bool m_active{ false };
};

class corners_view {
	using piece_holder = std::shared_ptr<view_object>;

public:
	corners_view(uint16_t width, uint16_t heigth) {
		board.set_pos(0.f, 0.f);
		board.set_scale(
			float(width) / board.shape().x,
			float(heigth) / board.shape().y
		);

		m_pieces.resize(8);
		for (int8_t i = 0; i < 8; ++i)
			m_pieces[i].resize(8);

		m_window = std::make_shared<sf::RenderWindow>(sf::VideoMode(width, heigth), "Corners");
		m_window->setVerticalSyncEnabled(true);

		crate_pieces(width, heigth);
	}


	void loop() {
		while (m_window->isOpen()) {
			make_sizes();

			board.draw(*m_window);
			draw_pieces();

			if (m_active_cell.active())
				m_active_cell.draw(*m_window);

			m_window->display();

			sf::Event event;
			while (m_window->pollEvent(event)) {
				if (event.type == sf::Event::Closed)
					m_window->close();

				if (
					event.mouseButton.button == sf::Mouse::Left
					&& event.type == sf::Event::MouseButtonPressed
				) {
					process_click(event.mouseButton.x, event.mouseButton.y);

					std::cout << "the left button was pressed" << std::endl;
					std::cout << "mouse x: " << event.mouseButton.x << std::endl;
					std::cout << "mouse y: " << event.mouseButton.y << std::endl;
				}
			}
			

		}
	}

private:
	void make_sizes() {
		auto size = m_window->getView().getSize();

		m_border_size_x = border_coeff * size.x;
		m_border_size_y = border_coeff * size.y;
		m_cell_size_x = (float(size.x) - 2.f * m_border_size_x) / 8.f;
		m_cell_size_y = (float(size.y) - 2.f * m_border_size_y) / 8.f;
	}

	void crate_pieces(const int width, const int heigth) {
		make_sizes();

		for (const auto& piece : m_model.pieces()) {
			if (piece->m_color == white)
				m_pieces[piece->m_pos.x][piece->m_pos.y] = std::make_shared<white_piece>();
			else
				m_pieces[piece->m_pos.x][piece->m_pos.y] = std::make_shared<black_piece>();

			m_pieces[piece->m_pos.x][piece->m_pos.y]->set_scale(
				m_cell_size_x / m_pieces[piece->m_pos.x][piece->m_pos.y]->shape().x,
				m_cell_size_y / m_pieces[piece->m_pos.x][piece->m_pos.y]->shape().y
			);
		}

		m_active_cell.set_scale(
			m_cell_size_x / m_active_cell.shape().x,
			m_cell_size_x / m_active_cell.shape().y
		);
	}

	void draw_pieces() {
		for (const auto& piece : m_model.pieces()) {
			auto coords = cell_to_coord(piece->m_pos.x, piece->m_pos.y);

			m_pieces[piece->m_pos.x][piece->m_pos.y]->set_pos(coords.x, coords.y);
			m_pieces[piece->m_pos.x][piece->m_pos.y]->draw(*m_window);
		}
	}

	void process_click(int x, int y) {
		auto cell = coord_to_cell(x, y);

		if (
			cell.x < 0 || cell.x >= 8
			|| cell.y < 0 || cell.y >= 8
		)
			return;

		auto piece = m_model.get(cell.x, cell.y);

		if (m_active_cell.active()) {
			auto current_pos = m_active_cell.pos();
			auto current_cell = coord_to_cell(current_pos.x, current_pos.y);

			if (current_cell == cell || !piece || piece->m_color != white) {
				m_active_cell.set_active(false);
				return;
			}
		}

		if (!piece || piece->m_color != white)
			return;

		auto coords = cell_to_coord(cell.x, cell.y);
		m_active_cell.set_pos(coords.x, coords.y);

		m_active_cell.set_active(true);
	}

	sf::Vector2i coord_to_cell(int x, int y) {
		return {
			(int)(float(x - m_border_size_x + 0.01 * m_cell_size_x) / m_cell_size_x),
			(int)(float(y - m_border_size_y + 0.01 * m_cell_size_y) / m_cell_size_y)
		};
	}

	sf::Vector2f cell_to_coord(int cx, int cy) {
		return { m_border_size_x + m_cell_size_x * cx, m_border_size_y + m_cell_size_y * cy };
	}

	inline static const float border_coeff = 0.068;

	board board;
	corners_model m_model;
	active_cell m_active_cell;
	std::shared_ptr<sf::RenderWindow> m_window;
	std::vector<std::vector<piece_holder>> m_pieces;

	float m_cell_size_x;
	float m_cell_size_y;
	float m_border_size_x;
	float m_border_size_y;
};
