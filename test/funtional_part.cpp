#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>

#include<sys/stat.h>
#include<fcntl.h>
#include <iostream>
#include <string>
#include <string.h>
#include <chrono>
#include <vector>
#include <ext2fs/ext2_fs.h>
#include <sys/ioctl.h>
#include <bitset>
#include <cmath>
#include <linux/fs.h>
#include "funtional_part.h"



#define BLOCK_SIZE 4096
#define INODE_SIZE 128
#define BASE_OFFSET 1024
#define INODE_TABLE 5

std::string date_transform(int seconds){
    time_t now = seconds;

    // convert now to string form
    char* dt = ctime(&now);
    return std::string(dt);
}
int convert_binary_decimal(std::string binary){
    char* endPtr;
    int out;
    out = std::stoi(binary.c_str(), 0, 2);
    return out;
}

void update_block_bitmap(int fd, int number, int create_delete){
    auto *bitmap = (unsigned char*)malloc(BLOCK_SIZE);
    lseek(fd,3 * BLOCK_SIZE, SEEK_SET);
    int r = read(fd, bitmap, BLOCK_SIZE);
    std::cout << "number read = " << r;
    std::string binary;
    for (size_t i = 0; i < 10; i++){
        if (i * 8 > number){
            int bit = number - i*8;
            binary = std::bitset<8>(bitmap[i]).to_string();
            if (create_delete == 1){
                binary[bit - 1] = '1';
            }
            else{
                binary[bit - 1] = '0';
            }
            break;
        }
    }
    int to_change = convert_binary_decimal(binary);

    lseek(fd, 3 * 4096 + ceil(convert_binary_decimal(binary) / 8), SEEK_SET);
    write(fd, &to_change, 1);
    free(bitmap);
};

void update_inode_bitmap(int fd, int number, int create_delete){
    auto *bitmap = (unsigned char*)malloc(BLOCK_SIZE);
    lseek(fd,4 * BLOCK_SIZE, SEEK_SET);
    int r = read(fd, bitmap, BLOCK_SIZE);
    std::cout << "number read = " << r;
    std::string binary;
    for (size_t i = 0; i < 10; i++){
        if (i * 8 > number){
            int bit = number - i*8;
            binary = std::bitset<8>(bitmap[i]).to_string();
            if (create_delete == 1){
                binary[bit - 1] = '1';
            }
            else{
                binary[bit - 1] = '0';
            }
            break;
        }
    }
    int to_change = convert_binary_decimal(binary);
    lseek(fd, 4 * 4096 + ceil(convert_binary_decimal(binary) / 8), SEEK_SET);
    write(fd, &to_change, 1);
    free(bitmap);
}

void sync_inode(int fd, int inode_number, ext2_inode * inode){
    lseek(fd, (INODE_TABLE*BLOCK_SIZE) + ((inode_number - 1) * INODE_SIZE), SEEK_SET);
    write(fd, &inode->i_mode,2);
}

void change_mode(int fd,int inode_number, int mode){
    struct ext2_inode inode;
    lseek(fd, (INODE_TABLE*BLOCK_SIZE) + ((inode_number - 1) * INODE_SIZE), SEEK_SET);
    read(fd,&inode,128);
    inode.i_mode |= mode;
    write(fd, &mode, 2);
    sync_inode(fd,inode_number, &inode);
}

void change_uid(int fd,int inode_number, int uid){
    lseek(fd,2 + (INODE_TABLE*BLOCK_SIZE) + ((inode_number - 1) * INODE_SIZE), SEEK_SET);
    write(fd, &uid, 2);
}
void change_size(int fd,int inode_number, int size){
    lseek(fd,4 + (INODE_TABLE*BLOCK_SIZE) + ((inode_number - 1) * INODE_SIZE), SEEK_SET);
    write(fd, &size, 2);
}

