#include <stdio.h>
#include <stdlib.h>
#include <string.h>


// We use structs to efficiently read the textual, extended,
// and binary headers as well as trace headers and traces.
//
// We currently will only support rev2 version segy files,
// but it shouldn't be too hard to implement rev1 and older
// formats as long as they are somewhat consistent.
//
// Additionally, specific byte locations for trace header
// data should be able to be specified by the user because
// it is very common to see non-standard bytes being used
// for standard data.


static const unsigned char E2A[256] = {
    0,  1,  2,  3,  156,9,  134,127,151,141,142, 11,12, 13, 14, 15,
    16, 17, 18, 19, 157,133,8,  135,24, 25, 146,143,28, 29, 30, 31,
    128,129,130,131,132,10, 23, 27, 136,137,138,139,140,5,  6,  7,
    144,145,22, 147,148,149,150,4,  152,153,154,155,20, 21, 158,26,
    32, 160,161,162,163,164,165,166,167,168,91, 46, 60, 40, 43, 33,
    38, 169,170,171,172,173,174,175,176,177,93, 36, 42, 41, 59, 94,
    45, 47, 178,179,180,181,182,183,184,185,124,44, 37, 95, 62, 63,
    186,187,188,189,190,191,192,193,194,96, 58, 35, 64, 39, 61, 34,
    195,97, 98, 99, 100,101,102,103,104,105,196,197,198,199,200,201,
    202,106,107,108,109,110,111,112,113,114,203,204,205,206,207,208,
    209,126,115,116,117,118,119,120,121,122,210,211,212,213,214,215,
    216,217,218,219,220,221,222,223,224,225,226,227,228,229,230,231,
    123,65, 66, 67, 68, 69, 70, 71, 72, 73, 232,233,234,235,236,237,
    125,74, 75, 76, 77, 78, 79, 80, 81, 82, 238,239,240,241,242,243,
    92, 159,83, 84, 85, 86, 87, 88, 89, 90, 244,245,246,247,248,249,
    48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 250,251,252,253,254,255
};


// Temporary buffer for ascii output from converted formats
// such as ebcdic
char ASCII_BUFFER[3200];


void encode(char* to, const char* from, const unsigned char* conv, size_t n) {
    for (size_t i=0; i<n; i++) {
        to[i] = (char)conv[(unsigned char)from[i]];
    }
}


void ebcdic2ascii(const char* ebcdic, char* ascii) {
    size_t len = strlen(ebcdic);
    encode(ascii, ebcdic, E2A, len);
    ascii[len] = '\0';
}


typedef struct TextualFileHeader {
    char buffer[3200];
} TextualFileHeader;

typedef struct BinaryFileHeader {
    char buffer[400];
} BinaryFileHeader;

struct TraceHeader {
} TraceHeader;

struct TraceData {
} TraceData;


int main(int argc, char *argv[]) {

    char filename[] = "npr3_field.sgy";
    FILE *fptr;
    int fileSize;
    int result;

    // Open file
    fptr = fopen(filename, "rb");
    if (fptr == NULL) {
        // Error reading file
        printf("Error opening file %s \n", filename);
        exit(1);
    }
    printf("Success opening segy file %s \n", filename);

    // Get size of file
    fseek(fptr, 0L, SEEK_END);
    fileSize = ftell(fptr);
    rewind(fptr);
    printf("File is of size %f Mb \n", sizeof(char)*fileSize/1e6);

    // First we want to read the textual file header
    TextualFileHeader tfh;
    result = fread(&tfh, 1, sizeof(TextualFileHeader), fptr);
    if (result != sizeof(TextualFileHeader)) {
        printf("Error reading file %s \n", filename);
    }
    printf("Success reading file %s \n", filename);
    printf("Textual File Header is as follows:\n");
    ebcdic2ascii(tfh.buffer, ASCII_BUFFER);
    printf("%s", ASCII_BUFFER);

    // Close file
    fclose(fptr);

    return 0;
}
