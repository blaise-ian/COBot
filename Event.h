#ifndef EVENT_H_INCLUDED
#define EVENT_H_INCLUDED

#include <vector>

class CEvent
{
public:
    class CBase
    {
    public:
        virtual void operator()(void* sender, void** args) = 0;
    };
private:
    std::vector<CBase*> _callbacks;
public:
    template <class Class>
    class T: public CBase
    {
    public:
        typedef void (Class::*Func)(void*, void**);
    private:
        Class* _obj;
        Func   _func;
    public:
        T(Class* obj, Func func): _obj(obj), _func(func) {}

        void operator()(void* sender, void** args)
        {
            (_obj->*_func)(sender, args);
        }
    };

    class S: public CBase
    {
    public:
        typedef void (*Func)(void*, void**);
    private:
        Func _func;
    public:
        S(Func func): _func(func) {}

        void operator()(void* sender, void** args)
        {
            (*_func)(sender, args);
        }
    };

    CEvent()
    {
    }

    ~CEvent()
    {
        for (int i = 0; i < (int)_callbacks.size(); ++i)
            delete _callbacks[i];
    }

    CEvent& operator += (CBase* base)
    {
        _callbacks.push_back(base);
        return *this;
    }

    void operator()(void* sender, void** args)
    {
        for (int i = 0; i < (int)_callbacks.size(); ++i)
        {
            (*_callbacks[i])(sender, args);
        }
    }
};

#endif // EVENT_H_INCLUDED
