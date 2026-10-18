#!/bin/bash
set -e

gcc -static -O2 core/init.c -o core/init

mkdir -p build/rootfs/{dev,proc,sys,bin}
cp core/init build/rootfs/init

cd build/rootfs
find . -print0 | cpio --null -ov --format=newc | gzip -9 > ../initramfs.cpio.gz
cd ../..

KERNEL=$(ls -1 /boot/vmlinuz* 2>/dev/null | head -n 1 || true)
if [ -z "$KERNEL" ]; then
    echo "Using installed WSL kernel..."
    KERNEL="/boot/vmlinuz-$(uname -r)"
fi

echo "Booting NexusOS directly in QEMU..."
qemu-system-x86_64 \
    -m 1024 \
    -kernel "$KERNEL" \
    -initrd build/initramfs.cpio.gz \
    -append "console=ttyS0 quiet loglevel=0 vt.global_cursor_default=0" \
    -nographic
