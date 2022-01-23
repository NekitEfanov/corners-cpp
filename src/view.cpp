#include "view.hpp"
#include "alpha_beta_ai.hpp"

using namespace std;
using namespace sf;


void view_object::draw(RenderWindow& window) const {
	window.draw(m_sprite);
};

void view_object::load_sprite(const string& filename) {
	m_texture.loadFromFile(filename);
	m_sprite.setTexture(m_texture);

	m_size = m_texture.getSize();
}

Vector2u view_object::shape() const {
	return m_size;
}

Vector2f view_object::pos() const {
	return m_sprite.getPosition();
}

void view_object::set_pos(float x, float y) {
	m_sprite.setPosition(x, y);
}

void view_object::set_scale(float sx, float sy) {
	m_sprite.setScale(sx, sy);
}


black_piece::black_piece() {
	load_sprite("black_figure.png");
}

white_piece::white_piece() {
	load_sprite("white_figure.png");
}

board::board() {
	load_sprite("board_img.jpg");
}

active_cell::active_cell() {
	load_sprite("mark.png");
}

bool active_cell::active() const {
	return m_active;
}

void active_cell::set_active(bool flag) {
	m_active = flag;
}


movement_mark::movement_mark() {
	load_sprite("point.png");
}


corners_view::corners_view(uint16_t width, uint16_t heigth) {
	board.set_pos(0.f, 0.f);
	board.set_scale(
		float(width) / board.shape().x,
		float(heigth) / board.shape().y
	);

	m_pieces.resize(8);
	for (int8_t i = 0; i < 8; ++i)
		m_pieces[i].resize(8);

	m_window = make_shared<RenderWindow>(VideoMode(width, heigth), "Corners");
	m_window->setVerticalSyncEnabled(true);

	make_pieces(width, heigth);
}

void corners_view::loop() {
	while (m_window->isOpen()) {
		make_sizes();

		board.draw(*m_window);
		draw_pieces();

		if (m_active_cell.active()) {
			m_active_cell.draw(*m_window);
			draw_movements();
		}

		m_window->display();

		// Сначала рисуем ход за белых, только потом
		// отвечаем. Иначе ход не отобразится вовремя.
		if (m_waiting_answer) {
			auto answer = find_the_best_move(black, m_model, 7);
			Vector2i c = cell_to_sf(answer.second);
			move(answer.first, c);
			m_waiting_answer = false;

			check_winner();
		}

		Event event;
		while (m_window->pollEvent(event)) {
			if (event.type == Event::Closed)
				m_window->close();

			if (
				event.mouseButton.button == Mouse::Left
				&& event.type == Event::MouseButtonPressed
				)
				process_click(event.mouseButton.x, event.mouseButton.y);
		}
	}
}


void corners_view::make_sizes() {
	auto size = m_window->getView().getSize();

	m_border_size_x = border_coeff * size.x;
	m_border_size_y = border_coeff * size.y;
	m_cell_size_x = (float(size.x) - 2.f * m_border_size_x) / 8.f;
	m_cell_size_y = (float(size.y) - 2.f * m_border_size_y) / 8.f;
}

void corners_view::make_pieces(const int width, const int heigth) {
	make_sizes();

	for (const auto& piece : m_model.pieces()) {
		if (piece->color() == white)
			m_pieces[piece->pos().x][piece->pos().y] = make_shared<white_piece>();
		else
			m_pieces[piece->pos().x][piece->pos().y] = make_shared<black_piece>();

		m_pieces[piece->pos().x][piece->pos().y]->set_scale(
			m_cell_size_x / m_pieces[piece->pos().x][piece->pos().y]->shape().x,
			m_cell_size_y / m_pieces[piece->pos().x][piece->pos().y]->shape().y
		);
	}

	m_active_cell.set_scale(
		m_cell_size_x / m_active_cell.shape().x,
		m_cell_size_x / m_active_cell.shape().y
	);

	m_movement_mark.set_scale(
		m_cell_size_x / m_movement_mark.shape().x,
		m_cell_size_x / m_movement_mark.shape().y
	);
}

