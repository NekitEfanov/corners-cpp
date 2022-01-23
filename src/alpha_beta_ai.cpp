#include "alpha_beta_ai.hpp"

using namespace std;


float alpha_beta_best_move(
    color_t color,
    board_game_model& game,
    move_t& move,
    int max_depth,
    int depth,
    float alpha,
    float beta
) {
    bool finish;
    float weigth;
    cell reverse_pos;
    move_t the_best_move;
    vector<cell> moves;

    if (color == white)
        weigth = numeric_limits<float>::lowest();
    else
        weigth = numeric_limits<float>::max();

    finish = false;

    // Делаем должный ход
    if (move.first) {
        reverse_pos = move.first->pos();
        game.move(*move.first, move.second);
    }

    bool has_winned = game.has_winner();

    if (depth >= max_depth || has_winned) {
        // Оценка позиции и быстрый выход

        weigth = game.position_weight();

        // Лайфхак, чтобы искать самый короткий путь к победе
        if (has_winned)
            weigth /= depth;

        finish = true;
    }

    // Обходим все возможные ходы, вычисляем мин-макс
    const auto pieces = game.pieces(color);
    for (const auto& piece : pieces) {
        if (finish)
            continue;

        auto current_pos = piece->pos();
        game.possible_moves(*piece, moves);

        for (const auto& move : moves) {
            move_t current_move = { piece, move };
            float wtmp = alpha_beta_best_move(
                !color, game, current_move, max_depth, depth + 1, alpha, beta
            );

            if (color == white) {
                if (wtmp > weigth) {
                    weigth = wtmp;
                    the_best_move = { piece, move };
                }
                else if (wtmp == weigth) {
                    // В случае равенства выбирается тот ход,
                    // который мгновенно улучшает позицию

                    move_t best_cpy = the_best_move;
                    move_t this_cpy = { piece, move };

                    float best_instantly_w = alpha_beta_best_move(
                        !color, game, best_cpy, max_depth, max_depth, alpha, beta
                    );
                    float this_instantly_w = alpha_beta_best_move(
                        !color, game, this_cpy, max_depth, max_depth, alpha, beta
                    );

                    if (this_instantly_w > best_instantly_w)
                        the_best_move = { piece, move };
                }

                // Бета отсечение
                if (weigth >= beta) {
                    finish = true;
                    break;
                }

                alpha = max(alpha, weigth);
            }
            else {
                if (wtmp < weigth) {
                    weigth = wtmp;
                    the_best_move = { piece, move };
                }
                else if (wtmp == weigth) {
                    // В случае равенства выбирается тот ход,
                    // который мгновенно улучшает позицию

                    move_t best_cpy = the_best_move;
                    move_t this_cpy = { piece, move };

                    float best_instantly_w = alpha_beta_best_move(
                        !color, game, best_cpy, max_depth, max_depth, alpha, beta
                    );
                    float this_instantly_w = alpha_beta_best_move(
                        !color, game, this_cpy, max_depth, max_depth, alpha, beta
                    );

                    if (this_instantly_w < best_instantly_w)
                        the_best_move = { piece, move };
                }

                // Альфа отсечение
                if (weigth <= alpha) {
                    finish = true;
                    break;
                }

                beta = min(beta, weigth);
            }
        }
    }

    if (move.first)
        game.reverse(*move.first, reverse_pos);

    move = the_best_move;

    return weigth;
}


move_t find_the_best_move(color_t color, board_game_model& game, int max_depth) {
    float alpha = numeric_limits<float>::lowest();
    float beta = numeric_limits<float>::max();

    move_t best_move;
    alpha_beta_best_move(color, game, best_move, max_depth, 0, alpha, beta);

    return best_move;
}