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

	/*! \brief Расчет оценки текущей позиции
	 *	\return ....
	 */
	float position_weight() override;

	/*! \brief Расчет оценки текущей позиции
	 *	\return ....
	 */
	std::shared_ptr<piece> get(const cell& c) override;

	/*! \brief Возвращает все 
	 *	\return ....
	 */
	std::vector<std::shared_ptr<piece>> pieces() override;

	/*! \brief Возвращает все 
	 *	\return ....
	 */
	const std::vector<std::shared_ptr<piece>>& pieces(color_t color) override;

	/*! \brief Возвращает все 
	 *	\return ....
	 */
	void possible_moves(const piece& p, std::vector<cell>& result) override;

	/*! \brief Возвращает все 
	 *	\return ....
	 */
	bool is_move_possible(const piece& p, const cell& c) override;

	/*! \brief Возвращает все 
	 *	\return ....
	 */
	void move(const piece& p, const cell& c) override;

	bool has_winner() const override;

//protected:
	/*! \brief Возвращает все 
	 *	\return ....
	 */
	void reverse(const piece& p, const cell& c) override {
		move_unchecked(p, c);
	}

	bool is_white_winner() const;

	bool is_black_winner() const;

private:
	bool primary_check(const cell& c);

	void move_unchecked(const piece& p, const cell& c);


	std::vector<std::shared_ptr<piece>> m_whites;
	std::vector<std::shared_ptr<piece>> m_blacks;

	std::vector<std::vector<std::shared_ptr<piece>>> m_board;

	static const cell white_finish;
	static const cell black_finish;
};