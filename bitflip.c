/*
 *
 * Description:
 *   This program reads a 16-bit unsigned integer (1–20000) from the command
 *   line and flips its bits based on the flags provided:
 *     -e: flips even-positioned bits (bit positions 0, 2, 4, ...)
 *     -f: flips odd-positioned bits (bit positions 1, 3, 5, ...)
 *     -a: flips all bits
 *     -o filename: writes output to the specified file instead of stdout
 *   Bitwise XOR (^) is used with specific masks to perform these operations.
 *   Only bitwise logic is used—no arithmetic or string-based flipping.
 */

#include <stdio.h>  // For input/output functions
#include <stdlib.h> // For exit(), strtoul(), malloc
#include <stdint.h> // For fixed-width integer types like uint16_t
#include <unistd.h> // For getopt()
#include <ctype.h>  // For character tests like isdigit()

// Prints a usage message showing how the user should run the program.
void PrintUsage(const char *prog)
{
    fprintf(stderr,
            "Usage: %s [-e] [-f] [-a] [-o outputfile] intval\n"
            "  intval: integer between 1 and 20000 inclusive\n"
            "  -e: flip even bits (bit positions 0, 2, 4, ...)\n"
            "  -f: flip odd bits (bit positions 1, 3, 5, ...)\n"
            "  -a: flip all bits\n"
            "  -o file: write output to file instead of screen\n",
            prog);
}

int main(int argc, char **argv)
{
    int opt;
    int do_even = 0, do_odd = 0, do_all = 0;
    char *outfile_name = NULL;

    // Parse command-line options using getopt
    // -e, -f, -a: toggle which bitwise flip actions to perform
    // -o: followed by a filename to output results
    while ((opt = getopt(argc, argv, "efao:")) != -1)
    {
        switch (opt)
        {
        case 'e':
            do_even = 1;
            break; // Flip even bits
        case 'f':
            do_odd = 1;
            break; // Flip odd bits
        case 'a':
            do_all = 1;
            break; // Flip all bits
        case 'o':
            outfile_name = optarg;
            break; // Set output file
        default:
            PrintUsage(argv[0]);
            return 1;
        }
    }

    // Expect exactly one non-option argument remaining: the input integer
    if (optind != argc - 1)
    {
        fprintf(stderr, "Error: missing or too many arguments\n");
        PrintUsage(argv[0]);
        return 1;
    }

    // Convert string to unsigned long and validate range
    char *endptr;
    unsigned long val = strtoul(argv[optind], &endptr, 10);
    if (*endptr != '\0' || val < 1 || val > 20000)
    {
        fprintf(stderr, "Error: intval must be integer in [1, 20000]\n");
        return 1;
    }
    uint16_t original = (uint16_t)val; // Use 16-bit unsigned type

    // Determine where to output results (stdout or file)
    FILE *out = stdout;
    if (outfile_name)
    {
        out = fopen(outfile_name, "w");
        if (!out)
        {
            perror("Error opening output file");
            return 1;
        }
    }

    // Print the original value
    fprintf(out, "Value: %u\n", original);

    // Flip even bits using bitmask 0x5555 (binary: 0101010101010101)
    // ^ (XOR) flips bits where the mask is 1
    if (do_even)
    {
        uint16_t flipped = original ^ 0x5555;
        fprintf(out, "Even bits flipped: %u\n", flipped);
    }

    // Flip odd bits using bitmask 0xAAAA (binary: 1010101010101010)
    if (do_odd)
    {
        uint16_t flipped = original ^ 0xAAAA;
        fprintf(out, "Odd bits flipped: %u\n", flipped);
    }

    // Flip all bits using 0xFFFF (binary: 1111111111111111)
    if (do_all)
    {
        uint16_t flipped = original ^ 0xFFFF;
        fprintf(out, "All bits flipped: %u\n", flipped);
    }

    // Close output file if one was opened
    if (outfile_name)
    {
        fclose(out);
    }

    return 0;
}
