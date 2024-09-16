#pragma once

#include <stdio.h>

#define NUM 103
#define Body '='
#define Head '>'

typedef void (*callback_t)(double);

//version 1
void process();
//version 2
void process_flush(double rate);
