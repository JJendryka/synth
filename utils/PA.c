#include <stdio.h>
#include "PA.h"

PaStream* open_stream(int sample_rate, int num_of_inputs, int num_of_outputs, int frames_per_buffer, void *data, PaStreamCallback *callback) {
    PaStream *stream;

    PaError err = Pa_OpenDefaultStream( &stream,
                                        num_of_inputs,
                                        num_of_outputs,
                                        paFloat32,
                                        sample_rate,
                                        frames_per_buffer,
                                        callback,
                                        data );
    if (err != paNoError) {
        printf(  "PortAudio error: %s\n", Pa_GetErrorText( err ) );
    }
    return stream;
}

void start_stream(PaStream *stream) {
    PaError err = Pa_StartStream(stream);
    if (err != paNoError) {
        printf(  "PortAudio error: %s\n", Pa_GetErrorText( err ) );
    }
}

void stop_stream(PaStream *stream) {
    PaError err = Pa_StopStream(stream);
    if (err != paNoError) {
        printf(  "PortAudio error: %s\n", Pa_GetErrorText( err ) );
    }
}

void close_stream(PaStream *stream) {
    PaError err = Pa_CloseStream(stream);
    if (err != paNoError) {
        printf(  "PortAudio error: %s\n", Pa_GetErrorText( err ) );
    }
}


void initialize_pa() {
    PaError err = Pa_Initialize();
    if (err != paNoError) {
        printf(  "PortAudio error: %s\n", Pa_GetErrorText( err ) );
    }
}

void terminate_pa() {
    PaError err = Pa_Terminate();
    if (err != paNoError) {
        printf(  "PortAudio error: %s\n", Pa_GetErrorText( err ) );
    }
}