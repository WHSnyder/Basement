#pragma once

#include "GL/glew.h"


int initInterop(void *context);
void * createSharedRenderTarget();
void * createSharedComputeTarget();