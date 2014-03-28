#include <linux/i2c-dev.h>
#include <iostream>
#include <sys/ioctl.h>
#include <unistd.h>
#include <fcntl.h>

#include "I2CBus.h"

I2CBus::I2CBus(const char* const fileName, const unsigned char address)
{
	count=0;
	
    if ((fd = open(fileName, O_RDWR)) < 0) {                    // Open port for reading and writing
        printf("Failed to open i2c port\n");
        exit(1);
    }
    if (ioctl(fd, I2C_SLAVE, address) < 0) {                    // Set the port options and set the address of the device we wish to speak to
        printf("Unable to get bus access to talk to slave\n");
        exit(1);
    }
}

I2CBus::~I2CBus()
{
    if ((close(fd)) < 0) {                    // Open port for reading and writing
        printf("Failed to close i2c port\n");
        exit(1);
    }
	std::cout<<"Destructor I2CBus\n";
}
	
void I2CBus::write_to_buffer(const unsigned char byte)
{
	buf[count]=byte;
	count++;
};

void I2CBus::send_buffer()
{
	int result=write(fd, buf, count);
    if (result != count) {                             // Send register we want to read from
        printf("Error writing to i2c slave=%d\n",result);
        exit(1);
    }
	/*printf("Send bytes=%d\n",count);
    for(int i=0;i<count;i++) printf(" %X",buf[i]);
	printf("\n");*/

	count=0;
};
