#ifndef SYNTH_SYNTH_H
#define SYNTH_SYNTH_H

#include "api/device.h"
struct synth_config {
    int sample_rate;
};

struct synth {
    struct synth_config config;

    struct device *devices;
    unsigned int device_count;

    float *buffer;
    unsigned int buffer_size;

    unsigned long long counter;
    unsigned short buffer_index;

};

void tick_synth(struct synth *synth);

void run(struct synth *synth, unsigned long long limit);
int  add_device(struct synth *synth, struct device device);
void connect_devices(struct synth *synth, int out_device, int output, int in_device, int input);

#endif //SYNTH_SYNTH_H
