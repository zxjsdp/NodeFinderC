#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_INDEX_LIST_NUM 200
#define MULTIPLE_OF_BUFSIZE 2

struct Calibration {
    char *name_a;
    char *name_b;
    char *cali_info;
};


// =======================================================
// print functions
// =======================================================

void print_array(const int arr[], int size)
{
    int i;

    for (i=0; i<size; i++)
        printf("%d ", arr[i]);
    printf("\n");
}

void print_cali_struct(size_t cali_num, struct Calibration *calis[cali_num])
{
    int i;

    for (i=0; i<cali_num; i++) {
        printf("%s, %s, %s\n", calis[i]->name_a, calis[i]->name_b, calis[i]->cali_info);
    }
}

void printcali(struct Calibration cali)
{
    printf("%s, %s, %s\n", cali.name_a, cali.name_b, cali.cali_info);
}



// =======================================================
// Utils functions
// =======================================================

char* make_str_clean(char* input)
{
    int i,j;
    char *output=input;
    for (i = 0, j = 0; i<strlen(input); i++,j++)
    {
        if (input[i] != ' ' && input[i] != '\t' && input[i] != '\n' && input[i] != '\r')
            output[j]=input[i];
        else
            j--;
    }
    output[j]=0;
    return output;
}

// int startswithchar(const char *str_a, const *char str_b)
// {
//     size_t len_a = strlen(str_a), len_b = strlen(str_b);
//     return len_b < len_a ? 0 : strncmp(str_a, str_b, len_a) == 0;
// }

char *stripchar(char *str, const char c)
{
    size_t size;
    char *s;
    char *end;

    s = strdup(str);

    size = strlen(s);
    if (!size)
        return s;
    end = s + size - 1;
    while (end >= s && *end == c)
        end--;
    *(end + 1) = '\0';

    while (*s && *s == c)
        s++;

    return s;
}

size_t countchar(const char *s, const char c)
{
    int i;
    size_t count;

    count = 0;

    for (i=0; s[i]; i++) {
        if (s[i] == c)
            count++;
    }

    return count;
}

size_t get_index_of_substring(const char *srcstr, const char *substr)
{
    return strstr(srcstr, substr) - srcstr;
}


void reverse_array(int arr[], int start, int end)
{
    int temp;
    while (start < end) {
        temp = arr[start];
        arr[start] = arr[end];
        arr[end] = temp;
        start++;
        end--;
    }
}

char *sliced_string(const char *str, int start, int end)
{
    if (start >= end) {
        printf("start index must be smaller than end index while slicing\n");
        exit(EXIT_FAILURE);
    }
    char *s = strdup(str);

    s += start;
    *(s + (end - start)) = '\0';

    return s;
}

char **split_by_delim(const char *str, const char *delim, size_t *num_tokens)
{
    char *s = strdup(str);
    size_t tokens_alloc = 1;
    size_t tokens_used = 0;
    char **tokens = calloc(tokens_alloc, sizeof(char *));
    char *token, *strtok_ctx;

    for (token = strtok_r(s, delim, &strtok_ctx);
            token != NULL;
            token = strtok_r(NULL, delim, &strtok_ctx)) {
        if (tokens_used == tokens_alloc) {
            tokens_alloc *= 2;
            tokens = realloc(tokens, tokens_alloc * sizeof(char *));
        }
        tokens[tokens_used++] = strdup(token);
    }

    if (tokens_used == 0) {
        free(tokens);
        tokens = NULL;
    } else {
        tokens = realloc(tokens, tokens_used * sizeof(char *));
    }
    *num_tokens = tokens_used;
    free(s);
    return tokens;
}

char *concat_three_str(const size_t bufsize, const char *str1,
                       const char *str2, const char *str3)
{
    char *buf = (char *) malloc(bufsize);
    snprintf(buf, bufsize, "%s%s%s", str1, str2, str3);

    return buf;
}

char *read_whole_str(const char *filename)
{
    FILE *fp;
    long lsize;
    char *buffer;

    fp = fopen(filename , "r");
    if(!fp) {
        perror(filename);
        exit(1);
    }

    fseek(fp, 0L, SEEK_END);
    lsize = ftell(fp);
    rewind(fp);

    /* allocate memory for entire content */
    buffer = calloc(1, lsize+1);
    if(!buffer) {
        fclose(fp);
        fputs("memory alloc fails",stderr);
        exit(1);
    }

    /* copy the file into the buffer */
    if(1 != fread(buffer, lsize, 1, fp)) {
        fclose(fp);
        free(buffer);
        fputs("entire read fails", stderr);
        exit(1);
    }

    fclose(fp);
    buffer = make_str_clean(buffer);
    return buffer;
}