void change_atime(int fd,int inode_number, int seconds){
    if (seconds == 0) {
        time_t seconds_past_epoch = time(0);
        seconds = seconds_past_epoch;
    }
    struct ext2_inode inode;
    lseek(fd,8 + (INODE_TABLE*BLOCK_SIZE) + ((inode_number - 1) * INODE_SIZE), SEEK_SET);
    write(fd, &seconds, 4);
}

void change_ctime(int fd,int inode_number, int seconds){

    struct ext2_inode inode;
    lseek(fd,12 + (INODE_TABLE*BLOCK_SIZE) + ((inode_number - 1) * INODE_SIZE), SEEK_SET);
    write(fd, &seconds, 4);
}
void change_mtime(int fd,int inode_number, int seconds){
    if (seconds == 0) {
        time_t seconds_past_epoch = time(0);
        seconds = seconds_past_epoch;
    }
    struct ext2_inode inode;
    lseek(fd,16 + (INODE_TABLE*BLOCK_SIZE) + ((inode_number - 1) * INODE_SIZE), SEEK_SET);
    write(fd, &seconds, 4);
}

void change_dtime(int fd,int inode_number, int seconds){
    struct ext2_inode inode;
    lseek(fd,20 + (INODE_TABLE*BLOCK_SIZE) + ((inode_number - 1) * INODE_SIZE), SEEK_SET);
    write(fd, &seconds, 4);
}

void change_gid(int fd,int inode_number, int gid){
    struct ext2_inode inode;
    lseek(fd,24 + (INODE_TABLE*BLOCK_SIZE) + ((inode_number - 1) * INODE_SIZE), SEEK_SET);
    write(fd, &gid, 2);
}

void change_links(int fd,int inode_number, int number){
    struct ext2_inode inode;
    lseek(fd,26 + (INODE_TABLE*BLOCK_SIZE) + ((inode_number - 1) * INODE_SIZE), SEEK_SET);
    write(fd, &number, 2);
}

void change_flags(int fd,int inode_number, int flag){
    struct ext2_inode inode;
    lseek(fd,32 + (INODE_TABLE*BLOCK_SIZE) + ((inode_number - 1) * INODE_SIZE), SEEK_SET);
    write(fd, &flag, 4);
}


void delete_block(int fd,int inode_number){
    struct ext2_inode inode;
    lseek(fd,(INODE_TABLE*BLOCK_SIZE) + ((inode_number - 1) * INODE_SIZE), SEEK_SET);
    read(fd,&inode, 128);
    int last_non_zero_block = 0;
    for (size_t i = 0; i < 15; i++){
        if (inode.i_block[i] == 0){
            last_non_zero_block = i - 1;
            break;
        }
    }
    if (last_non_zero_block >= 1){
        lseek(fd,40+last_non_zero_block*4 +(INODE_TABLE*BLOCK_SIZE) + ((inode_number - 1) * INODE_SIZE), SEEK_SET);
        int value = 0;
        write(fd,&value,4);
    }
}

void append_block(int fd,int inode_number, int block_number){
    struct ext2_inode inode;
    lseek(fd,(INODE_TABLE*BLOCK_SIZE) + ((inode_number - 1) * INODE_SIZE), SEEK_SET);
    read(fd,&inode, 128);
    int first_zero_block = 0;
    for (size_t i = 0; i < 15; i++){
        if (inode.i_block[i] == 0){
            first_zero_block = i;
            break;
        }
    }
    lseek(fd,40+first_zero_block*4 +(INODE_TABLE*BLOCK_SIZE) + ((inode_number - 1) * INODE_SIZE), SEEK_SET);
    int value = block_number;
    write(fd,&value,4);
}

std::string file_type(ext2_inode *inode){
    if (S_ISREG(inode->i_mode)){
        return "REGULAR FILE";
    }
    else if (S_ISDIR(inode->i_mode)){
        return "DIRECTORY FILE";
    }
    else if (S_ISCHR(inode->i_mode)){
        return "CHARACTER DEVICE";
    }
    else if (S_ISBLK(inode->i_mode)){
        return "BLOCK DEVICE";
    }
    else if (S_ISFIFO(inode->i_mode)){
        return "FIFO FILE";
    }
    else if (S_ISSOCK(inode->i_mode)){
        return "SOCKET FILE";
    }
    else if (S_ISLNK(inode->i_mode)){
        return "SYMBOLIC LINK";
    }
    else{
        return "UNKNOWN";
    }
}

