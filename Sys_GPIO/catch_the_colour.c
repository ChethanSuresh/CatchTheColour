#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

// Export pin numbers
#define RED_GPIO 24  // GPIO1_24
#define YEL_GPIO 200 // GPIO7_8
#define BLU_GPIO 90  // GPIO3_26
#define BTN_GPIO 91  // GPIO3_27
#define SWITCH_DIR "/sys/class/gpio/gpio91"

// To access /sys/class/gpio direct
#define RED gpio24
#define YEL gpio200
#define BLU gpio90
//#define BTN gpio91

#define IN "in"
#define OUT "out"

#define GPIO_SYS "/sys/class/gpio"

struct color_map {
  const int number;
  const char *color;
};

struct color_map map[] = {{0, "RED"}, {1, "YELLO"}, {2, "BLUE"}};

char switch_value_str[3];
int switch_state;
int switch_fd;
int to_catch = 0;

static void export_gpio(int pin) {
  FILE *fd = fopen(GPIO_SYS "/export", "w");
  fprintf(fd, "%d", pin);
  fclose(fd);
}

static void direction_gpio(int pin, char direction[]) {

  static char filename[100];
  snprintf(filename, sizeof filename, GPIO_SYS "/gpio%d/direction", pin);

  FILE *fd = fopen(filename, "w");
  if (fd == NULL) {
    printf("Error opening %s/GPIO%d/direction", GPIO_SYS, pin);
    exit(1);
  }
  fprintf(fd, "%s", direction);
  fclose(fd);
}

// For this game only one LED can be on
// Caller should ON only one LED at a time
// on_led(1,0,0) (RED ON, YEL OFF, BLU OFF)
static void on_led(int red, int yel, int blu) {

  static char red_filename[100];
  static char yel_filename[100];
  static char blu_filename[100];
  FILE *fd;

  snprintf(red_filename, sizeof red_filename, GPIO_SYS "/gpio%d/value",
           RED_GPIO);
  snprintf(yel_filename, sizeof yel_filename, GPIO_SYS "/gpio%d/value",
           YEL_GPIO);
  snprintf(blu_filename, sizeof blu_filename, GPIO_SYS "/gpio%d/value",
           BLU_GPIO);

  // RED LED
  fd = fopen(red_filename, "w");
  if (fd == NULL) {
    printf("Error opening %s/GPIO%d/value", GPIO_SYS, RED_GPIO);
    exit(1);
  }
  fprintf(fd, "%d", red);
  fclose(fd);

  // YEL LED
  fd = fopen(yel_filename, "w");
  if (fd == NULL) {
    printf("Error opening %s/GPIO%d/value", GPIO_SYS, YEL_GPIO);
    exit(1);
  }
  fprintf(fd, "%d", yel);
  fclose(fd);

  // BLU LED
  fd = fopen(blu_filename, "w");
  if (fd == NULL) {
    printf("Error opening %s/GPIO%d/value", GPIO_SYS, BLU_GPIO);
    exit(1);
  }
  fprintf(fd, "%d", blu);
  fclose(fd);
}

void fancy_dance() {
  int i;
  for (i = 0; i < 5; i++) {
    on_led(1, 0, 0);
    usleep(100000);
    on_led(0, 1, 0);
    usleep(100000);
    on_led(0, 0, 1);
    usleep(100000);
  }
}

int game(int to_catch) {

  /* Set the life level speed variables*/
  int life = 3;
  int level = 1;
  int speed = 11;
  int i, j;
  // printf("main game to catch: %d\n", to_catch);

  while (1) {

    /* Toggle LED sequence */
    for (i = 0; i < 3; i++) {
      if (i == 0)
        on_led(1, 0, 0);
      if (i == 1)
        on_led(0, 1, 0);
      if (i == 2)
        on_led(0, 0, 1);

      /* Detect key press during sequence */
      for (j = 0; j < speed; j++) {
        switch_fd = open(SWITCH_DIR "/value", O_RDONLY);

        /* read key variable */
        read(switch_fd, switch_value_str, 3);
        switch_state = atoi(switch_value_str);
        usleep(100000);

        /* check key state, if enabled GPIO_DIR/value is 0 */
        if (switch_state == 0) {
          // printf("Pressed");
          if (i == to_catch) {
            printf("\rYes Level: %d  Life: %d", level, life);
            fflush(stdout);
            speed--;
            level++;
            if (level == 10) {
              return 0;
            }
            // break;
          } else {
            printf("\rNoo Level: %d  Life: %d", level, life);
            life--;
            if (life == 0) {
              return 1;
            }
            fflush(stdout);
          }
        }
        close(switch_fd);
      }
    }
  }
}

