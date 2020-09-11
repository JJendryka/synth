#include <stdio.h>
#include "synth.h"
#include "devices/printer.h"
#include "devices/generator.h"
#include "devices/speaker.h"
#include "utils/PA.h"
#include "devices/microphone.h"
#include "devices/filter.h"

int main() {
    struct synth_config synth_config = {
            .sample_rate = 44100
    };
    struct synth synth = {
            .buffer_index = 0,
            .device_count = 0,
            .counter = 0,
            .config = synth_config,
    };

    initialize_pa();

//    int sine_handle = add_device(&synth, sine(2000.0));
//    int printer_handle = add_device(&synth, printer());
    int size = 500;
    int filter_handles[size];
    for (int i=0; i<size; i++) {
        filter_handles[i] = add_device(&synth, filter());
    }
    int microphone_handle = add_device(&synth, microphone(&synth));
    int speaker_handle = add_device(&synth, speaker(&synth));

    connect_devices(&synth, microphone_handle, 0, filter_handles[0], 0);
    for (int i=0; i<size-1; i++) {
        connect_devices(&synth, filter_handles[i], 0, filter_handles[i+1], 0);
    }
    connect_devices(&synth, filter_handles[size-1], 0, speaker_handle, 0);

    run(&synth, 0);
    terminate_pa();
    return 0;
}
