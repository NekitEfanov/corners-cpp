#pragma once

#include <memory>

#include "corners_model.hpp"
#include "board_game_view.hpp"

#include <SFML/Graphics.hpp>


class black_piece : public view_object {
public:
	black_piece();
};

class white_piece : public view_object {
public:
	white_piece();
};


class corners_view : public board_game_view {
	using piece_holder = std::shared_ptr<view_object>;

public:
	corners_view(uint16_t width, uint16_t heigth);

protected:
	void make_pieces() override;
	void draw_pieces() override;
	void scale_objects_to_cell(float cell_width, float cell_height) override;
	void move(std::shared_ptr<piece> piece, const sf::Vector2i& pos) override;

private:
	corners_model m_model;
	std::vector<std::vector<piece_holder>> m_pieces;
};


cell sf_to_cell(const sf::Vector2i& v);
sf::Vector2i cell_to_sf(const cell& c);