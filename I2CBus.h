class I2CBus
{
public:
	I2CBus(const char *fileName, unsigned char address);
	~I2CBus();
	
	void write_to_buffer(unsigned char byte);
	void send_buffer();

	
	
//Возможна проблема - перед использованием буфер не очищается = в нём могут остаться из-за ошибки неиспользованные байты
	
private:
	unsigned char buf[2048];
	unsigned int count;
	
	//cont char *name_device;
	//unsigned char i2c_address;

	int fd;                                                     // File descrition
};
