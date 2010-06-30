PD-3000 USB Display Pole
========================

This is a small program which will write to the display pole at my arbitrary intervals.  Uses libusb.

I added it to udev since it wasn't detected right away:

Udev magic::
    SUBSYSTEM=="usb" ATTRS{idVendor}=="0fa8", ATTRS{idProduct}=="a030", \
                    MODE="0666" SYMLINK+="pole_display"
    
    if(LIBUSB_HAS_DETACH_KERNEL_DRIVER_NP)
    {
        // Detach kernel driver (usbhid) from device interface. (Linux hack)
        usb_detach_kernel_driver_np(pole, 0);
        usb_detach_kernel_driver_np(pole, 1);
    }

To compile, just run *gcc pd-3000.c -lusb*
