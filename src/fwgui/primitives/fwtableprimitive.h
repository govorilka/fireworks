#ifndef FIREWORKS_GRAPHICSTABLEITEM_H
#define FIREWORKS_GRAPHICSTABLEITEM_H

#include "fwgui/primitives/fwprimitivegroup.h"

class FwTablePrimitive;
class FwCellPrimitive;

class FwCellPrimitive : public FwRectPrimitive
{
    typedef FwRectPrimitive BaseClass;

public:
    friend class FwTablePrimitive;

    FwCellPrimitive(int row, int column, FwTablePrimitive* parent);

    inline int row() const;
    inline int column() const;

    inline int rowSpan() const;
    void setRowSpan(int span);

    inline int columnSpan() const;
    void setColumnSpan(int span);

private:
    int m_row;
    int m_column;
    int m_rowSpan;
    int m_columnSpan;
    bool needGeometryProcessed;
};

int FwCellPrimitive::row() const
{
    return m_row;
}

int FwCellPrimitive::column() const
{
    return m_column;
}

int FwCellPrimitive::rowSpan() const
{
    return m_rowSpan;
}

int FwCellPrimitive::columnSpan() const
{
    return m_columnSpan;
}

//////////////////////////////////////////////////////////////////////////////////

class FwTablePrimitive : public FwPrimitiveGroup
{
    typedef FwPrimitiveGroup BaseClass;

public:
    friend class FwCellPrimitive;

    FwTablePrimitive(const QByteArray& name, FwPrimitiveGroup* parent);

    void apply(FwMLObject *object);

    void resize(int rowCount, int columnCount);
    inline void clear();

    void prepareTableChanged();
    void updateTable();

    inline FwCellPrimitive* cell(int row, int column);

    static QVector<int> realSizeVector(const QVector<qreal>& vector, qreal maxValue);
    static void resizeSizeVector(QVector<qreal>& vector, int newSize);

    void setRowsHeight(QVector<qreal> heights);
    void setColumnsWidth(QVector<qreal> widths);

protected:
    void geometryChanged(const QRect &oldRect, QRect &rect);

    void updateRealSizes();
    void updateCellGeometry();

private:
    QList<FwCellPrimitive*> m_cells;

    QVector<FwCellPrimitive*> m_cellsGrid;
    bool m_cellsGridDirty;

    int m_rowCount;
    int m_columnCount;
    bool m_gridSizeDirty;

    QVector<qreal> m_rowsHeight;
    QVector<qreal> m_columnsWidth;
    QVector<int> m_realRowsHeight;
    QVector<int> m_realColumnsWidth;
    bool m_realSizeDirty;
    bool m_cellGeometryDirty;

    int m_startTableChanged;
};

void FwTablePrimitive::clear()
{
    resize(0, 0);
}

FwCellPrimitive* FwTablePrimitive::cell(int row, int column)
{
    if(row < m_rowCount && column < m_columnCount)
    {
        return m_cellsGrid[row * m_rowCount + column];
    }
    return 0;
}

#endif // FIREWORKS_GRAPHICSTABLEITEM_H
