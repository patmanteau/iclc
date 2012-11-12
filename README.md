iclc - Interactive Command Line Calculator
==========================================

iclc is a simple, interactive, command line calculator. It employs basic parsing 
technique. Beware, though: It's not bugfree and it leaks memory all over the plane.

iclc is written in plain C99 and should compile on most systems with only minor
adjustments. C99 support is required, though. 

Copyright 2012 Patrick Haas (@p7haas)

Released under the MIT and GPL licenses.

Manual
------

iclc understands basic arithmetic on (signed) floating-point numbers. Supported operators are:

+, -, *, /, %, ^

( and ) are fine, too. Sometimes, they even behave as expected!

Some basic functions are also supported:

(a)sin(h), (a)cos(h), (a)tan(h), log(10), ceil, fabs, floor

iclc runs until it has devoured all your system's memory or until you quit by entering \quit
or your favorite flavor of EOF. It reads from stdin, so pipe'ing works, too.



