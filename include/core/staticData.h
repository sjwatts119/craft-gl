#pragma once

static constexpr float EPSILON = 0.0001f; // AABB intersection epsilon

static constexpr int TICK_RATE = 20;
static constexpr float TIME_PER_TICK = 1.0f / static_cast<float>(TICK_RATE);

static constexpr int CHUNK_SIZE = 16;
static constexpr int BLOCK_COUNT = CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE;

static constexpr int SCREEN_WIDTH = 2560;
static constexpr int SCREEN_HEIGHT = 1440;

static constexpr auto SCREEN_TITLE = "craft-gl";

static constexpr int CHUNK_COUNT_X = 4;
static constexpr int CHUNK_COUNT_Y = 16;
static constexpr int CHUNK_COUNT_Z = 4;

static constexpr float JUMP_VELOCITY = 0.45f; // instant upward velocity when jumping

static constexpr float WALKING_ACCELERATION = 0.08f; // blocks per tick^2

static constexpr float GRAVITY_ACCELERATION = 0.08f; // blocks per tick^2

static constexpr float HORIZONTAL_RESISTANCE_FACTOR = 0.91f; // speed retained / tick after resistance
static constexpr float VERTICAL_RESISTANCE_FACTOR = 0.98f;   // speed retained / tick after resistance
static constexpr float FLYING_RESISTANCE_FACTOR = 0.90f;    // speed retained / tick after resistance when flying

static constexpr float NO_SLIPPERINESS = 0.02f; // base horizontal resistance when no slipperiness
static constexpr float BLOCK_SLIPPERINESS_FACTOR = 0.6f; // multiplier for horizontal resistance when on a block

static constexpr float TERMINAL_VELOCITY = 3.92f; // blocks per tick




