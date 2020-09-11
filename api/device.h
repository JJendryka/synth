#ifndef SYNTH_DEVICE_H
#define SYNTH_DEVICE_H

struct synth;
struct device;
typedef void (*tick_f)(struct synth *synth, struct device *device);
typedef int (*output_count_f)();

struct device {
    char *name;
    tick_f tick;
    output_count_f output_count;
    int *inputs;
    float *default_inputs;
    int *outputs;
    void *state;
};

float* get_input(struct synth *synth, struct device *device, int input);
float* get_output(struct synth *synth, struct device *device, int output);

#endif //SYNTH_DEVICE_H
