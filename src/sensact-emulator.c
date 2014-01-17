#include <stdio.h>
#include <execinfo.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "include/sensact/packet.h"
#include "include/sensact/sensact.h"
#include "sensact-emulator/emulator_sensors/sensact_emulator_engine.h"
#include "sensact-emulator/emulator_sensors/sensact_emulator_senshub.h"
#include "include/sensact/sensact-emulator.h"

int32_t id = 0;

const char * var = "";
/**
 * handles returned on connect
 */
enum sensors {
	vid_senshub = 0, vid_engine
};

/**
 * Handles used for read/write
 */
enum handles {
	pid_engine = 1, pid_senshub = 2
};

engine_t *engine;
senshub_t *senshub;

/**
 * modify the direction of the engine
 */
size_t set_direction(struct request_packet_t *req) {
	char direction = 0;
	memcpy(&direction, &req->data[req->name_length], req->value_length);
	engine->setdirection(direction);
	return req->value_length;
}

/**
 *modify the rpm
 */
size_t set_rpm(struct request_packet_t *req) {
	int rpm = 0;
	memcpy(&rpm, &req->data[req->name_length], req->value_length);
	engine->setrpm(rpm);
	return req->value_length;
}

/**
 * set point, used for motor speed control
 */
size_t set_point(struct request_packet_t *req) {
	int point = 0;
	memcpy(&point, &req->data[req->name_length], req->value_length);
	engine->setrpm(point);
	return req->value_length;
}
/**
 * set a integer var to the engine
 */
size_t engine_setint(const char * name, struct request_packet_t *req) {
	size_t retval = 0;
	if (!strcmp(name, engine->rpm_name)) {
		set_rpm(req);
	}
	if (!strcmp(name, "setpoint")) {
		set_point(req);
	}
	return retval;
}

/**
 * set a char var to the engine
 */
size_t engine_setchar(const char * name, struct request_packet_t *req) {
	size_t retval = 0;

	if (!strcmp(name, engine->direction_name)) {
		retval = set_direction(req);
	}
	return retval;
}
/**
 * read a var on the engine
 */
void engine_read(const char * name, char *data, size_t size) {

	if (!strcmp(var, engine->direction_name)) {
		char direction = engine->getdirection();
		memcpy(data, &direction, size);
	}

	if (!strcmp(var, "setpoint")) {
		int point = engine->getrpm();
		memcpy(data, &point, size);
	}

	if (!strcmp(var, engine->rpm_name)) {
		int rpm = engine->getrpm();
		memcpy(data, &rpm, size);
	}

}
/**
 * set roll to the senshub
 */
size_t senshub_setroll(struct request_packet_t *req) {
	float roll = 0;
	memcpy(&roll, &req->data[req->name_length], req->value_length);
	senshub->setroll(roll);
	return req->value_length;
}
/**
 * set pitch to the senshub
 */
size_t senshub_setpitch(struct request_packet_t *req) {
	float pitch = 0;
	memcpy(&pitch, &req->data[req->name_length], req->value_length);
	senshub->setpitch(pitch);
	return req->value_length;
}
/**
 * set yaw to the senshub
 */
size_t senshub_setyaw(struct request_packet_t *req) {
	float yaw = 0;
	memcpy(&yaw, &req->data[req->name_length], req->value_length);
	senshub->setyaw(yaw);
	return req->value_length;
}

/**
 * set light to the senshub
 */
size_t senshub_setlight(struct request_packet_t *req) {
	float light = 0;
	memcpy(&light, &req->data[req->name_length], req->value_length);
	senshub->setlight(light);
	return req->value_length;
}

/**
 * set presure to the senshub
 */
size_t senshub_setpresure(struct request_packet_t *req) {
	float presure = 0;
	memcpy(&presure, &req->data[req->name_length], req->value_length);

	senshub->setpresure(presure);
	printf("set preasure %f \n", presure);
	return req->value_length;
}
/**
 * set the temperature on the senshub
 */
size_t senshub_setobjTemp(struct request_packet_t *req) {
	float objTemp = 0;
	memcpy(&objTemp, &req->data[req->name_length], req->value_length);
	senshub->setobjtemp(objTemp);
	return req->value_length;
}
/**
 * set the ambient temperature on the senshub
 */
size_t senshub_setampTemp(struct request_packet_t *req) {
	float ampTemp = 0;
	memcpy(&ampTemp, &req->data[req->name_length], req->value_length);
	senshub->setambtemp(ampTemp);
	return req->value_length;
}

/**
 * set the humidity on the sensehub
 */
size_t senshub_sethumidity(struct request_packet_t *req) {
	float humidity = 0;
	memcpy(&humidity, &req->data[req->name_length], req->value_length);
	senshub->sethumidity(humidity);
	return req->value_length;
}
/**
 * read vars from the senshub
 */
void senshub_read(const char * name, char *data, size_t size) {
	if (!strcmp(name, senshub->ambtemp_name)) {
		float ambtemp = senshub->getambtemp();
		memcpy(data, &ambtemp, size);

	}
	if (!strcmp(name, senshub->humidity_name)) {
		float humidity = senshub->gethumidity();
		memcpy(data, &humidity, size);
	}
	if (!strcmp(name, senshub->objtemp_name)) {
		float objtemp = senshub->getobjtemp();
		memcpy(data, &objtemp, size);
	}
	if (!strcmp(name, senshub->light_name)) {
		float light = senshub->getlight();
		memcpy(data, &light, size);
	}
	if (!strcmp(name, senshub->pitch_name)) {
		uint32_t pitch = senshub->getpitch();
		memcpy(data, &pitch, size);
	}
	if (!strcmp(name, senshub->presure_name)) {
		float presure = senshub->getpresure();
		memcpy(data, &presure, size);
	}
	if (!strcmp(name, senshub->roll_name)) {
		uint32_t roll = senshub->getroll();
		memcpy(data, &roll, size);
	}
	if (!strcmp(name, senshub->yaw_name)) {
		uint32_t yaw = senshub->getyaw();
		memcpy(data, &yaw, size);
	}

}
/**
 * connect to the sensor from device_id
 */
