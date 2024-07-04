#include "player.h"

#define MIN_SPEED -5
#define TRACE_LENGTH 10

void init_player(Player *p, int x, int y, int size, uint16_t color)
{
    p->x = x;
    p->y = y;
    // p->speed = speed;
    p->size = size;
    p->color = color;
    // p->lives = lives;
    p->score = 0;
    // p->gravity = gravity;
    p->last_passed_wall_index = -1;
    p->trace_index = 0;
    for (int i = 0; i < TRACE_LENGTH; i++)
    {
        p->trace[i].x = x;
        p->trace[i].y = y;
    }
}

void move_player_up(Player *p)
{
    p->last_y = p->y;
    p->speed = p->const_speed;
    p->y -= p->speed;
    p->last_x = p->x;
    if (p->y < 0)
        p->y = 0;
    if (p->last_y != p->y)
    {                                                                                    // only draw when position changes
        LCD_Fill(p->last_x, p->last_y, p->last_x + p->size, p->last_y + p->size, BLACK); // clear old position
        LCD_Fill(p->x, p->y, p->x + p->size, p->y + p->size, p->color);                  // draw new position
    }
}

void apply_gravity(Player *p)
{
    p->last_y = p->y;
    p->last_x = p->x;
    p->speed -= p->gravity;
    if (p->speed < MIN_SPEED)
        p->speed = MIN_SPEED;
    p->y -= p->speed;
    if (p->y > 160 - p->size)
        p->y = 160 - p->size;
    if (p->last_y != p->y) // only draw when position changes
    {
        LCD_Fill(p->last_x, p->last_y, p->last_x + p->size, p->last_y + p->size, BLACK);
        LCD_Fill(p->x, p->y, p->x + p->size, p->y + p->size, p->color);
    }
}

void reset_player_position(Player *p, int x, int y)
{
    LCD_Fill(p->x, p->y, p->x + p->size, p->y + p->size, BLACK);
    p->x = x;
    p->y = y;
    LCD_Fill(p->x, p->y, p->x + p->size, p->y + p->size, p->color);
}

void update_player_score(Player *p, int points)
{
    p->score += points;
    if (p->score > 10000)
        p->score = 10000;
    else if (p->score < -10000)
        p->score = -10000;
}

void update_player_lives(Player *p, int change)
{
    p->lives -= change;
}

void draw_player(Player *p)
{
    // LCD_Fill(p->x, p->y, p->x + p->size, p->y + p->size, p->color);
    if (p->x != p->last_x || p->y != p->last_y)
    {
        LCD_Fill(p->last_x, p->last_y, p->last_x + p->size, p->last_y + p->size, BLACK);
        LCD_Fill(p->x, p->y, p->x + p->size, p->y + p->size, p->color);
        p->last_x = p->x;
        p->last_y = p->y;
    }
}

void init_walls(Wall *walls, int count)
{
    for (int i = 0; i < count - 2; i += 2)
    {
        walls[i].x = 75 - i / 2 * walls[i].distance;
        walls[i].y = 38;
        if (i % 6 == 0)
            walls[i].height = 40;
        else if (i % 6 == 2)
            walls[i].height = 50;
        else if (i % 6 == 4)
            walls[i].height = 45;
        walls[i].width = 5;
        walls[i].moving = true;
        walls[i].color = WHITE;

        walls[i + 1].x = walls[i].x;
        walls[i + 1].y = walls[i].y + walls[i].height + walls[i].gap;
        walls[i + 1].height = 158 - walls[i + 1].y;
        walls[i + 1].width = 5;
        walls[i + 1].moving = true;
        walls[i + 1].color = WHITE;
    }

    walls[count - 2].x = 0;
    walls[count - 2].y = 36;
    walls[count - 2].width = 80;
    walls[count - 2].height = 2;
    walls[count - 2].color = BLUE;
    walls[count - 2].moving = false;

    walls[count - 1].x = 0;
    walls[count - 1].y = 158;
    walls[count - 1].width = 80;
    walls[count - 1].height = 2;
    walls[count - 1].color = RED;
    walls[count - 1].moving = false;
}

