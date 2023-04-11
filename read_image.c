#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>

// Function to read image binary data and return as a string
unsigned char* imageToString(char* imagePath) {
    FILE *imageFile;
    char *buffer;
    long imageSize;
    size_t result;

    // Open image file in binary mode
    imageFile = fopen(imagePath, "rb");
    if (imageFile == NULL) {
        fputs("File error", stderr);
        exit(1);
    }

    // Get image file size
    fseek(imageFile, 0, SEEK_END);
    imageSize = ftell(imageFile);
    rewind(imageFile);

    // Allocate memory for buffer to hold image binary data
    buffer = (char*) malloc(sizeof(char) * imageSize);
    if (buffer == NULL) {
        fputs("Memory error", stderr);
        exit(2);
    }

    // Read image binary data into buffer
    result = fread(buffer, 1, imageSize, imageFile);
    if (result != imageSize) {
        fputs("Reading error", stderr);
        exit(3);
    }

    // Close image file
    fclose(imageFile);

    // Encode binary data to string using Base64 encoding
    size_t outputSize = 4 * ((imageSize + 2) / 3); // Calculate size of output string
    char* output = (char*) malloc(outputSize + 1); // Allocate memory for output string
    memset(output, 0, outputSize + 1); // Initialize output string to 0s
    const char* base64Chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"; // Base64 character set
    for (int i = 0, j = 0; i < imageSize;) {
        uint32_t octet_a = i < imageSize ? (unsigned char)buffer[i++] : 0;
        uint32_t octet_b = i < imageSize ? (unsigned char)buffer[i++] : 0;
        uint32_t octet_c = i < imageSize ? (unsigned char)buffer[i++] : 0;

        uint32_t triple = (octet_a << 0x10) + (octet_b << 0x08) + octet_c;

        output[j++] = base64Chars[(triple >> 3 * 6) & 0x3F];
        output[j++] = base64Chars[(triple >> 2 * 6) & 0x3F];
        output[j++] = base64Chars[(triple >> 1 * 6) & 0x3F];
        output[j++] = base64Chars[(triple >> 0 * 6) & 0x3F];
    }

    // Add padding characters to output string
    for (int i = 0; i < (3 - imageSize % 3) % 3; i++) {
        output[outputSize - 1 - i] = '=';
    }

    // Return output string
    return output;
}

// Function to decode Base64 encoded string to binary data
unsigned char* base64Decode(const char* input, size_t inputLen, size_t* outputLen) {
    const char* base64Chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    size_t padding = 0;
    if (input[inputLen - 1] == '=') {
        padding++;
    }
    if (input[inputLen - 2] == '=') {
        padding++;
    }
    *outputLen = inputLen * 3 / 4 - padding;
    unsigned char* output = (unsigned char*) malloc(*outputLen);
    memset(output, 0, *outputLen);

    uint32_t sextet_a, sextet_b, sextet_c, sextet_d, triple;
    int i, j;
    for (i = 0, j = 0; i < inputLen;) {
        sextet_a = input[i] == '=' ? 0 & i++ : base64Chars[(unsigned char)input[i++]];
        sextet_b = input[i] == '=' ? 0 & i++ : base64Chars[(unsigned char)input[i++]];
        sextet_c = input[i] == '=' ? 0 & i++ : base64Chars[(unsigned char)input[i++]];
        sextet_d = input[i] == '=' ? 0 & i++ : base64Chars[(unsigned char)input[i++]];

        triple = (sextet_a << 3 * 6) + (sextet_b << 2 * 6) + (sextet_c << 1 * 6) + (sextet_d << 0 * 6);

        if (j < *outputLen) {
            output[j++] = (triple >> 2 * 8) & 0xFF;
        }
        if (j < *outputLen) {
            output[j++] = (triple >> 1 * 8) & 0xFF;
        }
        if (j < *outputLen) {
            output[j++] = (triple >> 0 * 8) & 0xFF;
        }
    }

    return output;
}

// Function to write binary data to image file
void writeImageFile(char* imagePath, unsigned char* imageData, size_t imageSize) {
    FILE *imageFile;

    // Open image file in binary write mode
    imageFile = fopen(imagePath, "wb");
    if (imageFile == NULL) {
        fputs("File error", stderr);
        exit(1);
    }

    // Write image binary data to file
    fwrite(imageData, 1, imageSize, imageFile);

    // Close image file
    fclose(imageFile);
}

// Function to decode string to image binary data and write to image file
void stringToImage(char* imagePath, char* imageString) {
    size_t inputSize = strlen(imageString);
    size_t outputSize;
    unsigned char* imageData = base64Decode(imageString, inputSize, &outputSize);
    writeImageFile(imagePath, imageData, outputSize);
    free(imageData); // Free memory used by image binary data
}

//Example usage
int main() {
    char* imagePath = "example.png";
    char* imageString = imageToString(imagePath);
    

    char* imagePathOut = "output.png";
    stringToImage(imagePathOut, imageString);
    //free(imageString); // Free memory used by output string
    return 0;
}