void access_rights(ext2_inode *inode){
    std::vector<std::string> permissions;
    bool a = inode->i_mode & 0x102342341;
    std::cout << "\nTHIS IS A MODE "<< S_IRUSR;
    if (inode->i_mode & S_IRUSR){
        permissions.emplace_back("user read");
    }
    if (inode->i_mode & S_IWUSR){
        permissions.emplace_back("user write");
    }
    if (inode->i_mode & S_IXUSR){
        permissions.emplace_back("user execute");
    }
    if (inode->i_mode & S_IRGRP){
        permissions.emplace_back("group read");
    }
    if (inode->i_mode & S_IWGRP){
        permissions.emplace_back("group write");
    }
    if (inode->i_mode & S_IXGRP){
        permissions.emplace_back("group execute");
    }
    if (inode->i_mode & S_IROTH){
        permissions.emplace_back("others read");
    }
    if (inode->i_mode & S_IWOTH){
        permissions.emplace_back("others write");
    }
    if (inode->i_mode & S_IXOTH){
        permissions.emplace_back("others execute");
    }
    std::cout << "\nPERMISSIONS:";
    for (size_t i = 0; i < permissions.size(); i++){
        std::cout << "\n" <<permissions[i];
    }
}
void get_flags(ext2_inode *inode){
    std::vector<std::string> flags;
    inode->i_flags |= EXT2_SYNC_FL;
//    tried to set flag here, and it shows, but there are no flags by default
//    inode->i_flags |= EXT2_SYNC_FL;
//    inode->i_flags &= ~EXT2_SYNC_FL;
    if (inode->i_flags & EXT2_SYNC_FL){
        flags.emplace_back("EXT2_SYNC_FL");
        std::cout << "FLAG HERE";
    }
    if (inode->i_flags & EXT2_IMMUTABLE_FL){
        flags.emplace_back("EXT2_IMMUTABLE_FL");
    }
    if (inode->i_flags & EXT2_APPEND_FL){
        flags.emplace_back("EXT2_APPEND_FL");
    }
    std::cout << "\nFLAGS:";
    if (!flags.empty()){
        for (size_t i = 0; i < flags.size(); i++){
            std::cout << "\n" <<flags[i];
        }
    }
    else{
        std::cout << "\n" << "none";
    }
}

int find_root(int fd){
    struct ext2_inode inode;
    int main_directory_inode = 0;
    int root_number = 2;
    lseek(fd, (INODE_TABLE*BLOCK_SIZE) + ((root_number - 1) * INODE_SIZE), SEEK_SET);
    int r = read(fd, &inode, sizeof(struct ext2_inode));
    if (r < 0){
        std::cout << "CANT READ";
        exit(1);
    }
    if (S_ISDIR(inode.i_mode)) {
        ext2_inode current_inode;
        std::cout << "THIS IS ROOT";
        int s = 0;
        auto * block = (unsigned char*)malloc(4096);
        struct ext2_dir_entry_2 *entry;
        lseek(fd, inode.i_block[0]*4096, SEEK_SET);
        read(fd, block, 4096);
        entry = (ext2_dir_entry_2 *)block;
        while ((s < inode.i_size) && entry->inode) {
            char filename[EXT2_NAME_LEN + 1];
            if (std::string(entry->name) == "1.txt"){
                main_directory_inode = entry->inode;
                lseek(fd, (INODE_TABLE*BLOCK_SIZE) + ((entry->inode - 1) * INODE_SIZE), SEEK_SET);
                r = read(fd, &current_inode, sizeof(struct ext2_inode));
                if (r < 0){
                    std::cout << "CANT READ";
                    exit(1);
                }
            }
            memcpy(filename, entry->name, entry->name_len);
            filename[entry->name_len] = 0;
            entry  = (ext2_dir_entry_2 *)((uint64_t)entry + entry->rec_len);
            s += entry->rec_len;
        }
    }
    return main_directory_inode;
}

