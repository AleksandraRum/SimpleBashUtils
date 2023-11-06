#include <stdio.h>
#include <string.h>


void open_file(char *name, char *flags);
void pars_flags(int argc, char *argv[], char *flags);
void change_file(char c, char *flags, int *i, char *last_c, int *empty_str);
void add_flag(char *flags, char flag);


int main(int argc, char *argv[])
{    
    if (argc > 1){
        char flags[10] = "";
        pars_flags(argc, argv, flags);
        open_file(argv[argc-1], flags);
       
    }
    else printf("Введите название файла\n");
    return 0;
}

void open_file(char* name, char *flags){
    FILE *fp = fopen(name, "r");
    if (fp != NULL){
        char c, last_c = '\n';
        int i = 1;
        int empty_str = 0;
        while ((c = fgetc(fp)) != EOF) {
            
            change_file(c, flags, &i, &last_c, &empty_str);
            if ((empty_str == 0) || (empty_str == 1)) putc(c, stdout);    
        }   
    }  
    fclose(fp); 
}

void add_flag(char *flags, char flag)
{
    if (strchr(flags, flag) == NULL) 
    {
        char buff[3] = "";
        buff[0] = flag;
        strcat(flags, buff);
    }
}

void pars_flags(int argc, char *argv[], char *flags)
{    
    for (int i = 1; i < argc; i++){
        if ((argv[i][0] == '-') && (argv[i][1] != '-')) {
            for (int j = 1; j < (int)strlen(argv[i]); j++) {
                if (argv[i][j] == 'e') 
                {
                    add_flag(flags, 'E');
                    add_flag(flags, 'v');
                }
                if (argv[i][j] == 't') 
                {
                    add_flag(flags, 'T');
                    add_flag(flags, 'v');
                }
                
                if ((argv[i][j] != 't') && (argv[i][j] != 'e')) add_flag(flags, argv[i][j]);
                
            }
        }
        if ((argv[i][0] == '-') && (argv[i][1] == '-') && (96 < argv[i][2]) && (argv[i][2] < 123))
        {
            if (strcmp("--number-nonblank", argv[i]) == 0) add_flag(flags, 'b');
            if (strcmp("--number", argv[i]) == 0) add_flag(flags, 'n');
            if (strcmp("--squeeze-blank", argv[i]) == 0) add_flag(flags, 's');
        }
    }
    //printf("%s\n", flags);
}  


void change_file(char c, char *flags, int *i, char *last_c, int *empty_str)
{    
    if (strchr(flags, 's') != NULL && (*last_c == '\n') && (c == '\n')) *empty_str += 1;
    else *empty_str = 0;
    
    if (strchr(flags, 'b') != NULL && (c != '\n') && (*last_c == '\n')) {
        printf("%6d\t", *i);
        *i += 1;
    }

    if (strchr(flags, 'n') != NULL && (*last_c == '\n') && strchr(flags, 'b') == NULL && ((*empty_str == 0) || (*empty_str == 1))) {
        printf("%6d\t", *i);
        *i += 1;
    }
    if (strchr(flags, 'E') != NULL && c == '\n') printf("$");

    if (strchr(flags, 'T') != NULL && (c == '\t'))
    {
        printf("^");
        c = 'I';
    }

    if ((strchr(flags, 'v') != NULL) && ((c >= 0 && c < 9) || (c > 10 && c < 32))) {
        printf("^");
        c = c + 64;
    }
    *last_c = c;   
}
