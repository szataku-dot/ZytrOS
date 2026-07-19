#pragma once
#include <stdint.h>

void pc_speaker_on(uint32_t frequency);
void pc_speaker_off();
void beep(uint32_t freq, uint32_t duration);