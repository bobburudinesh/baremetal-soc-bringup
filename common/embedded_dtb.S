.section .rodata.dtb, "a"
.global _dtb_begin
.global _dtb_end
_dtb_begin:
    .incbin "boards/rpi5/bcm2712-rpi-5-b.dtb"
_dtb_end:
