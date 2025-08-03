#!/bin/bash

mkdir -p /tmp/fs
sudo losetup -P /dev/loop0  /home/debian/OS/drive
mkdir -p /tmp/fs
sudo mount -t vfat /dev/loop0p1 /tmp/fs

cp ./dist/x86_64/kernel.bin ./fs/kernel/
cp -r ./fs/* /tmp/fs/

sudo umount /tmp/fs
sudo losetup -d /dev/loop0
rm -rf /tmp/fs