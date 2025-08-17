#!/bin/bash

sudo mkdir -p /tmp/fs
sudo losetup -P /dev/loop0  /home/debian/OS/drive
sudo mkdir -p /tmp/fs
sudo mount -t vfat /dev/loop0p1 /tmp/fs

sudo cp ./dist/x86_64/kernel.bin ./fs/kernel/
sudo cp -r ./fs/* /tmp/fs/

sudo umount /tmp/fs
sudo losetup -d /dev/loop0
sudo rm -rf /tmp/fs