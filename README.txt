������ ��� ������ � ������� �� C++

����� ���� ����� ����������� ����� �� � ���� ������� - � � ���� �����, ������������ � ������ (����� ������ ����������). �� ����������� ����� ���������. ����������� �������� ��-������ �� ������ - �.�. ��� �������, ������������ ����������.

class View � ����������� ���������� ������ ������ � �����, �����
� ����� ������ - �.�. �� �������� ������ (������� � ��������� �����?), ������� ��� ����������� ����� ������ � ������������ ������.
����� ����� � ����� View (���) ���������� ��� ������������ �� ������ ������ ��� ��������� (������� �������� ������)
View ���� ����� ������������ �� View - ��� ������� ����� ������� ����� � ������ - �������� � ����� ����� ����� usleep(40) - 25 ������ � �������

//������������ ������� - ��������� (1 ������ � ������ 8 ��������) ����� ������� ������ ����
unsigned char* row_screen[height_LCD/8];
class row_LCD
{
row_LCD(int column) {new data(column);};
~row_LCD(int column) {delete data(column);};
public:
write(int column);
read(int column) const;
const* get_row(int i) const;//���������� ���������
private:
unsigned char* data;
}

abstuct class ProtoView
class View : public ProtoView
{
row_LCD* rows[height_LCD/8];
}
����� ������� ������ ������� row_lcd - ������� ����� ��������, ���� ����� ������������ - ������� isolated?



***** ������

���������� ������ �����
gcc main.cc -o main

� �������������� std ���������� ������ ���
g++ main.cc -o main

������������ ��������� ������
classA.h:1:1: error: unknown type name �class�
classA.h:2:1: error: expected �=�, �,�, �;�, �asm� or �__attribute__� before �{� token
�������: ��������� ��������������
���-�� ����� ��������� �������� ������ - ����� ReBuild All?

������ sudo ./main


const int* - ����� �� * - ������
int* const - ������ �� * - ����������� ���������
http://books.google.ru/books?id=uPvvxRYj-NIC&pg=PA380&lpg=PA380&dq=C%2B%2B+const+%D1%81%D0%BB%D0%B5%D0%B2%D0%B0+%D0%BE%D1%82+*&source=bl&ots=K5mme-V8bM&sig=tPk_0Vzqx5yivPZRbK9VykQnijY&hl=ru&sa=X&ei=V2n8UsHLF_H44QTw0ICoAQ&ved=0CC8Q6AEwAQ#v=onepage&q=C%2B%2B%20const%20%D1%81%D0%BB%D0%B5%D0%B2%D0%B0%20%D0%BE%D1%82%20*&f=false


//���������� - ��������� 5-5.2
������ 5.3 - �� ����� ���� ����� ��������� ����� 3 - �.�. ��� �������� �� ��������� ����������� ���. 70


�������� ������:
���������� sudo make
������ sudo ./prog


