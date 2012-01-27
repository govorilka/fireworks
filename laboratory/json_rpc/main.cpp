#include <QtCore/QCoreApplication>
#include <QtCore/qdebug.h>
#include <QtNetwork/QNetworkAccessManager>

#include "cjson/cJSON.h"

class JsonRpc : public QObject
{
    Q_OBJECT
    typedef QObject BaseClass;

public:
    explicit JsonRpc(QObject* parent);
    virtual JsonRpc();

    void request(QString json);

private:
    QNetworkAccessManager* m_networkManager;

    slots:
        void replyFinished(QNetworkReply* reply);

};

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    cJSON* coreObject = cJSON_CreateObject();

    cJSON_AddNumberToObject(coreObject, "number", 13);
    cJSON_AddStringToObject(coreObject, "string", "text");

    char* text = cJSON_PrintUnformatted(coreObject);
    qDebug() << text;

    cJSON_Delete(coreObject);
    coreObject = 0;

    ///////////////////////////////////////////////////////////////

    cJSON* inputObject = cJSON_Parse(text);

    free(text);
    text = 0;


    text = cJSON_Print(inputObject);
    qDebug() << text;

    free(text);
    text = 0;

    cJSON_Delete(inputObject);
    inputObject = 0;

    ////////////////////////////////////




    
    return 0;
}
