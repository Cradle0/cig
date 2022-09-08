#ifndef _MESSAGE_H_
#define _MESSAGE_H_

#define CIGLEN 20 // the fix message length (20 Characters) 
#define INTERVAL 3 	  // Time to sleep in between messages

// Code to send for 
#define ON 1
#define OFF 0

// Sensor device types
#define TEMPERATURE 1
#define HUMIDITY 2

// Message types
#define HANDSHAKE 1 // Intial message
#define UPDATE 2    // Get ssensor update
#define FAN_STATUS 3 // Fan status of air conditioner or dehumidifier


struct header{
	int device_id; //Two digits number between 11 to 99.
	int device_type; //One digit number --> 1 for temperature and 2 for humidity
	int type; //One digit number --> 1 for handshake, 2 for sensors update, and 3 for Fan status report.
};

struct cignal{
	struct header hdr;
	float value; //Contains the value for the Sensors read. (Format: AB.WXYZ)
	int cooler; //0 for OFF and 1 for ON
    int dehumid; //0 for OFF and 1 for ON	
};

char *serialize_cignal(struct cignal);
void unpack_cignal(char *, struct cignal *);
#endif
