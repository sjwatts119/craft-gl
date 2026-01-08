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

static constexpr float GRAVITY = 0.38f;
static constexpr float JUMP_VELOCITY = 10.5f;

static constexpr float GROUND_FRICTION =  0.88f;
static constexpr float AIR_FRICTION = 0.95f;

static constexpr float MAX_HORIZONTAL_MOMENTUM = 15.0f;
static constexpr float MAX_VERTICAL_MOMENTUM = 50.0f;

