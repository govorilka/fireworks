#include <QtCore/qdebug.h>

#include "fwtableprimitive.h"

FwCellPrimitive::FwCellPrimitive(int row, int column, FwTablePrimitive* parent) :
    BaseClass("R" + QByteArray::number(row) + "C" + QByteArray::number(column), parent),
    m_row(row),
    m_column(column),
    m_rowSpan(1),
    m_columnSpan(1),
    needGeometryProcessed(true)
{
    parent->prepareTableChanged();
    parent->m_cells.append(this);

    parent->resize(qMax(parent->m_rowCount, row + 1),
                   qMax(parent->m_columnCount, column + 1));

    parent->m_cellsGridDirty = true;

    parent->updateTable();
}

void FwCellPrimitive::setRowSpan(int span)
{
    if(m_rowSpan != span && span > 0)
    {
        FwTablePrimitive* table = static_cast<FwTablePrimitive*>(parent());
        table->prepareTableChanged();

        table->m_cellsGridDirty = true;
        m_rowSpan = span;
        if(m_row + m_rowSpan > table->m_rowCount)
        {
            m_rowSpan = table->m_rowCount - m_row;
        }

        table->updateTable();
    }
}

void FwCellPrimitive::setColumnSpan(int span)
{
    if(m_columnSpan != span && span > 0)
    {
        FwTablePrimitive* table = static_cast<FwTablePrimitive*>(parent());
        table->prepareTableChanged();

        table->m_cellsGridDirty = true;
        m_columnSpan = span;
        if(m_column + m_columnSpan > table->m_columnCount)
        {
            m_columnSpan = table->m_columnCount - m_column;
        }

        table->updateTable();
    }
}

//////////////////////////////////////////////////////////////////////////////////

FwTablePrimitive::FwTablePrimitive(const QByteArray& name, FwPrimitiveGroup* parent) :
    BaseClass(name, parent),
    m_rowCount(0),
    m_columnCount(0),
    m_gridSizeDirty(false),
    m_startTableChanged(0),
    m_cellsGridDirty(false),
    m_realSizeDirty(false),
    m_cellGeometryDirty(false)
{
}

void FwTablePrimitive::apply(FwMLObject *object)
{
    prepareGeometryChanged();

    BaseClass::apply(object);

    update();
}

/*!
Изменяет размер таблицы до переданных в функцию. При этом ячейки, находящиеся
за указанными пределами таблицы удаляются.
\param rowCount Новое количество строк
\param columnCount Новое количество столбцов
\sa void FwTableWidget::setMinSize(int rowCount, int columnCount)
*/
void FwTablePrimitive::resize(int rowCount, int columnCount)
{
    if(m_rowCount != rowCount || m_columnCount != columnCount)
    {
        prepareTableChanged();
        m_rowCount = rowCount;
        m_columnCount = columnCount;
        m_cellsGrid.resize(m_rowCount * m_columnCount);
        m_cellsGrid.fill(0);
        m_gridSizeDirty = true;
        m_cellsGridDirty = true;
        updateTable();
    }
}

void FwTablePrimitive::prepareTableChanged()
{
    m_startTableChanged++;
}

void FwTablePrimitive::updateTable()
{
    if(m_startTableChanged && (--m_startTableChanged == 0))
    {
        prepareGeometryChanged();

        if(m_gridSizeDirty)
        {
            m_cellsGridDirty = true;

            QList<FwCellPrimitive*> cells = m_cells;
            foreach(FwCellPrimitive* cell, cells)
            {
                int xRow = cell->row();
                int yColumn = cell->column();
                if(xRow >= m_rowCount || yColumn >= m_columnCount)
                {
                    m_cells.removeOne(cell);
                    delete cell;
                }
                else
                {
                    if(xRow + cell->rowSpan() > m_rowCount)
                    {
                        cell->setRowSpan(m_rowCount - xRow);
                    }
                    if(yColumn + cell->columnSpan() > m_columnCount)
                    {
                        cell->setColumnSpan(m_columnCount - yColumn);
                    }
                }
            }

            resizeSizeVector(m_rowsHeight, m_rowCount);
            resizeSizeVector(m_columnsWidth, m_columnCount);
            m_realSizeDirty = true;
        }

        if(m_cellsGridDirty)
        {
            foreach(FwCellPrimitive* cell, m_cells)
            {
                for(int i = cell->row(); i < (cell->row() + cell->rowSpan()); i++)
                {
                    int delta = i * m_columnCount;
                    for(int j = cell->column(); j < (cell->column() + cell->columnSpan()); j++)
                    {
                        m_cellsGrid[delta + j] = cell;
                    }
                }
            }

            m_cellsGridDirty = false;
        }

        updateRealSizes();
        updateCellGeometry();

        update();
    }
}

