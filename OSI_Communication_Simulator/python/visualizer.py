#!/usr/bin/env python3
"""
A terminal visualizer for the OSI 7-layer encapsulation/decapsulation path.
This visualization complements the real C TCP client/server communication.
"""
import argparse
import sys
import time
from datetime import datetime

LAYERS = [
    (7, "Application"),
    (6, "Presentation"),
    (5, "Session"),
    (4, "Transport"),
    (3, "Network"),
    (2, "Data Link"),
    (1, "Physical"),
]

def color(text, code):
    if sys.stdout.isatty():
        return f"\033[{code}m{text}\033[0m"
    return text

def show_group(title, items, direction, delay):
    print(f"\n{color(title, '1;36')}")
    for number, name in items:
        arrow = "  ↓" if direction == "down" else "  ↑"
        print(f"  Layer {number}: {name}{arrow}")
    print(color("  Progress checkpoint reached.", "1;32"))
    if delay:
        time.sleep(delay)

def visualize(message, delay):
    print("=" * 64)
    print(color("             OSI 7-LAYER COMMUNICATION VISUALIZER", "1;35"))
    print("Started:", datetime.now().strftime("%Y-%m-%d %H:%M:%S"))
    print("Message :", message)
    print("=" * 64)

    print(color("\nSENDER: ENCAPSULATION (Layer 7 -> Layer 1)", "1;33"))
    for start in range(0, 7, 2):
        show_group("Encapsulation progress", LAYERS[start:start + 2],
                   "down", delay)
    print("  Layer 1: Physical - bits transmitted through the medium")
    print(color("\n        >>> TCP SOCKET: CLIENT -> SERVER >>>", "1;32"))

    receive_layers = [
        (1, "Physical"), (2, "Data Link"), (3, "Network"),
        (4, "Transport"), (5, "Session"), (6, "Presentation"),
        (7, "Application")
    ]
    print(color("\nRECEIVER: DECAPSULATION (Layer 1 -> Layer 7)", "1;33"))
    for start in range(0, 7, 2):
        show_group("Decapsulation progress", receive_layers[start:start + 2],
                   "up", delay)
    print("  Layer 7: Application - original message delivered")
    print("\n" + "=" * 64)
    print(color("VISUALIZATION COMPLETE", "1;32"))
    print("Note: Run server.exe and client.exe separately for real TCP transfer.")
    print("=" * 64)

def main():
    parser = argparse.ArgumentParser(description="OSI 7-layer data-flow visualizer")
    parser.add_argument("--message", default="Hello from OSI Communication Simulator!")
    parser.add_argument("--delay", type=float, default=0.35,
                        help="Delay in seconds between progress checkpoints (0 for instant)")
    args = parser.parse_args()
    visualize(args.message, max(0, args.delay))

if __name__ == "__main__":
    main()
