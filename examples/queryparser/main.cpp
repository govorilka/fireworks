#include <QtCore/qcoreapplication.h>
#include <QtCore/qdebug.h>

#include "fwdb/fwpg.h"

int main(int argc, char **argv)
{
    QCoreApplication app(argc, argv); 

    FwPg::TokenVector tokens;
    FwPg::parseQuery("select * from  ?? where id = ?1 and name=?2", tokens);


    foreach(FwPg::QueryToken token, tokens)
    {
        qDebug() << token.param << token.value;
    }
    return 0;
}
