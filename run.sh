#!/bin/bash

if [[ "$1" = "gdb" ]]; then

 qemu-system-x86_64 -s -S -hda Image/x64BareBonesImage.qcow2 -m 512 -d int

else

 qemu-system-x86_64 -hda Image/x64BareBonesImage.qcow2 -m 512

fi


# qemu-system-x86_64 -hda Image/x64BareBonesImage.qcow2 -m 512 -audiodev pa,id=snd0 -device ich9-intel-hda  -device hda-duplex,audiodev=snd0
# qemu-system-x86_64 -hda Image/x64BareBonesImage.qcow2 -m 512
