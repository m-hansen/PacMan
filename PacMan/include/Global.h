#pragma once
#include <string>

const std::string DEBUG_LOG_FOLDER = "Logs/";
const int GRID_SIZE = 16; // use increments of 8
const int NUM_HORIZONTAL_TILES = 28;
const int NUM_VERTICAL_TILES = 36;
const int SCREEN_WIDTH = NUM_HORIZONTAL_TILES * GRID_SIZE; // 224 width with 8x8 tiles
const int SCREEN_HEIGHT = NUM_VERTICAL_TILES * GRID_SIZE; // 288 height with 8x8 tiles