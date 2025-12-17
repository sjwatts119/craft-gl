#pragma once

static constexpr float EPSILON = 0.0001f; // AABB intersection epsilon

static constexpr int CHUNK_SIZE = 16;
static constexpr int BLOCK_COUNT = CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE;

static constexpr int SCREEN_WIDTH = 1920;
static constexpr int SCREEN_HEIGHT = 1080;

// Screen Title
static constexpr auto SCREEN_TITLE = "craft-gl";

static constexpr int CHUNK_COUNT_X = 11;
static constexpr int CHUNK_COUNT_Y = 3;
static constexpr int CHUNK_COUNT_Z = 11;