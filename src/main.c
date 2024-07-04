#include "lcd/lcd.h"
#include "utils.h"
#include "assembly/example.h"
#include "player.h"
#include <stdio.h>
#include <stdbool.h>
#include "systick.h"
#define NUM_WALLS 8
Player player;
Wall walls[NUM_WALLS];

void Inp_init(void)
{
  rcu_periph_clock_enable(RCU_GPIOA);
  rcu_periph_clock_enable(RCU_GPIOC);

  gpio_init(GPIOA, GPIO_MODE_IPD, GPIO_OSPEED_50MHZ,
            GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3);
  gpio_init(GPIOC, GPIO_MODE_IPD, GPIO_OSPEED_50MHZ,
            GPIO_PIN_13 | GPIO_PIN_14 | GPIO_PIN_15);
}

void IO_init(void)
{
  Inp_init(); // inport init
  Lcd_Init(); // LCD init
}

void set_difficulty(int level)
{
  switch (level)
  {
  case 0: // Easy
    player.const_speed = 5;
    player.lives = 100;
    player.gravity = 1;
    for (int i = 0; i < NUM_WALLS - 2; i++)
    {
      walls[i].gap = 60;
      walls[i].distance = 32;
    }

    break;
  case 1: // Medium
    player.const_speed = 4;
    player.lives = 60;
    player.gravity = 1.5;
    for (int i = 0; i < NUM_WALLS - 2; i++)
    {
      walls[i].gap = 50;
      walls[i].distance = 27;
    }
    break;

  case 2: // Hard
    player.const_speed = 3;
    player.lives = 40;
    player.gravity = 2;
    for (int i = 0; i < NUM_WALLS - 2; i++)
    {
      walls[i].gap = 35;
      walls[i].distance = 22;
    }
    break;
  }
}

void select_difficulty(int color_choice)
{
  int selected = 0;
  LCD_Clear(BLACK);
  while (!Get_Button(JOY_CTR))
  {
    if (color_choice == 0)
    {
      LCD_ShowString(20, 10, (const u8 *)"Easy", selected == 0 ? RED : WHITE);
      LCD_ShowString(20, 60, (const u8 *)"Medium", selected == 1 ? RED : WHITE);
      LCD_ShowString(20, 110, (const u8 *)"Hard", selected == 2 ? RED : WHITE);
    }
    else
    {
      LCD_ShowString(20, 10, (const u8 *)"Easy", selected == 0 ? WHITE : GRAY);
      LCD_ShowString(20, 60, (const u8 *)"Medium", selected == 1 ? WHITE : GRAY);
      LCD_ShowString(20, 110, (const u8 *)"Hard", selected == 2 ? WHITE : GRAY);
    }

    static uint64_t last_time_1 = 0;
    static uint64_t last_time_2 = 0;
    uint64_t t1 = get_timer_value();
    uint64_t t2 = get_timer_value();

    if (Get_Button(JOY_LEFT))
    {
      if (t1 - last_time_1 > (SystemCoreClock / 4000.0 * 300))
      {
        selected--;
        if (selected == -1)
        {
          selected = 2;
        }
        last_time_1 = t1;
      }
    }

    else if (Get_Button(JOY_RIGHT))
    {
      if (t2 - last_time_2 > (SystemCoreClock / 4000.0 * 300))
      {
        selected++;
        if (selected == 3)
        {
          selected = 0;
        }
        last_time_2 = t2;
      }
    }
  }
  set_difficulty(selected);
  LCD_Clear(BLACK);
}
void display_stats()
{
  char buffer[32];
  sprintf(buffer, "HP: %3d", player.lives);
  // sprintf(buffer, "S: %3d", player.speed);
  LCD_ShowString(1, 1, (const u8 *)buffer, WHITE);
  sprintf(buffer, "P: %3d", player.score);
  // sprintf(buffer, "Y: %3d", player.y);
  LCD_ShowString(1, 17, (const u8 *)buffer, WHITE);
}

void setup()
{
  IO_init(); // Initialize all I/O
  LCD_Clear(BLACK);
  u16 color = WHITE;
  int color_choice = 0;

  while (!Get_Button(JOY_RIGHT))
  {
    color_choice = display_splash_screen(color);
  }
  LCD_Clear(BLACK);
  select_difficulty(color_choice);
  init_player(&player, 10, 98, 5, GRED);
  init_walls(walls, NUM_WALLS);
  LCD_Clear(BLACK);
}

void handle_input()
{
  if (Get_Button(JOY_LEFT) || Get_Button(JOY_RIGHT) || Get_Button(JOY_UP) || Get_Button(JOY_DOWN) || Get_Button(JOY_CTR) || Get_Button(BUTTON_1) || Get_Button(BUTTON_2))
  // if (Get_Button(BUTTON_1) || Get_Button(BUTTON_2))
  {
    move_player_up(&player);
  }
  else
  {
    apply_gravity(&player);
  }
}

void game_loop()
{
  uint64_t last_frame_time = get_timer_value();
  while (1)
  {
    handle_input();
    move_walls(walls, NUM_WALLS);

    if (check_collision_with_walls(&player, walls, NUM_WALLS))
    {
      reset_player_position(&player, 10, 98);
      update_player_lives(&player, 1);
    }
    else
    {
      // check if player passed a wall
      for (int i = 0; i < NUM_WALLS - 2; i += 2) // only check the moving walls
      {
        if (player.x > walls[i].x + walls[i].width && i != player.last_passed_wall_index)
        {
          update_player_score(&player, 1);
          player.last_passed_wall_index = i; // update last passed wall index
          break;
        }
      }
      update_trace(&player);
    }

    draw_player(&player);
    draw_trace(&player);
    draw_walls(walls, NUM_WALLS);
    display_stats();
    delay_until_next_frame(last_frame_time);
    last_frame_time = get_timer_value();
  }
}

int main(void)
{

  setup();
  game_loop();
  return 0;
}
