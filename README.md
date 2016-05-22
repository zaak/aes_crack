AES-128 CBC bruteforce tool
===================
AES-128 CBC brute force tool based on AESNI instruction set. The speed of brute forcing seems to be
a few times faster in compare to implementation based on OpenSSL.

# Building
Building requires `nasm`.

```
mkdir build
cd build
cmake ..
make
```

# Usage
All parameters are hardcoded - modify, recompile, run.

**Note:** This tool supports only AES-128 CBC cracking, and brute forces only the first block.
To be able to obtain a valid key you need to know a reasonable part of the plaintext.