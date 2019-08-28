#include "idt.h"
#include "pic.h"
#include "io.h"
#include "kprint.h"

extern void _resolve_keyboard_interrupt();

#define KBD_GET_KEY 0x60
#define KBD_GET_CODE 0x64

#define KEY_PRESS_SHIFT_LEFT 0x2a
#define KEY_PRESS_SHIFT_RIGHT 0x36
#define KEY_PRESS_CAPSLOCK 0x3a
#define KEY_PRESS_TAB 0xf

#define KEY_RELEASE_SHIFT_LEFT 0xaa
#define KEY_RELEASE_SHIFT_RIGHT 0xb6

uint8_t shift_on;
uint8_t capslock_on;

static const char key_mapping[] = {
    // i'm marking unsupported or skipped with 0
    0, 0, '1', '2', '3', '4', '5', '6', '7', '8', '9',
    '0', '-', '=', 0, '\t', 'q', 'w', 'e', 'r', 't',
    'y', 'u', 'i', 'o', 'p', '[', ']', '\n', 0, 'a',
    's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'',
    '`', 0, '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm',
    ',', '.', '/', 0, '*', 0, ' ', 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    '7', '8', '9', '-', '4', '5', '6', '+', '1', '2',
    '3', '0', '.', 0, 0
};

static const char key_mapping_caps[] = {
    0, 0, '1', '2', '3', '4', '5', '6', '7', '8', '9',
    '0', '-', '=', 0, '\t', 'Q', 'W', 'E', 'R', 'T',
    'Y', 'U', 'I', 'O', 'P', '[', ']', '\n', 0, 'A',
    'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ';', '\'',
    '`', 0, '\\', 'Z', 'X', 'C', 'V', 'B', 'N', 'M',
    ',', '.', '/', 0, '*', 0, ' ', 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    '7', '8', '9', '-', '4', '5', '6', '+', '1', '2',
    '3', '0', '.', 0, 0
};

static const char key_mapping_shift[] = {
    0, 0, '!', '@', '#', '$', '%', '^', '&', '*', '(',
    ')', '_', '+', 0, '\t', 'Q', 'W', 'E', 'R', 'T',
    'Y', 'U', 'I', 'O', 'P', '{', '}', '\n', 0, 'A',
    'S', 'D', 'F', 'G', 'H', 'J', 'K', 'L', ':', '\"',
    '~', 0, '|', 'Z', 'X', 'C', 'V', 'B', 'N', 'M',
    '<', '>', '?', 0, '*', 0, ' ', 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    '7', '8', '9', '-', '4', '5', '6', '+', '1', '2',
    '3', '0', '.', 0, 0
};

static const char key_mapping_mixed[] = {
    0, 0, '!', '@', '#', '$', '%', '^', '&', '*', '(',
    ')', '_', '+', 0, '\t', 'q', 'w', 'e', 'r', 't',
    'y', 'u', 'i', 'o', 'p', '{', '}', '\n', 0, 'a',
    's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ':', '\"',
    '~', 0, '|', 'z', 'x', 'c', 'v', 'b', 'n', 'm',
    '<', '>', '?', 0, '*', 0, ' ', 0, 0, 0,
    0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
    '7', '8', '9', '-', '4', '5', '6', '+', '1', '2',
    '3', '0', '.', 0, 0
};

void setup_keyboard() {
    install_interrupt_gate(IRQ1_KEYBOARD_IDT_OFFSET, _resolve_keyboard_interrupt);
    kprint("\t* Installed keyboard IDT gate\n");

    pic_clear_mask(IRQ1_KEYBOARD);
    kprint("\t* Cleared IRQ1 mask\n");
}

void resolve_key_pressed(uint8_t key) {
    switch (key) {
    case KEY_PRESS_SHIFT_LEFT:
    case KEY_PRESS_SHIFT_RIGHT:
        shift_on++;
        return;
    case KEY_PRESS_CAPSLOCK:
        capslock_on = capslock_on ? 0 : 1;
        return;
    case KEY_PRESS_TAB:
        // hackish. i'm not supporting \t in kprint yet
        kprint("    ");
        return;
    default:
        break;
    }


    // test if we know how to display this
    if (key < (sizeof(key_mapping) / sizeof(char)) && 0 != key_mapping[key]) {
        char c = key_mapping[key];

        // i'm mimicking the behaviour of my linux machine
        if (capslock_on && !shift_on) {
            c = key_mapping_caps[key];
        } else if (!capslock_on && shift_on) {
            c = key_mapping_shift[key];
        } else if (capslock_on && shift_on) {
            c = key_mapping_mixed[key];
        }

        kprint("%c", c);
    }
}

void resolve_key_release(uint8_t key) {
    switch (key) {
    case KEY_RELEASE_SHIFT_LEFT:
    case KEY_RELEASE_SHIFT_RIGHT:
        shift_on--;
        return;
    default:
        break;
    }
}

void resolve_keyboard_interrupt() {
    pic_send_eoi(IRQ1_KEYBOARD);
    
    uint8_t key = inb(KBD_GET_KEY);

    // this test should be decent enough for the printable
    // characters on my keyboard at least
    if (key < 0x80) {
        resolve_key_pressed(key);
    } else {
        resolve_key_release(key);
    }
}
