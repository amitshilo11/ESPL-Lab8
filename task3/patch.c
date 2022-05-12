
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdlib.h>
#include <elf.h>

int main(int argc, char **argv) {   // (src_file, src_poss, size, dest_file, dest_poss)
                                    //      [1]     [2]     [3]     [4]         [5]
                                    
    char* src_name = argv[1];
    char* dest_name = argv[4];
    int src_offset = atoi(argv[2]);
    int dest_offset = atoi(argv[5]);
    int size = atoi(argv[3]);

    int src_fd = open(src_name, O_RDONLY);
    fseek(src_fd, 0L, SEEK_END);
    int src_size = ftell(src_fd);
    char* src_elf =  mmap(0, src_size, PROT_READ, MAP_SHARED, src_fd, 0);
    //char* src_arr = src_elf->e_ident;

    int dest_fd = open(dest_name, O_RDWR);
    struct stat dest_fd_stat;
    char* dest_elf =  mmap(0, dest_fd_stat.st_size, PROT_READ | PROT_WRITE , MAP_SHARED, dest_fd, 0);
    //char* dest_arr = dest_elf->e_ident; 

    memcpy(dest_elf+dest_offset, src_elf+src_offset, size);

    close(src_fd);
    close(dest_fd);
    return 0;
}
 