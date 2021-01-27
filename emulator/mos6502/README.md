## MOS6502 Emulator in C++ ##

Original project: https://github.com/gianlucag/mos6502.git (commit 8ae598411793f3e9d6b9586b3d1e36905fa2e14b)

## Changes from original project ##

- Bus functions are std::function
  - compatible with lambdas
- Bus functions set separately from construction
  - allow easy access to a previously constructed mos6502 instance from function
- Run() counts cycles in a public member variable
- Cycle count is exact
  - Adressing with a page-cross for read operations take one more cycle than in table
  - Branching operations take one or two more cycles if branch is taken (depending on page-cross)
