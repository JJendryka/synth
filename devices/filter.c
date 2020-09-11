#include <stdlib.h>
#include <immintrin.h>


#include "filter.h"

#define OUTPUT_COUNT 1
#define INPUT_COUNT 1

#define FILTER_TAP_NUM 104

static float filter_taps[FILTER_TAP_NUM] = {
        -0.00453025853724764,
        -0.0004244722309373247,
        -0.00041295007768144893,
        -0.0003781988848909957,
        -0.0003175951241515943,
        -0.00022917448175370633,
        -0.00011081109075649312,
        0.00003958957302493641,
        0.00022426757965491413,
        0.0004453847176242226,
        0.0007053587391462585,
        0.0010045477428064324,
        0.0013453444177285276,
        0.0017298247112267028,
        0.002157077575095166,
        0.002628894182740032,
        0.003145429632782208,
        0.003706665131748196,
        0.004312164940015707,
        0.004961224896791399,
        0.005652720671790257,
        0.006385028800924381,
        0.007161326828994566,
        0.007961336930079598,
        0.008807920645154111,
        0.009683726786808632,
        0.010584107739091724,
        0.011506965311913671,
        0.012449238516861987,
        0.013406978116419756,
        0.014375321482703669,
        0.015349043624356053,
        0.016322744553461308,
        0.017291723507424986,
        0.018249222608488432,
        0.019188847949022307,
        0.020107669431794525,
        0.020997794196471003,
        0.021854696892686622,
        0.022673062128141123,
        0.023447786031139586,
        0.024173871022769744,
        0.024846529566715153,
        0.025461240327586815,
        0.0260132285475316,
        0.026501821362618177,
        0.02691776965543959,
        0.027263012997580726,
        0.027534718520563696,
        0.027729588439538162,
        0.02784669376232335,
        0.027885721678220857,
        0.02784669376232335,
        0.027729588439538162,
        0.027534718520563696,
        0.027263012997580726,
        0.02691776965543959,
        0.026501821362618177,
        0.0260132285475316,
        0.025461240327586815,
        0.024846529566715153,
        0.024173871022769744,
        0.023447786031139586,
        0.022673062128141123,
        0.021854696892686622,
        0.020997794196471003,
        0.020107669431794525,
        0.019188847949022307,
        0.018249222608488432,
        0.017291723507424986,
        0.016322744553461308,
        0.015349043624356053,
        0.014375321482703669,
        0.013406978116419756,
        0.012449238516861987,
        0.011506965311913671,
        0.010584107739091724,
        0.009683726786808632,
        0.008807920645154111,
        0.007961336930079598,
        0.007161326828994566,
        0.006385028800924381,
        0.005652720671790257,
        0.004961224896791399,
        0.004312164940015707,
        0.003706665131748196,
        0.003145429632782208,
        0.002628894182740032,
        0.002157077575095166,
        0.0017298247112267028,
        0.0013453444177285276,
        0.0010045477428064324,
        0.0007053587391462585,
        0.0004453847176242226,
        0.00022426757965491413,
        0.00003958957302493641,
        -0.00011081109075649312,
        -0.00022917448175370633,
        -0.0003175951241515943,
        -0.0003781988848909957,
        -0.00041295007768144893,
        -0.0004244722309373247,
        -0.00453025853724764,
        0
};

struct IFilter {
    float history[FILTER_TAP_NUM + 14];
    unsigned int last_index;
};


void ifilter_init(struct IFilter* f) {
    int i;
    srand(0);
    for (i = FILTER_TAP_NUM + 13; i > FILTER_TAP_NUM + 6; i--) {
        f->history[i] = 0;
    }
    for (i = FILTER_TAP_NUM + 6; i > 6; i--) {
        f->history[i] = ((float)(rand() - RAND_MAX / 2)) / (RAND_MAX / 2);
    }
    for (i = 6; i >= 0; i--) {
        f->history[i] = 0;
    }
    f->last_index = FILTER_TAP_NUM + 6;
}

void ifilter_put(struct IFilter* f, float input) {
    f->history[f->last_index] = input;
    if (f->last_index == 7) {
        f->last_index = FILTER_TAP_NUM + 6;
    }
    else {
        f->last_index--;
    }
}

float sum(__m256 x) {
    __m128 hi = _mm256_extractf128_ps(x, 1);
    __m128 lo = _mm256_extractf128_ps(x, 0);
    lo = _mm_add_ps(hi, lo);
    hi = _mm_movehl_ps(hi, lo);
    lo = _mm_add_ps(hi, lo);
    hi = _mm_shuffle_ps(lo, lo, 1);
    lo = _mm_add_ss(hi, lo);
    return _mm_cvtss_f32(lo);
}

float ifilter_get(struct IFilter* f) {
    int i, j = 0;
    int index = f->last_index + 1;

    __m256 taps;
    __m256 data;

    __m256 acc = _mm256_setzero_ps();
    for (i = index; i < FILTER_TAP_NUM+7;) {
        taps = _mm256_load_ps(filter_taps + j);
        data = _mm256_loadu_ps(f->history + i);
        acc = _mm256_fmadd_ps(data, taps, acc);
        j+=8;
        i+=8;
    }
    j -= 8;
    for (i = i-(FILTER_TAP_NUM+8); i < index;) {
        taps = _mm256_load_ps(filter_taps + j);
        data = _mm256_loadu_ps(f->history + i);
        acc = _mm256_fmadd_ps(data, taps, acc);
        j += 8;
        i += 8;
    }
    return sum(acc);
}

static int output_count() {
    return OUTPUT_COUNT;
}

static void tick(struct synth *synth, struct device *device) {
    ifilter_put(device->state, *get_input(synth, device, 0));
    *get_output(synth, device, 0) = ifilter_get(device->state);
}

struct device filter() {

    int *inputs = malloc(INPUT_COUNT*sizeof(float));
    inputs[0] = -1;
    float *default_inputs = malloc(INPUT_COUNT*sizeof(float));
    default_inputs[0] = 0;

    struct IFilter *state = malloc(sizeof(struct IFilter));
    ifilter_init(state);

    struct device d = {
            .name = "filter",
            .tick = &tick,
            .output_count = &output_count,
            .outputs = malloc(sizeof(int)*OUTPUT_COUNT),
            .inputs = inputs,
            .default_inputs = default_inputs,
            .state = state,
    };

    return d;
}
