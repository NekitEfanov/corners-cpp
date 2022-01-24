#pragma once

#include <memory>
#include <string>

#include "board_game_model.hpp"

#include <SFML/Graphics.hpp>


/*!
 * Базовый класс для отображения объектов на доске.
 */
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

/*!
 * Базовый класс для визуализации игр на шахматной доске.
 * Для того, чтобы создать игру, необходимо переопределить
 * следующие методы:
 *	  1. void make_pieces();
 *	  2. void draw_pieces();
 *	  3. void scale_objects_to_cell(float cell_width, float cell_height);
 *	  4. void move(std::shared_ptr<piece> piece, const sf::Vector2i& pos);
 *
 * Так же необходимо инициализировать board_game_view с актуальной
 * игровой моделью.
 * 
 * Остальную логику, user-input, а так же ходы в игровой модели и
 * ответы от ai board_game_view берет на себя.
 */
class board_game_view {
public:
	board_game_view(
		uint16_t width,
		uint16_t heigth,
		const std::string title,
		std::shared_ptr<board_game_model> model
	);

	/*!
	 * Игровой цикл. Блокирующий метод для выполнения игрового процесса.
	 */
	void loop();

protected:
	//! A pure virtual member.
	/*!
	 * Перегрузите данный метод для создания фигур на доске.
	 */
	virtual void make_pieces() = 0;

	//! A pure virtual member.
	/*!
	 * Перегрузите данный метод для отрисовки фигур на доске.
	 * При отрисовке необходимо использовать поле m_window.
	 */
	virtual void draw_pieces() = 0;

	//! A pure virtual member.
	/*!
	 * Перегрузите данный метод для масштабирования объектов на доске
	 * на величину одной клетки.
	 */
	virtual void scale_objects_to_cell(float cell_width, float cell_height) = 0;

	//! A pure virtual member.
	/*!
	 * Перегрузите данный метод для выполнения сопутствующих ходу действий.
	 * NOTE: ход в модели уже сделан. Перегрузка может быть пустой.
	 */
	virtual void move(std::shared_ptr<piece> piece, const sf::Vector2i& pos) = 0;

	sf::Vector2i coord_to_cell(float x, float y);
	sf::Vector2f cell_to_coord(int cx, int cy);

	std::shared_ptr<sf::RenderWindow> m_window;

private:
	void make_sizes();
	void draw_banner();
	void draw_movements();

	void check_winner();

	void process_click(int x, int y);
	bool process_with_active_piece(const sf::Vector2i& dst_cell);

	bool is_move(std::shared_ptr<piece> piece, const sf::Vector2i& pos);


	float m_cell_size_x;
	float m_cell_size_y;
	float m_border_size_x;
	float m_border_size_y;

	int m_font_size{ 100 };
	std::string m_banner_text;
	bool m_banner_active{ false };

	bool m_waiting_answer{ false };

	board board;

	active_cell m_active_cell;
	movement_mark m_movement_mark;
	std::shared_ptr<board_game_model> m_model;

	static const float border_coeff;
	static sf::Font font;
};


cell sf_to_cell(const sf::Vector2i& v);
sf::Vector2i cell_to_sf(const cell& c);