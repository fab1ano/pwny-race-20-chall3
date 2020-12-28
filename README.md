chall3
======

This binary was the challenge for the pwny race at the (inofficial?) German CTF Meet-Up in 2020.

## Player Info
The players get the binary itself, the source code, and the dockerfile.

## Challenge Idea
A simple `uint64_t` store on the stack.
Stack is executable.
Should be solvable within 20 minutes.

## Exploit Approach
Vulnerability: Index out ouf bound read and write access on a stack array.
Just leak a stack address with the out of bounds access and place some shellcode within the array itself.
Then, manipulate the `rip` to jump to the shellcode.
