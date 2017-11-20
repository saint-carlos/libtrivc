Libtrivc Test/Dev Quickstart Guide
===

## What is libtrivc?

So you're going to start a C/C++ project.

Uh.

Now you have to recode all the trivial stuff that aren't part of libc: _ARRAY\_SIZE()_, _container\_of()_, some wrapper around asserts, some wrapper for logging...

Enter libtrivc, the trivial C stuff. It's there exactly to save you this trivial coding.

## Portability

* libtrivc currently only supports Linux.
* libtrivc uses GCC extentions to C.
* libtrivc assumes libc is glibc.

## Dependencies

build dependencies:

* cmake
* make
* gcc

Debian-based:

	# sudo apt-get install cmake make gcc

RHEL-based:

	# sudo yum install cmake make gcc

## Build
	# make

## Install
	# sudo make install
	...
	# sudo make uninstall

## Use

For general usage, include headers from _include/trivc/_.

All identifiers begin with _tvc\__ or _TVC\__ prefixes.

If the prefix annoys you, include headers from _include/trivc/sys/_ instead.

link with

	-ltrivc

## Test
	# make test