void move_walls(Wall *walls, int count)
{
    for (int i = 0; i < count - 2; i++)
    {
        walls[i].last_x = walls[i].x;
        // walls[i].last_y = walls[i].y;

        walls[i].x -= 1;
        if (walls[i].x < -walls[i].width)
        {
            walls[i].x = 75;
        }
        if (walls[i].last_x != walls[i].x)
        {
            LCD_Fill(walls[i].last_x, walls[i].y, walls[i].last_x + walls[i].width, walls[i].y + walls[i].height, BLACK);
            LCD_Fill(walls[i].x, walls[i].y, walls[i].x + walls[i].width, walls[i].y + walls[i].height, walls[i].color);
        }
    }
}

void draw_walls(Wall *walls, int count)
{
    for (int i = 0; i < count; i++)
    {

        if (walls[i].moving)
        {
            if (walls[i].x != walls[i].last_x || walls[i].y != walls[i].last_y) // only draw when position changes
            {
                LCD_Fill(walls[i].last_x, walls[i].last_y, walls[i].last_x + walls[i].width, walls[i].last_y + walls[i].height, BLACK);
                LCD_Fill(walls[i].x, walls[i].y, walls[i].x + walls[i].width, walls[i].y + walls[i].height, walls[i].color);
                walls[i].last_x = walls[i].x;
                walls[i].last_y = walls[i].y;
            }
        }
        else
        {
            LCD_Fill(walls[i].x, walls[i].y, walls[i].x + walls[i].width, walls[i].y + walls[i].height, walls[i].color);
        }
    }
}

bool check_collision_with_walls(const Player *player, const Wall *walls, int count)
{
    for (int i = 0; i < count; i++)
    {
        if (player->x < walls[i].x + walls[i].width &&
            player->x + player->size > walls[i].x &&
            player->y < walls[i].y + walls[i].height &&
            player->y + player->size > walls[i].y)
        {
            return true;
        }
    }
    return false;
}

void update_trace(Player *p)
{
    for (int i = 0; i < 10; i++)
    {
        p->trace[i].x -= 1;
        // if (p->trace[i].x < 0)
        // {
        //     p->trace[i].x = 10;
        // }
    }

    p->trace[p->trace_index].x = p->x;
    p->trace[p->trace_index].y = p->y;
    p->trace_index = (p->trace_index + 1) % 10;
}

uint16_t calculate_fade_color(int index, uint16_t original_color)
{
    int fade_factor = 255 * (10 - index) / 10; // gradually fade out the trace
    int red = (original_color >> 11) & 0x1F;
    int green = (original_color >> 5) & 0x3F;
    int blue = original_color & 0x1F;

    red = (red * fade_factor) / 255;
    green = (green * fade_factor) / 255;
    blue = (blue * fade_factor) / 255;

    return (uint16_t)((red << 11) | (green << 5) | blue);
}

void draw_trace(Player *p)
{
    // int next_index = (p->trace_index + 1) % 10;
    // if (p->trace[next_index].x != 0 && p->trace[next_index].y != 0)
    // {
    //     // uint16_t faded_color = calculate_fade_color(p->trace_index, p->color);
    //     // LCD_DrawLine(p->trace[p->trace_index].x, p->trace[p->trace_index].y, p->trace[next_index].x, p->trace[next_index].y, faded_color);
    //     LCD_DrawLine(p->trace[p->trace_index].x, p->trace[p->trace_index].y, p->trace[next_index].x, p->trace[next_index].y, p->color);
    //     p->trace_index = next_index;
    // }
    static TracePoint last_trace[10];
    int i, next_index;

    for (i = 0; i < 9; i++)
    {
        if (last_trace[i].x != 0 && last_trace[i + 1].x != 0)
        {
            LCD_DrawLine(last_trace[i].x, last_trace[i].y, last_trace[i + 1].x, last_trace[i + 1].y, BLACK);
        }
    }

    for (i = 1; i < 10; i++)
    {
        next_index = (p->trace_index - 1 - i + 10) % 10;
        int current_index = (next_index + 1) % 10;
        if (p->trace[current_index].x != 0 && p->trace[next_index].x != 0)
        {
            uint16_t faded_color = calculate_fade_color(i, p->color);
            LCD_DrawLine(p->trace[current_index].x, p->trace[current_index].y,
                         p->trace[next_index].x, p->trace[next_index].y, faded_color);
            last_trace[i] = p->trace[current_index];
            last_trace[i + 1] = p->trace[next_index];
        }
    }
}
