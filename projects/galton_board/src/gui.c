#include <stdio.h>
#include <stdlib.h>
#include "pico/stdlib.h"
#include "display.h"
#include "gui.h"
#include "hardware/adc.h"

int up_pressed = 0;
int down_pressed = 0;
int select_pressed = 0;

Page pages[MAX_PAGES] = {
    {.title = "Bem-vindo!",
     .items = {"Iniciar"},
     .item_count = 1,
     .selected_index = 0,
     .saved_index = -1},
    {.title = "RNG",
     .items = {"Simples", "Pico SDK"},
     .item_count = 2,
     .selected_index = 0,
     .saved_index = -1},
    {.title = "Enviesar?",
     .items = {"Sem vies", "Esquerda", "Direita"},
     .item_count = 3,
     .selected_index = 0,
     .saved_index = -1},
    {.title = "Bolas",
     .items = {"100", "200", "300", "400"},
     .item_count = 4,
     .selected_index = 0,
     .saved_index = -1}};

int current_page_index = 0;

void draw_page(Page *page)
{
  ssd1306_draw_string(&display, D_WIDTH / 4, 0, 1, page->title);

  for (int i = 0; i < page->item_count && i < MAX_MENU_ITEMS; i++)
  {
    int y = 20 + i * 10;
    if (i == page->selected_index)
    {
      ssd1306_draw_empty_square(&display, 0, y - 2, D_WIDTH - 1, 10);
      ssd1306_draw_string(&display, 10, y, 1, page->items[i]);
    }
    else
    {
      ssd1306_draw_string(&display, 10, y, 1, page->items[i]);
    }
  }

  // if (page->saved_index >= 0)
  // {
  //   char buf[64];
  //   snprintf(buf, sizeof(buf), "-- %s --", page->items[page->saved_index]);
  //   ssd1306_draw_string(&display, 0, 62, 1, buf);
  // }
}

void check_input()
{
  static int flag = 0;
  adc_select_input(0);
  uint16_t jy = adc_read();
  adc_select_input(1);
  uint16_t jx = adc_read();

  if (jy > 3000)
    up_pressed = true;
  else if (jy < 1000)
    down_pressed = true;
  else if (!gpio_get(22) && !flag)
  {
    select_pressed = true;
    flag = 1;
  }
  else if (gpio_get(22))
  {
    flag = 0;
  }
}

void handle_input()
{
  Page *page = &pages[current_page_index];

  check_input();
  if (up_pressed)
  {
    page->selected_index = (page->selected_index - 1 + page->item_count) % page->item_count;
  }
  else if (down_pressed)
  {
    page->selected_index = (page->selected_index + 1) % page->item_count;
  }
  else if (select_pressed)
  {
    page->saved_index = page->selected_index;
    current_page_index++;
  }

  up_pressed = 0;
  down_pressed = 0;
  select_pressed = 0;
}
