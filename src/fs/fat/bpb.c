#include "bpb.h"
#include "fs.h"
#include "include/types.h"

struct fat_bpb* get_bpb(dev_t* dev) {
    struct fat_bpb* bpb = kalloc(sizeof(struct fat_bpb));

    bdev_read(dev, bpb, 0, 1);

    return bpb;
}