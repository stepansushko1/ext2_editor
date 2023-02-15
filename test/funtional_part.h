#ifndef FUNTIONAL_PART_H
#define FUNTIONAL_PART_H

#endif // FUNTIONAL_PART_H

#include <vector>
#include <string>

int get_fd();

std::vector<std::string> print_super(int fd);

std::vector<std::string> print_inode(int fd, int inode_number);

std::vector<std::string> print_group(int fd);

std::string print_inode_content(int fd, int inode_number);

std::string transform(std::string seq);

void change_mode(int fd,int inode_number, int mode);

void change_uid(int fd,int inode_number, int uid);

void change_size(int fd,int inode_number, int size);

void change_atime(int fd,int inode_number, int seconds);

void change_ctime(int fd,int inode_number, int seconds);

void change_mtime(int fd,int inode_number, int seconds);

void change_dtime(int fd,int inode_number, int seconds);

void change_gid(int fd,int inode_number, int gid);

void change_links(int fd,int inode_number, int number);

void change_flags(int fd,int inode_number, int flag);

int copy_inode_content_to_another_inode(int fd, std::string name_s, std::string name_des);

std::string toBinary(int n);

std::string block_bitmap(int fd);

std::string transform(std::string seq);

std::string inode_bitmap(int fd);

void delete_block(int fd,int inode_number);

void append_block(int fd,int inode_number, int block_number);


void test(int fd);
