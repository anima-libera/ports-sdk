
# Ports version 2.0 documentation

## Syntax

### Char set

Source code char set is `0123456789-,&{}[]#`, except in a comment or in
brackets where it is the printable ascii chars.

### Comments

Comments are blocks started and ended by `#`.

### Whitespace

Whitespace is a name token separator, and is ignored if found anywhere else.

### Code syntax

See instructions. Every number is a name can could be replaced by an other
number. Leading zeros are ignored.
Code is a sequence of instructions uwu.

## Instructions

### Port instruction

Syntax: `8`
This instruction act as a port to its own position in the code it is in.

### Link instruction

Syntax: `8-4`
This instruction consists in cutting any link involving 8 or 4 and then linking
them together.

### Create port instruction

Syntax: `4,2,8`
Create a port named 4 on the surface of 8, the other end is 2.

### Create space instruction

Syntax 1: `4,2,&`
Syntax 2: `4,2,{...}` where `...` is some code.
Syntax 3: `4,2,[filename]` where `filename` is a Ports file name.
Create a new space whose surface is visible, and create a port on it named 4,
the other end is 2 and is linked to the first port of the code of the new space
that is `...` or is a copy of the code of the current space if it is `&` or is
the code contained in the `filename` Ports source file if it is `[filename]`.

## Special ports

Except for the initial special port 0 through which the spark enters the root
space at the beginning of execution, every other special port must be created
from the root space in 0's surface in order to be used. What identifies a
special port is the name of its other side, the name it has in the root space
doesn't matter.

### 0 

It is the equivalent of origin port (o in the version 1). Initially, the spark
comes from 0, and the only way to halt execution without an error is for the
spark to go through 0.

### 1

1 and 0 are the only special ports that are ...
wait no

o boi
that needs some refactoring

### 10,11,12,13,14,15,16,17

These are for reading/modifying the bits of the intermediate byte (the one
that can be read from stdin or sent to stdout). See other special ports for
usage.

### 18

This allows output. When the spark goes in, the intermediate byte is set to
zero and output is enabled, then the spark comes out from where it went.

### 19

This performs output. When the spark goes in, the intermediate byte is sent to
stdout and set to zero, then the spark comes out from where it went.
If output was not enabled, an error is raised.

# THE FOLLOWING NEED UPDATE

### 11

This allows input. When the spark goes in, the intermediate byte is set to zero
and input is enabled, then the spark comes out from where it went.

### 12

This performs input.

### 13

Next byte of input.

### 14

This allows for ?. (something basic-io related and simple)

### 15

This allows for ?. (something basic-io related and simple)



# IDEAS

> actually `8-8` to cut 8 might be a good idea. consider it.
actually NO, do `99,0,{0}` and then `8-99` to cut 8. this is better !!!!