std::vector<std::string> print_super(int fd){
    struct ext2_super_block super;

    std::vector<std::string> output_info_superblock;

    lseek(fd, 1024, SEEK_SET);
    int a = read(fd, &super, sizeof(struct ext2_super_block));
    if (super.s_magic != EXT2_SUPER_MAGIC) {
        fprintf(stderr, "Not a Ext2 filesystem\n");
        exit(1);
    }
    if (a < 0){
        std::cout << "CANT READ";
        exit(1);
    }

    output_info_superblock.emplace_back(std::to_string(super.s_inodes_count));
    output_info_superblock.emplace_back(std::to_string(super.s_blocks_count));
    output_info_superblock.emplace_back(std::to_string(super.s_r_blocks_count));
    output_info_superblock.emplace_back(std::to_string(super.s_free_blocks_count));
    output_info_superblock.emplace_back(std::to_string(super.s_free_inodes_count));
    output_info_superblock.emplace_back(std::to_string(super.s_first_data_block));
    output_info_superblock.emplace_back(std::to_string(super.s_log_block_size));
    output_info_superblock.emplace_back(std::to_string(super.s_blocks_per_group));
    output_info_superblock.emplace_back(std::to_string(super.s_mtime));
    output_info_superblock.emplace_back(std::to_string(super.s_wtime));
    output_info_superblock.emplace_back(std::to_string(super.s_mnt_count));
    output_info_superblock.emplace_back(std::to_string(super.s_max_mnt_count));
    output_info_superblock.emplace_back(std::to_string(super.s_magic));
    output_info_superblock.emplace_back(std::to_string(super.s_state));
    output_info_superblock.emplace_back(std::to_string(super.s_errors));
    output_info_superblock.emplace_back(std::to_string(super.s_minor_rev_level));
    output_info_superblock.emplace_back(std::to_string(super.s_lastcheck));
    output_info_superblock.emplace_back(std::to_string(super.s_checkinterval));
    output_info_superblock.emplace_back(std::to_string(super.s_creator_os));
    output_info_superblock.emplace_back(std::to_string(super.s_rev_level));
    output_info_superblock.emplace_back(std::to_string(super.s_def_resuid));
    output_info_superblock.emplace_back(std::to_string(super.s_def_resgid));
    output_info_superblock.emplace_back(std::to_string(super.s_first_ino));
    output_info_superblock.emplace_back(std::to_string(super.s_inode_size));
    output_info_superblock.emplace_back(std::to_string(super.s_block_group_nr));
    output_info_superblock.emplace_back(std::to_string(super.s_feature_compat));
    output_info_superblock.emplace_back(std::to_string(super.s_feature_incompat));
    output_info_superblock.emplace_back(std::to_string(super.s_feature_ro_compat));
    output_info_superblock.emplace_back(std::to_string(super.s_prealloc_dir_blocks));
    output_info_superblock.emplace_back(std::to_string(super.s_journal_inum));
    output_info_superblock.emplace_back(std::to_string(super.s_journal_dev));
    output_info_superblock.emplace_back(std::to_string(super.s_last_orphan));
    output_info_superblock.emplace_back(std::to_string(super.s_def_hash_version));
    output_info_superblock.emplace_back(std::to_string(super.s_first_meta_bg));


    return output_info_superblock;
}

std::vector<std::string> print_group(int fd){
    struct ext2_group_desc group;

    std::vector<std::string> group_info;


    lseek(fd, BLOCK_SIZE, SEEK_SET);  /* position head above the group descriptor block */
    read(fd, &group, sizeof(group));

   group_info.emplace_back(std::to_string(group.bg_block_bitmap));
   group_info.emplace_back(std::to_string(group.bg_inode_bitmap));
   group_info.emplace_back(std::to_string(group.bg_inode_table));
   group_info.emplace_back(std::to_string(group.bg_free_blocks_count));
   group_info.emplace_back(std::to_string(group.bg_free_inodes_count));
   group_info.emplace_back(std::to_string(group.bg_used_dirs_count));

   return group_info;

}


