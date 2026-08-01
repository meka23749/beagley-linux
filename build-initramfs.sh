#!/bin/bash
# build-initramfs.sh
# Rebuilds the BusyBox rootfs and packages it into an initramfs.
# beagley-linux — Embedded Linux From Scratch
#
# Requirements:
#   - aarch64-linux-gnu- toolchain installed
#   - BusyBox already built and installed into $ROOTFS (see README step 5.2)
#
# Usage: ./build-initramfs.sh

set -e

ROOTFS="$HOME/beagley/rootfs"
OUT="$HOME/beagley/initramfs.cpio.gz"

echo ">>> Checking rootfs directory tree..."
for d in bin sbin etc proc sys dev usr; do
    if [ ! -d "$ROOTFS/$d" ]; then
        echo "ERROR: $ROOTFS/$d missing. See README step 5.1."
        exit 1
    fi
done

echo ">>> Checking /init..."
if [ ! -x "$ROOTFS/init" ]; then
    echo "ERROR: $ROOTFS/init missing or not executable."
    echo "       Copy the 'init' file from this repo to $ROOTFS/init"
    echo "       then: chmod +x $ROOTFS/init"
    exit 1
fi

echo ">>> Packaging the initramfs..."
cd "$ROOTFS"
find . | cpio -H newc -o 2>/dev/null | gzip > "$OUT"

echo ">>> Verifying /init is at the root and executable..."
zcat "$OUT" | cpio -tv 2>/dev/null | grep " init$"

SIZE=$(ls -lh "$OUT" | awk '{print $5}')
echo ""
echo ">>> OK! initramfs created: $OUT ($SIZE)"
echo ""
echo "Next step: transfer to the board"
echo "  scp $OUT steve@<ip>:~/"
echo "  # then on the board: sudo cp ~/initramfs.cpio.gz /boot/firmware/ && sync"
