#include "fwjustification.h"

FwJustification::FwJustification() :
    m_blockRect(0, 0, 0, 0),
    stringHeight(0),
    stringAscender(0),
    stringDescender(0),
    spaceWidth(0),
    blockWidth(0),
    maxLineWidth(0),
    minLineWidth(0),
    state(Error)
{
}

void FwJustification::setFont(const FwFont& font)
{
    m_font = font;
    stringHeight = qAbs(m_font.height());
    stringAscender = qAbs(m_font.ascender());
    stringDescender = qAbs(m_font.descender());
    spaceWidth = m_font.stringWidth(" ");
}

bool FwJustification::validateData() const
{
    if(m_font.isNull() ||
       stringAscender == 0 ||
       stringDescender == 0 ||
       stringHeight == 0 ||
       spaceWidth == 0 ||
       m_blockRect.height() <= 0 ||
       m_blockRect.width() <= 0 ||
       stringHeight > m_blockRect.height())
    {
        return false;
    }

    return true;
}

QVector<FwTextString> FwJustification::processing(const QString& text)
{
    if(!validateData())
    {
        return QVector<FwTextString>();
    }

    QString simplifiedText = text.simplified();
    if(simplifiedText.isEmpty())
    {
        return QVector<FwTextString>();
    }

    state = ProcessWord;

    blockWidth = m_blockRect.width();

    QList<FwTextString> strings = processingWord(simplifiedText);
    if(!strings.isEmpty())
    {
        qreal wMax = maxLineWidth;
        qreal wMin = minLineWidth;
        if((wMax / wMin) > 1.15)
        {
            blockWidth = qRound((wMax + wMin) / 2);
            strings = processingWord(simplifiedText);
        }
    }

    return strings.toVector();
}

QList<FwTextString> FwJustification::processingWord(const QString& text)
{
    QString str = text;

    minLineWidth = blockWidth;
    maxLineWidth = 0;

    QList<FwTextString> strings;

    int y = m_blockRect.y();
    do
    {
        FwTextString string = nextString(str);
        if(!string.isEmpty())
        {
            string.m_pos.setX(m_blockRect.x());
            string.m_pos.setY(y + stringHeight - stringDescender);
            strings.append(string);

            if(!string.isEndBreak())
            {
                if(string.width() > maxLineWidth)
                {
                    maxLineWidth = string.width();
                }
                if(string.width() < minLineWidth && !str.isEmpty())
                {
                    minLineWidth = string.width();
                }
            }
        }

        if(state == Error)
        {
            return QList<FwTextString>();
        }

        y += stringHeight;
        if((y + stringHeight) >= m_blockRect.bottom())
        {
            break;
        }
    }
    while(!str.isEmpty());

    return strings;
}

FwTextString FwJustification::nextString(QString& text) const
{
    FwTextString newString;

    int stringWidth = 0;
    do
    {
        if(text[0] == ' ')
        {
            text.remove(0, 1);
        }

        QString word;
        int wordWidth = 0;
        if(nextWord(text, word, wordWidth) == FwJustification::C_NewLine)
        {
            newString.m_endBreak = true;
            break;
        }

        stringWidth += wordWidth;

        if(newString.m_value.isEmpty())
        {
            newString.m_value = word;
        }
        else
        {
            stringWidth += spaceWidth;
            if(stringWidth <= blockWidth)
            {
                newString.m_value += (' ' + word);
            }
            else
            {
                text = word + text;
                break;
            }
        }

        newString.m_width = stringWidth;
    }
    while(!text.isEmpty() && stringWidth < blockWidth);

    newString.m_utf8 = newString.m_value.toUtf8();
    return newString;
}

FwJustification::TextCommand FwJustification::nextWord(QString& text, QString& word, int& wordWidth) const
{
    int wordIndex = text.indexOf(' ');
    word = (wordIndex == -1 ? text : text.left(wordIndex));

    int indexBr = word.indexOf("\\br");
    if(indexBr != -1)
    {
        if(indexBr > 0)
        {
            word = word.left(indexBr);
        }
        else
        {
            text = text.right(text.length() - FwJustification::CW_NewLine);
            word = "";
            wordWidth = 0;
            return FwJustification::C_NewLine;
        }
    }

    word.replace("\\\\", "\\");
    wordWidth = m_font.stringWidth(word);
    text = text.right(text.length() - word.length());
    return FwJustification::C_AddWord;
}
