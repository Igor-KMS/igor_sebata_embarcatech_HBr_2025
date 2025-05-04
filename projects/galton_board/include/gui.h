#ifndef GUI_H
#define GUI_H

  #define MAX_MENU_ITEMS 4
  #define MAX_PAGES 4

  typedef struct
  {
    char *title;
    char *items[MAX_MENU_ITEMS];
    int item_count;
    int selected_index;
    int saved_index;
  } Page;

  extern Page pages[MAX_PAGES];

  extern int up_pressed;
  extern int down_pressed;
  extern int select_pressed;

  extern int current_page_index;

  void draw_page(Page *);
  void handle_input(void);

#endif
