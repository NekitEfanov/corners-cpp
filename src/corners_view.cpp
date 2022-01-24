#include "corners_view.hpp"
#include "alpha_beta_ai.hpp"

using namespace std;
using namespace sf;


black_piece::black_piece() {
	load_sprite("resources/black_figure.png");
}

white_piece::white_piece() {
	load_sprite("resources/white_figure.png");
}


corners_view::corners_view(
	uint16_t width,
	uint16_t heigth
) : board_game_view(
		width,
		heigth,
		"Corners",
		std::shared_ptr<board_game_model>(&m_model, [](auto*) {})
) { }

void corners_view::make_pieces() {
	m_pieces.resize(8);
	for (int8_t i = 0; i < 8; ++i)
		m_pieces[i].resize(8);

	for (const auto& piece : m_model.pieces()) {
		if (piece->color() == white)
			m_pieces[piece->pos().x][piece->pos().y] = make_shared<white_piece>();
		else
			m_pieces[piece->pos().x][piece->pos().y] = make_shared<black_piece>();
	}
}

void corners_view::draw_pieces() {
	for (const auto& piece : m_model.pieces()) {
		auto coords = cell_to_coord(piece->pos().x, piece->pos().y);

		m_pieces[piece->pos().x][piece->pos().y]->set_pos(coords.x, coords.y);
		m_pieces[piece->pos().x][piece->pos().y]->draw(*m_window);
	}
}

void corners_view::scale_objects_to_cell(float cell_width, float cell_height) {
	for (const auto& piece : m_model.pieces()) {
		m_pieces[piece->pos().x][piece->pos().y]->set_scale(
			cell_width / m_pieces[piece->pos().x][piece->pos().y]->shape().x,
			cell_height / m_pieces[piece->pos().x][piece->pos().y]->shape().y
		);
	}

}

void corners_view::move(shared_ptr<piece> piece, const Vector2i& pos) {
	swap(
		m_pieces[piece->pos().x][piece->pos().y],
		m_pieces[pos.x][pos.y]
	);
	m_model.move(*piece, sf_to_cell(pos));
}
