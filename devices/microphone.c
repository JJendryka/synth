#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "portaudio.h"

#include "../synth.h"
#include "../utils/PA.h"

#include "microphone.h"
#include "../utils/queue.h"

#define OUTPUT_COUNT 1
#define INPUT_COUNT 0
#define FRAMES_PER_BUFFER 1024
#define QUEUE_SIZE 8

struct state {
    PaStream *stream;
    struct queue *queue;
    int index;
};


static int microphoneCallback(const void *inputBuffer, void *outputBuffer,
                           unsigned long framesPerBuffer,
                           const PaStreamCallbackTimeInfo* timeInfo,
                           PaStreamCallbackFlags statusFlags,
                           void *userData ) {
    struct state *state = (struct state*)userData;
    float *in = (float*)inputBuffer;
    (void) outputBuffer; // Prevent unused variable warning.

    if (state->queue->size < state->queue->maxsize) {
        for (int i = 0; i < FRAMES_PER_BUFFER; i++) {
            ((float *) rear(state->queue))[i] = in[i];
        }
        enqueue(state->queue);
    }

    return 0;

}

static int output_count() {
    return OUTPUT_COUNT;
}

static void tick(struct synth *synth, struct device *device) {
    struct state *state = (struct state*)device->state;

    if (size(state->queue)<=0) {
        while (size(state->queue) <= 2) {

        }
    }

    *get_output(synth, device, 0) = ((float*)front(state->queue))[state->index];

    state->index++;
    if (state->index == FRAMES_PER_BUFFER) {
        state->index = 0;
        dequeue(state->queue);
    }

}

struct device microphone(struct synth *synth) {

    int *inputs = malloc(INPUT_COUNT*sizeof(float));
    inputs[0] = -1;
    float *default_inputs = malloc(INPUT_COUNT*sizeof(float));
    default_inputs[0] = 0;

    struct state *state = malloc(sizeof(struct state));
    state->queue = new_queue(QUEUE_SIZE);
    state->index = 0;

    for (int i=0; i<QUEUE_SIZE; i++) {
        state->queue->items[i] = malloc(FRAMES_PER_BUFFER * sizeof(float));
    }

    state->stream = open_stream(synth->config.sample_rate, 1, 0, FRAMES_PER_BUFFER, state, &microphoneCallback);
    start_stream(state->stream);

    struct device d = {
            .name = "microphone",
            .tick = &tick,
            .output_count = &output_count,
            .outputs = malloc(sizeof(int)*OUTPUT_COUNT),
            .inputs = inputs,
            .default_inputs = default_inputs,
            .state = state,
    };

    return d;
}
