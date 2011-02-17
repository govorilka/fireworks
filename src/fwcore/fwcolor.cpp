#include "fwcolor.h"
#include "fwml.h"

FwColor::FwColor() :
    m_allChannel(0),
    m_blending(true)
{
}

FwColor::FwColor(quint32 argb) :
    m_alpha((argb >> 24) & 0xFF),
    m_red((argb >> 16) & 0xFF),
    m_green((argb >> 8) & 0xFF),
    m_blue(argb & 0xFF),
    m_blending(m_alpha != 255)
{
}

FwColor::FwColor(quint8 a, quint8 r, quint8 g, quint8 b) :
    m_alpha(a),
    m_red(r),
    m_green(g),
    m_blue(b),
    m_blending(m_alpha != 255)
{
}

quint32 FwColor::nameToRGBA(const QByteArray& name, bool* bOk)
{
    QByteArray lowerName = name.trimmed().toLower();
    if(!lowerName.isEmpty())
    {
        //SVG color
        QHash<QByteArray, quint32> colors;
        colors.insert("aliceblue",      0xFFF0F8FF);
        colors.insert("antuquewhite",   0xFFFAEBD7);
        colors.insert("aqua",           0xFF00FFFF);
        colors.insert("aqumarine",      0xFF7FFFD4);
        colors.insert("azure",          0xFFF0FFFF);

        colors.insert("beige",          0xFFF5F5DC);
        colors.insert("bisque",         0xFFFFE4C4);
        colors.insert("black",          0xFF000000);
        colors.insert("blanchedalmond", 0xFFFFEBCD);
        colors.insert("blue",           0xFF0000FF);
        colors.insert("blueviolet",     0xFF8A2BE2);
        colors.insert("brown",          0xFFA52A2A);
        colors.insert("burlywood",      0xFFDEB887);

        colors.insert("cadetblue",      0xFF5F9EA0);
        colors.insert("chartreuse",     0xFF7FFF00);
        colors.insert("chocolate",      0xFFD2691E);
        colors.insert("coral",          0xFFFF7F50);
        colors.insert("cornflowerblue", 0xFF6495ED);
        colors.insert("cornsilk",       0xFFFFF8DC);
        colors.insert("crimson",        0xFFDC143C);
        colors.insert("cyan",           0xFF00FFFF);

        colors.insert("darkblue",       0xFF00008B);
        colors.insert("darkcyan",       0xFF008B8B);
        colors.insert("darkgoldenrod",  0xFFB8860B);
        colors.insert("darkgray",       0xFFA9A9A9);
        colors.insert("darkgreen",      0xFF006400);
        colors.insert("darkhaki",       0xFFBDB76B);
        colors.insert("darkmagenta",    0xFF8B008B);
        colors.insert("darkolivegreen", 0xFF556B2F);
        colors.insert("darkorange",     0xFFFF8C00);
        colors.insert("darkorchid",     0xFF9932CC);
        colors.insert("darkred",        0xFF8B0000);
        colors.insert("darksalmon",     0xFFE9967A);
        colors.insert("darkseagreen",   0xFF8FBC8F);
        colors.insert("darkslateblue",  0xFF483D8B);
        colors.insert("darkslategray",  0xFF2F4F4F);
        colors.insert("darkurquoise",   0xFF00CED1);
        colors.insert("darkviolet",     0xFF9400D3);
        colors.insert("deeppink",       0xFFFF1493);
        colors.insert("deepskyblue",    0xFF00BFFF);
        colors.insert("dimgray",        0xFF696969);
        colors.insert("dimgrey",        0xFF696969);
        colors.insert("dodgerblue",     0xFF1E90FF);

        colors.insert("firebrick",      0xFFB22222);
        colors.insert("floralwhite",    0xFFFFFAF0);
        colors.insert("forestgreen",    0xFF228B22);
        colors.insert("fuchsia",        0xFFFF00FF);

        colors.insert("gainsboro",      0xFFDCDCDC);
        colors.insert("ghostwhite",     0xFFF8F8F8);
        colors.insert("gold",           0xFFFFD700);
        colors.insert("goldenrod",      0xFFDAA520);
        colors.insert("gray",           0xFF808080);
        colors.insert("grey",           0xFF808080);
        colors.insert("green",          0xFF008000);
        colors.insert("greenyellow",    0xFFADFF2F);

        colors.insert("honeydew",       0xFFF0FFF0);
        colors.insert("hotpink",        0xFFFF69B4);

        colors.insert("indianred",      0xFFCD5C5C);
        colors.insert("indigo",         0xFF4B0082);
        colors.insert("ivory",          0xFFFFFFF0);

        colors.insert("khaki",          0xFFF0E68C);

        colors.insert("lavender",       0xFFE6E6FA);
        colors.insert("lavenderblush",  0xFFFFF0F5);
        colors.insert("lawngreen",      0xFF7CFC00);
        colors.insert("lemonchiffon",   0xFFFFFACD);
        colors.insert("lightblue",      0xFFADD8E6);
        colors.insert("lightcoral",     0xFFF08080);
        colors.insert("lightcyan",      0xFFE0FFFF);
        colors.insert("lightgoldenrodyellow",  0xFAFAD2);
        colors.insert("lightgray",      0xFFD3D3D3);
        colors.insert("lightgreen",     0xFF9CEE90);
        colors.insert("lightgrey",      0xFFD3D3D3);
        colors.insert("lightpink",      0xFFFFB6C1);
        colors.insert("lightsalmon",    0xFFFFA07A);
        colors.insert("lightseagreen",  0xFF20B2AA);
        colors.insert("lightskyblue",   0xFF87CEFA);
        colors.insert("lightslategray", 0xFF778899);
        colors.insert("lightslategrey", 0xFF778899);
        colors.insert("lightsteelblue", 0xFFB0C4DE);
        colors.insert("lightyellow",    0xFFFFFFE0);
        colors.insert("lime",           0xFF00FF00);
        colors.insert("limegreen",      0xFF32CD32);
        colors.insert("linen",          0xFFFAF0E6);

        colors.insert("magenta",        0xFFFF00FF);
        colors.insert("maroon",         0xFF800000);
        colors.insert("mediumaquamarine",  0xFF66CDAA);
        colors.insert("mediumblue",     0xFF0000CD);
        colors.insert("mediumorchid",   0xFFBA55D3);
        colors.insert("mediumpurple",   0xFF9370DB);
        colors.insert("mediumseagreen", 0xFF3CB371);
        colors.insert("mediumslateblue",0xFF7B68EE);
        colors.insert("mediumspringgreen", 0xFF00FA9A);
        colors.insert("mediumturquoise",0xFF48D1CC);
        colors.insert("mediumvioletred",0xFFC71585);
        colors.insert("midnightblue",   0xFF191970);
        colors.insert("mintcream",      0xFFF5FFFA);
        colors.insert("mistyrose",      0xFFFFE4E1);
        colors.insert("moccasin",       0xFFFFE4B5);

        colors.insert("navajowhite",    0xFFFFDEAD);
        colors.insert("navy",           0xFF000080);

        colors.insert("oldlace",        0xFFFDF5E6);
        colors.insert("olive",          0xFF808000);
        colors.insert("olivedrab",      0xFF6B8E23);
        colors.insert("orange",         0xFFFFA500);
        colors.insert("orangered",      0xFFFF4500);
        colors.insert("orchid",         0xFFDA70D6);

        colors.insert("palegoldenrod",  0xFFEEE8AA);
        colors.insert("palegreen",      0xFF98FB98);
        colors.insert("paleturquoise",  0xFFAFEEEE);
        colors.insert("palevioletred",  0xFFDB7093);
        colors.insert("papayawhip",     0xFFFFEFD5);
        colors.insert("peachpuff",      0xFFFFDAB9);
        colors.insert("peru",           0xFFCD853F);
        colors.insert("pink",           0xFFFFC0CB);
        colors.insert("plum",           0xFFDDA0DD);
        colors.insert("powderblue",     0xFFB0E0E6);
        colors.insert("purple",         0xFF800080);

        colors.insert("red",            0xFFFF0000);
        colors.insert("rosybrown",      0xFFBC8F8F);
        colors.insert("royalblue",      0xFF4169E1);

        colors.insert("saddlebrown",    0xFF8B4513);
        colors.insert("salmon",         0xFFFA8072);
        colors.insert("sandybrown",     0xFFF4A460);
        colors.insert("seagreen",       0xFF2E8B57);
        colors.insert("seashell",       0xFFFFF5EE);
        colors.insert("sienna",         0xFFA0522D);
        colors.insert("silver",         0xFFC0C0C0);
        colors.insert("skyblue",        0xFF87CEEB);
        colors.insert("slateblue",      0xFF6A5ACD);
        colors.insert("slategray",      0xFF708090);
        colors.insert("slategrey",      0xFF708090);
        colors.insert("snow",           0xFFFFFAFA);
        colors.insert("springgreen",    0xFF00FF7F);
        colors.insert("steelblue",      0xFF4682B4);

        colors.insert("tan",            0xFFD2B48C);
        colors.insert("teal",           0xFF008080);
        colors.insert("thistle",        0xFFD8BFD8);
        colors.insert("tomato",         0xFFFF6347);
        colors.insert("turquoise",      0xFF40E0D0);

        colors.insert("violet",         0xFFEE82EE);

        colors.insert("wheat",          0xFFF5DEB3);
        colors.insert("white",          0xFFFFFFFF);
        colors.insert("whitesmoke",     0xFFF5F5F5);

        colors.insert("yellow",         0xFFFFFF00);
        colors.insert("yellowgreen",    0xFF9ACD32);

        colors.insert("transparent",    0x00000000);

        if(colors.contains(lowerName))
        {
            (*bOk) = true;
            return colors.value(lowerName);
        }
    }

    (*bOk) = false;
    return 0;
}
