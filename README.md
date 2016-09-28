# Embedded-Tool-Suite
A Collection of CLI (Command Line Interface) Tools created to help write embedded software. Currently includes:
- binconv (a binary converter)
- genreg (a bit mask generator)

###binconv
Displays the hexadecimal, decimal, and binary values of an input. It contains some specialized input options for binary numbers. Rather than inputting a raw binary number, a list of the position of '1' bits can be input. For example, an input of 3,5 will be interpreted as 101000. Alternatively, a range can be used as an input; in this case, an input of 3,5 would be interpreted as 111000. 
