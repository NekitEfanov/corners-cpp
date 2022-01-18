#pragma once

#include <stdexcept>

#include "board_game_model.hpp"


class checker : public piece {
public:
	checker(cell c, color_t color) {
		m_pos = c;
		m_color = color;
	}

	const std::vector<cell>& moves() override {
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
		return 0.f;
	}

	/*! \brief Возвращает все 
	 *	\return ....
	 */
	const std::vector<std::shared_ptr<piece>>& pieces() override {
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
		m_blacks;
	}

	/*! \brief Возвращает все 
	 *	\return ....
	 */
	bool is_move_possible(const piece& p, const cell& c) override {
		// TODO
		return
			c.x >= 0 && c.y >= 0
			&& c.x < 8 && c.y < 8
			&& !m_board[c.x][c.y];
	}

	/*! \brief Возвращает все 
	 *	\return ....
	 */
	void move(const piece& p, const cell& c) override {
		if (!is_move_possible(p, c))
			throw std::runtime_error("move is not possible");
		
		move_unchecked(p, c);
	}

//protected:
	/*! \brief Возвращает все 
	 *	\return ....
	 */
	void reverse(const piece& p, const cell& c) override {
		move_unchecked(p, c);
	}

private:
	void move_unchecked(const piece& p, const cell& c) {
		auto board_piece = m_board[p.m_pos.x][p.m_pos.y];
		std::swap(m_board[p.m_pos.x][p.m_pos.y], m_board[c.x][c.y]);
		board_piece->m_pos = c;
	}

	std::vector<std::shared_ptr<piece>> m_whites;
	std::vector<std::shared_ptr<piece>> m_blacks;

	std::vector<std::vector<std::shared_ptr<piece>>> m_board;
};