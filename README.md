# Spook Embedded Systems Implementations

Spook software implementations for Cortex-Mx microprocessors.

## Implementations

A straightforward, standard and portable C99 implementation. It is based on the reference implementation with changes to enable better compiler optimizations.
These primitives have much better performance than the reference implementation, however they are not fully optimized either: portability and code simplicity are also a concern.

## Configuration

Selecting the spook variant is done by means of the `#define`s `MULTI_USER` and `SMALL_PERM` (both default to `0`, which gives the primary candidate).


## Build

Have a look at `test/Makefile`.

## Test

```sh
$ cd test
$ ./test.sh
```

## Contributing

Contributions of any kind (code, bug reports, benckmarks, ...) are welcome. Please contact us at `team@spook.dev`.


## License

This software distributed under the terms of the MIT license. See [LICENSE](LICENSE) for details.
