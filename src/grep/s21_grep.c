#include <stdio.h>
#include <string.h>
#include <regex.h>

#define LEN 10000

void open_file(char *template, char *name, char *flags);
void add_letters(char *flags, char flag);
void parser(int argc, char *argv[], char *flags, char *template);
void change_file(char *flags, FILE *fp, regex_t reg, char *name);


int main(int argc, char *argv[])
{
    
        char template[LEN] = "";
        char flags[10] = "";
        if (argc < 3) printf("Введите шаблон или имя файла\n");
        if (argc == 3) 
        {
            sprintf(template, "%s", argv[1]);
            open_file(template, argv[argc-1], flags);
        }
       if (argc > 3)  {
    
            parser(argc, argv, flags, template);
       }
    
    return 0;
}

void parser(int argc, char *argv[], char *flags, char *template)
{   
    for (int i = 1; i < argc - 1; i++)
    {   
        if ((argv[i][0] == '-') && (argv[i][1] != '-'))
        {
            for (int j = 1; j < (int)strlen(argv[i]); j++) {
                add_letters(flags, argv[i][j]);
            }
            if (strchr(flags, 'e') !=  NULL)
            {
                sprintf(template, "%s", argv[i+1]);
                open_file(template, argv[argc-1], flags);
            }
        else {
            sprintf(template, "%s", argv[i+1]);
            open_file(template, argv[argc-1], flags);
            }
        }
    }   
    //printf("%s %s\n", template, flags); 
}

void add_letters(char *flags, char flag)
{
    if (strchr(flags, flag) == NULL)
    {
        char buff[3] = "";
        buff[0] = flag;
        strcat(flags, buff);
    }
}

void open_file(char *template, char *name, char *flags){
    
    regex_t reg;
    FILE *fp = fopen(name, "r");
    int cflags;
    if (strchr(flags, 'i') != NULL) regcomp(&reg, template, REG_ICASE);
    else regcomp(&reg, template, 0);
    change_file(flags, fp, reg, name);
    regfree(&reg);
    fclose(fp); 

}

void change_file(char *flags, FILE *fp, regex_t reg, char *name)
{
    char txt[LEN] = "";
    regmatch_t pmatch[1];
    int line_matches = 0;
    int cur_line = 1;
    while (fgets(txt, LEN, fp) != NULL) {
        int status = regexec(&reg, txt, 1, pmatch, 0);
        int flag = 0;
        if (status == REG_NOMATCH && (strchr(flags, 'v') != NULL)) flag = 1;
        if (status == 0 && (strchr(flags, 'v') == NULL)) flag = 1;
        if ((flag == 1) && (strchr(flags, 'n') != NULL) && (strchr(flags, 'c') == NULL)  && (strchr(flags, 'l') == NULL)) printf("%d:", cur_line);
        if ((flag== 1) && (strchr(flags, 'c') == NULL)  && (strchr(flags, 'l') == NULL)) printf("%s", txt);
        
        
        line_matches += flag;
        cur_line++;
        }
        if ((strchr(flags, 'l') != 0) && line_matches > 0) printf("%s\n", name);
        if (strchr(flags, 'c') != 0) printf("%d\n", line_matches);
}


