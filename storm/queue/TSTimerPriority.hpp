#include "storm/queue/CSBasePriority.hpp"

template <class T>
class TSTimerPriority : public CSBasePriority {
    public:
    // Member variables
    T m_val;

    // Member functions
    virtual bool Compare(CSBasePriority* a);
};

template <class T>
bool TSTimerPriority<T>::Compare(CSBasePriority* a) {
    return this->m_val - static_cast<TSTimerPriority<T>*>(a)->m_val <= 0;
}
