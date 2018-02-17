# AOS - AVR Operating System
## Overview
**AOS is a basic operating system for 8-bit Atmel AVR microcontrollers**. These
microcontrollers use the Harvard architecture, meaning that data cannot be
executed as code; due to this, AOS operates by executing programs written in
AOS bytecode (ABC).

AOS consists of three main components:

  - The AOS Virtual Machine (AVM) which executes ABC. 
  - The kernel, which configures an AVM environment and manages its resources.
  - The *shell.abc* program which provides a user interface for AOS.

## Notes

  - AOS uses a basic filesystem which is currently stored on the AVR's EEPROM
  memory. In the future, AOS will support storing the filesystem on external 
  flash instead. EEPROM has a relatively low write lifetime (approximately
  100,000 writes), meaning that **bugs in AOS' filesystem could instantly and
  permanently destroy your AVR's EEPROM.** Use a simulator where possible!

  - Upon booting, AOS loads the file at the beginning of its filesystem and
  executes it. (This file is called the *rootfile*.) You can find valid
  filesystems with rootfiles in the *Filesystem dumps* folder of this repo.
  Read the source to find out how files are structured if you'd like to make
  your own.

  - The *Tools* folder contains a basic ABC assembler written in Ruby. It's
  very simple and has absolutely no error checking, but if you're careful when
  writing programs it works well.

  - This has only been tested on an ATmega328P. It will probably work on higher-
  end chips, but is unlikely to work on lower-end ones unless `PAGE_COUNT` is
  reduced due to SRAM requirements.

## Notices

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
