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

    inline bool wordWrap() const;
    inline void setWordWrap(bool enable);

    QVector<FwTextString> processing(const QString& text, bool fixedHeight);

    bool validateData() const;

protected:
    FwTextString nextString(QString& text) const;

    QVector<FwTextString> processingWord(const QString& text, bool fixedHeight);

    TextCommand nextWord(QString& text, QString& word, int& wordWidth) const;

    QVector<FwTextString> singleLineProcessing(const QString& text);

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

    bool m_wordWrap;
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

bool FwJustification::wordWrap() const
{
    return m_wordWrap;
}

void FwJustification::setWordWrap(bool enable)
{
    m_wordWrap = enable;
}

#endif //FIREWORKS_JUSTIFICATION_H
