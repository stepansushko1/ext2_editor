
#include<stdio.h>
#include<unistd.h>
#include<stdlib.h>
#include<linux/fs.h>
#include<ext2fs/ext2_fs.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>
#include <iostream>
#include <string>
#define BASE_OFFSET 1024
#define USB_DEVICE "/dev/sda4"

int block_size = 4096;

int print_super(int fd,struct ext2_super_block * super){

    off_t scount;
    char * s;
    int rcount;
    int block_offset;


    if((scount = lseek(fd,BASE_OFFSET,SEEK_SET))!= BASE_OFFSET)
        printf("scount is %d seek failure!\n",(int)scount);

    rcount = read(fd,super,sizeof(struct ext2_super_block));
    printf("Reading super-block from device:\n"
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
           super->s_inodes_count,
           super->s_blocks_count,
           super->s_r_blocks_count,     /* reserved blocks count */
           super->s_free_blocks_count,
           super->s_free_inodes_count,
           super->s_first_data_block,
           block_size,
           super->s_blocks_per_group,
           super->s_inodes_per_group,
           super->s_creator_os,
           super->s_first_ino,          /* first non-reserved inode */
           super->s_inode_size);
}

int print_gdt(int fd, struct ext2_group_desc *gdt){
    int scount,rcount;
    scount = lseek(fd,4096,SEEK_SET);
//  printf("scount is %d seek gdt failure!\n",scount);
    rcount = read(fd,gdt,sizeof(struct ext2_group_desc));
    printf("bitmap block is %d\n",gdt->bg_block_bitmap);
}

int print_bitmap(int fd,int block_bitmap){
    int block_offset;
    int i;
    unsigned char bitmap[block_size];
    block_offset = (block_bitmap) * block_size;
    lseek(fd,block_offset,SEEK_SET);
    read(fd,bitmap,block_size);
    for (i =0; i<=320;i++) printf("%d ", bitmap[i]);printf("\n");
}


//void print_bitmap(int fd, unsigned char *block_bits, int num_bytes) {
//    int byte;
//    int bit;
//    unsigned char in_use;
//    std::cout << "HERE";
//    lseek(fd,1024 + 327 * block_size,SEEK_SET);
//    int a = read(fd,block_bits,4096);
//    std::cout << a;

//    for (byte = 0; byte < num_bytes; byte++) {
//        for (bit = 0; bit < 8; bit++) {
//            in_use = (block_bits[byte] & (1 << bit)) >> bit;
//            printf("%d", in_use);
//        }
//        printf(" ");
//    }
//    printf("\n");
//    return;
//}

int print_inode(int fd, int table, int size,struct ext2_inode *inode){

    lseek(fd,block_size*table+size,SEEK_SET);
    read(fd,inode,size);
    printf("mode is %x\n",inode->i_mode);
    printf("%5x %5x %5x\n",inode->i_atime,inode->i_mtime,inode->i_dtime);
    printf("block number is: %d\n",inode->i_blocks);
    printf("1st block is: %d\n",inode->i_block[0]);
}

int print_directory(int fd, int first,int block_size){

    int i,scount;
    struct ext2_dir_entry_2 dir;
    scount = lseek(fd,block_size*first,SEEK_SET);
    read(fd,&dir,sizeof(struct ext2_dir_entry_2));
    printf("%d\n",dir.inode);
    printf("%d\n",dir.file_type);
    printf("%d\n",dir.name_len);
    //for(i=0;i<dir.name_len;i++)
    //printf("%s",dir.charname[i]);
    printf("%s\n",dir.name);
    printf("\n");
}
std::string toBinary(int n)
{
    std::string r;
    while(n!=0) {r=(n%2==0 ?"0":"1")+r; n/=2;}
    return r;
}
int main(){
    int fd;
    struct ext2_super_block super;

    fd = open("/dev/sda4", O_RDONLY);    /* open floppy device */

    lseek(fd, 1024, SEEK_SET);                    /* position head above super-block */
    read(fd, &super, sizeof(super));              /* read super-block */

    if (super.s_magic != EXT2_SUPER_MAGIC){
        std::cout << "ERROR";
        exit(1); /* bad file system */}
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
           block_size,
           super.s_blocks_per_group,
           super.s_inodes_per_group,
           super.s_creator_os,
           super.s_first_ino,          /* first non-reserved inode */
           super.s_inode_size);

    block_size = 1024 << super.s_log_block_size;  /* calculate block size in bytes */

    unsigned int group_count = 1 + (super.s_blocks_count-1) / super.s_blocks_per_group;
    unsigned int descr_list_size = group_count * sizeof(struct ext2_group_desc);

    struct ext2_group_desc group;
    lseek(fd, 1024 + block_size, SEEK_SET);  /* position head above the group descriptor block */
    read(fd, &group, sizeof(group));

#define BASE_OFFSET 1024  /* location of the super-block in the first group */

//    #define BLOCK_OFFSET(block) (BASE_OFFSET + (block-1)*block_size)

    unsigned char *bitmap;
    bitmap = (unsigned char*)malloc(block_size);    /* allocate memory for the bitmap */
    lseek(fd, block_size * 327, SEEK_SET);
    read(fd, bitmap, block_size);   /* read bitmap from disk */
    for (int i =0; i<=320;i++) printf("%d ", bitmap[i]);printf("\n");

    unsigned int inodes_per_block = block_size / sizeof(struct ext2_inode);
    unsigned int itable_blocks = super.s_inodes_per_group / inodes_per_block;
    std::cout << itable_blocks;


    struct ext2_inode *inode;
    unsigned char * inode_bitmap = (unsigned char*)malloc(4096);
    lseek(fd,328*4096,SEEK_SET);
    read(fd,inode_bitmap,block_size);
    std::cout << "PRINT INODE BITMAP" << "\n";
    for (int i =0; i<=127;i++) {
        std::string res;
        res = toBinary((int) inode_bitmap[i]);
        std::cout << res;
//        printf("\n");
    }
    std::cout<< "\n" << super.s_inodes_count << "\n";

//    free(bitmap);
//    free(inode_bitmap);
    std::cout<< "INODE TABLE\n";
//    inode table is located in 329 * 4096 offset
    lseek(fd, 329*4096 + (84- 1) * 256, SEEK_SET);
    read(fd, inode, 256);
    if(S_ISREG(inode->i_mode) != 0){
        std::cout<< "INODE PRINT "<< " REGULAR" << "\n";
    }
    unsigned int *p = NULL;
    p = inode->i_block;
    std::cout << p;
    char *array[100];
    int i = 0;
    while (i < 8){
        std::cout<< "\n"<<*(p++);
        break;
    }






}