
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
void print_sections(char* name){
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
              int num_sec=hdr->e_shnum; //how many sections do we have
              Elf32_Shdr* currentSection=(Elf32_Shdr*)(((char *)hdr)+(hdr->e_shoff)); //get the address to the section (header+offset);
              char * offset= ((char *)hdr)+(currentSection[hdr->e_shstrndx].sh_offset);
              printf("  %s %-19s %s\t %s\t %s\n", "index", "Name  ", "Addr ", "Off", "Size");
    
              for(int i=0;i<num_sec;i++){
                   printf("  [%d]\t %-20s %08X\t %06X\t %06X\n",i,offset+currentSection[i].sh_name,currentSection[i].sh_addr,currentSection[i].sh_offset,currentSection[i].sh_size);
               }
        }
        else {
            perror("Not ELF File");
        }   
    }  
}
 void Print_Symbols(char * name){
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

            Elf32_Shdr* sections=(Elf32_Shdr*)(map_addr + hdr->e_shoff);
             char *sectionNames = (char*) (map_addr + sections[hdr->e_shstrndx].sh_offset);
            int num_sec=hdr->e_shnum; /*Section header count*/
     
            for(int i=0;i< num_sec;i++){
                if((sections[i].sh_type==SHT_DYNSYM)||(sections[i].sh_type==SHT_SYMTAB)){
                    Elf32_Sym *symbols = (Elf32_Sym *) (map_addr + sections[i].sh_offset);    /* Section file offset */
                   int size_of_table = sections[i].sh_size / sizeof(Elf32_Sym); 
                  char *symbolNames = (char *)(map_addr + sections[sections[i].sh_link].sh_offset );
                  if(sections[i].sh_type==2) {
                        printf("Symbol table '.symtab' contains %d entries:",size_of_table);
                   }
                  else {
                     printf("Symbol table '.dynsym' contains %d entries:",size_of_table);
                     }
                  printf("\n%s\t %s\t\t %s\t %s\t %s\n", "[index]", "Value", "Section index", "Section name", "Symbol name");
        
                    for(int j=0;j<size_of_table;j++){
                            char* secName="";
                     if((symbols[j].st_shndx)<=(hdr->e_shnum)){ /*section Index < numberOfsections in the header*/
                            secName=sectionNames + sections[symbols[j].st_shndx].sh_name;
                  }
                    printf("%d:\t %08x\t %d\t\t %s\t\t %s\n", j, symbols[j].st_value, symbols[j].st_shndx, secName, symbolNames + symbols[j].st_name);
            }
             printf("\n");
        }        
    }
}
    }

 }

  


int main(int argc, char** argv) {
    if(argc < 2){
        perror("file name needed");
        return 0;
    }
    Print_Symbols(argv[1]);
}