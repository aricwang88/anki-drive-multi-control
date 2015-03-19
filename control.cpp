#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <iostream>
using namespace std;

#define MAX_CARS_NUM 50



char * car1 = "F5:5C:27:8D:8C:6B";
char * car2 = "E0:9F:27:A7:2F:CF";

char * cars[MAX_CARS_NUM];
int pipes[MAX_CARS_NUM];
unsigned int cars_num = 0;

void initialize() {
	int i;
	for (i=0; i<MAX_CARS_NUM; i++) cars[i] = NULL;
	
	cars[0] = car1;
	cars[1] = car2;
	cars_num = 2;
}

void control_car(int car, int velocity, int accel, int lane_velocity, int lane_accel, int lane) {
	
}

int main(void)
{
	initialize();

	char cmd[100];

	int i;
	for(i=0; i<cars_num; i++) {
		char pipe[50] = "/tmp/car:";
		strcat(pipe, cars[i]);
		mkfifo(pipe, 0666);

		int pid = fork();
		cout << pid << endl;

		if (!pid) {
			char *name[] = {
				"./connect_car.sh",
				pipe,
				cars[i],
				NULL };
//			execvp(name[0], name);
			return 0;
		}

		int fd = open(pipe, O_WRONLY);
		pipes[i] = fd;
	}
	int fd;
	while(1) {
		if (fgets(cmd, 100, stdin) != NULL) {
			if(strcmp(cmd, "quit")==0) return 0;
			fd = pipes[0];
			cout << fd << endl;
			write(fd, cmd, strlen(cmd));
			usleep(100000);
			fd = pipes[1];
			cout << fd << endl;
			write(fd, cmd, strlen(cmd));
		}
	}
	
	return 0;
}
