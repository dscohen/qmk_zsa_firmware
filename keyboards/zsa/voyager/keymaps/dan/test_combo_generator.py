#!/usr/bin/env python3
"""
Test suite for combo_generator.py
Verifies that generated code can be parsed and matches expected output.
"""

import sys
import tempfile
from pathlib import Path
from combo_generator import ComboParser

def test_parse_basic_combo():
    """Test parsing a single combo definition."""
    combo_def = """
    combo_backspace {
        timeout-ms = <50>;
        key-positions = <26 27>;
        bindings = <KC_BSPC>;
        layers = <0 1 2 3 4 5>;
    };
    """

    with tempfile.NamedTemporaryFile(mode='w', suffix='.def', delete=False) as f:
        f.write(combo_def)
        f.flush()

        parser = ComboParser(f.name)

        assert len(parser.combos) == 1, f"Expected 1 combo, got {len(parser.combos)}"

        combo = parser.combos['combo_backspace']
        assert combo['timeout_ms'] == 50, f"Expected timeout 50, got {combo['timeout_ms']}"
        assert combo['key_positions'] == [26, 27], f"Expected positions [26, 27], got {combo['key_positions']}"
        assert combo['bindings'] == 'KC_BSPC', f"Expected KC_BSPC, got {combo['bindings']}"
        assert combo['layers'] == [0, 1, 2, 3, 4, 5], f"Expected layers [0-5], got {combo['layers']}"

        Path(f.name).unlink()

    print("✓ test_parse_basic_combo PASSED")

def test_parse_multiple_combos():
    """Test parsing multiple combo definitions."""
    combo_def = """
    combo_esc {
        timeout-ms = <50>;
        key-positions = <8 9>;
        bindings = <KC_ESC>;
        layers = <0 1 2 3 4 5>;
    };

    combo_tab {
        timeout-ms = <50>;
        key-positions = <7 8>;
        bindings = <KC_TAB>;
        layers = <0 1 2 3 4 5>;
    };

    combo_enter {
        timeout-ms = <50>;
        key-positions = <9 10>;
        bindings = <KC_ENT>;
        layers = <0 1 2 3 4 5>;
    };
    """

    with tempfile.NamedTemporaryFile(mode='w', suffix='.def', delete=False) as f:
        f.write(combo_def)
        f.flush()

        parser = ComboParser(f.name)

        assert len(parser.combos) == 3, f"Expected 3 combos, got {len(parser.combos)}"
        assert 'combo_esc' in parser.combos
        assert 'combo_tab' in parser.combos
        assert 'combo_enter' in parser.combos

        Path(f.name).unlink()

    print("✓ test_parse_multiple_combos PASSED")

def test_comments_ignored():
    """Test that comments are properly ignored."""
    combo_def = """
    // This is a line comment
    combo_backspace {
        timeout-ms = <50>;  // Comment in definition
        key-positions = <26 27>;
        bindings = <KC_BSPC>;
        /* Multi-line
           comment */
        layers = <0 1 2 3 4 5>;
    };
    """

    with tempfile.NamedTemporaryFile(mode='w', suffix='.def', delete=False) as f:
        f.write(combo_def)
        f.flush()

        parser = ComboParser(f.name)

        assert len(parser.combos) == 1, f"Expected 1 combo (comments ignored), got {len(parser.combos)}"
        combo = parser.combos['combo_backspace']
        assert combo['bindings'] == 'KC_BSPC'

        Path(f.name).unlink()

    print("✓ test_comments_ignored PASSED")

def test_generate_code():
    """Test that code generation produces valid C syntax."""
    combo_def = """
    combo_backspace {
        timeout-ms = <50>;
        key-positions = <26 27>;
        bindings = <KC_BSPC>;
        layers = <0 1 2 3 4 5>;
    };
    """

    with tempfile.NamedTemporaryFile(mode='w', suffix='.def', delete=False) as f:
        f.write(combo_def)
        f.flush()

        parser = ComboParser(f.name)
        arrays, enums, entries = parser.generate_c_code()

        # Check array generation
        assert 'const uint16_t PROGMEM' in arrays
        assert 'combo_combo_backspace' in arrays
        assert '26 27' in arrays
        assert 'COMBO_END' in arrays

        # Check enum generation
        assert 'COMBO_COMBO_BACKSPACE' in enums

        # Check entries generation
        assert 'COMBO(combo_combo_backspace, KC_BSPC)' in entries

        Path(f.name).unlink()

    print("✓ test_generate_code PASSED")

def test_combo_count():
    """Test that combo count matches."""
    combo_def = """
    combo_1 { timeout-ms = <50>; key-positions = <0 1>; bindings = <KC_A>; layers = <0>; };
    combo_2 { timeout-ms = <50>; key-positions = <2 3>; bindings = <KC_B>; layers = <0>; };
    combo_3 { timeout-ms = <50>; key-positions = <4 5>; bindings = <KC_C>; layers = <0>; };
    combo_4 { timeout-ms = <50>; key-positions = <6 7>; bindings = <KC_D>; layers = <0>; };
    combo_5 { timeout-ms = <50>; key-positions = <8 9>; bindings = <KC_E>; layers = <0>; };
    """

    with tempfile.NamedTemporaryFile(mode='w', suffix='.def', delete=False) as f:
        f.write(combo_def)
        f.flush()

        parser = ComboParser(f.name)

        assert parser.get_combo_count() == 5, f"Expected 5 combos, got {parser.get_combo_count()}"

        Path(f.name).unlink()

    print("✓ test_combo_count PASSED")

def test_custom_timeout():
    """Test parsing custom timeout values."""
    combo_def = """
    combo_fast { timeout-ms = <25>; key-positions = <0 1>; bindings = <KC_A>; layers = <0>; };
    combo_slow { timeout-ms = <100>; key-positions = <2 3>; bindings = <KC_B>; layers = <0>; };
    """

    with tempfile.NamedTemporaryFile(mode='w', suffix='.def', delete=False) as f:
        f.write(combo_def)
        f.flush()

        parser = ComboParser(f.name)

        assert parser.combos['combo_fast']['timeout_ms'] == 25
        assert parser.combos['combo_slow']['timeout_ms'] == 100

        Path(f.name).unlink()

    print("✓ test_custom_timeout PASSED")

def main():
    """Run all tests."""
    print("Running combo_generator tests...\n")

    try:
        test_parse_basic_combo()
        test_parse_multiple_combos()
        test_comments_ignored()
        test_generate_code()
        test_combo_count()
        test_custom_timeout()

        print("\n✅ All tests PASSED!")
        return 0
    except AssertionError as e:
        print(f"\n❌ Test FAILED: {e}")
        return 1
    except Exception as e:
        print(f"\n❌ Unexpected error: {e}")
        import traceback
        traceback.print_exc()
        return 1

if __name__ == "__main__":
    sys.exit(main())