std::vector<std::string> print_inode(int fd, int inode_number){
    struct ext2_inode inode;
    lseek(fd, (INODE_TABLE * BLOCK_SIZE) + ((inode_number - 1) * sizeof(struct ext2_inode)), SEEK_SET);
    read(fd, &inode, sizeof(struct ext2_inode));
    std::vector<std::string> inode_info;

    inode_info.emplace_back(std::to_string(inode.i_mode));
    inode_info.emplace_back(std::to_string(inode.i_uid));
    inode_info.emplace_back(std::to_string(inode.i_size));
    inode_info.emplace_back(std::to_string(inode.i_atime));

    inode_info.emplace_back(std::to_string(inode.i_ctime));
    inode_info.emplace_back(std::to_string(inode.i_mtime));
    inode_info.emplace_back(std::to_string(inode.i_dtime));

    inode_info.emplace_back(std::to_string(inode.i_gid));
    inode_info.emplace_back(std::to_string(inode.i_links_count));
    inode_info.emplace_back(std::to_string(inode.i_blocks));


    inode_info.emplace_back(std::to_string(inode.i_flags));

    inode_info.emplace_back(std::to_string(inode.i_generation));

    inode_info.emplace_back(std::to_string(inode.i_file_acl));

    inode_info.emplace_back(std::to_string(inode.i_faddr));

    inode_info.emplace_back(file_type(&inode));

    return inode_info;
}

void print_main_directory(int fd, int inode_number){
    inode_number = 2;
    struct ext2_inode inode;
    lseek(fd, (INODE_TABLE * BLOCK_SIZE) + ((inode_number - 1) * INODE_SIZE), SEEK_SET);
    int r = read(fd, &inode, sizeof(struct ext2_inode));
    if (r < 0){
        std::cout << "CANT READ";
        exit(1);
    }
    if (S_ISDIR(inode.i_mode)) {
        struct ext2_inode current_inode;
        int s = 0;
        auto* block = (unsigned char*)malloc(4096);
        struct ext2_dir_entry_2 *entry;
        lseek(fd, inode.i_block[0]*4096, SEEK_SET);
        read(fd, block, 4096);
        entry = (ext2_dir_entry_2 *)block;
        while ((s < inode.i_size) && entry->inode) {
            char filename[EXT2_NAME_LEN + 1];
            printf("\nname of an entry: %s", entry->name);
            int current_node_number = entry->inode;
            lseek(fd, (INODE_TABLE*BLOCK_SIZE) + ((current_node_number - 1) * INODE_SIZE), SEEK_SET);
            r = read(fd, &current_inode, sizeof(struct ext2_inode));
            if (r < 0){
                std::cout << "CANT READ";
                exit(1);
            }
            std::cout << "\n" << "inode number: " << entry->inode;
            printf("\ninode size: %d", current_inode.i_size);
            if (S_ISDIR(current_inode.i_mode)){
                printf("\ndirectory?: yes");
            }
            else {
                printf("\ndirectory?: no");
            }
            std::string time_c = date_transform(current_inode.i_ctime);
            std::string time_m =  date_transform(current_inode.i_mtime);
            std::cout << "\n" << "time of creation: " << time_c;
            std::cout << "time of modification: " << time_m;

            std::string type = file_type(&current_inode);
            std::cout << "\n" << "file type: " << type;
            access_rights(&current_inode);
            std::cout << "\n" << "owner's ID: " << current_inode.i_uid;
            std::cout << "\n" << "number of hard links: " << current_inode.i_links_count;
            get_flags(&current_inode);
            std::cout << "\n" << "first block number: " << current_inode.i_block[0];

            memcpy(filename, entry->name, entry->name_len);
            filename[entry->name_len] = 0;
            std::cout << "\n";
            entry  = (ext2_dir_entry_2 *)((uint64_t)entry + entry->rec_len);
            s += entry->rec_len;
        }
        free(block);
    }
}


