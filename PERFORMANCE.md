# Performance evaluation

Performance has been evaluated for both primitives (Clyde128 and Shadow512) and for the whole AEAD mode (Spook128su512v1).
Performance for other Spook modes has not yet been evaluated, yet the Spook128mu512v1 is expected to have the same performance characteristics as Spook128su512v1.

## Build

The code was compiled with arm-none-eabi-gcc 9.2 for the following targets:
* STM32F0 (Cortex-M0)
* STM32F1 (Cortex-M3)

## Code size

To minimize the code size, the proposed implementation is compiled with the flag -Os.

 | |Cortex-M0|Cortex-M3|
|-|-|-|
|Code size (Bytes)|3376|3262|
|Clyde (Cycles)|3374|1876|
|Shadow (Cycles)|8626|5611|
|Spook (Cycles per Byte)|312|197|

## Speed

To minimize the number of cycles, the proposed implementation is compiled with the flag -O3.

 | |Cortex-M0|Cortex-M3|
|-|-|-|
|Code size (Bytes)|7700|7150|
|Clyde (Cycles)|2450|944|
|Shadow (Cycles)|6286|2360|
|Spook (Cycles per Byte)|218|85.6|
