Libtrivc Test/Dev Quickstart Guide
===

## What is libtrivc?

So you're going to start a C/C++ project.

Uh.

Now you have to recode all the trivial stuff that aren't part of libc: _ARRAY\_SIZE()_, _container\_of()_, some wrapper around asserts, some wrapper for logging...

Enter libtrivc, the trivial C stuff. It's there exactly to save you this trivial coding.

Not only does it provide you with those trivial things, it also provides you with trivial things you didn't even know you needed, or knew you needed but never bothered with:

* A struct for buffer + length! Yeah, man, 21st century rocks!
* Useful primitives for safe formatting and concise formatting
* A couple more primitives for string parsing

## Portability

* libtrivc currently only supports Linux.
* libtrivc uses GCC extentions to C.
* libtrivc assumes libc is glibc.

## Dependencies

runtime dependencies:

* zlib

Debian-based:

	# sudo apt-get install zlib1g

RHEL-based:

	# sudo yum install zlib

build dependencies:

* cmake
* make
* gcc
* libbsd
* zlib

Debian-based:

	# sudo apt-get install cmake make gcc libbsd-dev zlib1g-dev

RHEL-based:

	# sudo yum install cmake make gcc libbsd-devel zlib-devel

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
