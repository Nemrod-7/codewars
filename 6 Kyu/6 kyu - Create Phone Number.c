char *create_phone_number(char phnum[15], const unsigned char nums[10]) {

phnum[0] = '(';
phnum[1] = nums[0] + '0';
phnum[2] = nums[1] + '0';
phnum[3] = nums[2] + '0';
phnum[4] = ')';
phnum[5] = ' ';
phnum[6] = nums[3] + '0';
phnum[7] = nums[4] + '0';
phnum[8] = nums[5] + '0';
phnum[9] = '-';
phnum[10] = nums[6] + '0';
phnum[11] = nums[7] + '0';
phnum[12] = nums[8] + '0';
phnum[13] = nums[9] + '0';

    return phnum;
}
