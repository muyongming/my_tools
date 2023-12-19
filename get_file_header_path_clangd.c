#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define FILE_NAME_MAX_LEN			256
#define PATH_LINE_MAX_LEN			1024
#define PATH_DIR_HISTORY_NUM		30

#define HEAD_LINE                   ("\"clangd.fallbackFlags\" : [ \n    \"-D\",\n")
#define END_LINE                    ("    ],\n")

void printf_editer_info(void)
{
    printf("Anther: yongming.mu\n");
    printf("Time  : %s %s\n", __DATE__, __TIME__);
}

void printf_usage(void *argv)
{
    printf("Usage: %s [input.txt] \n", (char *)argv);
}

// 从路径中获取文件夹路径
void get_dir_path(char *path, char *dir_path)
{
    int i = 0;
    int j = 0;
    while (path[i] != '\0')
    {
        if (path[i] == '/')
        {
            j = i + 1;
        }
        i++;
    }
    for (int k = 0; k < j; k++)
    {
        dir_path[k] = path[k];
    }
    dir_path[j] = '\0';

}

// 删除换行符号
void delete_cr_lf(char *str)
{
    int i = 0;
    while (str[i] != '\0')
    {
        if (str[i] == '\r' || str[i] == '\n')
        {
            str[i] = '\0';
            break;
        }
        i++;
    }
}


int main(int argc, char *argv[])
{
    FILE *fp_input = NULL;
    FILE *fp_output = NULL;

    int ret = 0;

    int idx;
    int slen;
    char output_file_name[FILE_NAME_MAX_LEN];
    char path_one_line[PATH_LINE_MAX_LEN] = {0};
    char path_dir_now[PATH_LINE_MAX_LEN] = {0};
    char path_dir_history[PATH_DIR_HISTORY_NUM][PATH_LINE_MAX_LEN] = {0};
    char output_string[PATH_LINE_MAX_LEN] = {0};

    printf_editer_info();

    if (argc < 2)
    {
        printf_usage(argv[0]);
        return -1;
    }

    do
    {
        fp_input = fopen(argv[1], "r");
        if (!fp_input)
        {
            printf("Error opening file %s\n", argv[1]);
            ret = -2;
            break;
        }

        snprintf(output_file_name, sizeof(output_file_name), "%s_out.txt", argv[1]);
        fp_output = fopen(output_file_name, "w+");
        if (!fp_output)
        {
            printf("Error opening file %s\n", output_file_name);
            ret = -3;
            break;
        }
        
        fwrite(HEAD_LINE, 1, strlen(HEAD_LINE), fp_output);

        // 读取每一行数据
        while (!feof(fp_input))
        {
            fgets(path_one_line, sizeof(path_one_line), fp_input);
            // 删除换行符号
            delete_cr_lf(path_one_line);

            get_dir_path(path_one_line, path_dir_now);
            // printf(" %s  --->  %s\n", path_one_line, path_dir_now);

            slen = strlen(path_dir_now);
            for (idx = 0; idx < PATH_DIR_HISTORY_NUM; idx++)
            {
                if (slen == strlen(path_dir_history[idx]) &&
                        (!strncmp(path_dir_now, path_dir_history[idx], slen))
                   )    // 比较上一个/上上个/上上上个...  有相同则退出
                {
                    break;
                }
            }
            if (idx != PATH_DIR_HISTORY_NUM)   	// 不等说明有相同, 则不用写到文件里
            {
                continue;
            }

            // printf("%s\n", path_dir_now);
            // fwrite(path_dir_now, 1, strlen(path_dir_now), fp_output);
            // fwrite("\n", 1, 1, fp_output);

            snprintf(output_string, sizeof(output_string), "    \"-I${workspaceFolder}/%s\", \n", path_dir_now);
            fwrite(output_string, 1, strlen(output_string), fp_output);

            for (idx = PATH_DIR_HISTORY_NUM - 1; idx > 0; idx--)
            {
                snprintf(path_dir_history[idx], sizeof(path_dir_history[0]), "%s", path_dir_history[idx - 1]);
            }

            snprintf(path_dir_history[0], sizeof(path_dir_history[0]), "%s", path_dir_now);
        }
        
        fwrite(END_LINE, 1, strlen(END_LINE), fp_output);

    } while (0);

    if (fp_input)
    {
        fclose(fp_input);
    }

    if (fp_output)
    {
        fclose(fp_output);
    }

    printf("\n============================================\n");
    printf(">\n");
    printf(">> Process finished\n");
    printf(">> Input : %s\n", argv[1]);
    printf(">> Output: %s\n", output_file_name);
    printf(">\n");
    printf("============================================\n\n");

    return 0;
}
