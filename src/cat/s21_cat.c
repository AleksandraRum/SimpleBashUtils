#include <stdio.h>
#include <string.h>

void open_file(char *name, char *flags);
void pars_flags(int argc, char *argv[], char *flags, int *end_flags);
void add_flag(char *flags, char flag);

int main(int argc, char *argv[]) {
  if (argc > 1) {
    char flags[10] = "";
    int end_flags = 0;

    pars_flags(argc, argv, flags, &end_flags);
    for (int i = end_flags; i < argc; i++) open_file(argv[i], flags);
  } else
    printf("Введите название файла\n");
  return 0;
}

void open_file(char *name, char *flags) {
  FILE *fp = fopen(name, "r");
  if (fp != NULL) {
    char c, last_c = '\n';
    int i = 1;
    int empty_str = 0;
    while ((c = fgetc(fp)) != EOF) {
      if (strchr(flags, 's') != NULL && (last_c == '\n') && (c == '\n')) {
        empty_str += 1;
        if (empty_str > 1) continue;
      } else
        empty_str = 0;

      if (strchr(flags, 'b') != NULL && (c != '\n') && (last_c == '\n')) {
        printf("%6d\t", i);
        i += 1;
      }
      if (strchr(flags, 'n') != NULL && (last_c == '\n') &&
          strchr(flags, 'b') == NULL) {
        printf("%6d\t", i);
        i += 1;
      }
      if (strchr(flags, 'E') != NULL && c == '\n') printf("$");

      if (strchr(flags, 'T') != NULL && (c == '\t')) {
        printf("^");
        c = c + 64;
      }
      if ((strchr(flags, 'v') != NULL) &&
          ((c >= 0 && c < 9) || (c > 10 && c < 32))) {
        printf("^");
        c = c + 64;
      }
      printf("%c", c);
      last_c = c;
    }
  } else
    fprintf(stderr, "cat: %s: No such file or directory\n", name);
  fclose(fp);
}

void add_flag(char *flags, char flag) {
  if (strchr(flags, flag) == NULL) {
    char buff[3] = "";
    buff[0] = flag;
    strcat(flags, buff);
  }
}

void pars_flags(int argc, char *argv[], char *flags, int *end_flags) {
  for (int i = 1; i < argc; i++) {
    if ((argv[i][0] == '-') && (argv[i][1] != '-')) {
      for (int j = 1; j < (int)strlen(argv[i]); j++) {
        if (argv[i][j] == 'e') {
          add_flag(flags, 'E');
          add_flag(flags, 'v');
        }
        if (argv[i][j] == 't') {
          add_flag(flags, 'T');
          add_flag(flags, 'v');
        }
        if ((argv[i][j] != 't') && (argv[i][j] != 'e'))
          add_flag(flags, argv[i][j]);
      }
    }
    if ((argv[i][0] == '-') && (argv[i][1] == '-') && (96 < argv[i][2]) &&
        (argv[i][2] < 123)) {
      if (strcmp("--number-nonblank", argv[i]) == 0) add_flag(flags, 'b');
      if (strcmp("--number", argv[i]) == 0) add_flag(flags, 'n');
      if (strcmp("--squeeze-blank", argv[i]) == 0) add_flag(flags, 's');
    }
    if (argv[i][0] != '-') {
      *end_flags = i;
      break;
    }
  }
}
