//Майерса
class Singleton
{
private:
	Singleton();
	Singleton(const Singleton&);
	Singleton& operator=(Singleton&);
public:
	static Singleton& Intance()
	{
		static Singleton m_Singleton;
		return m_Singleton;
	}
}