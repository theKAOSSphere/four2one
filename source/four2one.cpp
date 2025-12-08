//
// integra.cpp - Integra & Pulverize LV2 Plugins
// Copyright (C) 2025 KAOSS
// SPDX-License-Identifier: GPL-3.0-or-later
//

#include "lv2/lv2plug.in/ns/lv2core/lv2.h"
#include <cstdlib>
#include <cmath>

// Dynamic URI based on build target
#define PLUGIN_URI "https://github.com/theKAOSSphere/four2one"

enum portIndex {
    PORT_INPUT1  = 0,
    PORT_INPUT2  = 1,
    PORT_INPUT3  = 2,
    PORT_INPUT4  = 3,
    PORT_OUTPUT  = 4,
    PORT_TOGGLE1 = 5,
    PORT_TOGGLE2 = 6,
    PORT_TOGGLE3 = 7,
    PORT_TOGGLE4 = 8,
    PORT_MIXLAW  = 9
};

typedef struct  four2one {
    const float* input1;
    const float* input2;
    const float* input3;
    const float* input4;
    float*       output;
    const float* toggle1;
    const float* toggle2;
    const float* toggle3;
    const float* toggle4;
    const float* mixLaw;
} Four2One;

static LV2_Handle instantiate(const LV2_Descriptor*     descriptor,
                              double                    rate,
                              const char*               bundle_path,
                              const LV2_Feature* const* features)
{
    Four2One* self = (Four2One*)std::calloc(1, sizeof(Four2One));
    return (LV2_Handle)self;
}

static void connect_port(LV2_Handle instance,
                         uint32_t   port,
                         void*      data)
{
    Four2One* self = (Four2One*)instance;

    switch ((portIndex)port) {
        case PORT_INPUT1:
            self->input1 = (const float*)data;
            break;
        case PORT_INPUT2:
            self->input2 = (const float*)data;
            break;
        case PORT_INPUT3:
            self->input3 = (const float*)data;
            break;
        case PORT_INPUT4:
            self->input4 = (const float*)data;
            break;
        case PORT_OUTPUT:
            self->output = (float*)data;
            break;
        case PORT_TOGGLE1:
            self->toggle1 = (const float*)data;
            break;
        case PORT_TOGGLE2:
            self->toggle2 = (const float*)data;
            break;
        case PORT_TOGGLE3:
            self->toggle3 = (const float*)data;
            break;
        case PORT_TOGGLE4:
            self->toggle4 = (const float*)data;
            break;
        case PORT_MIXLAW:
            self->mixLaw = (const float*)data;
            break;
    }
}

static void run(LV2_Handle instance, uint32_t n_samples)
{
    Four2One* self = (Four2One*)instance;
    if (!self) return;

    if (!self->output) return;

    if (!self->toggle1 || !self->toggle2 || !self->toggle3 || !self->toggle4 || !self->mixLaw) 
    {
        // If any of the control ports are not connected, output silence
        for (uint32_t i = 0; i < n_samples; i++) {
            *(self->output + i) = 0.0f;
        }
        return;
    }

    const float mix_law = *(self->mixLaw);
    const bool toggle1  = (*(self->toggle1) > 0.5f);
    const bool toggle2  = (*(self->toggle2) > 0.5f);
    const bool toggle3  = (*(self->toggle3) > 0.5f);
    const bool toggle4  = (*(self->toggle4) > 0.5f);
    
    if (!self->input1 || !self->input2 || !self->input3 || !self->input4 || !self->output) 
    {
        // If any of the audio ports are not connected, output silence
        for (uint32_t i = 0; i < n_samples; i++) {
            *(self->output + i) = 0.0f;
        }
        return;
    }

    const float* in1 = self->input1;
    const float* in2 = self->input2;
    const float* in3 = self->input3;
    const float* in4 = self->input4;
    float*       out = self->output;

    for (uint32_t i = 0; i < n_samples; i++) 
    {
        float out_sample = 0.0f;
        float total_weight = 0.0f;

        if (toggle1) {
            float weight = (mix_law < 1e-6f) ? 1.0f : std::pow(std::abs(in1[i]), mix_law);
            out_sample += in1[i] * weight;
            total_weight += weight;
        }
        if (toggle2) {
            float weight = (mix_law < 1e-6f) ? 1.0f : std::pow(std::abs(in2[i]), mix_law);
            out_sample += in2[i] * weight;
            total_weight += weight;
        }
        if (toggle3) {
            float weight = (mix_law < 1e-6f) ? 1.0f : std::pow(std::abs(in3[i]), mix_law);
            out_sample += in3[i] * weight;
            total_weight += weight;
        }
        if (toggle4) {
            float weight = (mix_law < 1e-6f) ? 1.0f : std::pow(std::abs(in4[i]), mix_law);
            out_sample += in4[i] * weight;
            total_weight += weight;
        }

        if (total_weight > 0.0f) {
            out_sample /= total_weight;
        }

        out[i] = out_sample;
    }
}

static void deactivate(LV2_Handle instance) {}

static void cleanup(LV2_Handle instance)
{
    Four2One* self = (Four2One*)instance;
    std::free(self);
}

static const LV2_Descriptor descriptor = {
    PLUGIN_URI,
    instantiate,
    connect_port,
    nullptr,
    run,
    deactivate,
    cleanup,
    nullptr
};

LV2_SYMBOL_EXPORT const LV2_Descriptor* lv2_descriptor(uint32_t index)
{
    switch (index) {
        case 0:
            return &descriptor;
        default:
            return nullptr;
    }
}
