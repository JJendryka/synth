#include "../synth.h"
#include "device.h"

float* get_input(struct synth *synth, struct device *device, int input) {
    if (device->inputs[input] >= 0) {
        return synth->buffer + device->inputs[input] * 2 + synth->buffer_index;
    }
    else {
        return device->default_inputs + input;
    }
}
float* get_output(struct synth *synth, struct device *device, int output) {
    return synth->buffer + device->outputs[output] * 2 + !synth->buffer_index;
}