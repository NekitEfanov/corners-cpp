#pragma once

#include <memory>
#include <iostream>

#include "corners_model.hpp"

#include <SFML/Graphics.hpp>


class view_object {
public:
	void draw(sf::RenderWindow& window) const;

	sf::Vector2f pos() const;
	sf::Vector2u shape() const;

	void set_pos(float x, float y);
	void set_scale(float sx, float sy);
	
protected:
	void load_sprite(const std::string& filename);

	sf::Vector2u m_size;
	sf::Sprite m_sprite;
	sf::Texture m_texture;
};

class black_piece : public view_object {
public:
	black_piece();
};

class white_piece : public view_object {
public:
	white_piece();
};

class board : public view_object {
public:
	board();
};

class active_cell : public view_object {
public:
	active_cell();

	bool active() const;

	void set_active(bool flag);

private:
	bool m_active{ false };
};

class movement_mark : public active_cell {
public:
	movement_mark();
};


class corners_view {
	using piece_holder = std::shared_ptr<view_object>;

public:
	corners_view(uint16_t width, uint16_t heigth);
	void loop();

private:
	void make_sizes();
	void make_pieces(const int width, const int heigth);

	void draw_pieces();
	void draw_movements();

	void process_click(int x, int y);
	bool process_with_active_piece(const sf::Vector2i& dst_cell);

	bool is_move(std::shared_ptr<piece> piece, const sf::Vector2i& pos);
	void move(std::shared_ptr<piece> piece, const sf::Vector2i& pos);

	sf::Vector2i coord_to_cell(float x, float y);
	sf::Vector2f cell_to_coord(int cx, int cy);

	void check_winner();

	float m_cell_size_x;
	float m_cell_size_y;
	float m_border_size_x;
	float m_border_size_y;

	bool m_waiting_answer{ false };

	board board;
	corners_model m_model;
	active_cell m_active_cell;
	movement_mark m_movement_mark;

	std::shared_ptr<sf::RenderWindow> m_window;
	std::vector<std::vector<piece_holder>> m_pieces;

	inline static const float border_coeff = 0.068f;
};


cell sf_to_cell(const sf::Vector2i& v);
sf::Vector2i cell_to_sf(const cell& c);