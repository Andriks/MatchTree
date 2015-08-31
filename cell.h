#ifndef CELL_H
#define CELL_H


class Cell
{
public:
    Cell();
    Cell(const Cell &cell);
    Cell(int row, int column);
    Cell(int index);

    int row() const;
    void setRow(int row);

    int col() const;
    void setCol(int col);

    bool operator ==(const Cell);

    bool valid() const;
    int index() const;

    Cell upper();
    Cell lower();
    Cell right();
    Cell left();

private:
    int m_row;
    int m_col;
};

#endif // CELL_H
