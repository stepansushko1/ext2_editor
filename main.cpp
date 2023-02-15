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

const constexpr int INODE_SIZE = 128;
const constexpr int BASE_OFFSET = 1024;
const constexpr int INODE_TABLE = 5;

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

    lseek(fd, 3 * BLOCK_SIZE + ceil(convert_binary_decimal(binary) / 8), SEEK_SET);
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
    lseek(fd, 4 * BLOCK_SIZE + ceil(convert_binary_decimal(binary) / 8), SEEK_SET);
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
    read(fd,&inode,INODE_SIZE);
    inode.i_mode |= mode;
    write(fd, &mode, 2);
}

void change_uid(int fd,int inode_number, int uid){
    lseek(fd,2 + (INODE_TABLE*BLOCK_SIZE) + ((inode_number - 1) * INODE_SIZE), SEEK_SET);
    write(fd, &uid, 2);
}
void change_size(int fd,int inode_number, int size){
    lseek(fd,4 + (INODE_TABLE*BLOCK_SIZE) + ((inode_number - 1) * INODE_SIZE), SEEK_SET);
    int temp = 200;
    write(fd, &temp, 2);
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
    read(fd,&inode, INODE_SIZE);
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
    read(fd,&inode, INODE_SIZE);
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
        free(block);
    }
    return main_directory_inode;
}

std::vector<int> print_super(int fd){
    struct ext2_super_block super;
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
    printf("Reading super-block from device :\n"
           "Inodes count            : %u\n"
           "Blocks count            : %u\n"
           "Reserved blocks count   : %u\n"
           "Free blocks count       : %u\n"
           "Free inodes count       : %u\n"
           "First data block        : %u\n"
           "Block size              : %u\n"
           "Blocks per group        : %u\n"
           "Inodes per group        : %u\n"
           "Creator OS              : %u\n"
           "First non-reserved inode: %u\n"
           "Size of inode structure : %hu\n",
           super.s_inodes_count,
           super.s_blocks_count,
           super.s_r_blocks_count,     /* reserved blocks count */
           super.s_free_blocks_count,
           super.s_free_inodes_count,
           super.s_first_data_block,
           BLOCK_SIZE,
           super.s_blocks_per_group,
           super.s_inodes_per_group,
           super.s_creator_os,
           super.s_first_ino,          /* first non-reserved inode */
           super.s_inode_size);
}

void print_group(int fd){
    struct ext2_group_desc group;
    lseek(fd, BLOCK_SIZE, SEEK_SET);  /* position head above the group descriptor block */
    read(fd, &group, sizeof(group));
    printf("\nReading first group-descriptor from device "":\n"
                                                                   "Blocks bitmap block: %u\n"
                                                                   "Inodes bitmap block: %u\n"
                                                                   "Inodes table block : %u\n"
                                                                   "Free blocks count  : %u\n"
                                                                   "Free inodes count  : %u\n"
                                                                   "Directories count  : %u\n"
            ,
            group.bg_block_bitmap,
            group.bg_inode_bitmap,
            group.bg_inode_table,
            group.bg_free_blocks_count,
            group.bg_free_inodes_count,
            group.bg_used_dirs_count);
}

void print_inode(int fd, ext2_inode * inode){
    std::cout << "\n" << "file type = " << file_type(inode);
    std::cout << "\n" << "size = " << inode->i_size;
    std::cout << "\n" << "uid = " << inode->i_uid;
    std::cout << "\n" << "time created = " << inode->i_ctime;
    std::cout << "\n" << "time modified = " << inode->i_mtime;
    std::cout << "\n" << "gid = " << inode->i_gid;
    std::cout << "\n" << "block 0  = " << inode->i_block[0];
    auto* block = (unsigned char*)malloc(BLOCK_SIZE);
    lseek(fd, inode->i_block[0]*BLOCK_SIZE, SEEK_SET);
    read(fd, block, BLOCK_SIZE);
    std::cout << "\n" << "this is block 0 of free inode " << block << "\n";
    std::cout << "\n" << "block 1  = " << inode->i_block[1];
    free(block);
}

