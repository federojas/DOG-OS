#!/bin/bash
qemu-system-x86_64  -hda Image/x64BareBonesImage.qcow2  -cpu qemu64,mmx=on,sse=on,sse2=on,sse3=on,sse4.2=on,sse4.1=on,aes=on,pclmulqdq=on -m 512 -rtc base=localtime
#qemu-system-x86_64.exe: warning: TCG doesn't support requested feature: CPUID.01H:ECX.fma [bit 12]
#qemu-system-x86_64.exe: warning: TCG doesn't support requested feature: CPUID.01H:ECX.avx [bit 28]
#qemu-system-x86_64.exe: warning: TCG doesn't support requested feature: CPUID.01H:ECX.f16c [bit 29]
#qemu-system-x86_64.exe: warning: TCG doesn't support requested feature: CPUID.07H:EBX.avx2 [bit 5]
#qemu-system-x86_64.exe: warning: TCG doesn't support requested feature: CPUID.07H:ECX.vaes [bit 9]
#qemu-system-x86_64.exe: warning: TCG doesn't support requested feature: CPUID.07H:ECX.vpclmulqdq [bit 10]