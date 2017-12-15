# `libvc4mem`

`libvc4mem` is a memory and cache manipulating library for RPi2 and RPi3.


## Requirements

You need to run
[Idein:rpi-4.9.y-vc4mem](https://github.com/Idein/linux/tree/rpi-4.9.y-vc4mem)
kernel to use this library.  See
[the official documentation](https://www.raspberrypi.org/documentation/linux/kernel/building.md)
to build and install the kernel on your pi.


## Installation

```
$ autoreconf -i -m
$ ./configure
$ make
$ sudo make install
```
