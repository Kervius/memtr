memtr
=====

basic simple dumb trivial primitive memory allocation tracer library for systems supporting the `LD_PRELOAD`

Pushing it here because I'm tired implementing it anew on every darn project.

This one is bit special though: it is very spartan to allow recursive calls. That is needed to see what `libc` itself is doing (esp when it is about to crash).

Usage
=====

`MEMTR` env var is used to set the file name where to print the trace. Must be set to activate the tracing!

`LD_PRELOAD` - the usual.

Example:

	$ MEMTR=./memtr.out LD_PRELOAD=/path/to/libmemtr.so /bin/sleep 1
	$ head ./memtr.out
	m<	5	0x00000000
	m>	5	0x0905f008
	f<	0	0x0905f008
	f>	0	0x0905f008
	f<	0	0x00000000
	f>	0	0x00000000
	m<	16	0x00000000
	m>	16	0x0905f018
	m<	16	0x00000000
	m>	16	0x0905f030

`<` is for entry into the call, `>` - exit from the call. The `m` is for `malloc()`, `f` - `free()`, `r` - `realloc()` and `c` - `calloc()`.

