# bklt

Quick hacky fix for MacBook backlight when using unofficial replacement display.
Tested on MacBookPro14,2 running macOS 13.6.4

## Building
`gcc -obklt bklt.c -framework IOKit -framework ApplicationServices`

## Usage
`./bklt <brightness>`

Where brightness is between 0 and 1.

## Info
AppleBacklight.kext maps the brightness control increments to the raw values for each monitor (in info.plist iirc). My monitor is either missing here since Big Sur or disabled because of an invalid / non-matching display.

Reasons my replacement display might not be recognised:
`IOService:/AppleACPIPlatformExpert/PCI0@0/AppleACPIPCI/IGPU@2/AppleIntelFramebuffer@0/display0/AppleBacklightDisplay`
 - DisplaySerialNumber = 0
 - DisplayProductID = 0xa039. Looking at linux-hardware.org dumps, this ID shows up on MacBookAir8,1 not MacBookPro14,2. The correct one for my MacBook is probably 0xa033

This tool sets bklt value in IODisplayParameters, which works well for me.
