#include <regex.h>
#include <stdio.h>
#include <string.h>

#define LEN 10000

void open_file(char *template, char *name, char *flags, int *file_count,
               int *f_file);
void add_letters(char *flags, char flag);
void parser(int argc, char *argv[], char *flags, char *template, int *end_flags,
            int *f_file);
void change_file(char *flags, FILE *fp, regex_t reg, char *name,
                 int *file_count, char *template);
void e_action(int *e_count, int *i, int *end_flags, char *template,
              char *argv[]);
void f_action(int *i, int *end_flags, char *template, char *patt, int *f_file);
void mult_e_action(int *e_count, int *i, int *j, int *end_flags, char *template,
                   char *argv[]);

int main(int argc, char *argv[]) {
  char template[LEN] = "";
  char flags[10] = "";
  int end_flags = 1;
  int file_count = 1;
  int f_file = 0;
  if (argc < 3) fprintf(stderr, "grep: option requires an argument -- e\n");
  if (argc == 3) {
    sprintf(template, "%s", argv[1]);
    open_file(template, argv[argc - 1], flags, &file_count, &f_file);
  }
  if (argc > 3) {
    parser(argc, argv, flags, template, &end_flags, &f_file);
    for (int i = (end_flags + 1); i < argc; i++) {
      file_count = argc - end_flags - 1;
      open_file(template, argv[i], flags, &file_count, &f_file);
    }
  }
  return 0;
}

void parser(int argc, char *argv[], char *flags, char *template, int *end_flags,
            int *f_file) {
  int e_count = 0;
  for (int i = 1; i < argc; i++) {
    if ((argv[i][0] == '-') && (argv[i][1] != 'e') && (strlen(argv[i]) == 2) &&
        (argv[i][1] != 'f')) {
      add_letters(flags, argv[i][1]);
      if (argv[i + 1][0] != '-') {
        sprintf(template, "%s", argv[i + 1]);
        *end_flags = i + 1;
        break;
      }
    }
    if ((argv[i][0] == '-') && (argv[i][1] == 'e') && (strlen(argv[i]) == 2)) {
      e_count += 1;
      add_letters(flags, argv[i][1]);
      e_action(&e_count, &i, end_flags, template, argv);
      if (argv[i + 2][0] != '-') break;
    }
    if ((argv[i][0] == '-') && (argv[i][1] == 'f') && (strlen(argv[i]) == 2)) {
      add_letters(flags, argv[i][1]);
      char patt[LEN] = "";
      sprintf(patt, "%s", argv[i + 1]);
      f_action(&i, end_flags, template, patt, f_file);
    }
    if (argv[1][0] != '-') {
      sprintf(template, "%s", argv[1]);
      *end_flags = 1;
      break;
    }
    if ((argv[i][0] == '-') && (strlen(argv[i]) > 2)) {
      for (int j = 1; j < (int)strlen(argv[i]); j++) {
        if ((argv[i][j] == 'e') && (((int)strlen(argv[i]) - 1) > j)) {
          add_letters(flags, argv[i][j]);
          e_count += 1;
          mult_e_action(&e_count, &i, &j, end_flags, template, argv);
          break;
        }
        if ((argv[i][j] != 'e') && (argv[i][j] != 'f'))
          add_letters(flags, argv[i][j]);
        if ((argv[i][j] == 'e') && (((int)strlen(argv[i]) - 1) == j)) {
          add_letters(flags, argv[i][j]);
          e_count += 1;
          e_action(&e_count, &i, end_flags, template, argv);
        }
        if ((argv[i][j] == 'f') && (((int)strlen(argv[i]) - 1) == j)) {
          add_letters(flags, argv[i][j]);
          char patt[LEN] = "";
          sprintf(patt, "%s", argv[i + 1]);
          f_action(&i, end_flags, template, patt, f_file);
        }
        if ((argv[i][j] == 'f') && (((int)strlen(argv[i]) - 1) > j)) {
          add_letters(flags, argv[i][j]);
          char patt[LEN] = "";
          sprintf(patt, "%c", argv[i][j + 1]);
          f_action(&i, end_flags, template, patt, f_file);
          break;
        }
      }
      if ((strchr(flags, 'e') == NULL) && (strchr(flags, 'f') == NULL)) {
        sprintf(template, "%s", argv[i + 1]);
        *end_flags = i + 1;
      }
    }
  }
}

void f_action(int *i, int *end_flags, char *template, char *patt, int *f_file) {
  char text[LEN] = "";
  FILE *fp = fopen(patt, "r");
  int x = 0;
  if (fp != NULL) {
    while (fgets(text, LEN, fp) != NULL) {
      if (text[0] != '\n')

      {
        if ((text[0] != ' ') && (x > 0)) {
          text[strlen(text) - 1] = '\0';
          strcat(template, "|");
          strcat(template, text);
        }
        if ((text[0] != ' ') && (x == 0)) {
          text[strlen(text) - 1] = '\0';
          sprintf(template, "%s", text);
          x += 1;
        }
      } else
        *f_file += 1;
    }
    *end_flags = *i + 1;
  } else
    fprintf(stderr, "grep: %s: No such file or directory\n", patt);
  fclose(fp);
}

