

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>

#include "ext2_fs.h"

#define BUFF_SIZE   1000
#define BLOCK_SIZE   1024

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
    printf("-- %s \t %hx\n", "blk->s_magic", blk->s_magic);
    printf("-- %s \t\t %hu\n", "s_state", blk->s_state);
    printf("-- %s \t\t %hu\n", "s_errors", blk->s_errors);
    printf("-- %s \t %hu\n", "s_minor_rev_level", blk->s_minor_rev_level);
    printf("-- %s \t\t %u\n", "s_lastcheck", blk->s_lastcheck);
    printf("-- %s \t %u\n", "s_checkinterval", blk->s_checkinterval);
    printf("-- %s \t %u\n", "s_creator_os", blk->s_creator_os);
    printf("-- %s \t\t %u\n", "s_rev_level", blk->s_rev_level);
    printf("-- %s \t %hu\n", "s_def_resuid", blk->s_def_resuid);
    printf("-- %s \t %hu\n", "s_def_resgid", blk->s_def_resgid);
    printf("\n\n");

}

int main()
{
    int fd;
    struct ext2_super_block super;
    // char buff[BUFF_SIZE] = {0};

    fd = open("/dev/ram0", O_RDONLY);    /* open floppy device */

    lseek(fd, BLOCK_SIZE, SEEK_SET);                    /* position head above super-block */
    read(fd, &super, sizeof(super));              /* read super-block */


    if (super.s_magic != EXT2_SUPER_MAGIC)
    {
        printf("-- error \n");
        close(fd);
        exit(1); /* bad file system */
    }

    PrintSuperBlock(&super);

    // block_size = 1024 << super.s_log_block_size;  /* calculate block size in bytes */

    close(fd);

    return 0;
}
