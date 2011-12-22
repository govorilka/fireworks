#include <QtCore/QCoreApplication>
#include <QtCore/qdebug.h>
#include <QtCore/qbytearray.h>

class A
{
public:

    void exec(bool b) throw (const QByteArray&);
    void begin() throw (const QByteArray&);
    void commit() throw (const QByteArray&);
    void rollback() throw (const QByteArray&);

};

void A::exec(bool b) throw (const QByteArray&)
{
    if(b)
    {
        throw QByteArray("A::exec(): exception");
    }
}

void A::begin() throw (const QByteArray&)
{
    qDebug() << "begin";
}

void A::commit() throw (const QByteArray&)
{
    throw QByteArray("123");
    qDebug() << "commit";
}

void A::rollback() throw (const QByteArray&)
{
    throw QByteArray("456");
    qDebug() << "rollback";
}

////////////////////////////////////////////////////////////////////////////////

class B
{
public:
    B(A* a) throw(const QByteArray&);
    ~B() throw();

    void commit() throw(const QByteArray&);
    inline bool begin() const throw(const QByteArray&);

private:
    A* m_a;
    bool m_begin;
};

B::B(A* a)  throw(const QByteArray&):
    m_a(a),
    m_begin(true)
{
    a->begin();
}

B::~B() throw()
{
    if(m_begin)
    {
        try
        {
            m_a->rollback();
        }
        catch(...)
        {
            qDebug() << "Rollback exception";
        }
    }
}

void B::commit() throw (const QByteArray&)
{
    if(m_begin)
    {
        m_begin = false;
        m_a->commit();
    }
}

bool B::begin() const throw (const QByteArray&)
{
    return m_begin;
}

#define transaction(a) for(B b(& a); b.begin(); b.commit())

////////////////////////////////////////////////////////////////////////////////

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);

    try
    {
        A a;
        transaction(a)
        {
            transaction(a) a.exec(false);
            a.exec(false);
        }
    }
    catch(const QByteArray& s)
    {
        qDebug() << s;
    }
    catch(...)
    {
        qDebug() << "FUCK";
    }



    return app.exec();
}