void print_main_directory(int fd, int inode_number){
//    inode_number = 2;
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
        lseek(fd, inode.i_block[0]*BLOCK_SIZE, SEEK_SET);
        read(fd, block, BLOCK_SIZE);
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
        }
        i++;
    }
    return i;
}

int find_file_inode1(int fd, std::string name){
    std::cout << "HEEELELELEOOOO";
    std::cout << "name = " << name;
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
            for (ssize_t i = 0; i < 2; i++){
                lseek(fd, current_inode.i_block[i]*BLOCK_SIZE, SEEK_SET);
                auto* block = (unsigned char*)malloc(BLOCK_SIZE);
                read(fd, block, BLOCK_SIZE);
                entry = (ext2_dir_entry_2 *)block;
                int counter = 0;
                while ((s < current_inode.i_size) && entry->inode) {
                    counter++;
                    std::cout << entry->name << "\n";
                    if (counter > 100){
                        return 1;
                    }
                    struct ext2_inode indir_inode;
                    char filename[EXT2_NAME_LEN + 1];
                    std::string temp = std::string(entry->name);
//                    if (strcmp(entry->name, "t")){
//                        std::cout << "\nentry inode = "<< entry->inode;
//                        std::cout << "\nFOUND\n";
//                        free(block);
//                        found = 1;
//                        return entry->inode;
//                    }
                    int current_node_number = entry->inode;
                    std::cout << "inode number = " << entry->inode;
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
                    if (strcmp(filename, "FASMW.exe")){
                        std::cout << "\nentry inode = "<< entry->inode;
                        std::cout << "\nFOUND\n";
                        free(block);
                        found = 1;
                        return entry->inode;
                    }
                }
                free(block);
            }
            current_inode = dirs_q.front();
            dirs_q.erase(dirs_q.begin());
            dirs_q_names.erase(dirs_q_names.begin());
        }
    }
    return -1;
}

int find_file_inode(int fd, std::string name){
//    this is a key function for copy, it is broken,
//an when i tried to fix it, i broke it even more
    int root_number = 12;
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
                    if (strcmp(entry->name, "eee.txt")){
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
    free(bitmap);
    return free_blocks;
}

ext2_inode find_file_dir(int fd, std::string name){
//    start looking for the file from root directory. keeping the last directory we enter
    std::cout << "name = " << name << "\n";
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
//                    if (counter == 7){
//                        return 0;
//                    }
                    counter++;
                    struct ext2_inode indir_inode;
                    char filename[EXT2_NAME_LEN + 1];
                    std::string temp = std::string(entry->name);
                    if (temp == name){
                        std::cout << "name = "<<name;
                        std::cout << "\nentry inode = "<< entry->inode;
                        std::cout << "\nFOUND\n";
                        free(block);
                        found = 1;
                        return current_inode;
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
                free(block);
            }
            current_inode = dirs_q.front();
            dirs_q.erase(dirs_q.begin());
            dirs_q_names.erase(dirs_q_names.begin());
        }
    }

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

std::string block_bitmap(int fd){
    auto *bitmap = (unsigned char*)malloc(BLOCK_SIZE);
    lseek(fd,1024 + (3 * BLOCK_SIZE), SEEK_SET);
    int r = read(fd, bitmap, sizeof(struct ext2_inode));
    std::string result;
    for (int i =0; i<=510;i++) {
        result += std::to_string(bitmap[i]) + " ";
    }
    std::cout<< result;
}