void write_str_to_file(const char *filename, const char *out_str)
{
    FILE *fp;

    fp = fopen(filename, "w");
    if (fp == NULL) {
        perror(filename);
        exit(1);
    }

    fputs(out_str, fp);
    printf("Write to file: %s\n", filename);

    fclose(fp);
    exit(EXIT_SUCCESS);
}


// =======================================================
// NodeFinder functions
// =======================================================

void get_insertion_list(const char *treestr, int *insertion_list, int *list_num, int indexnow)
{
    size_t treelen = strlen(treestr);
    int count;
    int i;

    count = 0;
    *list_num = 0;

    for (i=indexnow; i<treelen; i++) {
        if (treestr[i] == '(') {
            count++;
        } else if (treestr[i] == ')') {
            if (count == 0) {
                insertion_list[(*list_num)++] = i + 1;
            } else {
                count--;
            }
        }
    }
}

int get_index_of_tmrca(const char *treestr, const char *name_a, const char *name_b)
{
    int insertion_list_a[MAX_INDEX_LIST_NUM], insertion_list_b[MAX_INDEX_LIST_NUM];
    int list_num_a, list_num_b;
    size_t name_a_index, name_b_index;
    int shorter_list_num;
    int i;
    int index_of_tmrca;

    index_of_tmrca = -1;

    name_a_index = get_index_of_substring(treestr, name_a);
    name_b_index = get_index_of_substring(treestr, name_b);
//    printf("index a: %d, index b: %d\n", name_a_index, name_b_index);

    // get insertion list for each of the two species
    get_insertion_list(treestr, insertion_list_a, &list_num_a, name_a_index);
    get_insertion_list(treestr, insertion_list_b, &list_num_b, name_b_index);

    // reverse array for finding the first common value in arrays
    reverse_array(insertion_list_a, 0, list_num_a-1);
    reverse_array(insertion_list_b, 0, list_num_b-1);

    printf("---------------------------------\n");
    print_array(insertion_list_a, list_num_a);
    print_array(insertion_list_b, list_num_b);
    printf("---------------------------------\n");

    // find longer insertiton list
    shorter_list_num = list_num_a < list_num_b ? \
        list_num_a : list_num_b;

    // find last common value in two reversed arrays
    // [69, 37, 25, 14, 3]
    //          |
    // [69, 37, 25, 12]
    for (i=0; i<shorter_list_num; i++) {
        if (i == shorter_list_num - 1 && insertion_list_a[i] == insertion_list_b[i]) {
            index_of_tmrca = insertion_list_a[i];
            break;
        }
        // printf("%d | %d\n", insertion_list_a[i], insertion_list_b[i]);
        if (insertion_list_a[i] != insertion_list_b[i]) {
            index_of_tmrca = insertion_list_a[i-1];
            break;
        }
    }

    printf("[Common]:  %d\n", index_of_tmrca);
    printf("\n[Insert]:  %s%s\n",
        sliced_string(treestr, index_of_tmrca-20, index_of_tmrca),
        sliced_string(treestr, index_of_tmrca, index_of_tmrca+20));
    printf("[Insert]:                   ->||<-                  \n");
    printf("[Insert]:                 Insert Here               \n");

    // printf("Index of tMRCA: %d\n", index_of_tmrca);
    return index_of_tmrca;
}

char *get_exists_cali(const char *right_part)
{
    char *cali_pointer;
    char *exists_cali_info;
    int cali_len;
    int in_cali_flag;

    cali_pointer = strdup(right_part);
    exists_cali_info = strdup(right_part);
    cali_len = 0;
    in_cali_flag = 1;

    while (*cali_pointer != '\0') {
        if (in_cali_flag == 0)
            break;
        switch(*cali_pointer) {
            case ',': case ')': case ';':
                in_cali_flag = 0;
                break;
            default:
                cali_len++;
        }
        cali_pointer++;
    }
    *(exists_cali_info + cali_len) = '\0';

    return exists_cali_info;
}

char *find_non_cali_pointer(const char *right_part)
{
    char *non_cali_pointer;

    non_cali_pointer = strdup(right_part);

    while (*non_cali_pointer != '\0') {
        switch (*non_cali_pointer) {
            case ',': case ')': case ';':
                return non_cali_pointer;
            default:
                non_cali_pointer++;
                break;
        }
    }
    return NULL;
}

