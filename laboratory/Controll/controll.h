#ifndef CONTROLL_H
#define CONTROLL_H

#include <QWidget>
#include <QtCore/qdatetime.h>

class QCalendarWidget;
class QLineEdit;
class QPushButton;

class Controll : public QWidget
{
    Q_OBJECT
    typedef QWidget BaseClass;
public:
    explicit Controll(QWidget *parent = 0);

private:
    void changeDate();

    QCalendarWidget* m_calendar;
    QLineEdit* m_editPole;

    QPalette m_paletteForErr;
    QPalette m_standartPalette;

public slots:
    void clicked();
    void textChanged(const QString& text);
    void setedDateToText(const QDate& date);

};

#endif // CONTROLL_H
