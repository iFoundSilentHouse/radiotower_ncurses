/*
 * =====================================================================================
 *
 *       Filename:  radios.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  26.01.2024 21:33:56
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */
#include <stdlib.h>
#include <string.h>
#include <menu.h>

#define ARRAY_SIZE(a) (sizeof(a) / sizeof(a[0]))

char *choices[] = {
  "Industrial Junk",
  "Lonesome Town",
  "MadDog FM",
  "RRN Radio",
  "Vigilante2048",
  (void *)NULL,
};

void print_in_middle(WINDOW *win, int starty, int startx, int width, char *string) 
{
  int length, x, y;
  float temp;

  if(win == NULL)
    win = stdscr;
  getyx(win, y, x);
  if(startx != 0)
    x = startx;
  if(starty != 0)
    y = starty;
  if(width == 0)
    width = 80;

  length = strlen(string);
  temp = (width - length)/ 2;
  x = startx + (int)temp;
  mvwprintw(win, y, x, "%s", string);
  refresh();
}

int main() 
{
  FILE *filespc;
  char *filename;
  char signstr[80];
  char towerstr[40];

  ITEM **radio_names; 
  MENU *radio_menu;
  WINDOW *sign_win, *tower_win, *radio_menu_win;
  int c;
  int n_choices, i;
  int next_win_x, next_win_y;
  
  int radio_menu_size_x, radio_menu_size_y;
  char radio_link[60];
  char command[100];
  

  initscr();
  cbreak();
  noecho();
  keypad(stdscr, TRUE);
  if (has_colors()) {
    start_color();
    init_pair(1, COLOR_GREEN, COLOR_BLACK);
    attrset(COLOR_PAIR(1));
  }

  if ((COLS < 24) || (LINES < 9)) {
    endwin();
    printf("Looks like your terminal window \
is to small.\n\
just use mpg123 :)"); 
    exit(1);
  }
  
  next_win_x = 4, next_win_y = 2;

  if ((COLS > 85) && (LINES > 9 + 9)) {
    next_win_x = COLS/2 - 40;
    sign_win = newwin(6, 81, next_win_y, next_win_x);
    wattrset(sign_win, COLOR_PAIR(1));
    filename = malloc(30 * sizeof(char));
    strcpy(filename, "/etc/radiotower/sign_acii.txt");
    filespc = fopen(filename, "r");
    if (filespc == NULL) {
      endwin();
      printf("ERROR: NO ESSENTIAL FILE SIGN_ACII.TXT");
      exit(0);
    }
    while (fgets(signstr, 80, filespc) != NULL) 
      wprintw(sign_win, signstr);
    refresh();
    wrefresh(sign_win);
    
    next_win_y = 9;
    radio_menu_size_x = 81, radio_menu_size_y = LINES - 2 - 8;
  }
  else
    radio_menu_size_x = 22, radio_menu_size_y = 9;
  
  if ((COLS > 85) && (LINES > 9 + 9 + 22)) {
    next_win_x = COLS/2 - 22;
    tower_win = newwin(22, 41, next_win_y, next_win_x);
    wattrset(tower_win, COLOR_PAIR(1));
    filename = malloc(31 * sizeof(char));
    strcpy(filename, "/etc/radiotower/tower_acii.txt");
    filespc = fopen(filename, "r");
    if (filespc == NULL) {
      endwin();
      printf("ERROR: NO ESSENTIAL FILE TOWER_ACII.TXT");
      exit(0);
    }
    while (fgets(towerstr, 40, filespc) != NULL)
      wprintw(tower_win, towerstr);
    refresh();
    wrefresh(tower_win);
    next_win_y = 9 + 22;
    radio_menu_size_y = LINES - 2 - 8 - 22;
  }

  next_win_x = (COLS - radio_menu_size_x)/2;

  n_choices = ARRAY_SIZE(choices);
  radio_names = calloc(n_choices, sizeof(ITEM *));
  for(i = 0; i < n_choices; ++i)
    radio_names[i] = new_item(choices[i], choices[i]);
  
  radio_menu = new_menu((ITEM **)radio_names);
  menu_opts_off(radio_menu, O_SHOWDESC);
  set_menu_back(radio_menu, COLOR_PAIR(1));
  
  radio_menu_win = newwin(radio_menu_size_y, radio_menu_size_x, next_win_y, next_win_x);
  wattrset(radio_menu_win, COLOR_PAIR(1));
  keypad(radio_menu_win, TRUE);

  set_menu_win(radio_menu, radio_menu_win);
  set_menu_sub(radio_menu, derwin(radio_menu_win, n_choices-1, 20, 3, 1));
  set_menu_format(radio_menu, n_choices-1, 1);

  set_menu_mark(radio_menu, " > ");

  box(radio_menu_win, 0, 0);
  print_in_middle(radio_menu_win, 1, 0, radio_menu_size_x-1, "Radio Tower");
  mvwhline(radio_menu_win, 2, 1, ACS_HLINE, radio_menu_size_x-2);
  mvprintw(LINES - 1, 0, "\"q\" to exit");
  refresh();

  post_menu(radio_menu);
  wrefresh(radio_menu_win);

  while((c = wgetch(radio_menu_win)) != 'q') {
    switch(c){
      case KEY_DOWN:
      case 'j': /* vi-like binding */
        if(current_item(radio_menu) == menu_items(radio_menu)[n_choices-2]) {
          menu_driver(radio_menu, REQ_FIRST_ITEM);
          break;
        }
        menu_driver(radio_menu, REQ_DOWN_ITEM);
        break;
      case KEY_UP:
      case 'k': /* vi-like binding */
        if (current_item(radio_menu) == menu_items(radio_menu)[0]) {
          menu_driver(radio_menu, REQ_LAST_ITEM);
          break;
        }
        menu_driver(radio_menu, REQ_UP_ITEM);
        break;
      case 10: /* ENTER */
      case 'l': /* vi-like binding */
        if(current_item(radio_menu) == menu_items(radio_menu)[0])
          strcpy(radio_link, "http://listen.radiotower.su:8000/industrial_junk_320");
        else if(current_item(radio_menu) == menu_items(radio_menu)[1])
          strcpy(radio_link, "http://listen.radiotower.su:8000/lonesome_town_320");
        else if(current_item(radio_menu) == menu_items(radio_menu)[2])
          strcpy(radio_link, "http://listen.radiotower.su:8000/mad_dog_fm_320");       
        else if(current_item(radio_menu) == menu_items(radio_menu)[3])
          strcpy(radio_link, "http://listen.radiotower.su:8000/rrn_radio_320");
        else if(current_item(radio_menu) == menu_items(radio_menu)[4])
          strcpy(radio_link, "http://listen.radiotower.su:8000/vigilante_2084_320");

        def_prog_mode();
        endwin();
        strcpy(command, "mpg123 ");
        strcat(command, radio_link);
        system(command); 
        refresh();
    }
  }

  unpost_menu(radio_menu);
  free_menu(radio_menu);
  for(i = 0; i < n_choices; ++i)
    free_item(radio_names[i]);
  endwin();
  printf("%s", radio_link);
  return 0;
}
