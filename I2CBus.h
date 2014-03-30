class I2CBus
{
public:
    I2CBus(const char* const fileName, const unsigned char address);
    ~I2CBus();

    void write_to_buffer(const unsigned char byte);
    void send_buffer();

    //Для непосредственных операциях в буфере
    unsigned char* get_currect_ptr_buffer()
    {
        return buf+count;
    }

//Буфер не очищается
    void reserve(unsigned short reserve_count)
    {
        count+=reserve_count;
    }

//Возможна проблема - перед использованием буфер не очищается = в нём могут остаться из-за ошибки неиспользованные байты

private:
    unsigned char buf[2048];
    unsigned short count;

    //cont char *name_device;
    //unsigned char i2c_address;

    int fd;                                                     // File descrition
};
