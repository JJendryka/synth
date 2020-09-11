#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "../synth.h"

#include "generator.h"

#define OUTPUT_COUNT 1
#define INPUT_COUNT 1

static int output_count() {
    return OUTPUT_COUNT;
}

static void tick(struct synth *synth, struct device *device) {
    float frequency = *get_input(synth, device, 0);
    float value = sin(frequency * synth->counter / synth->config.sample_rate);
    *get_output(synth, device, 0) = value;
}

struct device sine(float default_frequency) {
    int *inputs = malloc(INPUT_COUNT*sizeof(float));
    inputs[0] = -1;

    float *default_inputs = malloc(INPUT_COUNT*sizeof(float));
    default_inputs[0] = default_frequency;

    struct device d = {
            .name = "sine",
            .tick = &tick,
            .output_count = &output_count,
            .outputs = malloc(sizeof(int)*OUTPUT_COUNT),
            .inputs = inputs,
            .default_inputs = default_inputs,
            .state = NULL,
    };

    return d;
}
