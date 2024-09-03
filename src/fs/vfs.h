#pragma once

#include "include/fs.h"
#include "include/list.h"

vfs_dir_t g_vfs_root;

vfs_fs_t* g_fs_list;


file_t* vfs_open_file(char* path);
void vfs_close_file(file_t* file);
void vfs_seek(file_t* file, size_t position);

size_t vfs_read_file(file_t* file, byte* buffer, size_t count);
size_t vfs_write_file(file_t* file, byte* buffer, size_t count);
vfs_file_t* vfs_create_file(char* path);
void vfs_delete_file(char* path);
vfs_dir_t* vfs_create_dir(char* path);
void vfs_delete_dir(char* path);

void init_vfs();
