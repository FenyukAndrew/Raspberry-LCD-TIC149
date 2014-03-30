#include "Singleton.h"

Singleton_Standart* Singleton_Standart::p_Singleton_Standart=nullptr;

Singleton_Best_Destroyer::~Singleton_Best_Destroyer()
{
    delete p_Singleton_Best;
}
Singleton_Best* Singleton_Best::m_Singleton_Best=nullptr;