char *add_cali(const char *treestr, int index_of_tmrca, const char *cali_info)
{
    char *left_part, *right_part;
    size_t treelen = strlen(treestr);
    // Use some dirty code. Assumes that length of tree with calibration is no
    // longer than 2 times of tree without calibration
    size_t buflen = MULTIPLE_OF_BUFSIZE * treelen;

    left_part = sliced_string(treestr, 0, index_of_tmrca);
    right_part = sliced_string(treestr, index_of_tmrca, treelen);

    return concat_three_str(buflen, left_part, cali_info, right_part);
}

char *modify_exists_cali(const char *treestr, int index_of_tmrca, const char *cali_info)
{
    char *left_part, *right_part;
    size_t treelen = strlen(treestr);
    // Use some dirty code. Assumes that length of tree with calibration is no
    // longer than 2 times of tree without calibration
    size_t buflen = MULTIPLE_OF_BUFSIZE *treelen;

    left_part = sliced_string(treestr, 0, index_of_tmrca);
    right_part = sliced_string(treestr, index_of_tmrca, treelen);

    // Remove exists calibration infomation
    printf("[Label Exists]:          %s  [- Old]\n", get_exists_cali(right_part));
    printf("[Label Replaced By]:     %s  [+ New]\n", cali_info);
    right_part = find_non_cali_pointer(right_part);

    return concat_three_str(buflen, left_part, cali_info, right_part);
}

char *single_cali(char *treestr, struct Calibration *cali)
{
    int index_of_tmrca;
    char insertion_char;

    printf("\n\n===============================================================================\n");
    printf("%s, %s, %s\n", cali->name_a, cali->name_b, cali->cali_info);
    printf("===============================================================================\n");
    index_of_tmrca = get_index_of_tmrca(treestr, cali->name_a, cali->name_b);
    insertion_char = treestr[index_of_tmrca];

    switch (insertion_char) {
        case ',':
        case ';':
        case ')':
            // no calibration at this index before
            treestr = add_cali(treestr, index_of_tmrca, cali->cali_info);
            break;
        case '>': case '<': case '@':
        case '0': case '1': case 39:  // 39 means '\''
        case '"': case '$': case ':':
            // there is already calibration at this index
            treestr = modify_exists_cali(treestr, index_of_tmrca, cali->cali_info);
            break;
    }

    return treestr;
}

char *multi_cali(char *clean_str, int cali_num, struct Calibration *calis[cali_num]) {
    int i;
    struct Calibration *cali_now;

    for (i=0; i<cali_num; i++) {
        cali_now = calis[i];
        // printf("%s, %s, %s\n", cali_now->name_a, cali_now->name_b, cali_now->cali_info);
        clean_str = single_cali(clean_str, cali_now);
    }
    return clean_str;
}

char *read_config_file(const char *config_file_name, size_t *line_num)
{
    char *config_content;

    config_content = read_whole_str(config_file_name);

    // strip leading and traling ; character
    config_content = stripchar(config_content, ';');

    // get config line number according to count of ; character
    *line_num = countchar(config_content, ';') + 1;

    return config_content;
}

void parse_config(const char *config_content, size_t line_num, struct Calibration *calis[line_num])
{
    int i;
    char **config_lines;
    char **elements;
    size_t line_num_useless;
    size_t elements_num_per_line;

    // // get all config lines by split config_content by ;
    config_lines = split_by_delim(config_content, ";", &line_num_useless);

    for (i=0; i<line_num; i++) {
        // elements: {"name_a", "name_b", "cali_info"}
        elements = split_by_delim(config_lines[i], ",", &elements_num_per_line);
        if (elements_num_per_line != 3) {
            printf("Config file syntax error. Line: %d\n", i+1);
            exit(1);
        }

        calis[i] = malloc(sizeof(struct Calibration));
        calis[i]->name_a = elements[0];
        calis[i]->name_b = elements[1];
        calis[i]->cali_info = elements[2];
    }
}


