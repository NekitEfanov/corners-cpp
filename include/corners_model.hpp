#pragma once

#include <stdexcept>

#include "board_game_model.hpp"


float sqr(float x) {
	return x * x;
}

float squared_dist(const cell& p, const cell& q) {
	return sqr((float)p.x - (float)q.x) + sqr((float)p.y - (float)q.y);
}

class checker : public piece {
public:
	checker(cell c, color_t color) {
		m_pos = c;
		m_color = color;
	}

	const std::vector<cell>& moves() const override {
		return m_moves;
	}

protected:
	inline static const std::vector<cell> m_moves = { { 1, 0 }, { -1, 0 }, { 0, 1 }, { 0, -1 } };
};


class corners_model : public board_game_model {
public:
	corners_model() {
		m_board.resize(8);
		for (int8_t i = 0; i < 8; ++i)
			m_board[i].resize(8);

		for (int8_t i = 5; i < 8; ++i)
			for (int8_t j = 0; j < 3; ++j) {
				m_board[i][j] = std::make_shared<checker>(cell{ i, j }, white);
				m_whites.push_back(m_board[i][j]);
			}
		
		for (int8_t i = 0; i < 3; ++i)
			for (int8_t j = 5; j < 8; ++j) {
				m_board[i][j] = std::make_shared<checker>(cell{ i, j }, black);
				m_blacks.push_back(m_board[i][j]);
			}
	}

	/*! \brief Расчет оценки текущей позиции
	 *	\return ....
	 */
	float position_weight() override {
		// Суммарные квадраты расстояний до финиша
		float white_sdist = 0.f;
		float black_sdist = 0.f;

		auto calc_sdist = [this](const auto& pieces, const auto& finish) {
			float sdist = 0.f;

			for (const auto& p : pieces)
				sdist += squared_dist(finish, p->m_pos);

			return sdist;
		};

		white_sdist = calc_sdist(m_whites, white_finish);
		black_sdist = calc_sdist(m_blacks, black_finish);

		if (is_white_winner())
			return std::numeric_limits<float>::max();

		if (is_black_winner())
			return std::numeric_limits<float>::lowest();

		return black_sdist - white_sdist;
	}

	std::shared_ptr<piece> get(const cell& c) override {
		if (!primary_check(c))
			throw std::invalid_argument("invalid board cell.");

		return m_board[c.x][c.y];
	}

	/*! \brief Возвращает все 
	 *	\return ....
	 */
	std::vector<std::shared_ptr<piece>> pieces() override {
		std::vector<std::shared_ptr<piece>> result;

		result.insert(result.end(), m_whites.cbegin(), m_whites.cend());
		result.insert(result.end(), m_blacks.cbegin(), m_blacks.cend());

		return result;
	}

	/*! \brief Возвращает все 
	 *	\return ....
	 */
	const std::vector<std::shared_ptr<piece>>& pieces(color_t color) override {
		if (color == white)
			return m_whites;
		else
			return m_blacks;
	}

	/*! \brief Возвращает все 
	 *	\return ....
	 */
	void possible_moves(const piece& p, std::vector<cell>& result) override {
		cell c;
		result.clear();

		for (const auto& m : p.moves()) {
			c.x = p.m_pos.x + m.x;
			c.y = p.m_pos.y + m.y;

			if (primary_check(c) && !m_board[c.x][c.y])
				result.push_back(c);
		}
	}

	/*! \brief Возвращает все 
	 *	\return ....
	 */
	bool is_move_possible(const piece& p, const cell& c) override {
		// TODO
		if (!primary_check(c) || !m_board[p.m_pos.x][p.m_pos.y])
			return false;

		cell diff;
		diff.x = c.x - p.m_pos.x;
		diff.y = c.y - p.m_pos.y;

		return 
			!m_board[c.x][c.y]
			&& std::find(
				p.moves().cbegin(),
				p.moves().cend(),
				diff
			) != p.moves().cend();
	}

	/*! \brief Возвращает все 
	 *	\return ....
	 */
	void move(const piece& p, const cell& c) override {
		if (!is_move_possible(p, c))
			throw std::runtime_error("move is not possible");
		
		move_unchecked(p, c);
	}

	bool has_winner() const override {
		return is_white_winner() || is_black_winner();
	}

//protected:
	/*! \brief Возвращает все 
	 *	\return ....
	 */
	void reverse(const piece& p, const cell& c) override {
		move_unchecked(p, c);
	}

	bool is_white_winner() const {
		bool flag = true;
		for (int8_t i = 0; i < 3; ++i)
			for (int8_t j = 5; j < 8; ++j)
				flag = flag && m_board[i][j] && m_board[i][j]->m_color == white;

		return flag;
	}

	bool is_black_winner() const {
		bool flag = true;
		for (int8_t i = 5; i < 8; ++i)
			for (int8_t j = 0; j < 3; ++j)
				flag = flag && m_board[i][j] && m_board[i][j]->m_color == black;

		return flag;
	}

private:
	bool primary_check(const cell& c) {
		return c.x >= 0 && c.x < 8 && c.y >= 0 && c.y < 8;
	}

	void move_unchecked(const piece& p, const cell& c) {
		auto board_piece = m_board[p.m_pos.x][p.m_pos.y];
		std::swap(m_board[p.m_pos.x][p.m_pos.y], m_board[c.x][c.y]);
		board_piece->m_pos = c;
	}



	std::vector<std::shared_ptr<piece>> m_whites;
	std::vector<std::shared_ptr<piece>> m_blacks;

	std::vector<std::vector<std::shared_ptr<piece>>> m_board;

	inline static const cell white_finish = { -1, 8 };
	inline static const cell black_finish = { 8, -1 };
};