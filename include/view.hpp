#pragma once

#include <memory>
#include <iostream>

#include "corners_model.hpp"
#include "dfs_ai.hpp"

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

class movement_mark : public active_cell {
public:
	movement_mark() {
		load_sprite("point.png");
	}
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

			if (m_active_cell.active()) {
				m_active_cell.draw(*m_window);
				draw_movements();
			}

			m_window->display();

			// Сначала рисуем ход за белых, только потом
			// отвечаем. Иначе ход не отобразится вовремя.
			if (m_waiting_answer) {
				float w;
				auto answer = find_the_best_move(black, m_model, 5, 0, w);
				sf::Vector2i c = { (int)answer.second.x, (int)answer.second.y };
				move(answer.first, c);

				m_waiting_answer = false;
			}

			sf::Event event;
			while (m_window->pollEvent(event)) {
				if (event.type == sf::Event::Closed)
					m_window->close();

				if (
					event.mouseButton.button == sf::Mouse::Left
					&& event.type == sf::Event::MouseButtonPressed
				)
					process_click(event.mouseButton.x, event.mouseButton.y);
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

		m_movement_mark.set_scale(
			m_cell_size_x / m_movement_mark.shape().x,
			m_cell_size_x / m_movement_mark.shape().y
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
		auto clicked_cell = coord_to_cell(x, y);

		if (
			clicked_cell.x < 0 || clicked_cell.x >= 8
			|| clicked_cell.y < 0 || clicked_cell.y >= 8
		)
			return;

		if (m_active_cell.active()) {
			bool moved = process_with_active_piece(clicked_cell);

			if (moved)
				return;
		}

		auto piece = m_model.get(cell{ (int8_t)clicked_cell.x, (int8_t)clicked_cell.y });
		if (!piece || piece->m_color != white)
			return;

		auto coords = cell_to_coord(clicked_cell.x, clicked_cell.y);
		m_active_cell.set_pos(coords.x, coords.y);

		m_active_cell.set_active(true);
	}

	bool process_with_active_piece(const sf::Vector2i& dst_cell) {
		bool result = false;

		auto current_pos = m_active_cell.pos();
		auto current_cell = coord_to_cell(current_pos.x, current_pos.y);
		
		auto clicked_piece = m_model.get(cell{ (int8_t)dst_cell.x, (int8_t)dst_cell.y });
		auto active_piece = m_model.get(cell{ (int8_t)current_cell.x, (int8_t)current_cell.y });

		if (is_move(active_piece, dst_cell)) {
			move(active_piece, dst_cell);
			result = true;
		}

		m_active_cell.set_active(false);

		return result;
	}

	bool is_move(std::shared_ptr<piece> piece, const sf::Vector2i& pos) {
		return m_model.is_move_possible(*piece, cell{ (int8_t)pos.x, (int8_t)pos.y });
	}

	void move(std::shared_ptr<piece> piece, const sf::Vector2i& pos) {
		swap(
			m_pieces[piece->m_pos.x][piece->m_pos.y],
			m_pieces[pos.x][pos.y]
		);
		m_model.move(*piece, cell{ (int8_t)pos.x, (int8_t)pos.y });

		m_waiting_answer = true;
	}

	void draw_movements() {
		auto current_pos = m_active_cell.pos();
		auto current_cell = coord_to_cell(current_pos.x, current_pos.y);
		auto active_piece = m_model.get(cell{ (int8_t)current_cell.x, (int8_t)current_cell.y });

		std::vector<cell> moves;

		m_model.possible_moves(*active_piece, moves);

		for (const auto& m : moves) {
			auto coord = cell_to_coord((int)m.x, (int)m.y);
			m_movement_mark.set_pos(coord.x, coord.y);
			m_movement_mark.draw(*m_window);
		}
	}

	sf::Vector2i coord_to_cell(int x, int y) {
		// NOTE: Начало отсчета у модели и у view
		// NOTE: отличаются на 7 клеток по оси Y.
		return {
			(int)(float(x - m_border_size_x + 0.01 * m_cell_size_x) / m_cell_size_x),
			7 - (int)(float(y - m_border_size_y + 0.01 * m_cell_size_y) / m_cell_size_y)
		};
	}

	sf::Vector2f cell_to_coord(int cx, int cy) {
		// NOTE: Начало отсчета у модели и у view
		// NOTE: отличаются на 7 клеток по оси Y.
		return {
			m_border_size_x + m_cell_size_x * cx,
			m_border_size_y + m_cell_size_y * (7 - cy)
		};
	}

	inline static const float border_coeff = 0.068;

	board board;
	corners_model m_model;
	active_cell m_active_cell;
	movement_mark m_movement_mark;
	std::shared_ptr<sf::RenderWindow> m_window;
	std::vector<std::vector<piece_holder>> m_pieces;

	bool m_waiting_answer{ false };

	float m_cell_size_x;
	float m_cell_size_y;
	float m_border_size_x;
	float m_border_size_y;
};
