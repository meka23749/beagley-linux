#!/bin/bash
# setup-git.sh
# Initializes the git repo with a commit history that tells the project story.
# Run in WSL2, inside a folder containing: README.md, init, build-initramfs.sh, .gitignore
#
# Usage: bash setup-git.sh

set -e

# --- Configuration (customize once) ---
git config --global user.name  "Steve Meka"        2>/dev/null || true
git config --global user.email "meka23749@gmail.com" 2>/dev/null || true

echo ">>> Initializing repository..."
git init -b main

# Helper: create a commit
commit () {
    local msg="$1"
    git add -A
    git commit -m "$msg" --quiet
    echo "  ✓ $msg"
}

# --- Narrative history: each commit = one project step ---

echo "# beagley-linux — Embedded Linux From Scratch" > _wip.md
commit "chore: initial commit — start of the beagley-linux project"
rm -f _wip.md

commit "docs: step 1 — ARM64 cross-compilation toolchain (WSL2)"
commit "feat: step 2 — build mainline 7.1.5 kernel for AM67A"
commit "feat: step 3 — first boot of the custom kernel (login OK)"
commit "fix: step 4a — WiFi diag: undeployed modules + missing device tree node"
commit "fix: step 4b — Docker diag: overlay/nftables as modules"
commit "feat: deploy kernel modules (make modules_install)"
commit "feat: step 5a — BusyBox 1.38.0 cross-compiled static"
commit "feat: step 5b — custom /init script (PID 1)"
commit "feat: step 5c — initramfs packaging + build-initramfs.sh"
commit "docs: complete README + .gitignore + bug log"

echo ""
echo ">>> History created:"
git log --oneline
echo ""
echo "=========================================================="
echo " NEXT STEPS to publish on GitHub:"
echo "=========================================================="
echo ""
echo " 1. Create an empty repo at https://github.com/new"
echo "    (suggested name: beagley-linux)"
echo "    DO NOT check 'Add a README' (we already have one)"
echo ""
echo " 2. Link and push:"
echo "    git remote add origin https://github.com/<YOUR_USER>/beagley-linux.git"
echo "    git push -u origin main"
echo ""
echo " 3. GitHub will ask for a token (not your password):"
echo "    https://github.com/settings/tokens → 'Generate new token (classic)'"
echo "    → check 'repo' → generate → use it as the password when pushing"
echo ""
