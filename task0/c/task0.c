
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <elf.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
 int fd;
 struct stat f_stat;
 void* map_addr =NULL;
 Elf32_Ehdr * hdr=NULL; 
 

int use_mmap(char* name){
   
     if( (fd = open(name, O_RDONLY)) < 0 ) {
        perror("file open failed.");
        fd = -1;
        return 0;
    }
    if(fstat(fd,&f_stat)!=0){
        perror("fstat failed.");
        return 0;
    }
    if((map_addr= mmap(0,f_stat.st_size,PROT_READ,MAP_PRIVATE,fd,0))==MAP_FAILED){
        perror("mmap failed.");
        return 0;
    }
    hdr=(Elf32_Ehdr *) map_addr;
    return 1;

}
void ExamineELF(char* name) {
    
    if(fd !=-1){
        close(fd);
        if(map_addr!=NULL){
            munmap(map_addr,f_stat.st_size);
        }
    }
    if(use_mmap(name)){
        
        unsigned char *magic;
        magic=hdr->e_ident;
        if((magic[1]=='E')&&(magic[2]=='L')&&(magic[3]=='F')&&(magic[0]==0x7f)){
            printf("first: %c, second:%c, third: %c\n",magic[1],magic[2],magic[3]);
            printf("entry point: %4x\n",hdr->e_entry);
            printf("data encoding scheme: %d\n",magic[5]);
            printf("section header table file offset: %d\n",hdr->e_shoff);
            printf("section header entries: %d\n",hdr->e_shnum);
            printf("section header size: %d\n",hdr->e_shentsize);
            printf("program header file offset: %d\n",hdr->e_phoff);
            printf("program header entries: %d\n",hdr->e_phnum);
            printf("program header size: %d\n",hdr->e_phentsize);
        }
        else {
            perror("Not ELF File");
        }   
    }  
}


int main(int argc, char** argv) {
    if(argc < 2){
        perror("file name needed");
        return 0;
    }
    ExamineELF(argv[1]);
}