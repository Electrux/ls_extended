# ls_extended

[![Build Status](https://api.cirrus-ci.com/github/Electrux/ls_extended.svg?branch=master)](https://cirrus-ci.com/github/Electrux/ls_extended)

ls with coloring and icons

To use this program, one **needs** to have the [Nerd Fonts](https://github.com/ryanoasis/nerd-fonts) installed since it utilizes those fonts for the icons.

I recommend the *Mononoki Nerd font* for the characters, and *Overpass Mono Nerd font* for the icons if possible
(although most terminals don't permit use of separate fonts for Non-ASCII characters).

## Example

![ls_extended](/images/ls_extended.png)

## Usage

The basic usage is similar to the native *ls* command, and for the options, take a look at:

`ls_extended --help`

Where ls_extended is the binary of this program which must be built.

## Installation

### Arch Linux

This project is available on AUR under the name `ls_extended`.

### With CCP4M

The project uses CCP4M build system which is located [here](https://github.com/Electrux/ccp4m).
The installation instructions for CCP4M are in its own repository.

Once it is installed, just use `ccp4m project build` to build the code. The executable will be generated in the `bin/` directory of the project and you can then copy it to your PATH (`/usr/bin/`, `/usr/local/bin/` etc).


### Without CCP4M

To install without CCP4M, execute the following shell script in the project's root directory (This will not compile the tests though):

`./build.sh`

This will generate an executable in bin directory named `ls_extended`. Just copy it to your PATH.
