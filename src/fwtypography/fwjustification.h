#ifndef FIREWORKS_JUSTIFICATION_H
#define FIREWORKS_JUSTIFICATION_H

#include "fwtypography/fwtextstring.h"
#include "fwtypography/fwfont.h"

#include <QtCore/qrect.h>
#include <QtCore/qvector.h>

class FwJustification
{
public:

    enum State
    {
        Error,
        ProcessWord,
        ProcessLetter,
    };

    enum TextCommand
    {
        C_AddWord,
        C_NewLine,
        C_EndProcess,
    };

    enum CommandWidth
    {
        CW_NewLine = 3,
    };

    FwJustification();

    inline QRect blockRect() const;
    inline void setBlockRect(const QRect& blockRect);

    inline FwFont font() const;
    void setFont(const FwFont& font);

    QVector<FwTextString> processing(const QString& text);

    bool validateData() const;

protected:
    FwTextString nextString(QString& text) const;

    QList<FwTextString> processingWord(const QString& text);

    TextCommand nextWord(QString& text, QString& word, int& wordWidth) const;

private:
    FwFont m_font;
    int stringHeight;
    int stringAscender;
    int stringDescender;
    int spaceWidth;

    int blockWidth;
    int maxLineWidth;
    int minLineWidth;

    QRect m_blockRect;
    State state;
};

QRect FwJustification::blockRect() const
{
    return m_blockRect;
}

void FwJustification::setBlockRect(const QRect& blockRect)
{
    m_blockRect = blockRect;
}

FwFont FwJustification::font() const
{
    return m_font;
}

#endif //FIREWORKS_JUSTIFICATION_H
