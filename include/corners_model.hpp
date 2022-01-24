#pragma once

#include <stdexcept>

#include "board_game_model.hpp"


class checker : public piece {
public:
	checker(cell c, color_t color);
	const std::vector<cell>& moves() const override;

protected:
	static const std::vector<cell> m_moves;
};


class corners_model : public board_game_model {
public:
	corners_model();

	float position_weight() override;

	std::shared_ptr<piece> get(const cell& c) override;
	std::vector<std::shared_ptr<piece>> pieces() override;
	const std::vector<std::shared_ptr<piece>>& pieces(color_t color) override;
	void possible_moves(const piece& p, std::vector<cell>& result) override;

	bool is_move_possible(const piece& p, const cell& c) override;
	void move(const piece& p, const cell& c) override;

	bool has_winner() const override;

protected:
	void reverse(const piece& p, const cell& c) override;

private:
	bool primary_check(const cell& c);
	void move_unchecked(const piece& p, const cell& c);

	bool is_white_winner() const;
	bool is_black_winner() const;

	std::vector<std::shared_ptr<piece>> m_whites;
	std::vector<std::shared_ptr<piece>> m_blacks;

	std::vector<std::vector<std::shared_ptr<piece>>> m_board;

	static const cell white_finish;
	static const cell black_finish;
};