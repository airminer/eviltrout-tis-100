#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <ncurses.h>

#include "util.h"
#include "program.h"
#include "output.h"
#include <sys/time.h>


int main(int argc, char **argv) {
  struct timeval  tv1, tv2;
  gettimeofday(&tv1, NULL);

  Program program;
  Program *p = &program;

#ifdef RICH_OUTPUT
  initscr();
	raw();
	keypad(stdscr, TRUE);
	noecho();
#endif

  program_init(p);
  program_load_system(p, "refimpl.sys");
  program_load_code(p, "refimpl.tis");

  int tickcount = 0;

#ifdef RICH_OUTPUT
  output_program(p);
  int code = getch();
  while (code != 'q') {
    program_tick(p);
    tickcount++;
    clear();
    output_program(p);
    refresh();
    code = getch();
  }
#else
  int all_blocked_count = 0;
  while (all_blocked_count < 5) {
    int all_blocked = program_tick(p);
    tickcount++;
    if (all_blocked) {
      all_blocked_count++;
    } else {
      all_blocked_count = 0;
    }
  }
#endif

#ifdef RICH_OUTPUT
  endwin();
#endif
  program_clean(p);


  gettimeofday(&tv2, NULL);
  printf ("Clock cycles computed: %d\nElapsed milliseconds: %f\n", tickcount,
    (double) (tv2.tv_usec - tv1.tv_usec) / 1000 +
    (double) (tv2.tv_sec - tv1.tv_sec) * 1000);
}