void test_main(void)
{
    // =======================================================
    // Test make tree clean
    // =======================================================
    // char *test = "the fox jump\tover\n";
    // printf("%s\n", make_str_clean(test));
    // =======================================================


    // =======================================================
    // Initialization
    // =======================================================
    // int i;
    char *newicktree = "((((human,(chimpanzee,bonobo)),gorilla),(borneo,sumatran)),gibbon);";
    // char *name_a = "human";
    // char *name_b = "sumatran";
    // char *name_c = "gibbon";
    // =======================================================


    // =======================================================
    // Get insertion_list
    // =======================================================
    // int insertion_list[MAX_INDEX_LIST_NUM];
    // int insertion_list_num;

    // get_insertion_list(newicktree, insertion_list, insertion_list_num, 5);

    // for (i=0; i<insertion_list_num; i++) {
    //     printf("%d\n", insertion_list[i]);
    // }
    // =======================================================


    // =======================================================
    // Test get index of the most recent ancestor
    // =======================================================
    // get_index_of_tmrca(newicktree, "human", "sumatran");

    //            111111111122222222223333333333444444444455555555556666666
    //  0123456789012345678901234567890123456789012345678901234567890123456
    //             |
    // "((((human,(chimpanzee,bonobo)),gorilla),(borneo,sumatran)),gibbon);"

    // get_insertion_list(newicktree, list, &list_num, aa);
    // get_insertion_list(newicktree, list, &list_num, bb);
    // get_index_of_tmrca(newicktree, "chimpanzee", "bonobo");
    // =======================================================


    // =======================================================
    // Test slice string
    // =======================================================
    // printf("Sliced: %s\n", sliced_string("abcdefghij", 3, 5));
    // =======================================================


    // =======================================================
    // Test single calibration
    // =======================================================
    // printf("%s\n", add_cali(newicktree, 7, ">0.13<0.16"));
    // printf("%s\n", single_cali(newicktree, name_a, name_b, ">0.13<0.16"));
    // printf("%s\n", single_cali(newicktree, name_a, name_c, ">0.29"));

    // struct Calibration *cali1;
    // cali1 = malloc(sizeof(struct Calibration));
    // cali1->name_a = "human";
    // cali1->name_b = "bonobo";
    // cali1->cali_info = ">0.1";
    // printf("%s, %s, %s\n", cali1->name_a, cali1->name_b, cali1->cali_info);
    // =======================================================


    // =======================================================
    // Test multiple calibrations
    // =======================================================
    // struct Calibration *calibrations[] = {
    //     &(struct Calibration) {"human", "bonobo", ">0.1<0.2"},
    //     &(struct Calibration) {"human", "sumatran", ">0.4<0.5"},
    // };

    // printf("%s\n", multi_cali(newicktree, 2, calibrations));
    // =======================================================


    // =======================================================
    // remove exists calibration information
    // =======================================================
    // printf("%s\n", find_non_cali_pointer(">0.12<0.24),gorilla),(borneo,sumatran)),gibbon);"));
    // printf("%s\n", find_non_cali_pointer(">0.12<0.24,(borneo,sumatran)),gibbon);"));
    // printf("%s\n", find_non_cali_pointer(">0.12<0.24;"));

    // char *right_part;
    // right_part = ">0.12<0.24,(borneo,sumatran)),gibbon);";
    // printf("%s\n", get_exists_cali(right_part));
    // printf("%s\n", find_non_cali_pointer(right_part));

    struct Calibration *calibrations[] = {
        &(struct Calibration) {"human", "bonobo", ">0.1<0.2"},
        &(struct Calibration) {"human", "bonobo", ">0.2<0.3"},
        &(struct Calibration) {"human", "sumatran", ">0.4<0.5"},
    };
    // printf("%s\n", multi_cali(newicktree, 3, calibrations));
    // =======================================================


    // =======================================================
    // Test count char from string
    // =======================================================
    // printf("%d\n", countchar("the.quick.fox.jumped..over", '.'));
    // =======================================================


    // =======================================================
    // Test read whole string from file
    // =======================================================
    // printf("%s\n", read_whole_str("test.txt"));
    // =======================================================


    // =======================================================
    // Test read config file
    // =======================================================
    // char *config_content;
    // size_t line_num;
    // config_content = read_config_file("config.txt", &line_num);
    // // printf("%s\n", config_content);
    // // printf("%d\n", line_num);

    // struct Calibration *calis[line_num];
    // parse_config(config_content, line_num, calis);
    // =======================================================


    // =======================================================
    // Test print calibration struct
    // =======================================================
    // print_cali_struct(3, calibrations);
    // =======================================================

}


int main(void)
{
    int i;
    char *newicktree = "((((human,(chimpanzee,bonobo)),gorilla),(borneo,sumatran)),gibbon);";
    // test_main();
    char *clean_str, *config_content;
    size_t line_num;

    clean_str = read_whole_str("data/tree.nwk");
    config_content = read_config_file("data/config.txt", &line_num);

    /* Configuration parser */
    struct Calibration *calis[line_num];
    parse_config(config_content, line_num, calis);

    /* Do calibrations */
    clean_str = multi_cali(clean_str, line_num, calis);

//    printf("%s\n", clean_str);
//    write_str_to_file("_out_beetle.nwk", clean_str);

    return 0;
}