int count_non_zero_blocks(ext2_inode * inode){
    int is_zero = 1;
    ssize_t i = 0;
    while (is_zero == 1){
        if (inode->i_block[i] == 0){
            is_zero = 0;
            break;
        }
        i++;
    }
    return i;
}

int find_file_inode(int fd, std::string name){
    int root_number = 2;
    int number_of_nonzero = 0;
    std::vector<ext2_inode> dirs_q;
    std::vector<std::string> dirs_q_names;
    struct ext2_inode current_inode;
    lseek(fd, (INODE_TABLE * BLOCK_SIZE) + ((root_number - 1) * INODE_SIZE), SEEK_SET);
    int r = read(fd, &current_inode, sizeof(struct ext2_inode));
    if (r < 0){
        std::cout << "CANT READ";
        exit(1);
    }
    int found = 0;
    while (found == 0){
        if (S_ISDIR(current_inode.i_mode)){
            int s = 0;

            struct ext2_dir_entry_2 *entry;
            number_of_nonzero = count_non_zero_blocks(&current_inode);
            for (ssize_t i = 0; i < number_of_nonzero; i++){
                lseek(fd, current_inode.i_block[i]*BLOCK_SIZE, SEEK_SET);
                auto* block = (unsigned char*)malloc(BLOCK_SIZE);
                read(fd, block, BLOCK_SIZE);
                entry = (ext2_dir_entry_2 *)block;
                int counter = 0;
                while ((s < current_inode.i_size) && entry->inode) {
                    std::cout << "\nentry name = " << entry->name;
                    counter++;
                    struct ext2_inode indir_inode;
                    char filename[EXT2_NAME_LEN + 1];
                    std::string temp = std::string(entry->name);
                    if (temp == name){
                        std::cout << "\nentry inode = "<< entry->inode;
                        std::cout << "\nFOUND\n";
                        free(block);
                        found = 1;
                        return entry->inode;
                    }
                    int current_node_number = entry->inode;
                    lseek(fd, (INODE_TABLE*BLOCK_SIZE) + ((current_node_number - 1) * INODE_SIZE), SEEK_SET);
                    r = read(fd, &indir_inode, sizeof(struct ext2_inode));
                    if (r < 0){
                        std::cout << "CANT READ";
                        exit(1);
                    }
                    if (S_ISDIR(indir_inode.i_mode)){
                        if (std::string(entry->name) !="." and std::string(entry->name) !=".."){
                            dirs_q.push_back(indir_inode);
                            dirs_q_names.push_back(std::string(entry->name));
                        }
                    }
                    memcpy(filename, entry->name, entry->name_len);
                    filename[entry->name_len] = 0;
                    entry  = (ext2_dir_entry_2 *)((uint64_t)entry + entry->rec_len);
                    s += entry->rec_len;
                }
            }
            current_inode = dirs_q.front();
            dirs_q.erase(dirs_q.begin());
            dirs_q_names.erase(dirs_q_names.begin());
        }
    }
}

int size_of_block_text(unsigned char * block){
    for (size_t i = 0; i < 4096; i++){
        if (block[i] == '\0'){
            return i;
        }
    }
    return 0;
}

