

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>

#include "ext2_fs.h"

#define BUFF_SIZE   1025
#define BLOCK_SIZE   1024
#define BLOCK_OFFSET(block) (BLOCK_SIZE + (block-1)*BLOCK_SIZE)

void PrintSuperBlock(struct ext2_super_block *blk)
{
    printf("\n-- superblock:\n\n");
    printf("-- %s \t %u\n", "s_inodes_count", blk->s_inodes_count);
    printf("-- %s \t %u\n", "s_blocks_count", blk->s_blocks_count);
    printf("-- %s \t %u\n", "s_r_blocks_count", blk->s_r_blocks_count);
    printf("-- %s \t %u\n", "s_free_blocks_count", blk->s_free_blocks_count);
    printf("-- %s \t %u\n", "s_free_inodes_count", blk->s_free_inodes_count);
    printf("-- %s \t %u\n", "s_first_data_block", blk->s_first_data_block);
    printf("-- %s \t %u\n", "s_log_block_size", blk->s_log_block_size);
    printf("-- %s \t %d\n", "s_log_frag_size", blk->s_log_frag_size);
    printf("-- %s \t %u\n", "s_blocks_per_group", blk->s_blocks_per_group);
    printf("-- %s \t %u\n", "s_frags_per_group", blk->s_frags_per_group);
    printf("-- %s \t %u\n", "s_inodes_per_group", blk->s_inodes_per_group);
    printf("-- %s \t\t %u\n", "s_mtime", blk->s_mtime);
    printf("-- %s \t\t %u\n", "s_wtime", blk->s_wtime);
    printf("-- %s \t\t %hu\n", "s_mnt_count", blk->s_mnt_count);
    printf("-- %s \t %hd\n", "s_max_mnt_count", blk->s_max_mnt_count);
    printf("-- %s \t\t %hx\n", "s_magic", blk->s_magic);
    printf("-- %s \t\t %hu\n", "s_state", blk->s_state);
    printf("-- %s \t\t %hu\n", "s_errors", blk->s_errors);
    printf("-- %s \t %hu\n", "s_minor_rev_level", blk->s_minor_rev_level);
    printf("-- %s \t\t %u\n", "s_lastcheck", blk->s_lastcheck);
    printf("-- %s \t %u\n", "s_checkinterval", blk->s_checkinterval);
    printf("-- %s \t %u\n", "s_creator_os", blk->s_creator_os);
    printf("-- %s \t\t %u\n", "s_rev_level", blk->s_rev_level);
    printf("-- %s \t %hu\n", "s_def_resuid", blk->s_def_resuid);
    printf("-- %s \t %hu\n", "s_def_resgid", blk->s_def_resgid);

    printf("-- %s \t %u\n", "s_first_ino", blk->s_first_ino);
    printf("-- %s \t %hu\n", "s_inode_size", blk->s_inode_size);
    printf("-- %s \t %hu\n", "s_block_group_nr", blk->s_block_group_nr);
    printf("-- %s \t %u\n", "s_feature_compat", blk->s_feature_compat);
    printf("-- %s \t %u\n", "s_feature_incompat", blk->s_feature_incompat);
    printf("-- %s \t %u\n", "s_feature_ro_compat", blk->s_feature_ro_compat);
    printf("\n\n");

}

void PrintGroupDescriptor(struct ext2_group_desc *blk)
{
    char s[] = " \t ";
    char s2[] = "\t ";
    char s3[] = "\t\t ";

    printf("\n-- group descriptor:\n\n");

    printf("-- %s%s%u\n", "bg_block_bitmap", s, blk->bg_block_bitmap);
    printf("-- %s%s%u\n", "bg_inode_bitmap", s, blk->bg_inode_bitmap);
    printf("-- %s%s%u\n", "bg_inode_table", s, blk->bg_inode_table);
    printf("-- %s%s%hu\n", "bg_free_blocks_count", s2, blk->bg_free_blocks_count);
    printf("-- %s%s%hu\n", "bg_free_inodes_count", s2, blk->bg_free_inodes_count);
    printf("-- %s%s%hu\n", "bg_used_dirs_count", s, blk->bg_used_dirs_count);
    printf("-- %s%s%u\n", "bg_pad", s3, blk->bg_pad);

    printf("\n\n");
}

static void read_inode(int fd, int inode_no, const struct ext2_group_desc *group, struct ext2_inode *inode)
{
	lseek(fd, BLOCK_OFFSET(group->bg_inode_table)+(inode_no-1)*sizeof(struct ext2_inode), SEEK_SET);
	read(fd, inode, sizeof(struct ext2_inode));
}

static void read_block(int fd, struct ext2_inode *inode, char *block)
{
    lseek(fd, BLOCK_OFFSET(inode->i_block[0]), SEEK_SET);
    read(fd, block, BLOCK_SIZE);
}

void PrintFiles(struct ext2_inode *inode, char *block)
{
    struct ext2_dir_entry_2 *entry;
    char file_name[1000] = {0};

    unsigned int size = 0;
    entry = (struct ext2_dir_entry_2 *) block;

    while(size < inode->i_size)
    {
        memcpy(file_name, entry->name, entry->name_len);

        file_name[entry->name_len] = 0;

        printf("%10u %s\n", entry->inode, file_name);
        entry = (void*)((char*)entry + entry->rec_len);
        size += entry->rec_len;
    }
}

unsigned int GetInodeNum(struct ext2_inode *inode, char *block, char *locate)
{
    struct ext2_dir_entry_2 *entry;

    unsigned int size = 0;
    entry = (struct ext2_dir_entry_2 *) block;

    while(size < inode->i_size)
    {
        char file_name[EXT2_NAME_LEN+1];

        memcpy(file_name, entry->name, entry->name_len);
        file_name[entry->name_len] = 0;

        if (!strcmp(file_name, locate))
        {
            return entry->inode;
        }

        entry = (void*)((char*)entry + entry->rec_len);
        size += entry->rec_len;
    }

    return 0;
}

int main(int ac, char **av)
{
    int fd;
    struct ext2_super_block super;
    struct ext2_group_desc group_desc;
    struct ext2_inode node;
    char buff[BUFF_SIZE] = {0};
    unsigned int next_inode = 0;
    char buff2[1000] = {0};

    if (ac != 2)
    {
        printf(" argssss\n");
        return 1;
    }

    strcpy(buff2, av[1]);

    fd = open("/dev/ram0", O_RDONLY);

    lseek(fd, BLOCK_SIZE, SEEK_SET);
    read(fd, &super, sizeof(super));

    if (super.s_magic != EXT2_SUPER_MAGIC)
    {
        printf("-- error \n");
        close(fd);
        exit(1); /* bad file system */
    }

    read(fd, &group_desc, sizeof(group_desc));
    read_inode(fd, 2, &group_desc, &node);
    read_block(fd, &node, buff);
    PrintFiles(&node, buff);

    next_inode = GetInodeNum(&node, buff, av[1]);
    read_inode(fd, next_inode, &group_desc, &node);
    read_block(fd, &node, buff);

    PrintSuperBlock(&super);
    PrintGroupDescriptor(&group_desc);

    printf("-- buff: \n%s\n", buff);

    close(fd);

    return 0;
}


/*
int SetPath(char* path)
{
    int size = strlen(path);
    int ret = 0;

    while (size)
    {
        if ('/' == *path)
        {
            *path = '\0';
            ++ret;
        }
        ++path;
        --size;
    }
    return ret;
}

void GetNextPath(char** path)
{
    while (**path)
    {
        ++*path;
    }
    ++*path;
}
*/
