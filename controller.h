#ifndef _CONTROLLER_H_
#define _CONTROLLER_H_

#define TEMPSET 21 // Set point for temperature
#define HUMSET 30  // Set point for humidity
#define MAXDEV 89 // maximum number of devices
#define LOWEST_ID 11 //the lowest device ID

#include "message.h"

int is_valid_type(struct cignal *);
int is_registered(int, int *);
int register_device(int *);
void adjust_fan(struct cignal *);
int process_message(struct cignal *, int *);

#endif
