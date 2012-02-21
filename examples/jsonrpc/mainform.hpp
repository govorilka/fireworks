#ifndef MAINFORM_HPP
#define MAINFORM_HPP

#include <QtGui/qwidget.h>

#include "fw/jsonrpc/jsonrpc.hpp"

namespace Ui
{
    class MainForm;
}

class QTreeWidgetItem;
class QNetworkAccessManager;

class MainForm : public QWidget
{
    Q_OBJECT
    
public:
    explicit MainForm(QWidget *parent = 0);
    ~MainForm();

    QByteArray requestText() const;
    void setRequestText(const QByteArray& text);

public slots:
    void resetRequest();
    void sendRequest();

private:
    void addNode(QTreeWidgetItem* parent, Fw::JSON::Node* node);

    Ui::MainForm *ui;
    QNetworkAccessManager* m_networkManager;
    Fw::JSON::RPC* m_jsonRpc;

private slots:
    void finish(const Fw::JSON::RPC::Response& r);
};

#endif // MAINFORM_HPP
