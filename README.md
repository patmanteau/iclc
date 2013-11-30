iclc - Interactive Command Line Calculator
==========================================

iclc is a simple, interactive, command line calculator. It employs basic parsing 
technique. Beware, though: It has bugs and it leaks. Bad dog.

iclc is written in plain C99 and should compile on most systems with only minor
adjustments. C99 support is required, though. 

Copyright 2012 Patrick Haas (@p7haas)

Released under the MIT and GPL licenses.

Manual
------

iclc understands basic arithmetic on (signed) floating-point numbers. Supported operators are:

+, -, *, /, %, ^, = (i.e. assignment)

( and ) are fine, too. Sometimes, they even behave as expected!

Some basic functions are also supported:

(a)sin(h), (a)cos(h), (a)tan(h), log(10), ceil, fabs, floor, sqrt

There are three preassigned variables: Pi, E and Fibo.

iclc runs until it has devoured all your system's memory or until you quit by entering \quit
or your favorite flavor of EOF. It reads from stdin, so pipe'ing works, too.
