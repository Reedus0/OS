#pragma once

#include <stdint.h>
#include <stddef.h>

#include "include/types.h"
#include "include/fdata.h"

struct file {
    fdata_t* fdata;
    char* name;
};
typedef struct file file_t;