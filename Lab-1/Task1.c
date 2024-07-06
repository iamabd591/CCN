#include <stdio.h>

int main() {
    unsigned int num = 0x12345678;
    unsigned char *ptr = (unsigned char *)&num;

    printf("Number 0x%08x is stored in memory as:\n", num);

    if (*ptr == 0x78) {
        printf("Little Endian\n");
    } else if (*ptr == 0x12) {
        printf("Big Endian\n");
    } else {
        printf("Unknown Endian\n");
    }

    printf("Byte order: ");
    for (int i = 0; i < sizeof(num); i++) {
        printf("%02x ", *(ptr + i));
    }
    printf("\n");

    return 0;
}
