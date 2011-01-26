#ifndef FIREWORKS_GRAPHICSTABLEITEM_H
#define FIREWORKS_GRAPHICSTABLEITEM_H

#include "fwprimitives/fwprimitivegroup.h"

class FwGraphicsTableItem;
class FwGraphicsCellItem;

class FwGraphicsCellItem : public FwRectPrimitive
{
    typedef FwRectPrimitive BaseClass;

public:
    friend class FwGraphicsTableItem;

    FwGraphicsCellItem(int row, int column, FwGraphicsTableItem* parent);

    inline int row() const;
    inline int column() const;

    inline int rowSpan() const;
    void setRowSpan(int span);

    inline int columnSpan() const;
    void setColumnSpan(int span);

protected:
    QRect updateGeometry(const QRect &rect);

private:
    int m_row;
    int m_column;
    int m_rowSpan;
    int m_columnSpan;
    bool needGeometryProcessed;
};

int FwGraphicsCellItem::row() const
{
    return m_row;
}

int FwGraphicsCellItem::column() const
{
    return m_column;
}

int FwGraphicsCellItem::rowSpan() const
{
    return m_rowSpan;
}

int FwGraphicsCellItem::columnSpan() const
{
    return m_columnSpan;
}

//////////////////////////////////////////////////////////////////////////////////

class FwGraphicsTableItem : public FwPrimitiveGroup
{
    typedef FwPrimitiveGroup BaseClass;

public:
    friend class FwGraphicsCellItem;

    FwGraphicsTableItem(FwPrimitiveGroup* parent);

    void apply(FwMLObject *object);

    void resize(int rowCount, int columnCount);
    inline void clear();

    void prepareTableChanged();
    void updateTable();

    inline FwGraphicsCellItem* cell(int row, int column);

    static QVector<int> realSizeVector(const QVector<qreal>& vector, qreal maxValue);
    static void resizeSizeVector(QVector<qreal>& vector, int newSize);

    void setRowsHeight(QVector<qreal> heights);
    void setColumnsWidth(QVector<qreal> widths);

protected:
    QRect updateGeometry(const QRect &rect);

    void updateRealSizes();
    void updateCellGeometry();

private:
    QList<FwGraphicsCellItem*> m_cells;

    QVector<FwGraphicsCellItem*> m_cellsGrid;
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

void FwGraphicsTableItem::clear()
{
    resize(0, 0);
}

FwGraphicsCellItem* FwGraphicsTableItem::cell(int row, int column)
{
    if(row < m_rowCount && column < m_columnCount)
    {
        return m_cellsGrid[row * m_rowCount + column];
    }
    return 0;
}

#endif // FIREWORKS_GRAPHICSTABLEITEM_H
