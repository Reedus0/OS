#include "bpb.h"
#include "fs.h"
#include "include/types.h"

struct fat_bpb* get_bpb(dev_t* dev) {
    struct fat_bpb* bpb = kalloc(512);

    bdev_read(dev, bpb, 0, 1);

    return bpb;
}