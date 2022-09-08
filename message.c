#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "message.h"

/* Return the string represenation of a cignal message.
 */
char *serialize_cignal(struct cignal cig){
	char *result = malloc(sizeof(char)*CIGLEN);
	snprintf(result, CIGLEN, "%d|%d|%d|%.4f|%d|%d|", 
				cig.hdr.device_id, 
				cig.hdr.device_type, 
				cig.hdr.type, 
				cig.value, 
				cig.cooler, 
				cig.dehumid);

	return result;
}

/* Unpack a string representation of a cignal message, and store the values
 * in cig.
 */
void unpack_cignal(char *msg, struct cignal *cig){
	sscanf(msg, "%d|%d|%d|%f|%d|%d|", 
			&cig->hdr.device_id, 
			&cig->hdr.device_type, 
			&cig->hdr.type, 
			&cig->value, 
			&cig->cooler, 
			&cig->dehumid);
}
