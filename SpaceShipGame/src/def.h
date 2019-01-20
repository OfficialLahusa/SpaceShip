#pragma once
#include <SFML/Graphics.hpp>

/* General Game Layout info */
#define TILE_SCALING 3
#define TILES_X 24
#define TILES_Y 12
#define GENERATE_MAP true

/* Player Info UI element width in tiles */
#define INFO_WIDTH	7

/* Gamplay Properties */
#define POWERUP_SPAWN_CHANCE 0.333333333f;
#define MOVEMENT_SPEED 100.f
#define HP_DEFAULT 3
#define BOMB_COUNT_DEFAULT 1
#define METEORITE_SPEED_MIN 100.f
#define METEORITE_SPEED_MAX 250.f
#define METEORITE_ROTATIONSPEED_MIN 20.f
#define METEORITE_ROTATIONSPEED_MAX 50.f
#define METEORITE_HP_MIN 100.f
#define METEORITE_HP_MAX 120.f
#define METEORITE_SCALE_MIN 0.1f
#define METEORITE_SCALE_MAX 0.3f
#define METEORITE_IMPACT_DAMAGE 30.f
#define METEORITE_SPAWN_DISTANCE 500.f
#define METEORITE_SPAWN_CHANCE 0.004f
#define METEORITE_SPRITE_COUNT 4
#define METEORITE_MAXIMUM_LIFETIME 60.f
#define SHOT_MAXIMUM_LIFETIME 15.f


/* Health bar calculation */
#define HEALTH_BAR_VARIATIONS 29
#define HEALTH_BAR_HEIGHT 8
#define HEALTH_BAR_WIDTH 32
#define HEALTH_BAR_PROTECTED 32

/* Joystick Input Definitions */
#define CALIBRATION_FAULT_THRESHOLD 1.f