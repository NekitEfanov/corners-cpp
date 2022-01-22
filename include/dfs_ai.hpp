#pragma once

#include <limits>

#include "board_game_model.hpp"


using move_t = std::pair<std::shared_ptr<piece>, cell>;

move_t find_the_best_move(
    color_t color,
    board_game_model& game,
    int max_depth,
    int depth,
    float &depth_weight
) {
    cell current_pos;
    cell current_enemy_pos;
    std::vector<cell> moves;

    move_t enemy_move;
    move_t the_best_move;

    float min_weight;
    if (color == white)
        min_weight = std::numeric_limits<float>::lowest();
    else
        min_weight = std::numeric_limits<float>::max();

    const auto pieces = game.pieces(color);
    for (const auto& piece : pieces) {
        auto current_pos = piece->m_pos;
        game.possible_moves(*piece, moves);

        for (const auto& move : moves) {
            game.move(*piece, move);

            if (depth < max_depth) {
                float w = 0.f;

                enemy_move = find_the_best_move(!color, game, max_depth, depth + 1, w);

                if (enemy_move.first) {
                    current_enemy_pos = enemy_move.first->m_pos;
                    game.move(*enemy_move.first, enemy_move.second);
                }

                float k = (color == white) ? -1.f : 1.f;
                if (k * w < k * min_weight) {
                    min_weight = w;
                    the_best_move = { piece, move };
                }

                if (enemy_move.first)
                    game.reverse(*enemy_move.first, current_enemy_pos);
            }

            game.reverse(*piece, current_pos);
        }
    }

    if (depth >= max_depth)
        depth_weight = game.position_weight();

    return the_best_move;
}