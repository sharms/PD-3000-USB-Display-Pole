/*
Author: Steven Harms
To compile:  Make sure libusb is installed, then:
gcc pd-3000-example.c -lusb
./a.out 
*/
#include <stdio.h>
#include <sys/types.h>
#include <string.h>
#include <time.h>
#include <usb.h>

/* PD-3000 Vendor and product ids.  You can view all vendor / product ids on a 
system using 'lsusb'*/
#define LED_VENDOR_ID   0x0fa8
#define LED_PRODUCT_ID  0xa030

/* Used to sleep in while loop below, not needed for usb */
int __nsleep(const struct timespec *req, struct timespec *rem)  
{  
    struct timespec temp_rem;  
    if(nanosleep(req,rem)==-1)  
        __nsleep(rem,&temp_rem);  
    else  
        return 1;  
}  
  
/* Used to sleep in while loop below, not needed for usb */
int msleep(unsigned long milisec)  
{  
    struct timespec req={0},rem={0};  
    time_t sec=(int)(milisec/1000);  
    milisec=milisec-(sec*1000);  
    req.tv_sec=sec;  
    req.tv_nsec=milisec*1000000L;  
    __nsleep(&req,&rem);  
    return 1;  
}  

/* Send message to USB pole display.  10000 is the maximum response time before
timeout.  0x02 is the endpoint */
static void sendMessage (struct usb_dev_handle *handle, char *sendBuffer)
{
    usb_bulk_write(handle, 0x02, sendBuffer, strlen(sendBuffer), 10000);
}

/* Search USB for our device */
static struct usb_device *device_init(void)
{
    struct usb_bus *usb_bus;
    struct usb_device *dev;

    usb_init();
    usb_find_busses();
    usb_find_devices();

    for (usb_bus = usb_busses;
         usb_bus;
         usb_bus = usb_bus->next) {
        for (dev = usb_bus->devices;
             dev;
             dev = dev->next) {
            if ((dev->descriptor.idVendor
                  == LED_VENDOR_ID) &&
                (dev->descriptor.idProduct
                  == LED_PRODUCT_ID))
                return dev;
        }
    }
    return NULL;
}

int main(int argc, char **argv)
{
    struct usb_device *usb_dev;
    struct usb_dev_handle *usb_handle;
    int retval = 1;
    int i;
    char messageBuffer[512];

    usb_dev = device_init();
    if (usb_dev == NULL) {
        fprintf(stderr, "Device not foundn\n");
        goto exit;
    }

    usb_handle = usb_open(usb_dev);
    if (usb_handle == NULL) {
        fprintf(stderr, "Cannot open\n");
        goto exit;
    }

    usb_handle = usb_open(usb_dev);
    if (usb_handle == NULL) {
        fprintf(stderr, "Not able to claim the USB device\n");
        goto exit;
    }

    //sendMessage(usb_handle, messageBuffer);
    char cursor[] = { '|', '/', '-', '\\' };
    char brightness[] = { 0xFF, 0x60, 0x40, 0x20 };
    char changingMessage[512];

    for(i = 0; i < 5000; i++)
    {
        if(i == 4999)
                i = 0;

        if(i == 0) {
                strcpy(changingMessage, "Steven Harms      ");
        }

        if(i == 300) {
                strcpy(changingMessage, "Sr. Network Ingr");
        }


        if(i == 1000) {
                strcpy(changingMessage, "IN OFFICE TODAY ");
        }

        if(i == 1500) {
                strcpy(changingMessage, "9:00-5:00 EST   ");
        }

        if(i == 2000) {
                strcpy(changingMessage, "www.ubuntu.com  ");
        }

        if(i == 3000) {
                strcpy(changingMessage, ":-)             ");
        }

        int pos = i % 4;
        sprintf(messageBuffer, "%c%c%c%c%c%s", 0x14, 0x0D, 0x04, brightness[0], cursor[pos], changingMessage);
        sendMessage(usb_handle, messageBuffer);
        msleep(100);


    } 
    retval = 0;

exit:
    usb_close(usb_handle);
    return retval;
}
