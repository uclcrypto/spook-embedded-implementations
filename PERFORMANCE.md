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
|Code size (Byte)|3620|52.30|
|Clyde (cycle)|3374|1876|
|Shadow (cycle)|8626|5611|
|Spook (cycle/byte)|312|197|

## Speed

To minimize the number of cycles, the proposed implementation is compiled with the flag -O3.

 | |Cortex-M0|Cortex-M3|
|-|-|-|
|Code size (Byte)|11024|7320|
|Clyde (cycle)|2450|944|
|Shadow (cycle)|6286|2360|
|Spook (cycle/byte)|218|85.6|
