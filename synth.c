#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdio.h>

#include "utils/PA.h"

#include "synth.h"


void tick_synth(struct synth *synth) {
    for (int i=0; i<synth->device_count; i++) {
        synth->devices[i].tick(synth, &synth->devices[i]);
    }
    synth->counter++;
    synth->buffer_index = !synth->buffer_index;
}

void run(struct synth *synth, unsigned long long limit) {
    clock_t start = clock(), diff;
    while (limit == 0 || synth->counter < limit) {
        tick_synth(synth);
    }
    diff = clock() - start;
    int msec = diff * 1000 / CLOCKS_PER_SEC;
    printf("Time taken %d seconds %d milliseconds", msec/1000, msec%1000);
}

int add_device(struct synth *synth, struct device device) {
    if (synth->device_count == 0) {
        synth->devices = (struct device*) malloc(sizeof(struct device));
        synth->buffer = (float*) malloc(device.output_count() * 2 * sizeof(float));
    }
    else {
        ////TODO: Check if realloc worked
        synth->devices = realloc(synth->devices, sizeof(struct device) * (synth->device_count + 1));
        synth->buffer = realloc(synth->buffer, (synth->device_count + device.output_count()) * 2 * sizeof(float));
    }
    synth->devices[synth->device_count] = device;
    memset(synth->buffer + (synth->buffer_size*2), 0, device.output_count() * 2 * sizeof(float));
    for (int i=0; i<device.output_count(); i++) {
        device.outputs[i]=synth->buffer_size+i;
    }

    synth->device_count++;
    synth->buffer_size += device.output_count();

    return synth->device_count - 1;
}

void connect_devices(struct synth *synth, int out_device, int output, int in_device, int input) {
    synth->devices[in_device].inputs[input] = synth->devices[out_device].outputs[output];
}