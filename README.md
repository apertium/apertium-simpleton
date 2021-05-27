# Apertium Simpleton
See https://wiki.apertium.org/wiki/Apertium_Simpleton_UI

This tool is designed for Windows and macOS.
It may work on Linux, but no real effort has been made to do it correctly.

## Installation on Debian/Ubuntu and derivatives
### Via nightly apt-get repo
- `wget https://apertium.projectjj.com/apt/install-nightly.sh -O - | sudo bash`
- `sudo apt-get install apertium-simpleton`

### From source
- `sudo apt-get install --no-install-recommends build-essential qt5-default qt5-qmake qtbase5-dev git`
- `git clone https://github.com/apertium/apertium-simpleton`
- `cd apertium-simpleton`
- `qmake`
- `make -j4`
- `sudo make install`

If you do build from source, you will need all the Apertium core tools either also from source or from the nightly repos:
- `wget https://apertium.projectjj.com/apt/install-nightly.sh -O - | sudo bash`
- `sudo apt-get install binutils apertium apertium-anaphora apertium-lex-tools apertium-recursive apertium-separable cg3 hfst lttoolbox`
