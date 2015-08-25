#ifndef CELL_H
#define CELL_H


class Cell
{
public:
    Cell();
    Cell(const Cell &cell);
    Cell(int r, int c);
    Cell(int _index);

    bool operator ==(const Cell);

    bool valid() const;
    int index() const;

    Cell upper();
    Cell lower();
    Cell right();
    Cell left();

// no need to do setter/getter for such simle private class
public:
    int row;
    int col;

};

#endif // CELL_H
