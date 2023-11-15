#include <stdio.h>
#include <string.h>
#include <regex.h>

#define LEN 10000

void open_file(char *template, char *name, char *flags, int *file_count);
void add_letters(char *flags, char flag);
void parser(int argc, char *argv[], char *flags, char *template, int *end_flags);
void change_file(char *flags, FILE *fp, regex_t reg, char *name, int *file_count);
void e_action(int *e_count, int *i, int *end_flags, char *template, char *argv[]);
//void find_files(int argc, char *argv[], int *index);


int main(int argc, char *argv[])
{
    
        char template[LEN] = "";
        char flags[10] = "";
        int end_flags = 1;
        int file_count = 1;
        if (argc < 3) fprintf(stderr, "grep: Введите шаблон и/или имя файла\n");
        if (argc == 3) 
        {
            sprintf(template, "%s", argv[1]);
            open_file(template, argv[argc-1], flags, &file_count);
        }
       if (argc > 3)  {
      
            parser(argc, argv, flags, template, &end_flags);
            //printf("%s %s %d\n", flags, template, end_flags);
            for (int i = (end_flags + 1);i < argc; i++ ) {
                file_count = argc - end_flags - 1;
                open_file(template, argv[i], flags, &file_count);
            }
       }
    
    return 0;
}

void parser(int argc, char *argv[], char *flags, char *template, int *end_flags)
{   
    int e_count = 0;
    for (int i = 1; i < argc; i++)
    {   
        if ((argv[i][0] == '-') && (argv[i][1] != 'e') && (strlen(argv[i]) == 2)) {
            add_letters(flags, argv[i][1]);
            if (argv[i+1][0] != '-')
            { 
                sprintf(template, "%s", argv[i+1]);
                *end_flags = i + 1;
                break;
            }
        }
        if ((argv[i][0] == '-') && (argv[i][1] == 'e') && (strlen(argv[i]) == 2))
        {
            e_count += 1;
            add_letters(flags, argv[i][1]);
            e_action(&e_count, &i, end_flags, template, argv);
            /*if (e_count == 1) 
            {
                sprintf(template, "%s", argv[i+1]);
                *end_flags = i + 1;
            }
            else 
            {
                strcat(template, "|");
                strcat(template, argv[i + 1]);
                *end_flags = i + 1;
            }*/
            if (argv[i + 2][0] != '-') break;
        }  
        if (argv[1][0] != '-')
        {
            sprintf(template, "%s", argv[1]);
            *end_flags =  1;
            break;
        }
        if ((argv[i][0] == '-') && (strlen(argv[i]) > 2))
        {
            for (int j = 1; j < (int)strlen(argv[i]); j++) {
                
                add_letters(flags, argv[i][j]);
            }
                if ((strchr(flags, 'e') != NULL) && (argv[i+1][0] != '-'))
                { 
                    e_count += 1;
                    e_action(&e_count, &i, end_flags, template, argv);
                if (strchr(flags, 'e') == NULL)  
                {
                    sprintf(template, "%s", argv[i+1]);
                    *end_flags = i + 1;
                    //break;
                }  
            } 
        }
    }
}

void e_action(int *e_count, int *i, int *end_flags, char *template, char *argv[])
{
    if (*e_count == 1) 
    {
        sprintf(template, "%s", argv[*i+1]);
        *end_flags = *i + 1;
    }
    else 
    {
        strcat(template, "|");
        strcat(template, argv[*i + 1]);
        *end_flags = *i + 1;
    }
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

void open_file(char *template, char *name, char *flags, int *file_count){
    
    regex_t reg;
    FILE *fp = fopen(name, "r");
    if (strchr(flags, 'i') != NULL) regcomp(&reg, template, REG_ICASE | REG_EXTENDED);
    else regcomp(&reg, template, REG_EXTENDED);
    change_file(flags, fp, reg, name, file_count);
    regfree(&reg);
    fclose(fp); 

}

void change_file(char *flags, FILE *fp, regex_t reg, char *name, int *file_count)
{
    //printf("%d\n", *file_count);
    char txt[LEN] = "";
    regmatch_t pmatch[1];
    int line_matches = 0;
    int cur_line = 1;
    while (fgets(txt, LEN, fp) != NULL) {
        int status = regexec(&reg, txt, 1, pmatch, 0);
        int flag = 0;
        if (status == REG_NOMATCH && (strchr(flags, 'v') != NULL)) flag = 1;
        if (status == 0 && (strchr(flags, 'v') == NULL)) flag = 1;
        
        if ((*file_count > 1) && (flag== 1) && (strchr(flags, 'c') == NULL)  && (strchr(flags, 'l') == NULL) && (strchr(flags, 'n') == NULL)) printf("%s:%s", name, txt);
        if ((*file_count > 1) && (flag== 1) && (strchr(flags, 'c') == NULL)  && (strchr(flags, 'l') == NULL) && (strchr(flags, 'n') != NULL)) printf("%s:%d:%s", name, cur_line, txt);
        if ((*file_count == 1) && (flag == 1) && (strchr(flags, 'n') != NULL) && (strchr(flags, 'c') == NULL)  && (strchr(flags, 'l') == NULL)) printf("%d:", cur_line);
        if ((flag== 1) && (strchr(flags, 'c') == NULL)  && (strchr(flags, 'l') == NULL) && (*file_count == 1)) printf("%s", txt);
       
        
        
        line_matches += flag;
        cur_line++;
        }
        if ((strchr(flags, 'l') != 0) && line_matches > 0) printf("%s\n", name);
        if (strchr(flags, 'c') != 0) 
        {
            if (*file_count > 1) printf("%s:%d\n", name, line_matches);
            else printf("%d\n", line_matches);
        }
}


