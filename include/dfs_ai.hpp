#pragma once

#include <limits>

#include "board_game_model.hpp"


using move_t = std::pair<std::shared_ptr<piece>, cell>;

move_t find_the_best_move(
    color_t color,
    std::shared_ptr<board_game_model> game,
    int max_depth,
    int depth
) {
    cell current_pos;
    cell current_enemy_pos;
    std::vector<cell> moves;

    move_t enemy_move;
    move_t the_best_move;

    float min_weight = std::numeric_limits<float>::max();

    const auto pieces = game->pieces(color);
    for (const auto& piece : pieces) {
        auto current_pos = piece->m_pos;
        game->possible_moves(*piece, moves);

        for (const auto& move : moves) {
            game->move(*piece, move);

            if (depth >= max_depth) {
                enemy_move = find_the_best_move(!color, game, max_depth, depth + 1);
                current_enemy_pos = enemy_move.first->m_pos;
                game->move(*enemy_move.first, enemy_move.second);
            }

            float w = game->position_weight();
            float k = (color == white) ? -1.f : 1.f;
            
            if (k * w < k * min_weight) {
                min_weight = w;
                the_best_move = { piece, move };
            }

            if (depth >= max_depth)
                game->reverse(*enemy_move.first, current_enemy_pos);

            game->reverse(*piece, current_pos);
        }
    }

    return the_best_move;
}