int emulator_connect(int device_id, void *device) {
	int retval = 0;
	switch (device_id) {
	case vid_engine:
		engine = create_emulator_engine();
		break;
	case vid_senshub:
		senshub = create_senshub_emulator();
		break;
	}
	return retval;
}

int emulator_disconnect(int handle) {

	return 1;
}

int emulator_reconnect(int handle) {

	return 1;
}

/**
 * set a float to senshub
 */
size_t senshub_setfloat(const char * name, struct request_packet_t *req) {

	size_t retval = 0;

	if (!strcmp(name, senshub->ambtemp_name)) {
		retval = senshub_setampTemp(req);
	}

	if (!strcmp(name, senshub->humidity_name)) {
		retval = senshub_sethumidity(req);
	}

	if (!strcmp(name, senshub->objtemp_name)) {
		retval = senshub_setobjTemp(req);
	}

	if (!strcmp(name, senshub->light_name)) {
		retval = senshub_setlight(req);
	}

	if (!strcmp(name, senshub->presure_name)) {

		retval = senshub_setpresure(req);
	}

	return retval;

}
/**
 * set a int to senshub
 */
size_t senshub_setint(const char * name, struct request_packet_t *req) {
	size_t retval = 0;
	if (!strcmp(name, senshub->pitch_name)) {
		retval = senshub_setpitch(req);
	}

	if (!strcmp(name, senshub->roll_name)) {
		retval = senshub_setroll(req);
	}
	if (!strcmp(name, senshub->yaw_name)) {
		retval = senshub_setyaw(req);
	}

	return retval;
}

/**
 * write to handle
 */
int emulator_write(int handle, char *data, int length, int timeout) {

	struct request_packet_t *req;
	req = (struct request_packet_t*) data;

	unsigned char prefix = req->prefix;
	id = req->id;

	unsigned char name_length = req->name_length;
	unsigned short value_length = req->value_length;

	char *name = malloc(name_length);
	memcpy(name, req->data, name_length);
	name[name_length] = 0;

	var = name;
	enum command_t code = (int) req->command_code;
	switch (code) {
	case GET_CHAR:
	case GET_DATA:
	case GET_FLOAT:
	case GET_INT:
	case GET_SHORT:
	case SET_SHORT:
		break;
	case SET_DATA:
	case SET_FLOAT:
		switch (handle) {
		case pid_senshub:
			senshub_setfloat(var, req);
			break;
		case pid_engine:
		default:
			break;

		}
		break;

	case SET_CHAR:
		switch (handle) {
		case pid_engine:
			engine_setchar(var, req);
			break;
		case pid_senshub:
			break;
		}
		break;
	case SET_INT:
		switch (handle) {
		case pid_engine:
			engine_setint(var, req);
			break;
		case pid_senshub:
			senshub_setint(var, req);
			break;
		}
		break;
	}
	return length;
}
/**
 * get the lenght of a certain var
 */
size_t getlenght(const char * var) {
	size_t size = 0;
	if (engine != NULL) {
		if (!strcmp(var, engine->direction_name)) {
			size = sizeof(char);
		}
		if (!strcmp(var, engine->rpm_name) || (!strcmp(var, "setpoint"))) {
			size = sizeof(uint32_t);
		}
	}
	if (senshub != NULL) {
		if (!strcmp(var, senshub->ambtemp_name)) {
			size = sizeof(float);
		}
		if (!strcmp(var, senshub->humidity_name)) {
			size = sizeof(float);
		}
		if (!strcmp(var, senshub->light_name)) {
			size = sizeof(float);
		}
		if (!strcmp(var, senshub->objtemp_name)) {
			size = sizeof(float);
		}
		if (!strcmp(var, senshub->pitch_name)) {
			size = sizeof(int);
		}
		if (!strcmp(var, senshub->presure_name)) {
			size = sizeof(float);
		}
		if (!strcmp(var, senshub->roll_name)) {
			size = sizeof(int);
		}
		if (!strcmp(var, senshub->yaw_name)) {
			size = sizeof(int);
		}

	}
	return size;
}
/**
 * read data from sensors
 */
int emulator_read(int handle, char *data, int length, int timeout) {
	size_t size = getlenght(var);
	if (length == PACKET_RSP_HEADER_SIZE) {
		struct response_packet_t *response = (struct response_packet_t*) data;
		response->prefix = PACKET_PREFIX;
		response->data_length = sizeof(uint16_t);
		response->id = id;
		response->response_code = RSP_OK;
		response->data_length = getlenght(var);
	} else {
		size_t size = getlenght(var);
		switch (handle) {
		case pid_engine:
			engine_read(var, data, size);
			break;
		case pid_senshub:
			senshub_read(var, data, size);
			break;
		}
	}
	return size;
}

senshub_t * getEmuSensHub() {
	return senshub;
}
engine_t * getEmuEngine() {
	return engine;
}

struct sa_backend_t emulator_backend = { .name = "emulator", .connect =
		emulator_connect, .disconnect = emulator_disconnect, .read =
		emulator_read, .write = emulator_write, };
