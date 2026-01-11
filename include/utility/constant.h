#pragma once
#include <siv/perlin.h>

static constexpr siv::PerlinNoise::seed_type WORLD_SEED = 1236;

static constexpr float EPSILON = 0.0001f; // AABB intersection epsilon

static constexpr int TICK_RATE = 20;
static constexpr float TIME_PER_TICK = 1.0f / static_cast<float>(TICK_RATE);
static constexpr int MAX_TICKS_PER_FRAME = 5;

static constexpr int CHUNK_SIZE = 32;
static constexpr int BLOCK_COUNT = CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE;

static constexpr int SCREEN_WIDTH = 2560;
static constexpr int SCREEN_HEIGHT = 1440;

static constexpr auto SCREEN_TITLE = "craft-gl";

static constexpr int WORLD_HEIGHT = 8; // in chunks
static constexpr int RENDER_DISTANCE = 4; // in chunks

static constexpr int CHUNK_DELETIONS_PER_FRAME = 1;
static constexpr int REMESHES_PER_THREAD_PER_FRAME = 2;

static constexpr float STANDING_ON_NEGATIVE_Y_OFFSET = -0.5f; // amount to push down detection point when checking for stood on block

static constexpr float JUMP_VELOCITY = 0.45f; // instant upward velocity when jumping

static constexpr float WALKING_ACCELERATION = 0.08f; // blocks per tick^2

static constexpr float GRAVITY_ACCELERATION = 0.08f; // blocks per tick^2

static constexpr float HORIZONTAL_RESISTANCE_FACTOR = 0.91f; // speed retained / tick after resistance
static constexpr float VERTICAL_RESISTANCE_FACTOR = 0.98f;   // speed retained / tick after resistance
static constexpr float FLYING_RESISTANCE_FACTOR = 0.90f;    // speed retained / tick after resistance when flying

static constexpr float NO_SLIPPERINESS = 0.02f; // base horizontal resistance when no slipperiness
static constexpr float BLOCK_SLIPPERINESS_FACTOR = 0.6f; // multiplier for horizontal resistance when on a block

static constexpr float TERMINAL_VELOCITY = 3.92f; // blocks per tick




