#pragma once

#include <limits>

#include "board_game_model.hpp"


using move_t = std::pair<std::shared_ptr<piece>, cell>;

move_t find_the_best_move(color_t color, board_game_model& game, int max_depth);