int copy_inode_content_to_another_inode(int fd, std::string name_s, std::string name_des){
    int s = find_file_inode(fd, name_s);
    int d = find_file_inode(fd, name_des);

    std::cout << "\n" << s;
    std::cout << "\n" << d;

    struct ext2_inode source;
    struct ext2_inode destination;
    lseek(fd, (5*4096) + ((s - 1) * 128), SEEK_SET);
    read(fd, &source, 128);
    lseek(fd, (INODE_TABLE*BLOCK_SIZE) + ((d - 1) * 128), SEEK_SET);
    read(fd, &destination, 128);

    int number_of_nonzero_blocks_source = count_non_zero_blocks(&source);
    int number_of_nonzero_blocks_des = count_non_zero_blocks(&destination);
    auto* block = (unsigned char*)malloc(BLOCK_SIZE);
    for (ssize_t i = 0; i < number_of_nonzero_blocks_source; i++){
        lseek(fd, source.i_block[0] * 4096, SEEK_SET);
        read(fd, block, 4096);
        std::cout << block;

        std::string temp = "";
        int ss = size_of_block_text(block);
        for (size_t i = 0; i < ss; i++){
            temp += block[i];
        }
        lseek(fd,4 + (INODE_TABLE*BLOCK_SIZE) + ((d - 1) * INODE_SIZE), SEEK_SET);
        int new_size = temp.size();
        write(fd, &new_size, 4);
        lseek(fd, destination.i_block[0] * 4096, SEEK_SET);
        write(fd, temp.c_str(), temp.size());
////        update size of the destination file here
        break;
    }

    free(block);
    change_mtime(fd,d,0);
    return 0;
}

std::string toBinary(int n)
{
    std::string r;
    while(n!=0) {r=(n%2==0 ?"0":"1")+r; n/=2;}
    return r;
}

void find_free_bits(std::string binary,int count, int number_to_find, std::vector<int> *free_blocks){
    int counter = number_to_find;
    for (ssize_t i = 0; i < binary.size(); i++){
        if (counter == 0){
            break;
        }
        if (binary[i] == '0'){
            free_blocks->emplace_back(count * 8 + i);
            counter = counter - 1;
        }
    }
}


std::vector<int> find_n_free_blocks(int fd, int n){
    int blocks_bitmap = 3;
    unsigned char *bitmap;
    bitmap = (unsigned char*)malloc(BLOCK_SIZE);
    lseek(fd, 3 * 4096, SEEK_SET);
    read(fd, bitmap, BLOCK_SIZE);
    int maximum = 255;
    std::vector<int> free_blocks;
    for (ssize_t i = 0; i < 4096; i++){
        if (bitmap[i] < 255) {
            std::string binary = std::bitset<8>(bitmap[i]).to_string();
            find_free_bits(binary,i, n, &free_blocks);
            n -= free_blocks.size();
        }
        if (n <= 0){
            break;
        }
    }
    return free_blocks;
}

std::vector<int> find_free_inode(int fd){
    int number = 10;
    int inode_b_index = 4;
    auto *bitmap = (unsigned char*)malloc(BLOCK_SIZE);
    lseek(fd,4 * BLOCK_SIZE, SEEK_SET);
    read(fd, bitmap, BLOCK_SIZE);
    std::vector<int> free_inode;
    for (ssize_t i = 0; i < 4096; i++){
        if (bitmap[i] < 255) {
            std::string binary = std::bitset<8>(bitmap[i]).to_string();
            find_free_bits(binary,i, 10, &free_inode);
            number -= free_inode.size();
        }
        if (number <= 0){
            break;
        }
        }
    if (!free_inode.empty()){
        free(bitmap);
        return free_inode;
    }
    free(bitmap);
}

void fill_block(int fd,int number,std::string content){
    lseek(fd, number * BLOCK_SIZE, SEEK_SET);
    write(fd,content.c_str(), content.size());
}

