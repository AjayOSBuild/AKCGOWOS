# Ensure you are in your project root
cd ~/nexus-os

# Stage all updated scripts, C++ code, and configurations
git add .

# Commit the changes with a descriptive message
git commit -m "feat: implement 16-bit graphics engine, bounds checking, and VBoxVGA support"

# Force the current branch to be named 'main'
git branch -M main

# Push to your remote repository (if you have one configured)
git push -u origin main