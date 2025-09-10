#pragma once

enum control
{
    CORRIENTE,
    TENSION
};

void pi_set_ctrl(int ctrl);
int pi_cb(float xread);