void init_gpio() {
  // Export GPIO pins for use
  export_gpio(RED_GPIO);
  export_gpio(BLU_GPIO);
  export_gpio(YEL_GPIO);
  export_gpio(BTN_GPIO);

  // Set directions for pins
  direction_gpio(RED_GPIO, OUT);
  direction_gpio(BLU_GPIO, OUT);
  direction_gpio(YEL_GPIO, OUT);
  direction_gpio(BTN_GPIO, IN);
}

int selection() {
  /* Color selection by user */
  int roll_count = 0;
  int i;
  on_led(1, 0, 0); // int to_catch = 0; // Default if no switch pressed

  /* User needs to select withing countdown 10sec*/
  /* If key pressed, countdown again */
  for (i = 100; i > 0; i--) {
    switch_fd = open(SWITCH_DIR "/value", O_RDONLY);

    /* read key variable */
    read(switch_fd, switch_value_str, 3);
    switch_state = atoi(switch_value_str);
    usleep(100000);

    /* check key state, if enabled GPIO_DIR/value is 0 */
    if (switch_state == 0) {

      /*Reset countdown timer*/
      i = 100;

      // printf("pressed\n"); //DEBUG stetement
      roll_count++;
      if (roll_count == 1) {
        // Select YELLOW LED
        on_led(0, 1, 0);
        to_catch = 1;
        // printf("yellow led\n"); //DEBUG statement
      }
      if (roll_count == 2) {
        // Select BLUE LED
        on_led(0, 0, 1);
        to_catch = 2;
        // printf("blue led\n"); //DEBUG statement
      }
    }
    if (roll_count == 3) {
      roll_count = 0;
      // Select RED LED, roll back
      on_led(1, 0, 0);
      to_catch = 0;
      // printf("red led\n"); //DEBUG statement
    }
    close(switch_fd);
    printf("\rCOLOR TO CATCH: %s \t\t\t Game starts in:%d sec",
           map[roll_count].color, i / 10);
    fflush(stdout);
  }

  /* Off all LED for a brief time before game starts */
  on_led(0, 0, 0);
  sleep(1);

  printf("\n\n\n");
  printf("                      START                   \n");
  printf("        !!!  CATCH THE COLOR: %s!!!         \n",
         map[roll_count].color);
  printf("*********************************************\n");
}

int main(void) {
  system("@cls||clear");
  init_gpio();

  printf("*****************************************\n");
  printf("   !!! WELCOME TO CATCH THE COLOR !!!    \n");
  printf("      Select and catch the color         \n");
  printf("!!! Test your hand-eye coordination !!!  \n");
  printf("*****************************************\n");

  fancy_dance();

  // fancy_dance();
  sleep(1);

  on_led(0, 0, 0);

  printf("\n\n\n");
  printf("STEP1: Click once to select over colors\n");
  printf("STEP2: Wait for countdown to start game\n");
  printf("\n\n");

  /* Press key to continue, wait for user to read manual */
  printf("Press Key to continue...\n");
  for (;;) {
    switch_fd = open(SWITCH_DIR "/value", O_RDONLY);

    /* read key variable */
    read(switch_fd, switch_value_str, 3);
    switch_state = atoi(switch_value_str);
    usleep(100000);

    /* check key state, if enabled GPIO_DIR/value is 0 */
    if (switch_state == 0) {
      break;
    }
  }

  to_catch = selection();

  /* Exit, based on win or lose */
  int ret = game(to_catch);
  if (ret == 0) {
    printf("\n\n\n!!! GRAND WIN !!!\n\n\n");
  } else if (ret == 1) {
    printf("\n\n\n!!! GRAND LOSE !!!\n\n\n");
  }

  /* Reset all LEDs to 0*/
  on_led(0, 0, 0);

  return 0;
}
