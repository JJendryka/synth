#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "portaudio.h"

#include "../synth.h"
#include "../utils/PA.h"

#include "speaker.h"
#include "../utils/queue.h"

#define OUTPUT_COUNT 0
#define INPUT_COUNT 1
#define FRAMES_PER_BUFFER 1024
#define QUEUE_SIZE 8

struct state {
    PaStream *stream;
    struct queue *queue;
    int index;
};


static int speakerCallback(const void *inputBuffer, void *outputBuffer,
                           unsigned long framesPerBuffer,
                           const PaStreamCallbackTimeInfo* timeInfo,
                           PaStreamCallbackFlags statusFlags,
                           void *userData ) {
    struct state *state = (struct state*)userData;
    float *out = (float*)outputBuffer;
    (void) inputBuffer; // Prevent unused variable warning.

    if (size(state->queue) > 0) {
        for(int i=0; i<framesPerBuffer; i++)
        {
            out[i] = ((float*)front(state->queue))[i];
        }
        dequeue(state->queue);
    }
    else {
        printf("SPEAKER QUEUE EMPTY\n");
        for(int i=0; i<framesPerBuffer; i++ )
        {
            out[i] = 0;
        }
    }
    return 0;

}

static int output_count() {
    return OUTPUT_COUNT;
}

static void tick(struct synth *synth, struct device *device) {
    struct state *state = (struct state*)device->state;

    while (state->queue->size >= state->queue->maxsize) {
        ////TODO: Implement better waiting
    }

    ((float*)rear(state->queue))[state->index] = *get_input(synth, device, 0);

    state->index++;
    if (state->index == FRAMES_PER_BUFFER) {
        state->index = 0;
        enqueue(state->queue);
    }
}

struct device speaker(struct synth *synth) {

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

    state->stream = open_stream(synth->config.sample_rate, 0, 1, FRAMES_PER_BUFFER, state, &speakerCallback);
    start_stream(state->stream);

    struct device d = {
            .name = "speaker",
            .tick = &tick,
            .output_count = &output_count,
            .outputs = malloc(sizeof(int)*OUTPUT_COUNT),
            .inputs = inputs,
            .default_inputs = default_inputs,
            .state = state,
    };

    return d;
}
