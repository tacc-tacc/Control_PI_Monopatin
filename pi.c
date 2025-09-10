#include "pi.h"

#define KADCI 10.0f/1023.0f
#define KADCV 50.0f/1023.0f

#define KP 1
#define KI 100
#define A 0.998
#define B -0.81

#define ICLAMP 1.0f
#define OCLAMP 10000.0f
#define KOUT 50.0f
#define OFFSET KOUT

float delta_t = 0;
float e = 0.0, i = 0.0;
float sp = 25.0f;
float out = 0;
int ctrl = 0;

void pi_init(float delta_t)
{

}

void pi_set_ctrl(int ctrl_)
{
	ctrl = ctrl_;
	e = 0;
	i = 0;
}

int pi_cb(float vread, float iread)
{
    xread = ctrl == CORRIENTE ? xread * KADCI : xread * KADCV;
	e = sp - xread;
	i += e * DT;
	if (i > ICLAMP)
		i = ICLAMP;
	else if (i < -ICLAMP)
		i = -ICLAMP;

	float tmp = KP * e + KI * i;
	out = KOUT*tmp + OFFSET;
	if (out > OCLAMP)
		out = OCLAMP;
	if (out < 0)
		out = 0;

	return out;
}

