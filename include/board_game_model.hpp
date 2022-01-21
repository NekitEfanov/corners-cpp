﻿#pragma once

#include <vector>
#include <memory>
#include <cstdint>


using color_t = bool;
constexpr const color_t white = true;
constexpr const color_t black = false;


struct cell {
	int8_t x;
	int8_t y;
};


class piece {
public:
	virtual const std::vector<cell>& moves() = 0;

	cell m_pos;
	color_t m_color;
};


class board_game_model {
public:
	/*! \brief Расчет оценки текущей позиции
	 *	\return ....
	 */
	virtual float position_weight() = 0;

	/*! \brief Возвращает все 
	 *	\return ....
	 */
	virtual std::vector<std::shared_ptr<piece>> pieces() = 0;

	/*! \brief Возвращает все 
	 *	\return ....
	 */
	virtual const std::vector<std::shared_ptr<piece>>& pieces(color_t color) = 0;

	/*! \brief Возвращает все 
	 *	\return ....
	 */
	virtual void possible_moves(const piece& p, std::vector<cell> &result) = 0;

	/*! \brief Возвращает все 
	 *	\return ....
	 */
	virtual bool is_move_possible(const piece& p, const cell& c) = 0;

	/*! \brief Возвращает все 
	 *	\return ....
	 */
	virtual void move(const piece& p, const cell& c) = 0;

//protected:
	/*! \brief Возвращает все 
	 *	\return ....
	 */
	virtual void reverse(const piece& p, const cell& c) = 0;
};