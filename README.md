# Embedded-Tool-Suite
A Collection of CLI (Command Line Interface) Tools created to help write embedded software. Currently includes:
- binconv (a binary converter)
- genreg (a bit mask generator)

##binconv
A binary converter with some specialized input options for binary numbers. Rather than inputting a raw binary number, a list of the position of '1' bits can be inputtec. For example, an input of 3,5 will be interpreted as 101000. Alternatively, a range can be used as an input; in this case, an input of 3,5 would be interpreted as 111000. 

Here's the output of the following command: binconv -d 32 -bp 11,13,14 -br 44,46 -h 2B
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
hex:                         0x20    dec:                          32 |
bin:                                 00000000000000000000000000100000 |
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
hex:                       0x6800    dec:                       26624 |
bin:                                 00000000000000000110100000000000 |
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
hex:               0x700000000000    dec:             123145302310912 |
bin: 0000000000000000011100000000000000000000000000000000000000000000 |
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
hex:                         0x2B    dec:                          43 |
bin:                                 00000000000000000000000000101011 |
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

##genreg
Generates a header file for C and C++ containing macros for bit masks and value offsets. Inputs specify the output file, register length, and finally the maximum size of a bit region to generate masks and value offsets for. For example, with a maximum region length of 3 with a register size of 8, the header will contain masks for 00000001, 00000010, ..., 10000000, 00000011, 00000110, ..., 11100000. Additionally, it will define macros for the numbers 1-7 offset by up to 7 bits. Of course,
all values are defined in hexadecimal. An example output file can be found in masks.h in the genreg directory.
