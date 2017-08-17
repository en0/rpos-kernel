# Styling Guide

This is a list of weak rules that i am trying to follow while building this
project.

## General

These rules are true unless explicity stated otherwise.

- Use snake case for function and variable names.
- Use spaces, not tabs.
- External code provided by 3rd parties, such as the multiboot header, will
  __not__ be altered to match convention.

## Global Variables

prefix with __g__. e.g. ```char g_my_global_variable;```

## Functions

### Public Functions

Functions that are driectly shared between pages should use lower case, snake
case and be prefixed with the name of the module.

Note: the name of the module is not the name of the file, but it could be.


```c
void module1_setup() { ... }
```

### Private Functions

Functions that are not directly shared between pages should use lower case and
snake case. They should be declared with static so they are not excessible by
other code pages.


```c
static void do_what_ever() { ... }
```

## Custom Types <small>structures, typedef</small>

Structes and typedefs break the snake_case rule. Structures and typedefs should
use title case for there respective names. However, a structures fields should
follow the snake case rule.

Structures should have typedefs created for them and unless it is not possible,
the typedef should be done in the same declaration.

```c
typedef struct MyStruct {
    int field1;
    char field2;
} MyStruct_t;

typedef unsinged long PhysAddr;
```
