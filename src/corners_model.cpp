#include "utils.hpp"
#include "corners_model.hpp"

using namespace std;


const cell corners_model::white_finish = { -1, 8 };
const cell corners_model::black_finish = { 8, -1 };
const vector<cell> checker::m_moves = { { 1, 0 }, { -1, 0 }, { 0, 1 }, { 0, -1 } };


checker::checker(cell c, color_t color) {
	m_pos = c;
	m_color = color;
}

const vector<cell>& checker::moves() const {
	return m_moves;
}


corners_model::corners_model() {
	m_board.resize(8);
	for (int8_t i = 0; i < 8; ++i)
		m_board[i].resize(8);

	for (int8_t i = 5; i < 8; ++i)
		for (int8_t j = 0; j < 3; ++j) {
			m_board[i][j] = make_shared<checker>(cell{ i, j }, white);
			m_whites.push_back(m_board[i][j]);
		}

	for (int8_t i = 0; i < 3; ++i)
		for (int8_t j = 5; j < 8; ++j) {
			m_board[i][j] = make_shared<checker>(cell{ i, j }, black);
			m_blacks.push_back(m_board[i][j]);
		}
}


float corners_model::position_weight() {
	// Суммарные квадраты расстояний до финиша
	float white_sdist = 0.f;
	float black_sdist = 0.f;

	auto calc_sdist = [this](const auto& pieces, const auto& finish) {
		float sdist = 0.f;

		for (const auto& p : pieces)
			sdist += squared_dist(finish, p->pos());

		return sdist;
	};

	white_sdist = calc_sdist(m_whites, white_finish);
	black_sdist = calc_sdist(m_blacks, black_finish);

	if (is_white_winner())
		return numeric_limits<float>::max();

	if (is_black_winner())
		return numeric_limits<float>::lowest();

	return black_sdist - white_sdist;
}

shared_ptr<piece> corners_model::get(const cell& c) {
	if (!primary_check(c))
		throw invalid_argument("invalid board cell.");

	return m_board[c.x][c.y];
}

vector<shared_ptr<piece>> corners_model::pieces() {
	vector<shared_ptr<piece>> result;

	result.insert(result.end(), m_whites.cbegin(), m_whites.cend());
	result.insert(result.end(), m_blacks.cbegin(), m_blacks.cend());

	return result;
}

const vector<shared_ptr<piece>>& corners_model::pieces(color_t color) {
	if (color == white)
		return m_whites;
	else
		return m_blacks;
}

void corners_model::possible_moves(const piece& p, vector<cell>& result) {
	cell c;
	result.clear();

	for (const auto& m : p.moves()) {
		c.x = p.pos().x + m.x;
		c.y = p.pos().y + m.y;

		if (primary_check(c) && !m_board[c.x][c.y])
			result.push_back(c);
	}
}

void corners_model::move(const piece& p, const cell& c) {
	if (!is_move_possible(p, c))
		throw runtime_error("move is not possible");

	move_unchecked(p, c);
}

void corners_model::reverse(const piece& p, const cell& c) {
	move_unchecked(p, c);
}

bool corners_model::has_winner() const {
	return is_white_winner() || is_black_winner();
}

bool corners_model::is_move_possible(const piece& p, const cell& c) {
	// TODO
	if (!primary_check(c) || !m_board[p.pos().x][p.pos().y])
		return false;

	cell diff;
	diff.x = c.x - p.pos().x;
	diff.y = c.y - p.pos().y;

	return
		!m_board[c.x][c.y]
		&& find(
			p.moves().cbegin(),
			p.moves().cend(),
			diff
		) != p.moves().cend();
}

bool corners_model::is_white_winner() const {
	bool flag = true;
	for (int8_t i = 0; i < 3; ++i)
		for (int8_t j = 5; j < 8; ++j)
			flag = flag && m_board[i][j] && m_board[i][j]->color() == white;

	return flag;
}

bool corners_model::is_black_winner() const {
	bool flag = true;
	for (int8_t i = 5; i < 8; ++i)
		for (int8_t j = 0; j < 3; ++j)
			flag = flag && m_board[i][j] && m_board[i][j]->color() == black;

	return flag;
}

bool corners_model::primary_check(const cell& c) {
	return c.x >= 0 && c.x < 8 && c.y >= 0 && c.y < 8;
}

void corners_model::move_unchecked(const piece& p, const cell& c) {
	auto board_piece = m_board[p.pos().x][p.pos().y];
	swap(m_board[p.pos().x][p.pos().y], m_board[c.x][c.y]);
	board_piece->set_pos(c);
}