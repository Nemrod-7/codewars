#define BUFFSIZE 64

int* parse(char* program) {

    int num = 0, index = 0;
    int *result = malloc (BUFFSIZE * sizeof(int));
    char c;

    while ((c = *program++))
        switch (c) {
            case 'i' : num++; break;
            case 'd' : num--; break;
            case 's' : num *= num; break;
            case 'o' :  result[index++] = num; break;
            default  : break;
        }

    return result;

}
