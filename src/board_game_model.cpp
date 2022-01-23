#include "board_game_model.hpp"


bool operator==(const cell& left, const cell& right) {
	return left.x == right.x && left.y == right.y;
}

cell piece::pos() const {
	return m_pos;
}

color_t piece::color() const {
	return m_color;
}
