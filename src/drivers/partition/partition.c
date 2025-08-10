#include "kernel/panic.h"
#include "partition.h"

static void partition_read(dev_t* dev, byte* buffer, size_t offset, size_t count) {
    partition_data_t* partition_data = dev->dev_data;

    if (offset + count > partition_data->sector_count) panic("Reading outside of partition!");

    return bdev_read(partition_data->drive, buffer, partition_data->first_sector + offset, count);
}

static void partition_write(dev_t* dev, byte* buffer, size_t offset, size_t count) {
    partition_data_t* partition_data = dev->dev_data;
    if (offset + count > partition_data->sector_count) panic("Writing outside of partition!");

    return bdev_write(partition_data->drive, buffer, partition_data->first_sector + offset, count);
}

static void partition_get_block_size(dev_t* dev) {
    partition_data_t* partition_data = dev->dev_data;
    return bdev_get_block_size(partition_data->drive);
}

static mbr_partition_table_entry_t* read_mbr_partition_table(dev_t* dev) {
    partition_data_t* partition_data = dev->dev_data;

    char* buffer = kalloc(bdev_get_block_size(partition_data->drive));
    bdev_read(partition_data->drive, buffer, 0, 1);

    mbr_partition_table_entry_t* partition_table = kalloc(sizeof(mbr_partition_table_entry_t) * 4);
    memcpy(partition_table, buffer + 0x1BE, sizeof(mbr_partition_table_entry_t) * 4);

    kfree(buffer);

    return partition_table;
}

static void init_partition(dev_t* dev) {
    partition_data_t* partition_data = dev->dev_data;

    mbr_partition_table_entry_t* partition_table = read_mbr_partition_table(dev);
    partition_data->first_sector = partition_table->relative_sector;
    partition_data->sector_count = partition_table->total_sectors;

    kfree(partition_table);
}

static void deinit_partition(dev_t* dev) {

}

module_t* init_partition_module() {
    module_t* partition_module = kalloc(sizeof(module_t));

    partition_module->name = "Partition module";

    partition_module->init = init_partition;

    MODULE_FUNCTION(partition_module, BDEV_DRIVER_READ_BLOCK) = partition_read;
    MODULE_FUNCTION(partition_module, BDEV_DRIVER_WRITE_BLOCK) = partition_write;
    MODULE_FUNCTION(partition_module, BDEV_DRIVER_GET_BLOCK_SIZE) = partition_get_block_size;

    partition_module->deinit = deinit_partition;

    return partition_module;
}

dev_t* init_partition_dev(dev_t* drive, uint8_t index) {
    dev_t* partition_dev = kalloc(sizeof(dev_t));

    partition_data_t* partition_data = kalloc(sizeof(partition_data_t));

    if (index < 0 || index > 5) {
        panic("Wrong partition number!");
    }

    partition_data->drive = drive;
    partition_data->index = index;

    partition_dev->dev_data = partition_data;
    partition_dev->driver = init_partition_module(partition_dev);
    partition_dev->driver->init(partition_dev);

    return partition_dev;
}

void deinit_partition_dev(dev_t* partition_dev) {
    partition_dev->driver->deinit(partition_dev);

    kfree(partition_dev->dev_data);
    kfree(partition_dev);
}