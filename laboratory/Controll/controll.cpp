#include <QtGui/qlineedit.h>
#include <QtGui/qcalendarwidget.h>
#include <QtGui/qpushbutton.h>
#include <QtGui/QHBoxLayout>
#include <QtGui/QVBoxLayout>

#include "controll.h"

Controll::Controll(QWidget *parent) :
    BaseClass(parent)
{
    m_editPole = new QLineEdit;
    m_editPole->setText(QDate::currentDate().toString("dd.MM.yyyy"));
    connect(m_editPole, SIGNAL(textChanged(QString)), this, SLOT(textChanged(QString)));

    m_paletteForErr = m_editPole->palette();
    m_standartPalette = m_editPole->palette();
    m_paletteForErr.setColor(QPalette::Text, QColor(255,0,0));

    QPushButton* button1 = new QPushButton("...");
    button1->setMinimumSize(40,20);
    connect(button1, SIGNAL(clicked()), this, SLOT(clicked()));

    m_calendar = new QCalendarWidget(this);
    m_calendar->setWindowFlags(Qt::Popup);
    connect(m_calendar, SIGNAL(clicked(QDate)), this, SLOT(setedDateToText(QDate)));
    m_calendar->hide();

    QHBoxLayout* horisontalLayout = new QHBoxLayout;
    horisontalLayout->setSpacing(0);
    horisontalLayout->addWidget(m_editPole);
    horisontalLayout->addWidget(button1);
    setLayout(horisontalLayout);
}

void Controll::clicked()
{
    if(m_calendar->isHidden())
    {
        m_calendar->move(pos().x(),
                                        pos().y() + size().height() + 15);
        changeDate();
    } else
    {
        m_calendar->hide();
    }
}

void Controll::changeDate()
{
        QDate date;
        date = QDate::fromString(m_editPole->text(), "dd.MM.yyyy");
        if(!date.isValid())
        {
            date = QDate::currentDate();
            m_editPole->setPalette(m_paletteForErr);
        } else
        {
            if(m_editPole->palette() == m_paletteForErr)
            {
                m_editPole->setPalette(m_standartPalette);
            }
        }
        m_calendar->setSelectedDate(date);
        m_calendar->show();
}

void Controll::setedDateToText(const QDate &date)
{
    QDate current_date;
    current_date = QDate::fromString(m_editPole->text(), "dd.MM.yyyy");
    if(!current_date.isValid() || current_date != date)
    {
        m_editPole->setText(date.toString("dd.MM.yyyy"));
    }
}

void Controll::textChanged(const QString &text)
{
    if(!m_calendar->isHidden())
    {
        QDate date = QDate::fromString(text, "dd.MM.yyyy");
        if(date.isValid())
        {
            m_editPole->setPalette(m_standartPalette);
            m_calendar->setSelectedDate(date);
        }
        else
        {
            m_editPole->setPalette(m_paletteForErr);
        }
    }
}
