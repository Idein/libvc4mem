# `libvc4mem`

`libvc4mem` is a memory and cache manipulating library for RPi2 and RPi3.


## Requirements

You need to run
[Idein/linux:rpi-4.9.y-vc4mem](https://github.com/Idein/linux/tree/rpi-4.9.y-vc4mem)
kernel to use this library.

To cross-compile the kernel, please use my Docker image and script:
[Terminus-IMRC/cross-rpi-kernel](https://github.com/Terminus-IMRC/cross-rpi-kernel).


## Installation

```
$ autoreconf -i -m
$ ./configure
$ make
$ sudo make install
```
