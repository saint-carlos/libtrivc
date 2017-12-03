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

## WTFAQ

Q: Why use libtrivc? GLib seems to be much more featureful and much more mature.

A: There are several things about GLib that warrant the existence of libtrivc:

1. GLib doesn't have everything libtrivc has, e.g. STKSTR().
2. libtrivc is easier to understand and use, since it's super-simple. In comparison:
 * GLib defines its own types e.g. "gpointer" and "gint"
 * It has its own GError
 * it has its own clones of standard functionality
3. Last I looked at GLib code, they it was just assuming that malloc(3) doesn't fail. libtrivc may be trivial, but it's robust.
4. Some of GLib's abstractions are not good enough Examples:
 * Atomic variables don't have their own types
 * The formatting functions (clones of printf and friends) are as unsafe as the standard ones
 * GString is necessarily on the heap
5. GLib is can be cosmetically annoying. Examples:
 * GLib's naming convention departs from the traditional C naming convention, and that is annoying to see thoughout code that otherwise doesn't depart from it.
 * GLib prefixes everything with "g\_", which could be very annoying if you use a convention where the "g\_" prefix denotes a global variable.

Q: What about a _tvc\_scnprintf()_ equivalent of strncpy()?

A: You want _strlcpy()_ from libbsd.

Q: "error while loading shared libraries\: libtrivc.so: cannot open shared object file: No such file or directory"

A: Add _LD\_LIBRARY\_PATH=/usr/loca/lib:/usr/local/lib64_ to your environment when running. when installing libraries without a package, it usually goes to _/usr/local/lib_ or _/usr/local/lib64_, which may not be in the dynamic loader's path.

Q: Passing and returning structs by value? Sacrilege!

A: Structs such as _tvc\_buf\_t_ are largely a way to pack 2 arguments into one. They are no more expensive to pass to functions than passing their constituents individually, and not really more expensive to return then returning a pointer (well, at least on on architectures I care about). It's just simpler in many cases to pass them by value.

Q: So you have a struct for buffer, which is supposed to prevent passing around buffer + length everywhere, but none of your interfaces uses it... what's the point?

A: If someone just wants to use e.g. the string functions, they should not be forced to use buffers for that. Perhaps in the future libtrivc will introduce variants of these functions which take a buffer struct.
