#ifndef PLAYER_H
#define PLAYER_H

#include "lcd/lcd.h"
#include <stdbool.h>

typedef struct
{
    int x, y;
} TracePoint;

typedef struct
{
    int x, y;
    int last_x, last_y;
    int speed;
    int const_speed;
    int size;
    uint16_t color;
    int lives;
    int score;
    int gravity;
    int last_passed_wall_index;
    TracePoint trace[10];
    int trace_index;
} Player;

typedef struct
{
    int x, y;
    int last_x, last_y;
    int width, height;
    int gap;
    int distance;
    uint16_t color;
    bool moving;
} Wall;

void init_walls(Wall *walls, int count);
void move_walls(Wall *walls, int count);
void draw_walls(Wall *walls, int count);
bool check_collision_with_walls(const Player *player, const Wall *walls, int count);

void init_player(Player *p, int x, int y, int size, uint16_t color);
void move_player_up(Player *p);
void apply_gravity(Player *p);
void reset_player_position(Player *p, int x, int y);
void update_player_score(Player *p, int points);
void update_player_lives(Player *p, int change);
void draw_player(Player *p);
uint16_t calculate_fade_color(int index, uint16_t original_color);
void update_trace(Player *p);
void draw_trace(Player *p);

#endif // PLAYER_H