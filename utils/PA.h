#ifndef SYNTH_PA_H
#define SYNTH_PA_H

#include "portaudio.h"

void initialize_pa();
void terminate_pa();

PaStream* open_stream(int sample_rate, int num_of_inputs, int num_of_outputs, int frames_per_buffer, void *data, PaStreamCallback *callback);
void start_stream(PaStream *stream);
void stop_stream(PaStream *stream);
void close_stream(PaStream *stream);

#endif //SYNTH_PA_H
