/* nullNvRam.c - dummy NVRAM access */

int sysNvLineSize(char string[]) { return 1; }
int sysNvRamCheck() { return FALSE; }
unsigned char sysNvRamCheckSum(void) { return 0; }
STATUS sysNvRamGet(char* string, int maxLen, int offset) { return -1; }
STATUS sysNvRamSet(char* string, int maxLen, int offset) { return -1; }
