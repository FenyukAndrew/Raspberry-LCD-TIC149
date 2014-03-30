//Классическая реализация Singleton
class Singleton_Standart
{
private:
    Singleton_Standart();
    Singleton_Standart(const Singleton_Standart&);
    Singleton_Standart& operator=(Singleton_Standart&);
    static Singleton_Standart* p_Singleton_Standart;
public:
    static Singleton_Standart* Instance()
    {
        if (!p_Singleton_Standart)
        {
            p_Singleton_Standart=new Singleton_Standart();
        }
        return p_Singleton_Standart;
    }
};

//Singleton Мэйерса
class Singleton_M
{
private:
    Singleton_M();
    Singleton_M(const Singleton_M&);
    Singleton_M& operator=(Singleton_M&);
public:
    static Singleton_M& Intance()
    {
        static Singleton_M m_Singleton;
        return m_Singleton;
    }
};

//Улучшенный singleton
class Singleton_Best;

class Singleton_Best_Destroyer
{
    private:
    Singleton_Best* p_Singleton_Best;
    public:
    void initialize(Singleton_Best* m_Singleton_Best)
    {
        p_Singleton_Best=m_Singleton_Best;
    };
    ~Singleton_Best_Destroyer();
};

class Singleton_Best
{
    private:
    static Singleton_Best* m_Singleton_Best;
    static Singleton_Best_Destroyer m_Singleton_Best_Destroyer;
    protected:
    Singleton_Best();
    Singleton_Best(const Singleton_Best&);
    Singleton_Best& operator=(Singleton_Best&);
    friend class Singleton_Best_Destroyer;
    public:
    Singleton_Best& Intance()
    {
        if (!m_Singleton_Best)
        {
            m_Singleton_Best=new Singleton_Best();
            m_Singleton_Best_Destroyer.initialize(m_Singleton_Best);
        }
        return *m_Singleton_Best;
    }
};

//На основе шаблонов с подсчетом ссылок
template <class T>
class Singleton
{
  static T* _self;
  static int _refcount;
protected:
  Singleton(){}
  virtual ~Singleton(){_self=nullptr;}
public:
  static T* Instance();
  void FreeInst();
};

template <class T>
T*  Singleton<T>::_self = nullptr;

template <class T>
int  Singleton<T>::_refcount=0;

template <class T>
T*  Singleton<T>::Instance()
{
  if(!_self)
    _self=new T;
  _refcount++;
  return _self;
}

template <class T>
void  Singleton<T>::FreeInst()
{
  if(--_refcount==0)
    delete this;
}