/*!
Рассчитывает массив реальных размеров.
Значения второго массива рассчитываются по формуле:

\fn N2[i] = (maxValue / sum(N1)) * N1[i], где:

\li N2[i]- элемент второго массива с индексом i
\li maxValue - значение переданное в параметре maxValue
\li sum(N1) - сумма элементов первого массива
\li N1[i] - элемент первого массива с индексом i

\param vector Массив со значениями относительно размера
\param maxValue Максимальная реальная величина измерения (высоты или ширины),
которой соответствует переданный массив
*/
QVector<int> FwTablePrimitive::realSizeVector(const QVector<qreal>& vector, qreal maxValue)
{
    int vectorSize = vector.size();
    QVector<int> realVector(vectorSize, 0);
    if(vectorSize)
    {
        qreal weightsCount = 0;
        const qreal* weight = vector.data();
        for(int i = 0; i < vectorSize; i++)
        {
            weightsCount += *weight;
            weight++;
        }

        qreal factor = maxValue / weightsCount; //(realMaxValue / sum(N1)

        int sumN2 = 0;
        const qreal* n1 = vector.data();
        int* n2 = realVector.data();
        for(int i = 0; i < vectorSize; i++)
        {
            *n2 = factor * (*n1);
            sumN2 += (*n2);
            n1++;
            n2++;
        }
        realVector[vectorSize - 1] += maxValue - sumN2;
    }

    return realVector;
}

/*!
Функция изменяет размер массива до указанного в параметре newSize.
Если старый размер массива был меньше, то новый элементы заполняются 1.
\param vector Массив, размер которого надо изменить
\param newSize новый размер массива
*/
void FwTablePrimitive::resizeSizeVector(QVector<qreal>& vector, int newSize)
{
    if(vector.size() != newSize)
    {
        int oldSize = vector.size();
        vector.resize(newSize);
        for(int i = oldSize; i < newSize; i++)
        {
            vector[i] = 1;
        }
    }
}

void FwTablePrimitive::geometryChanged(const QRect &oldRect, QRect &rect)
{
    if(!m_startTableChanged)
    {
        m_realSizeDirty = true;
        updateRealSizes();
        updateCellGeometry();
    }
    BaseClass::geometryChanged(oldRect, rect);
}

void FwTablePrimitive::updateRealSizes()
{
    if(m_realSizeDirty)
    {
        m_realRowsHeight = realSizeVector(m_rowsHeight, size().height());
        m_realColumnsWidth = realSizeVector(m_columnsWidth, size().width());
        m_cellGeometryDirty = true;
        m_realSizeDirty = false;
    }
}

void FwTablePrimitive::updateCellGeometry()
{
    if(m_cellGeometryDirty)
    {
        foreach(FwCellPrimitive* cell, m_cells)
        {
            cell->needGeometryProcessed = true;
        }

        int y = 0;
        for(int row = 0; row < m_rowCount; row++)
        {
            int x = 0;
            int rowDelta = row * m_columnCount;
            int rowHeight = m_realRowsHeight[row];
            for(int column = 0; column < m_columnCount; column++)
            {
                int columnWidth = m_realColumnsWidth[column];
                FwCellPrimitive* cell = m_cellsGrid[rowDelta + column];
                if(cell && cell->needGeometryProcessed)
                {
                    int iRow = cell->row();
                    int height = rowHeight;
                    for(iRow++; iRow < row + cell->m_rowSpan; iRow++)
                    {
                        height += m_realRowsHeight[iRow];
                    }

                    int iColumn = cell->column();
                    int width = columnWidth;
                    for(iColumn++; iColumn < column + cell->m_columnSpan; iColumn++)
                    {
                        width += m_realColumnsWidth[iColumn];
                    }
                    cell->prepareGeometryChanged();
                    cell->setPos(x, y);
                    cell->setSize(width, height);
                    cell->update();
                    cell->needGeometryProcessed = false;
                }
                x += columnWidth;
            }
            y += m_realRowsHeight[row];
        }

        m_cellGeometryDirty = false;
    }
}

void FwTablePrimitive::setRowsHeight(QVector<qreal> heights)
{
    if(m_rowsHeight != heights)
    {
        prepareTableChanged();
        m_realSizeDirty = true;
        m_rowsHeight = heights;
        resizeSizeVector(m_rowsHeight, m_rowCount);
        updateTable();
    }
}

void FwTablePrimitive::setColumnsWidth(QVector<qreal> widths)
{
    if(m_columnsWidth != widths)
    {
        prepareTableChanged();
        m_realSizeDirty = true;
        m_columnsWidth = widths;
        resizeSizeVector(m_columnsWidth, m_columnCount);
        updateTable();
    }
}
