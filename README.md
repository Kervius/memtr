memtr
=====

basic simple dumb trivial primitive memory allocation tracer library for systems supporting the `LD_PRELOAD`

Pushing it here because I'm tired implementing it anew on every darn project.

This one is bit special though: it is very spartan to allow recursive calls. That is needed to see what `libc` itself is doing (esp when it is about to crash).
