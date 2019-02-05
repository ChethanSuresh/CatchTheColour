#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>


// Export pin numbers
#define RED_GPIO 24  //GPIO1_24
#define YEL_GPIO 200 //GPIO7_8
#define BLU_GPIO 90  //GPIO3_26
#define BTN_GPIO 91  //GPIO3_27
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

struct color_map map[] = {
	{0, "RED"},
		{1, "YELLO"},
		{2, "BLUE"}
};


char switch_value_str[3];
int switch_state;
int switch_fd;
int to_catch = 0;

static void export_gpio(int pin){
	FILE *fd = fopen(GPIO_SYS "/export", "w");
	fprintf(fd,"%d",pin);
	fclose(fd);
}

static void direction_gpio(int pin, char direction[]){

	static char filename[100];
	snprintf (filename, sizeof filename, GPIO_SYS "/gpio%d/direction",pin);

	FILE *fd = fopen(filename, "w");
	if (fd == NULL){
		printf("Error opening %s/GPIO%d/direction", GPIO_SYS, pin);
		exit(1);
	}
	fprintf(fd,"%s", direction);
	fclose(fd);
}

// For this game only one LED can be on
// Caller should ON only one LED at a time
// on_led(1,0,0) (RED ON, YEL OFF, BLU OFF)
static void on_led(int red, int yel, int blu){

	static char red_filename[100];
	static char yel_filename[100];
	static char blu_filename[100];
	FILE *fd;

	snprintf (red_filename, sizeof red_filename, GPIO_SYS "/gpio%d/value",RED_GPIO);
	snprintf (yel_filename, sizeof yel_filename, GPIO_SYS "/gpio%d/value",YEL_GPIO);
	snprintf (blu_filename, sizeof blu_filename, GPIO_SYS "/gpio%d/value",BLU_GPIO);

	// RED LED 
	fd = fopen(red_filename, "w");
	if (fd == NULL){
		printf("Error opening %s/GPIO%d/value", GPIO_SYS, RED_GPIO);
		exit(1);
	}
	fprintf(fd,"%d", red);
	fclose(fd);

	// YEL LED 
	fd = fopen(yel_filename, "w");
	if (fd == NULL){
		printf("Error opening %s/GPIO%d/value", GPIO_SYS, YEL_GPIO);
		exit(1);
	}
	fprintf(fd,"%d", yel);
	fclose(fd);

	// BLU LED 
	fd = fopen(blu_filename, "w");
	if (fd == NULL){
		printf("Error opening %s/GPIO%d/value", GPIO_SYS, BLU_GPIO);
		exit(1);
	}
	fprintf(fd,"%d", blu);
	fclose(fd);
}

void fancy_dance(){
	int i;
	for (i=0;i<5;i++){
		on_led(1,0,0);
		usleep(200000);
		on_led(0,1,0);
		usleep(200000);
		on_led(0,0,1);
		usleep(200000);
	}
}

void init_gpio(){
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

int main(void){
	init_gpio();
	printf("*****************************************\n");
	printf("   !!! WELCOME TO CATCH THE COLOR !!!    \n");
	printf("      Select and catch the color         \n");
	printf("!!! Test your hand-eye coordination !!!  \n");
	printf("*****************************************\n");

	
	//fancy_dance();
	sleep(1);
	
	on_led(0,0,0);

	printf("\n\n\n");
	printf("STEP1: Click once to select color\n");
	printf("STEP2: Press and hold for 3sec to confirm selection\n");
	printf("\nAfter the game starts, click to choose color\n");

	int roll_count = 0;
	int i;
	on_led(1,0,0); //int to_catch = 0; // Default if no switch pressed

	/* User needs to select withing countdown 10sec*/
	/* If key pressed, countdown again */
	for (i=100;i>0;i--){
		switch_fd = open(SWITCH_DIR "/value", O_RDONLY);

		/* read key variable */
		read(switch_fd, switch_value_str, 3);
		switch_state = atoi(switch_value_str);
		usleep(100000);

		/* check key state, if enabled GPIO_DIR/value is 0 */
		if (switch_state == 0){
	
			/*Reset countdown timer*/
			i=100;

			//printf("pressed\n"); //DEBUG stetement
			roll_count++; 
			if (roll_count == 1){ 
				//Select YELLOW LED
				on_led(0,1,0);
				to_catch = 1;
				//printf("yellow led\n"); //DEBUG statement 
			} 
			if (roll_count == 2){ 
				//Select BLUE LED
				on_led(0,0,1);
				to_catch = 2;
				//printf("blue led\n"); //DEBUG statement
			}
		}
		if (roll_count == 3){
			roll_count = 0;
			//Select RED LED, roll back
			on_led(1,0,0);
			to_catch = 0;
			//printf("red led\n"); //DEBUG statement
		}
		close(switch_fd);
		printf("\rCOLOR TO CATCH: %s \t\t\t Game starts in:%d sec",map[roll_count].color ,i/10 );
		fflush(stdout);
	}
	printf("\n!!! START !!!\n");
	printf("\n\n\n");

	printf("*****************************************\n");
	printf("        !!!  CATCH THE COLOR !!!         \n");
	printf("*****************************************\n");

	return 0;
}