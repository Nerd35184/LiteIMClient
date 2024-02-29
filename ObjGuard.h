#ifndef OBJGUARD_H
#define OBJGUARD_H

#include <QObject>


class ObjGuard
{
public:
    ObjGuard(QObject *obj)
    {
        this->addObj(obj, nullptr);
    }
    ObjGuard(int cap) : objPairs_(cap)
    {
    }

    void addObj(QObject *obj)
    {
        this->addObj(obj, nullptr);
    }

    void addObj(QObject *obj, std::function<void()> cleanFunc)
    {

        std::pair<QObject *, std::function<void()>> objPair(obj, cleanFunc);
        this->objPairs_.push_back(objPair);
    }

    virtual ~ObjGuard()
    {
        for (auto objPair : this->objPairs_)
        {
            if (objPair.first == nullptr)
            {
                continue;
            }
            if (objPair.first->parent() != nullptr)
            {
                continue;
            }
            qDebug("ObjGuard deleteLater %s", objPair.first->metaObject()->className());
            objPair.first->deleteLater();
            if (objPair.second == nullptr)
            {
                continue;
            }
            objPair.second();
        }
    }

private:
    std::vector<std::pair<QObject *, std::function<void()>>> objPairs_;
    bool forceDeleteLater_ = false;
};

class Defer
{
public:
    Defer(std::function<void()> callback):callback_(callback){}
    ~Defer(){
        if(this->callback_==nullptr){
            return ;
        }
        this->callback_();
    }
private:
    std::function<void()> callback_;
};

#endif // OBJGUARD_H
