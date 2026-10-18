cat << 'EOF' > .gitignore
# Build artifacts
build/
*.o
*.so
*.efi
*.iso
*.img
*.cpio.gz
bzImage

# QEMU logs
*.log

# Editor/OS specifics
.vscode/
.DS_Store
EOF