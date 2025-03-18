#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <arpa/inet.h> 

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "syntax : %s <file1> [<file2>...]\n", argv[0]);
        return 1;
    }

    int num_files = argc - 1;
    uint32_t total_sum = 0;
    uint32_t *file_sums = malloc(num_files * sizeof(uint32_t));
    if (file_sums == NULL) {
        perror("malloc");
        return 1;
    }

    for (int i = 1; i < argc; i++) {
        FILE *fp = fopen(argv[i], "rb");
        if (fp == NULL) {
            fprintf(stderr, "Error: cannot open file %s\n", argv[i]);
            free(file_sums);
            return 1;
        }
        

        fseek(fp, 0, SEEK_END);
        long file_size = ftell(fp);
        rewind(fp);
        if (file_size < 4 || (file_size % 4) != 0) {
            fprintf(stderr, "Error: file %s size is invalid (not multiple of 4 bytes)\n", argv[i]);
            fclose(fp);
            free(file_sums);
            return 1;
        }
        
        uint32_t file_sum = 0;
        uint32_t number;
        while (fread(&number, sizeof(uint32_t), 1, fp) == 1) {

            file_sum += ntohl(number);
        }
        if (ferror(fp)) {
            fprintf(stderr, "Error: reading file %s\n", argv[i]);
            fclose(fp);
            free(file_sums);
            return 1;
        }
        fclose(fp);
        file_sums[i - 1] = file_sum;
        total_sum += file_sum;
    }

    uint32_t max_val = total_sum;
    for (int i = 0; i < num_files; i++) {
        if (file_sums[i] > max_val)
            max_val = file_sums[i];
    }
    int digits = 0;
    uint32_t tmp = max_val;
    do {
        digits++;
        tmp /= 16;
    } while(tmp > 0);
    if (digits < 4)
        digits = 4;
    int field_width = digits + 2;  
    
    for (int i = 0; i < num_files; i++) {
        if (i > 0)
            printf(" + ");
        printf("%u(%#0*x)", file_sums[i], field_width, file_sums[i]);
    }
    printf(" = %u(%#0*x)\n", total_sum, field_width, total_sum);
    
    free(file_sums);
    return 0;
}