void corners_view::draw_pieces() {
	for (const auto& piece : m_model.pieces()) {
		auto coords = cell_to_coord(piece->pos().x, piece->pos().y);

		m_pieces[piece->pos().x][piece->pos().y]->set_pos(coords.x, coords.y);
		m_pieces[piece->pos().x][piece->pos().y]->draw(*m_window);
	}
}

void corners_view::process_click(int x, int y) {
	auto clicked_cell = coord_to_cell((float)x, (float)y);

	if (
		clicked_cell.x < 0 || clicked_cell.x >= 8
		|| clicked_cell.y < 0 || clicked_cell.y >= 8
		)
		return;

	if (m_active_cell.active()) {
		bool moved = process_with_active_piece(clicked_cell);

		if (moved) {
			check_winner();

			return;
		}
	}

	auto piece = m_model.get(sf_to_cell(clicked_cell));
	if (!piece || piece->color() != white)
		return;

	auto coords = cell_to_coord(clicked_cell.x, clicked_cell.y);
	m_active_cell.set_pos(coords.x, coords.y);

	m_active_cell.set_active(true);
}

bool corners_view::process_with_active_piece(const Vector2i& dst_cell) {
	bool result = false;

	auto current_pos = m_active_cell.pos();
	auto current_cell = coord_to_cell(current_pos.x, current_pos.y);

	auto clicked_piece = m_model.get(sf_to_cell(dst_cell));
	auto active_piece = m_model.get(sf_to_cell(current_cell));

	if (is_move(active_piece, dst_cell)) {
		move(active_piece, dst_cell);
		result = true;
	}

	m_active_cell.set_active(false);

	return result;
}

bool corners_view::is_move(shared_ptr<piece> piece, const Vector2i& pos) {
	return m_model.is_move_possible(*piece, sf_to_cell(pos));
}

void corners_view::move(shared_ptr<piece> piece, const Vector2i& pos) {
	swap(
		m_pieces[piece->pos().x][piece->pos().y],
		m_pieces[pos.x][pos.y]
	);
	m_model.move(*piece, sf_to_cell(pos));

	m_waiting_answer = true;
}

void corners_view::draw_movements() {
	auto current_pos = m_active_cell.pos();
	auto current_cell = coord_to_cell(current_pos.x, current_pos.y);
	auto active_piece = m_model.get(sf_to_cell(current_cell));

	vector<cell> moves;
	m_model.possible_moves(*active_piece, moves);

	for (const auto& m : moves) {
		auto coord = cell_to_coord((int)m.x, (int)m.y);
		m_movement_mark.set_pos(coord.x, coord.y);
		m_movement_mark.draw(*m_window);
	}
}

Vector2i corners_view::coord_to_cell(float x, float y) {
	// NOTE: Начало отсчета у модели и у view
	// NOTE: отличаются на 7 клеток по оси Y.
	return {
		(int)(float(x - m_border_size_x + 0.01 * m_cell_size_x) / m_cell_size_x),
		7 - (int)(float(y - m_border_size_y + 0.01 * m_cell_size_y) / m_cell_size_y)
	};
}

Vector2f corners_view::cell_to_coord(int cx, int cy) {
	// NOTE: Начало отсчета у модели и у view
	// NOTE: отличаются на 7 клеток по оси Y.
	return {
		m_border_size_x + m_cell_size_x * cx,
		m_border_size_y + m_cell_size_y * (7 - cy)
	};
}

void corners_view::check_winner() {
	if (m_model.has_winner()) {
		// В данном случае это означает победу белых
		// (т.к. ждем ход черных).
		if (m_waiting_answer)
			cout << "White win!";
		else
			cout << "Black win!";

		// TODO: нормально выводить на экран победу и блокировать игру
		exit(0);
	}
}

cell sf_to_cell(const sf::Vector2i& v) {
	return cell{ (int8_t)v.x, (int8_t)v.y };
}

sf::Vector2i cell_to_sf(const cell& c) {
	return sf::Vector2i{ (int)c.x, (int)c.y };
}