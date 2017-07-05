FermiOS
=======

## About

FermiOS is my yet another try to make an OS. This time I am also reading a book on OSes, so maybe it'll yield something. At the moment it's going to
be a 32-bit system, but I will look into long mode later.


The current design goal is simple:

* [✓] It should boot;
* [ ] Basic kernel level IO. Aka `puts()` and `getchar()`.

After that, I will actually think what I really want from this. Definitely processes (what's the point otherwise) and FS.


Since you might be curious about the name: [Fermions](https://en.wikipedia.org/wiki/Fermion) I just thought it's kinda cool.

## Build

* You will need POSIX environment, Linux will work, BSDs should work, OS X might work, Windows with Cygwin or WSL (Windows Subsystem for Linux) maybe could work;
* You will need to have a cross-compiler and binutils for x86 (IA-32) platform (My build system expects the cross-tools to be `i686-elf-*`);
	* You either build `gcc` from scratch: [osdev wiki on cross-compiling](http://wiki.osdev.org/GCC_Cross-Compiler);
	* Or you you can download an existing one: [osdev links to prebuilt toolchains](http://wiki.osdev.org/GCC_Cross-Compiler#Prebuilt_Toolchains);
* You will also need `nasm` to assemble my x86 assembly, since I like intel syntax so my code is in that;
* The build system is a single `Makefile` so you'll need `make` too;
* To make a Live CD image you'll need `xorriso`/`libisoburn` and `grub2` to make it bootable;
* You can use any VM to test the OS, but if you want to have `make` integration you'll also need `qemu` with `i386` system;
* After you have all the tools needed, all you need to do is `git clone`;
* Copy the cross-compiler toolchain into `cross` directory in the root of the project;
* Now simply execute `make` to build the project;
* After the build is done (and you have `qemu`) you can simply run `make run` to boot the kernel directly, or `make run-iso` to boot from .iso;
* ...
* PROFIT!


## Project structure

The project is structured as following:

```
.
├── bin
├── cross
├── include
│   ├── arch
│   ├── kernel
│   └── libc
├── obj
├── src
│   ├── arch
│   │   └── i386
│   ├── kernel
│   └── libc
└── sysroot
```

* `bin` - this is where the linked binaries are placed into (like kernel, libk and (eventually), libc);
* `cross` - this is where the cross-compiler toolchain lives;
* `include` - here the `.h` header files live, the subfolder mirror (more-or-less) the `src` structure;
* `obj` - intermediate (unlinked) objects are put here during the build, the `.o` files are placed into mirrored subfolders;
* `src` - here lie all the source files:
	* `arch` - architecture specific code is placed here, atm only i386 is supported;
	* `kernel` - kernel specific code lives here;
	* `libc` - future libc as well as non-hosted libk lie here;
* `sysroot` - during the build this directory is created and filled with GRUB bootloader as well as the kernel binary. In the future this also will be the OS initial filesystem. GRUB uses this folder to create the ISO image.


## Installation

At the moment the OS is very simple, in fact it's not even an OS, rather an executable on bare-metal booted my GRUB. So there is no installation process, think of it as a LiveCD.


## License

This code is distributed with The MIT License (MIT) Copyright (c) 2017 Dmytro Kalchenko

Please check LICENSE.txt for more details.


## Who made this?

I did. Sorry about the mess.

* @redpandaua