int get_fd(){
    int fd;
//    fd = open("../ext2_fs_no_MBR.img", O_RDWR);    /* open floppy device */
    fd = open("../data_folder/ext2_custom.iso", O_RDWR);
    if (fd < 0){
        std::cout << "image cant open i";
        exit(0);
      }
    return fd;
}
std::string print_inode_content(int fd, int inode_number){
    struct ext2_inode current_inode;
    lseek(fd, (INODE_TABLE * BLOCK_SIZE) + ((inode_number - 1) * INODE_SIZE), SEEK_SET);
    int r = read(fd, &current_inode, sizeof(struct ext2_inode));
    if (r < 0){
        exit(0);
    }
    std::string file_content;
    size_t number_of_nonzero_blocks = count_non_zero_blocks(&current_inode);
    auto* content =(unsigned char*)malloc(BLOCK_SIZE * number_of_nonzero_blocks);
    auto* block = (unsigned char*)malloc(BLOCK_SIZE);
    int offset = 0;
    std::string result;
    int counter = 0;
    if (S_ISREG(current_inode.i_mode)){
        for (size_t i = 0; i < number_of_nonzero_blocks; ++i){
            lseek(fd, current_inode.i_block[0] * BLOCK_SIZE, SEEK_SET);
            read(fd, block, BLOCK_SIZE);
            strcpy(((char*)content+offset),(char*)block);
        }
        result = (char*)content;
    }
    else if (S_ISDIR(current_inode.i_mode)){
        std::string temp;
        result += "Directory contents :\n";
        unsigned int s = 0;
        struct ext2_dir_entry_2 *entry;
        lseek(fd, current_inode.i_block[0]*4096, SEEK_SET);
        read(fd, block, 4096);
        entry = (ext2_dir_entry_2 *)block;
        while ((s < current_inode.i_size) && entry->inode) {
            if (counter > 1000){
                break;
            }
            counter++;
            char filename[EXT2_NAME_LEN + 1];
            memcpy(filename, entry->name, entry->name_len);
            filename[entry->name_len] = 0;
            temp  = filename;
            result += temp + '\n';
            entry  = (ext2_dir_entry_2 *)((uint64_t)entry + entry->rec_len);
            s += entry->rec_len;
        }
    }
    std::cout << "FINISHED";
    free(block);
//    free(content);
    return result;
}

std::string block_bitmap(int fd){
    auto *bitmap = (unsigned char*)malloc(BLOCK_SIZE);
    lseek(fd,1024 + (3 * BLOCK_SIZE), SEEK_SET);
    int r = read(fd, bitmap, sizeof(struct ext2_inode));
    std::string result;
    for (int i =0; i<=510;i++) {
        result += std::to_string(bitmap[i]) + " ";
    }
    return result;
}


std::string inode_bitmap(int fd){
    auto *bitmap = (unsigned char*)malloc(BLOCK_SIZE);
    lseek(fd,1024 + (4 * BLOCK_SIZE), SEEK_SET);
    int r = read(fd, bitmap, sizeof(struct ext2_inode));
    std::string result;
    for (int i =0; i<=510;i++) {
        result += std::to_string(bitmap[i]) + " ";
    }
    return result;
}

std::string transform(std::string seq){
    std::string output;
    std::string temp_num;
    for (const auto & digit : seq) {
        if (digit == ' '){

            output += std::bitset<8>(std::stoi(temp_num)).to_string() + " ";
            temp_num.clear();
        }
        temp_num += digit;
    }
    return output;
}

void test(int fd){
    struct ext2_inode current_inode;
    for (int j = 0; j < 10; j++){
    for (int i = 13; i < 20; i++){
            lseek(fd, (INODE_TABLE * BLOCK_SIZE) + ((i - 1) * INODE_SIZE), SEEK_SET);
            int r = read(fd, &current_inode, sizeof(struct ext2_inode));
            if (r < 0){
                exit(0);
            }
            std::cout << "inode number " << i << "  " << current_inode.i_size<< "\n" ;
    }
    std::cout << "\nBREA\n";
    }
}





//int main() {
//    int fd;
//    fd = open("/dev/sda4", O_RDWR);    /* open floppy device */
//    if (fd < 0){
//        std::cout << "image cant open i";
//        exit(0);
//    }
//    ext2_inode inode;
//    lseek(fd,(5*BLOCK_SIZE) + ((20 - 1) * 128), SEEK_SET);
//    read(fd, &inode, 128);
////    change_size(fd, 20, 100);
////    change_mtime(fd,20,1);
//    change_flags(fd, 20,EXT2_SYNC_FL);
//    sync_inode(fd,20,&inode);
//    print_inode(fd, &inode);
//    get_flags(&inode);
////    print_inode(fd, &inode);
////    std::vector<int> v = find_n_free_blocks(fd,5);
////    for (size_t i = 0; i <v.size(); i++){
////        std::cout << "\n" << v[i];
////    }
//    return 0;
//}
