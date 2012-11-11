iclc - Interactive Command Line Calculator
==========================================

iclc is a simple, interactive, command line calculator. It employs and thus
demonstrates basic parsing technique. Beware, it leaks like a strainer and
probably has more bugs than Starship Troopers. Also, it just wants to be
your dog.

iclc is written in plain C99 and should compile on most systems with only minor
adjustments. C99 support is required, though. 

Copyright 2012 Patrick Haas (@patmanteau)

Released under the MIT and GPL licenses.

Manual
------

iclc understands basic arithmetic on floating-point numbers. Supported operators are:

+, -, *, /, %, ^

iclc runs until tired or until you quit by entering \quit or your favorite flavor of EOF.

That's all.