void e_action(int *e_count, int *i, int *end_flags, char *template,
              char *argv[]) {
  if (*e_count == 1) {
    sprintf(template, "%s", argv[*i + 1]);
    *end_flags = *i + 1;
  } else {
    strcat(template, "|");
    strcat(template, argv[*i + 1]);
    *end_flags = *i + 1;
  }
}

void mult_e_action(int *e_count, int *i, int *j, int *end_flags, char *template,
                   char *argv[]) {
  if (*e_count == 1) {
    for (int m = *j + 1; m < (int)strlen(argv[*i]); m++) {
      char buff[3] = "";
      buff[0] = argv[*i][m];
      strcat(template, buff);
    }
    *end_flags = *i;
  } else {
    strcat(template, "|");
    for (int m = *j + 1; m < (int)strlen(argv[*i]); *j = *j + 1) {
      char buff[3] = "";
      buff[0] = argv[*i][m];
      strcat(template, buff);
    }
    *end_flags = *i;
  }
}

void add_letters(char *flags, char flag) {
  if (strchr(flags, flag) == NULL) {
    char buff[3] = "";
    buff[0] = flag;
    strcat(flags, buff);
  }
}

void open_file(char *template, char *name, char *flags, int *file_count,
               int *f_file) {
  regex_t reg;
  FILE *fp = fopen(name, "r");
  if (fp != NULL) {
    if (*f_file == 0) {
      if (strchr(flags, 'i') != NULL)
        regcomp(&reg, template, REG_ICASE | REG_EXTENDED);
      else
        regcomp(&reg, template, REG_EXTENDED);
      change_file(flags, fp, reg, name, file_count, template);
      regfree(&reg);
    } else {
      char c;
      while ((c = fgetc(fp)) != EOF) printf("%c", c);
    }
  }
  if ((fp == NULL) && (strchr(flags, 's') == NULL))
    fprintf(stderr, "grep: %s: No such file or directory\n", name);
  fclose(fp);
}

void change_file(char *flags, FILE *fp, regex_t reg, char *name,
                 int *file_count, char *template) {
  char txt[LEN] = "";
  regmatch_t pmatch[1];
  int line_matches = 0;
  int cur_line = 1;
  while (fgets(txt, LEN, fp) != NULL) {
    int status = regexec(&reg, txt, 1, pmatch, 0);
    int flag = 0;
    if (status == REG_NOMATCH && (strchr(flags, 'v') != NULL)) flag = 1;
    if (status == 0 && (strchr(flags, 'v') == NULL)) flag = 1;
    if ((*file_count > 1) && (flag == 1)) {
      if ((strchr(flags, 'c') == NULL) && (strchr(flags, 'l') == NULL) &&
          (strchr(flags, 'n') == NULL) && (strchr(flags, 'h') == NULL)) {
        if (strchr(flags, 'o') == NULL)
          printf("%s:%s", name, txt);
        else
          printf("%s:%s\n", name, template);
      }
      if ((strchr(flags, 'c') == NULL) && (strchr(flags, 'l') == NULL) &&
          (strchr(flags, 'n') != NULL)) {
        if (strchr(flags, 'o') == NULL)
          printf("%s:%d:%s", name, cur_line, txt);
        else
          printf("%s:%d:%s\n", name, cur_line, template);
      }
      if ((strchr(flags, 'c') == NULL) && (strchr(flags, 'l') == NULL) &&
          (strchr(flags, 'n') == NULL) && (strchr(flags, 'h') != NULL)) {
        if (strchr(flags, 'o') == NULL)
          printf("%s", txt);
        else
          printf("%s\n", template);
      }
    }
    if ((*file_count == 1) && (flag == 1) && (strchr(flags, 'n') != NULL) &&
        (strchr(flags, 'c') == NULL) && (strchr(flags, 'l') == NULL))
      printf("%d:", cur_line);
    if ((flag == 1) && (strchr(flags, 'c') == NULL) &&
        (strchr(flags, 'l') == NULL) && (*file_count == 1)) {
      if (strchr(flags, 'o') == NULL)
        printf("%s", txt);
      else
        printf("%s\n", template);
    }
    line_matches += flag;
    cur_line++;
  }
  if ((strchr(flags, 'l') != 0) && line_matches > 0) printf("%s\n", name);
  if (strchr(flags, 'c') != 0) {
    if (*file_count > 1)
      printf("%s:%d\n", name, line_matches);
    else
      printf("%d\n", line_matches);
  }
}
