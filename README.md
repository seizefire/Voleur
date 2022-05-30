# Ladron
A Jitbit Macro Recorder Keygen

## 1. What is Jitbit Macro Recorder?
* It is a macro recorder created by Jitbit
* Allows you to create macros, run them, and even compile them as EXEs
* Macro Recorder is not free, and requires you to buy a license at their website

## 2. How are their license keys generated?
* Jitbit uses a simple DSA signature to generate keys
* More specifically, they use the `DSACryptoServiceProvider` in C#.

### 2.1. Specifics of `DSACryptoServiceProvider`
* Before signing, message is hashed using SHA-1
* For the macro recorder specifically, a 160-bit private key and a 512-bit public key is used
* Output is two 160-bit numbers that are combined to make the full signature

## 3. How to run this program?
* Simply run the latest exe in Releases

## 4. How to build this repository?
* Requires `make` and `g++`
* Depends on `libtommath` and `libtomcrypt`

1. Run `make`
