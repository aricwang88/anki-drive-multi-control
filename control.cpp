#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <string.h>
#include <iostream>
using namespace std;

#define MAX_CARS_NUM 50



char car1[20] = "F5:5C:27:8D:8C:6B";
char car2[20] = "E0:9F:27:A7:2F:CF";

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

void control_car_raw(int car, char* cmd) {
		if (car >=0 && car <cars_num) {
			int fd = pipes[car];
			write(fd, cmd, strlen(cmd));
			usleep(100000);
		}
		else {
			cout << "no such car" << endl;
		}
}

void control_car(int car, int velocity, int accel, int lane_velocity, int lane_accel, int lane) {
		if (car >=0 && car <cars_num) {
			int fd = pipes[car];
//			cout << fd << endl;
			char buffer[100];
		
			sprintf(buffer, "set-speed %d %d\n", velocity, accel);
			write(fd, buffer, strlen(buffer));
			usleep(100000);

			sprintf(buffer, "change-lane %d %d %d\n", lane_velocity, lane_accel, lane);
			write(fd, buffer, strlen(buffer));
			usleep(100000);
		}
		else {
			cout << "no such car" << endl;
		}
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

	for (i=0; i<cars_num; i++) {
		control_car_raw(i, "connect\n");
		control_car_raw(i, "sdk-mode 1\n");
	}

	while(1) {
		cout << "-----------------" << endl;
		cout << "-1: quit" << endl;
		cout << "0:all, 1,2:each" << endl;
		cout << "velocity, accel" << endl;
		cout << "lane: velocity, accel, position" << endl;
		cout << "> " << endl;
		int select, vel, acc, l_vel, l_acc, l_pos;
		cin >> select;
		if (select==-1) {
			for (i=0; i<cars_num; i++) {
				control_car_raw(i, "quit\n");
			}
			return 0;
		}
		cin >> vel >> acc >> l_vel >> l_acc >> l_pos;

		if(select == 0) {
			for (i=0; i<cars_num; i++) {
				control_car(i, vel, acc, l_vel, l_acc, l_pos);
			}
		} else {
			control_car(select-1, vel, acc, l_vel, l_acc, l_pos);
		}
	}
	
	return 0;
}
