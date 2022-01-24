#pragma once

#include <vector>
#include <memory>
#include <cstdint>


using color_t = bool;
constexpr const color_t white = true;
constexpr const color_t black = false;


struct cell {
	int8_t x;
	int8_t y;

	friend bool operator==(const cell& left, const cell& right);
};


class piece {
public:
	cell pos() const;
	color_t color() const;
	virtual const std::vector<cell>& moves() const = 0;

	void set_pos(const cell& c);

protected:
	cell m_pos;
	color_t m_color;
};


class board_game_model {
public:
	//! A pure virtual member.
	/*!
	 * Расчет оценки текущей позиции аналогично шахматным симуляторам.
	 * Возвращаемая оценка позиции должна быть от -inf до inf.
	 * При этом, положительная оценка указывает на преимущество
	 * белых, отрицательная - на преимущество черных.
	   \return Оценка текущей позиции.
	 */
	virtual float position_weight() = 0;

	//! A pure virtual member.
	/*!
	 * Возвращает фигуру, находящуюся в клетке. Может вернуть nullptr.
	   \param c клетка для получения фигуры.
	   \return фигура в клетке c.
	 */
	virtual std::shared_ptr<piece> get(const cell& c) = 0;

	//! A pure virtual member.
	/*!
	 * Возвращает все фигуры на доске.
	   \return массив фигур.
	 */
	virtual std::vector<std::shared_ptr<piece>> pieces() = 0;

	//! A pure virtual member.
	/*!
	 * Возвращает все фигуры заданного цвета на доске.
	   \param c цвет фигуры.
	   \return массив фигур.
	 */
	virtual const std::vector<std::shared_ptr<piece>>& pieces(color_t color) = 0;

	//! A pure virtual member.
	/*!
	 * Возвращает все возможные ходы для заданной фигуры.
	 */
	virtual void possible_moves(const piece& p, std::vector<cell> &result) = 0;

	//! A pure virtual member.
	/*!
	 * WRITEME
	 */
	virtual bool is_move_possible(const piece& p, const cell& c) = 0;

	//! A pure virtual member.
	/*!
	 * WRITEME
	 */
	virtual void move(const piece& p, const cell& c) = 0;

	virtual bool has_winner() const = 0;

protected:
	//! A pure virtual member.
	/*!
	 * Небезопасный откат (совершение) хода. Необходимо для ai.
	 */
	virtual void reverse(const piece& p, const cell& c) = 0;

	// FIXME
	using move_t = std::pair<std::shared_ptr<piece>, cell>;

	friend float alpha_beta_best_move(
		color_t color,
		board_game_model& game,
		move_t& move,
		int max_depth,
		int depth,
		float alpha,
		float beta
	);
};
