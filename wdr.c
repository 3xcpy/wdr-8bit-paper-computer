#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

typedef union op_t
{
    struct
    {
        uint8_t v : 5;
        uint8_t c : 3;
    };
    uint8_t opcode;
} op_t;

void execute_wdr(op_t ops[32])
{
    uint8_t r[32] = {0};
    int pc = 0;
    while(pc < 32)
    {
        op_t op = ops[pc];
        uint8_t oc = op.c;
        uint8_t val = op.v;
        switch(oc)
        {
            case 0:
                printf("halt\n");
                printf("Register contents:\n");
                for (int i = 0; i < 32; i++)
                {
                    printf("%i: %u\n", i, r[i]);
                }
                exit(EXIT_SUCCESS);
                break;
            case 1:
                printf("skip if %u is 0\n", val);
                if(r[val] == 0) {pc++;}
                break;
            case 2:
                printf("jump to %u\n", val);
                pc = val - 1;
                break;
            case 3:
                printf("inc %u\n", val);
                r[val]++;
                break;
            case 4:
                printf("dec %u\n", val);
                r[val]--;
                break;
            default:
                fprintf(stderr, "Unknown instruction %u with the value %u encountered\n", oc, val);
                break;
        }
        pc++;
    }
}

int main(int argc, char** argv)
{
    FILE* input = fopen(argv[1], "r");
    if(input == NULL) 
    {
        perror("ERROR: Failed to open input file");
        return EXIT_FAILURE;
    }

    uint8_t prog[32] = {0};
    uint8_t rc = 0;
    char c;

    while((c=getc_unlocked(input)) != EOF && rc < 32)
    {
        prog[rc] = (uint8_t)c;
        rc++;
    }

    execute_wdr((op_t*)prog);

    return 0;
}
