#include <stdlib.h>
#include <stdio.h>

#include "../synth.h"

#include "printer.h"

#define OUTPUT_COUNT 1
#define INPUT_COUNT 0

static int output_count() {
    return OUTPUT_COUNT;
}

static void tick(struct synth *synth, struct device *device) {
    printf("%f\n", *get_input(synth, device, 0));
}

struct device printer() {

    int *inputs = malloc(INPUT_COUNT*sizeof(float));
    inputs[0] = -1;
    float *default_inputs = malloc(INPUT_COUNT*sizeof(float));
    default_inputs[0] = 0;

    struct device d = {
            .name = "printer",
            .tick = &tick,
            .output_count = &output_count,
            .outputs = malloc(sizeof(int)*OUTPUT_COUNT),
            .inputs = inputs,
            .default_inputs = default_inputs,
            .state = NULL,
    };

    return d;
}
