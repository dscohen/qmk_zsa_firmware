#!/usr/bin/env python3
"""
Combo generator for QMK keymaps.
Parses a simple combo definition format and generates C code.

Input format:
    combo_name {
        timeout-ms = <50>;
        key-positions = <14 15>;
        bindings = <KC_BSPC>;
        layers = <0 1 2 3 4 5>;
    };
"""

import re
import sys
from typing import Dict, List, Tuple

class ComboParser:
    def __init__(self, filename: str):
        self.filename = filename
        self.combos: Dict[str, Dict] = {}
        self.parse()

    def parse(self) -> None:
        """Parse the combo definition file."""
        with open(self.filename, 'r') as f:
            content = f.read()

        # Remove comments
        content = re.sub(r'//.*?$', '', content, flags=re.MULTILINE)
        content = re.sub(r'/\*.*?\*/', '', content, flags=re.DOTALL)

        # Match combo blocks: combo_name { ... };
        pattern = r'(\w+)\s*\{\s*(.*?)\s*\};'
        matches = re.finditer(pattern, content, re.DOTALL)

        for match in matches:
            combo_name = match.group(1)
            block_content = match.group(2)

            combo_data = self._parse_block(block_content)
            combo_data['name'] = combo_name
            self.combos[combo_name] = combo_data

    def _parse_block(self, block: str) -> Dict:
        """Parse a single combo block."""
        data = {}

        # Extract timeout-ms
        timeout_match = re.search(r'timeout-ms\s*=\s*<(\d+)>', block)
        data['timeout_ms'] = int(timeout_match.group(1)) if timeout_match else 50

        # Extract key-positions
        positions_match = re.search(r'key-positions\s*=\s*<([^>]+)>', block)
        if positions_match:
            positions_str = positions_match.group(1).strip()
            data['key_positions'] = list(map(int, positions_str.split()))
        else:
            data['key_positions'] = []

        # Extract bindings
        bindings_match = re.search(r'bindings\s*=\s*<([^>]+)>', block)
        data['bindings'] = bindings_match.group(1).strip() if bindings_match else 'KC_NO'

        # Extract layers
        layers_match = re.search(r'layers\s*=\s*<([^>]+)>', block)
        if layers_match:
            layers_str = layers_match.group(1).strip()
            data['layers'] = list(map(int, layers_str.split()))
        else:
            data['layers'] = []

        return data

    def generate_c_code(self) -> Tuple[str, str, str]:
        """Generate C code for the combos."""
        combo_arrays = []
        combo_entries = []
        combo_enum = []

        for idx, (name, combo) in enumerate(self.combos.items()):
            # Generate combo array
            positions_str = ' '.join(map(str, combo['key_positions']))
            combo_arrays.append(
                f"const uint16_t PROGMEM combo_{name}[] = {{{positions_str}, COMBO_END}};"
            )

            # Generate enum entry
            combo_enum.append(f"COMBO_{name.upper()},")

            # Generate combo entry in the array
            bindings = combo['bindings']
            combo_entries.append(
                f"[COMBO_{name.upper()}] = COMBO(combo_{name}, {bindings}),"
            )

        combo_arrays_code = "\n".join(combo_arrays)
        combo_enum_code = "\n    ".join(combo_enum)
        combo_entries_code = "\n    ".join(combo_entries)

        return combo_arrays_code, combo_enum_code, combo_entries_code

    def get_combo_count(self) -> int:
        """Get the total number of combos."""
        return len(self.combos)

    def print_summary(self) -> None:
        """Print a summary of parsed combos."""
        print(f"Parsed {len(self.combos)} combos:")
        for name, combo in self.combos.items():
            positions = combo['key_positions']
            bindings = combo['bindings']
            timeout = combo['timeout_ms']
            layers = combo['layers']
            print(f"  {name:20} -> {bindings:15} positions: {positions}  timeout: {timeout}ms  layers: {layers}")


def main():
    if len(sys.argv) < 2:
        print("Usage: combo_generator.py <combo_file>")
        sys.exit(1)

    combo_file = sys.argv[1]
    parser = ComboParser(combo_file)

    print("// Auto-generated combo code")
    print()

    arrays, enums, entries = parser.generate_c_code()

    print("// Combo arrays")
    print(arrays)
    print()

    print("// Combo enum entries")
    print("enum combos {")
    print("    " + enums)
    print("};")
    print()

    print("// Combo array entries")
    print("combo_t key_combos[COMBO_COUNT] = {")
    print("    " + entries)
    print("};")
    print()

    print(f"// Set COMBO_COUNT = {parser.get_combo_count()} in config.h")
    print()

    parser.print_summary()


if __name__ == "__main__":
    main()
