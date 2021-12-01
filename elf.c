/*
 * @Author: 21181214207
 * @Date: 2021-11-29 07:30:37
 * @LastEditors: 21181214207
 * @LastEditTime: 2021-12-01 02:21:03
 * @FilePath: /Documents/demo/elf.c
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <elf.h>

int main(int argc,char* argv[]){
    // 需要传入3个参数
    if(argc<3){
        printf("命令格式：./elf -x xxx 这里只含: %d个参数\n",argc);
        exit(0);
        }
    //打开文件
    FILE *fp;
    //对head解析
    Elf64_Ehdr elf_header;
    fp = fopen(argv[2],"r");
    if(fp==0){exit(0);}

    int readfile;
    //将文件读取到elf_header
    readfile = fread(&elf_header,sizeof(Elf64_Ehdr),1,fp);
    if(readfile==0){exit(0);}
    //判断elf文件类型，如果不是elf文件就不执行之后的操作
    if(elf_header.e_ident[0]== 0x7F || elf_header.e_ident[1] == 'E'){
        // 查看指定elf文件的头结构
        if(strcmp(argv[1],"-h")==0){
            printf("header:");
        for(int x=0;x<16;x++){
            printf("%02x ",elf_header.e_ident[x]);
        }
        printf("\n");
        printf("Object file type: \t\t\t\t");
        switch(elf_header.e_type) {
            case 0 : printf("未知文件类型\n"); break;
            case 1 : printf("可重定位文件\n"); break;
            case 2 : printf("可执行文件\n"); break;
            case 3 : printf("动态链接库文件\n"); break;
            case 4 : printf("Core文件\n"); break;
            case 0xff00 : printf("特定处理器文件扩展下边界\n"); break;
            case 0xffff : printf("特定处理器文件扩展上边界\n"); break;
        }
        // printf("Object file type: %hx\n",elf_header.e_type);
        printf("Architecture: \t\t\t\t");
        switch(elf_header.e_machine) {
            case 0 : printf("未知体系结构\n"); break;
            case 1 : printf("AT&T WE 32100\n"); break;
            case 2 : printf("SPARC\n"); break;
            case 3 : printf("Intel Architecture\n"); break;
            case 4 : printf("Motorola 68000\n"); break;
            case 5 : printf("Motorola 88000\n"); break;
            case 7 : printf("Intel 80860\n"); break;
            case 8 : printf("MIPS RS3000 Big-Endian\n"); break;
            case 10 : printf("MIPS RS4000 Big-Endian\n"); break;
            case 62 : printf("AMD x86-64 architecture\n"); break;
    }
        // printf("Architecture: %hx\n",elf_header.e_machine);
        printf("Object file version:\t\t\t\t 0x%d\n",elf_header.e_version);
        printf("Entry point virtual address:\t\t\t 0x%x\n",elf_header.e_entry);
        printf("Program header file offset:\t\t\t %d(bytes)\n",elf_header.e_phoff);
        printf("Section header file offset:\t\t\t %d(bytes)\n",elf_header.e_shoff);
        printf("Processor-specific flags:\t\t\t 0x%d\n",elf_header.e_flags);
        printf("ELF header size in bytes:\t\t\t %d(bytes)\n",elf_header.e_ehsize);
        printf("Program header table entry size:\t\t %d\n",elf_header.e_phentsize);
        printf("Program header table entry count:\t\t %d\n",elf_header.e_phnum);
        printf("Section header table entry size:\t\t %d(bytes)\n",elf_header.e_shentsize);
        printf("Section header table entry count:\t\t %d\n",elf_header.e_shnum);
        printf("Section header string table index:\t\t %d\n",elf_header.e_shstrndx);
        }else if(strcmp(argv[1],"-S")==0){
            //查看节区表内所有信息
            int shnum,x;
            //解析section,分配相应内存空间
            Elf64_Shdr *shdr = (Elf64_Shdr*)malloc(sizeof(Elf64_Shdr) * elf_header.e_shnum);
            int temp;
            //将指针移动到section header处
            temp = fseek(fp,elf_header.e_shoff,SEEK_SET);
            //读取sectiono到shdr
            temp = fread(shdr,sizeof(Elf64_Shdr) * elf_header.e_shnum,1,fp);
            //重置指针
            rewind(fp);
            //把指针移动到字符串表偏移位置处
            fseek(fp,shdr[elf_header.e_shstrndx].sh_offset,SEEK_SET);
            char shstrtab[shdr[elf_header.e_shstrndx].sh_size];
            char *names = shstrtab;
            //读取字符串表到shstrtab
            temp = fread(shstrtab,shdr[elf_header.e_shstrndx].sh_size,1,fp);
            printf("序号\t节类型\t\t节地址\t\t\t节偏移\t\t节大小\t\t节名称\n");
            for(shnum=0;shnum<elf_header.e_shnum;shnum++){
                names = shstrtab;
                //names指向当前section的name位置
                names += shdr[shnum].sh_name;
                printf("%d\t",shnum);
                 switch(shdr[shnum].sh_type) {
                    case 0 : printf("SHT_NULL\t"); break;
                    case 1 : printf("SHT_PROGBITS\t"); break;
                    case 2 : printf("SHT_SYMTAB\t"); break;
                    case 3 : printf("SHT_STRTAB\t"); break;
                    case 4 : printf("SHT_RELA\t"); break;
                    case 5 : printf("SHT_HASH\t"); break;
                    case 6 : printf("SHT_DYNAMIC\t"); break;
                    case 7 : printf("SHT_NOTE\t"); break;
                    case 8 : printf("SHT_NOBITS\t"); break;
                    case 9 : printf("SHT_REL\t"); break;
                    case 10 : printf("SHT_SHLIB\t"); break;
                    case 11 : printf("SHT_DYNSYM\t"); break;
                    case 14 : printf("SHT_INIT_ARRAY\t"); break;
                    case 15 : printf("SHT_FINI_ARRAY\t"); break;
                    case 0x70000000 : printf("SHT_LOPROC\t"); break;
                    case 0x7fffffff : printf("SHT_HIPROC\t"); break;
                    case 0x80000000 : printf("SHT_LOUSER\t"); break;
                    case 0xffffffff : printf("SHT_HIUSER\t"); break;
                    case 0x6ffffff6 : printf("SHT_GNU_HASH\t"); break;
                    case 0x6fffffff : printf("SHT_GNU_versym\t"); break;
                    case 0x6ffffffe : printf("SHT_GNU_verneed\t"); break;
        }
                printf("0x%016x\t0x%08x\t%2lu bytes\t%s \n",shdr[shnum].sh_addr,shdr[shnum].sh_offset,shdr[shnum].sh_size,names);
            }
            
        }else if(strcmp(argv[1],"-s")==0){
            //查看符号表
            int shnum,x;
            //同上
            Elf64_Shdr *shdr = (Elf64_Shdr*)malloc(sizeof(Elf64_Shdr) * elf_header.e_shnum);
            
            int temp;
            temp = fseek(fp,elf_header.e_shoff,SEEK_SET);
            temp = fread(shdr,sizeof(Elf64_Shdr) * elf_header.e_shnum,1,fp);
            //重置指针
            rewind(fp);
            //把指针移动到离文件开头多少字节处
            fseek(fp,shdr[elf_header.e_shstrndx].sh_offset,SEEK_SET);
            char shstrtab[shdr[elf_header.e_shstrndx].sh_size];
            char *names = shstrtab;

            //从fp所指位置开始读取多少字节
            temp = fread(shstrtab,shdr[elf_header.e_shstrndx].sh_size,1,fp);
            for(shnum=0;shnum<elf_header.e_shnum;shnum++){
                names = shstrtab;
                names += shdr[shnum].sh_name;
                
                if(strcmp(names,".symtab")==0){
                    printf("symtab");
                    //解析符号表
                    Elf64_Sym *sym = (Elf64_Sym*)malloc(sizeof(Elf64_Sym)*shdr[shnum].sh_size);

                    temp = fseek(fp,shdr[shnum].sh_offset,SEEK_SET);
                    temp = fread(sym,sizeof(Elf64_Sym)*shdr[shnum].sh_size,1,fp);
                    //获取符号数量
                    int num = shdr[shnum].sh_size/shdr[shnum].sh_entsize;
                    printf("Symbol table '.symtab' contains %d entries:\n",num);
                    printf("序号\t\tValue\t\tSize\tType\tVis\tName\n");
                    for(int i=0;i<num;i++){
                        printf("%d\t",i);
                        printf("%016x\t",sym[i].st_value);
                        printf("%x\t",sym[i].st_size);
                        switch(sym[i].st_info) {
                            case 0 : printf("NOTYPE\t"); break;
                            case 1 : printf("OBJECT\t"); break;
                            case 2 : printf("FUNC\t"); break;
                            case 3 : printf("SECTION\t"); break;
                            case 4 : printf("FILE\t"); break;
                            case 5 : printf("COMMON\t"); break;
                            case 6 : printf("TLS\t"); break;
                            case 7 : printf("NUM\t"); break;
                            case 10 : printf("LOOS\t"); break;
                            case 12 : printf("HIOS\t"); break;
                            case 13 : printf("LOPROC\t"); break;
                        }
                        // printf("%x\t",sym[i].st_info);
                        //VIS
                        switch(sym[i].st_other) {
                            case 0 : printf("DEFAULT\t"); break;
                            case 1 : printf("INTERNAL\t"); break;
                            case 2 : printf("HIDDEN\t"); break;
                            case 3 : printf("PROTECTED\t"); break;
                        }
                        // printf("\t%x\t",sym[i].st_other);
                        printf("%x\n",sym[i].st_name);
                    }
                }
            }

        }
        else{
            printf("指令不正确，正确指令为 - h/S/s");
        }
        
    }else{
        printf("不是ELF文件！");
    }
    return 0;

}