int find_padding_dir(int fd, int inode_number){
    struct ext2_inode inode;
    lseek(fd, (INODE_TABLE * BLOCK_SIZE) + ((inode_number - 1) * INODE_SIZE), SEEK_SET);
    int r = read(fd, &inode, sizeof(struct ext2_inode));
    if (r < 0){
        std::cout << "CANT READ";
        exit(1);
    }
    int s = 0;
    int temp_s = 0;
    int counter = 0;
    int ifnull;
    if (S_ISDIR(inode.i_mode)) {
        auto* block = (unsigned char*)malloc(4096);
        struct ext2_dir_entry_2 *entry;
        lseek(fd, inode.i_block[0]*BLOCK_SIZE, SEEK_SET);
        read(fd, block, BLOCK_SIZE);
        entry = (ext2_dir_entry_2 *)block;
        while ((s < inode.i_size) && entry->inode) {
            temp_s = s;

            char filename[EXT2_NAME_LEN + 1];
            int current_node_number = entry->inode;
            memcpy(filename, entry->name, entry->name_len);
            filename[entry->name_len] = 0;
//            std::cout << "filename = " << filename << "\n";
//            std::cout << "inode if null = " << entry->inode << "\n";
//            std::cout << "padding = " << s << "\n";
//            std::cout << "rec len = " << entry->rec_len << "\n";
            entry  = (ext2_dir_entry_2 *)((uint64_t)entry + entry->rec_len);
            temp_s = s;
            s += entry->rec_len;
            if (entry->rec_len > 255){
                temp_s += 8 + entry->name_len + (4 - ((8 + entry->name_len) % 4) );
                std::cout << "final s = " << temp_s;
                return temp_s;
            }

//            if (s == 4084){
//                std::cout << "THIS IS INODE = "<< entry->inode << "\n";
//                std::cout << temp_s;
//                return temp_s;
//            }
        }
        free(block);
    }
    std::cout << temp_s;
    return s;
}

void move_copy(int fd, std::string file, std::string destination, int move){
    int file_inode = find_file_inode(fd, file);
    exit(0);
    int destination_directory_inode = find_file_inode(fd, destination);

//    int destination_directory_inode = 2;
    std::cout << "DESTINATION = " <<destination_directory_inode << "\n";
    std::cout << "\n";
    int record_len = 8;
    if (move){
        //    we need to find current files directory to delete this file from the current directory if it is mv
        ext2_inode current_directory = find_file_dir(fd, file);
    }
    else{
//        modify destination directory
        struct ext2_inode current_inode;
        lseek(fd, (INODE_TABLE * BLOCK_SIZE) + ((destination_directory_inode - 1) * INODE_SIZE), SEEK_SET);
        int r = read(fd, &current_inode, sizeof(struct ext2_inode));
        if (r < 0){
            std::cout << "CANT READ";
            exit(1);
        }
        if (!S_ISDIR(current_inode.i_mode)){
            std::cout<< "NOT A DIRECTORY";
            exit(1);
        }
        int s = find_padding_dir(fd,destination_directory_inode);
        exit(0);
        //    s is a padding, where we start to write
        record_len = 4096-s;
        int allignment_padding = 0;
        std::string nullstr;
        if (record_len % 4 != 0){
            allignment_padding = 4 - record_len % 4;
        }
        for (size_t i = 0; i < allignment_padding;++i){
            nullstr += '\0';
        }
        record_len -= allignment_padding;
//        inode number
        lseek(fd, (current_inode.i_block[0] * BLOCK_SIZE + s), SEEK_SET);
        write(fd,&file_inode, 4);
        s += 4;
//        length of a record
        lseek(fd, (current_inode.i_block[0] * BLOCK_SIZE  + s), SEEK_SET);
        write(fd,&record_len,2);
//        name length
        int name_length = file.size();
        s += 2;
        lseek(fd, (current_inode.i_block[0] * BLOCK_SIZE  + s), SEEK_SET);
        write(fd, &name_length, 1);
//        file type
        s += 1;
        int file_type = 1;
        lseek(fd, (current_inode.i_block[0] * BLOCK_SIZE  + s), SEEK_SET);
        write(fd, &file_type, 1);
//        file name
        s += 1;
        lseek(fd, (current_inode.i_block[0] * BLOCK_SIZE  + s), SEEK_SET);
        write(fd, file.c_str(), name_length);
//        allignement padding if needed
        s += name_length;
        if (allignment_padding) {
            lseek(fd, (current_inode.i_block[0] * BLOCK_SIZE  + s), SEEK_SET);
            write(fd, nullstr.c_str(), allignment_padding);
        }
    }
}


int main() {
    int fd;
    fd = open("/dev/sda4", O_RDWR);    /* open floppy device */
    if (fd < 0){
        std::cout << "image cant open i";
        exit(0);
    }
    return 0;
}
