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

//    static void setStaticParams(int width, int height);

// no need to do setter/getter for such simle private class
public:
    int row;
    int col;

//private:
//    static int width_;
//    static int height_;
};

#endif // CELL_H
