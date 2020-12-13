// Code by Justin Gewecke
// This code practices conversion between decimal, hexadecimal, and binary
// Additionally, this code also utilizes bitwise operators to change binary data


// References:
// https://beginnersbook.com/2017/09/c-program-to-convert-decimal-number-to-binary-number/
// https://codeforwin.org/2015/08/c-program-to-convert-binary-to-hexadecimal-number-system.html#:~:text=Binary%20to%20hexadecimal%20conversion%20table%20%20%20,%20%203%20%2012%20more%20rows%20

#include <stdio.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>

// New strrev function since string.h version is unavailable in linux' version
// Unable to explain since the post does not explain the process
// Obtained from here https://stackoverflow.com/questions/8534274/is-the-strrev-function-not-available-in-linux
char *strrev(char *str)
{
      char *p1, *p2;

      if (! str || ! *str)
            return str;
      for (p1 = str, p2 = str + strlen(str) - 1; p2 > p1; ++p1, --p2)
      {
            *p1 ^= *p2;
            *p2 ^= *p1;
            *p1 ^= *p2;
      }
      return str;
}

// Get binary from decimal value
char* GetBinary(int decimal) {

    // Our starting value
    static char bin32[] = "00000000000000000000000000000000";
    for (int pos = 31; pos >= 0; --pos)
    {
        bin32[pos] = '0'; // Need this here otherwise we will have the wrong values when revisiting this function

        // Formula for converting from decimal to binary
        if (decimal % 2) 
            bin32[pos] = '1';
        decimal /= 2;
    }

    char* str = NULL; 
    str = bin32;

    return str;
}

// Get hexadecimal from binary value
// Modified from https://codeforwin.org/2015/08/c-program-to-convert-binary-to-hexadecimal-number-system.html#:~:text=Binary%20to%20hexadecimal%20conversion%20table%20%20%20,%20%203%20%2012%20more%20rows%20
char* GetHexadecimal(char* value) {
    int hexConstant[] = {0, 1, 10, 11, 100, 101, 110, 111, 1000, 
                         1001, 1010, 1011, 1100, 1101, 1110, 1111};

    int binary = atoi(value);

    unsigned long tempBinary;

    /* We need to make this a pointer to return it */
    char* hex = malloc(sizeof(20));

    int index, i, digit;
    
    /* Copy binary number to temp variable */
    tempBinary = binary;
    index = 0;
    
    /* Find hexadecimal of binary number */
    while(tempBinary!=0)
    {
        /* Group binary to last four digits */
        digit = tempBinary % 10000;

        /* Find hexadecimal equivalent of last four digit */
        for(i=0; i<16; i++)
        {
            if(hexConstant[i] == digit)
            {
                if(i<10)
                {
                    /* 0-9 integer constant */
                    hex[index] = (char)(i + 48);
                }
                else
                {
                    /* A-F character constant */
                    hex[index] = (char)((i-10) + 65);
                }

                index++;
                break;
            }
        }

        /* Remove the last 4 digits as it is processed */
        tempBinary /= 10000;
    }
    hex[index] = '\0';

    /* Reverse the hex digits */
    strrev(hex);

    return hex;
}

// Gets our result when bitshifting
void GetBitShift(int decimal) {

    // char treats numbers as binary
    char* str = GetBinary(decimal<<16);

    // Shift the left
    printf("Shifted 16-bits to the left: %s\n", str);
    char* and_mask = "0000000000000000";

    // The equivalent of str | mask, works better then converting to
    for (int pos = 31; pos >= 15; --pos) {
        if (str[pos] != and_mask[pos]) {
            str[pos] = and_mask[pos];
        }
    }

    // Apply the AND mask
    printf("Applying the AND mask to the result: %s\n", str); // Display 'mask out' result

    // 11111111111 = 0x07FF, add this to our results
    char* or_mask = "11111111111";
    for (int pos = 15; pos >= 0; --pos) {
        if (str[pos] == '1' || or_mask[pos] == '1') {
            str[pos] = '1';
        }
    }

    // Apply the OR mask
    printf("Applying the OR mask to the result: %s\n", str); // Display 'mask out' result
}

int main() {
    int inputDecimal;

    printf("Please enter a decimal between 0 and 4095: ");
    scanf("%d", &inputDecimal);

    // Binary Stuff
    char* binary_char = GetBinary(inputDecimal);
    printf("Equivalent Binary Number is: %s\n", binary_char);

    // Hexadecimal Stuff
    char* hex_char = GetHexadecimal(binary_char);
    printf("Equivalent Hexadecimal is: %s\n", hex_char);

    // BitShift Stuff
    GetBitShift(inputDecimal);
    


    return 0;
}