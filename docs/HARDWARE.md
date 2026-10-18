# Hardware Compatibility & Validation Strategy

This document outlines the hardware subsystems supported by the OS kernel and how they are validated within the QEMU development loop.

## Supported Hardware (Phase 3 Baseline)

| Subsystem | Physical Target | QEMU Validation Device | Kernel Driver |
| :--- | :--- | :--- | :--- |
| **USB** | xHCI, EHCI, OHCI | `qemu-xhci` | `xhci_hcd` |
| **Storage (SATA)** | AHCI / SATA HDD/SSD | `ahci` | `ahci` |
| **Storage (NVMe)** | PCIe NVMe M.2 SSD | `nvme` | `nvme` |
| **Network (Wired)** | Intel Gigabit, Realtek | `e1000e` / `e1000` | `e1000e` / `e1000` |
| **Audio** | Intel HD Audio | `intel-hda`, `hda-output` | `snd_hda_intel` |
| **Display** | Intel i915, NVIDIA, Generic | `virtio-gpu` | `virtio_gpu` |
| **Input** | USB/PS2 KB, Multitouch | `usb-kbd`, `usb-tablet` | `usbhid`, `evdev` |

## Validation

Hardware detection is verified during the boot process by parsing the kernel ring buffer (`dmesg` / `qemu-serial.log`) for successful initialization signatures of the above drivers.
