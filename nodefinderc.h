#define _GNU_SOURCE
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#define NODEFINDERC_VERSION "1.1.5"
#define MAX_INDEX_LIST_NUM 200
#define MULTIPLE_OF_BUFSIZE 2
#define MAX_CONFIG_LINE_LEN 500
#define COMMENT_SYMBOL '#'

const char *BAR = "=======================================================================\n";
const char *THIN_BAR = "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";

struct Calibration {
    char *name_a;
    char *name_b;
    char *cali_info;
};


/**************************** FUNCTIONS ************************************/


// print functions
void print_array(const int arr[], int size);
void print_cali_struct(size_t cali_num, struct Calibration *calis[cali_num]);
void printcali(struct Calibration cali);


// Utils functions
char* make_str_clean(char* input);
char *stripchar(char *str, const char c);
size_t countchar(const char *s, const char c);
size_t get_index_of_substring(const char *srcstr, const char *substr);
void reverse_array(int arr[], int start, int end);
char *sliced_string(const char *str, int start, int end);
char **split_by_delim(const char *str, const char *delim, size_t *num_tokens);
char *concat_three_str(const size_t bufsize, const char *str1,
                       const char *str2, const char *str3);
char *read_whole_str(const char *filename);
void write_str_to_file(const char *filename, const char *out_str);


// NodeFinder functions
void print_version();
void get_insertion_list(const char *treestr, int *insertion_list, int *list_num, int indexnow);
int get_index_of_tmrca(const char *treestr, const char *name_a, const char *name_b);
char *get_exists_cali(const char *right_part);
char *find_non_cali_pointer(const char *right_part);
char *add_cali(const char *treestr, int index_of_tmrca, const char *cali_info);
char *modify_exists_cali(const char *treestr, int index_of_tmrca, const char *cali_info);
char *single_cali(char *treestr, struct Calibration *cali);
char *multi_cali(char *clean_str, int cali_num, struct Calibration *calis[cali_num]);
size_t read_config_lines(const char *config_file_name, char *valid_config_lines[]);
void parse_config(char *valid_config_lines[], size_t valid_line_num,
                  struct Calibration *calis[valid_line_num]);
void show_help_message();
int argparser (int argc, char **argv,
               char **infile_value,
               char **outfile_value,
               char **config_file_value);
