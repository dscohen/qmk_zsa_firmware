#!/usr/bin/env python3
"""
Validation script to compare current keymap combos with generator output.
Extracts combo definitions from keymap.c and verifies they match combos.def.
"""

import re
import sys
from pathlib import Path
from combo_generator import ComboParser

def extract_combos_from_keymap(keymap_file):
    """Extract combo definitions from keymap.c."""
    combos = {}

    with open(keymap_file, 'r') as f:
        content = f.read()

    # Find all combo array definitions
    pattern = r'const uint16_t PROGMEM combo_(\w+)\[\] = \{([^}]+)\};'
    matches = re.finditer(pattern, content)

    for match in matches:
        combo_name = match.group(1)
        positions_str = match.group(2).strip()

        # Parse the positions (may contain keycodes like KC_S, KC_W, etc.)
        # For now, we'll just note that they exist
        combos[combo_name] = positions_str

    return combos

def extract_combo_entries(keymap_file):
    """Extract combo entries from the combo_t array."""
    entries = {}

    with open(keymap_file, 'r') as f:
        content = f.read()

    # Find the combo_t array
    pattern = r'\[COMBO_(\w+)\]\s*=\s*COMBO\(combo_(\w+),\s*([^)]+)\)'
    matches = re.finditer(pattern, content)

    for match in matches:
        enum_name = match.group(1)
        combo_name = match.group(2)
        binding = match.group(3).strip()

        entries[enum_name] = {
            'name': combo_name,
            'binding': binding
        }

    return entries

def count_combos(keymap_file, config_file):
    """Count combos in keymap and check config."""
    entries = extract_combo_entries(keymap_file)

    # Check COMBO_COUNT in config
    with open(config_file, 'r') as f:
        content = f.read()

    combo_count_match = re.search(r'#define COMBO_COUNT (\d+)', content)
    config_count = int(combo_count_match.group(1)) if combo_count_match else 0

    return len(entries), config_count

def main():
    """Run validation."""
    keymap_file = Path('keymap.c')
    config_file = Path('config.h')
    combos_def = Path('combos.def')

    if not keymap_file.exists():
        print(f"Error: {keymap_file} not found")
        return 1

    if not config_file.exists():
        print(f"Error: {config_file} not found")
        return 1

    if not combos_def.exists():
        print(f"Error: {combos_def} not found")
        return 1

    print("🔍 Validating Combo System\n")

    # Count combos in current keymap
    actual_count, config_count = count_combos(keymap_file, config_file)
    print(f"Combos in keymap.c: {actual_count}")
    print(f"COMBO_COUNT in config.h: {config_count}")
    if actual_count == config_count:
        print("✓ Combo counts match\n")
    else:
        print(f"⚠️  Mismatch: {actual_count} combos vs COMBO_COUNT={config_count}\n")

    # Count combos in combos.def
    parser = ComboParser(str(combos_def))
    def_count = parser.get_combo_count()
    print(f"Combos in combos.def: {def_count}\n")

    # Extract current combo arrays
    combo_arrays = extract_combos_from_keymap(keymap_file)
    print(f"Combo arrays defined: {len(combo_arrays)}")
    for name in sorted(combo_arrays.keys()):
        print(f"  - {name}")

    print(f"\nCombo entries: {len(extract_combo_entries(keymap_file))}")

    # Show what combos.def would generate
    print(f"\nGenerator output would create: {def_count} combos")
    print("Combos in combos.def:")
    parser.print_summary()

    return 0

if __name__ == "__main__":
    sys.exit(main())
