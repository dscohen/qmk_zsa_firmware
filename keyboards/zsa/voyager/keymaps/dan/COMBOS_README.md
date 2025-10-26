# Combo Definition System

This keymap uses a helper system to make managing combos easier. Instead of manually writing combo arrays and enum entries, you define combos in a simple `combos.def` file and generate the C code.

## Overview

- **combos.h** - Header file with key position indexing and helper functions
- **combos.def** - Combo definitions (input file)
- **combo_generator.py** - Python script that parses combos.def and generates C code

## Key Position Indexing

The Voyager has 40 keys total (4 rows × 6 cols per side + 2 thumb keys per side).

Positions are indexed left-to-right across both sides, then down:

```
Row 0: L: 0  1  2  3  4  5     R: 6  7  8  9 10 11
Row 1: L: 12 13 14 15 16 17    R: 18 19 20 21 22 23
Row 2: L: 24 25 26 27 28 29    R: 30 31 32 33 34 35
Row 3: L: 36 37 38 39 40 41    R: 42 43 44 45 46 47
Row 4 (thumbs): L: 48 49       R: 50 51
```

## Combo Definition Format

Edit `combos.def` with the following format:

```
combo_name {
    timeout-ms = <50>;
    key-positions = <pos1 pos2 ...>;
    bindings = <keycode>;
    layers = <layer1 layer2 ...>;
};
```

### Fields:
- **timeout-ms**: How long keys must be pressed together (default: 50ms)
- **key-positions**: Space-separated key position numbers
- **bindings**: QMK keycode produced by the combo
- **layers**: Space-separated layer numbers where combo is active

### Example:

```
combo_backspace {
    timeout-ms = <50>;
    key-positions = <26 27>;
    bindings = <KC_BSPC>;
    layers = <0 1 2 3 4 5>;
};

combo_esc {
    timeout-ms = <50>;
    key-positions = <8 9>;
    bindings = <KC_ESC>;
    layers = <0 1 2 3 4 5>;
};
```

## Generating C Code

After editing `combos.def`, run the generator:

```bash
python3 combo_generator.py combos.def
```

This outputs:
1. **Combo array definitions** - `const uint16_t PROGMEM combo_*[]`
2. **Enum entries** - For indexing combos
3. **Combo array initialization** - `combo_t key_combos[]`
4. **Summary** - Count and details of parsed combos

## Integration with keymap.c

When you have the generated code, you need to:

1. **Copy the enum entries** into the `enum combos` section in keymap.c
2. **Copy the combo arrays** before the enum
3. **Copy the combo_t initialization** into `key_combos[]` in keymap.c
4. **Update COMBO_COUNT** in config.h to match the combo count

Currently, all 48 combos are manually defined in keymap.c. To migrate to the generator system:
- Edit combos.def with your combo definitions
- Run the generator
- Copy the output into keymap.c
- Update config.h with the new COMBO_COUNT

## Notes

- Comments (both `//` and `/* */`) are supported and ignored by the parser
- Layer mask is generated automatically from the layers list
- Position ordering matters for combo detection (positions should be in sequence)
- Maximum 8 key positions per combo (array size in combo_config_t)
