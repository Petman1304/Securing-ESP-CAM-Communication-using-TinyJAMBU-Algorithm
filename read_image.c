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

// Example usage
// int main() {
//     char* imagePath = "example.png";
//     char* imageString = imageToString(imagePath);
//     printf("%s", imageString);
//     free(imageString); // Free memory used by output string
//     return 0;
// }
