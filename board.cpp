#include <iostream>
#include <vector>
#include <memory>
using namespace std;

const int BOARD_WIDTH = 80;
const int BOARD_HEIGHT = 25;

class Shape {
protected:
    int id;
    static int nextID;

public:
    Shape() : id(nextID++) {}

    virtual void draw() = 0; //pure function

    virtual ~Shape() {} //virtual desctructor
};

int Shape::nextID = 1;

class Board {
private:
    vector<vector<char>> grid;
    vector<unique_ptr<Shape>> shapes; //to collect shapes

public:
    Board() : grid(BOARD_HEIGHT, vector<char>(BOARD_WIDTH, ' ')) {}

    void draw() {
        for (auto& row : grid) {
            fill(row.begin(), row.end(), ' '); //clear the grid
        }

        for (auto& shape : shapes) {
            shape->draw(); //calling the draw method on each shape in shapes
        }

        for (auto& row : grid) { //printing the current state of the code
            for (char c : row) {
                cout << c;
            }
            cout << "\n";
        }
    }

    void add(unique_ptr<Shape> shape) { //add to the list of shapes
        shapes.push_back(move(shape));
    }

    void undo() {
        if (!shapes.empty()) {
            shapes.pop_back(); //remove the last added shape from the shapes vector
        }
    }

    vector<vector<char>>& getGrid() { //so shapes can have access to the grid
        return grid;
    }
};

class Triangle : public Shape {
private:
    int x, y, height;
    Board& board;

public:
    Triangle(Board& board, int x, int y, int height) //x is column number and y is row+1
        : board(board) {
        this->x = x;
        this->y = y;
        this->height = height;
    }

    void draw() override {
        vector<vector<char>>& grid = board.getGrid();

        if (height <= 0) return; //height can't be negative

        for (int i = 0; i < height; ++i) {
            int leftMost = x - i;
            int rightMost = x + i;
            int posY = y + i; //vertical position
            if (posY < BOARD_HEIGHT) {
                if (leftMost >= 0 && leftMost < BOARD_WIDTH)
                    grid[posY][leftMost] = '*';
                if (rightMost >= 0 && rightMost < BOARD_WIDTH && leftMost != rightMost)
                    grid[posY][rightMost] = '*';
            }
        }

        for (int j = 0; j < 2 * height - 1; ++j) {
            int baseX = x - height + 1 + j;
            int baseY = y + height - 1;
            if (baseX >= 0 && baseX < BOARD_WIDTH && baseY < BOARD_HEIGHT) {grid[baseY][baseX] = '*';}
        }
    }
};

class Circle : public Shape {
private:
    int x, y, radius;
    Board& board;

public:
    Circle(Board& board, int x, int y, int radius)
        : board(board) {
        this->x = x;
        this->y = y;
        this->radius = radius;
    }

    void draw() override {
        vector<vector<char>>& grid = board.getGrid();

        if (radius <= 0) return; //radius in itself can't be negative

        for (int i = -radius; i <= radius; ++i) {  //distance to the left and to the right from the centre
            for (int j = -radius; j <= radius; ++j) { //distance to the top and to the bottom from the centre
                if ((i * i + j * j >= radius * radius - radius) && (i * i + j * j <= radius * radius)) { //formula check
                    int drawX = x + i; //end x position on the grid
                    int drawY = y + j; //end y position on the grid

                    if (drawX >= 0 && drawX < BOARD_WIDTH && drawY >= 0 && drawY < BOARD_HEIGHT) { //what will really be printed
                        grid[drawY][drawX] = '*';
                    }
                }
            }
        }
    }
};

class Rectangle : public Shape {
private:
    int x, y, width, height;
    Board& board;

public:
    Rectangle(Board& board, int x, int y, int width, int height)
        : board(board) {
        this->x = x;
        this->y = y;
        this->width = width;
        this->height = height;
    }

    void draw() override {
        vector<vector<char>>& grid = board.getGrid();

        if (width <= 0 || height <= 0) return;

        for (int i = 0; i < width; ++i) {
            int topX = x + i;
            int bottomX = x + i;
            if (topX >= 0 && topX < BOARD_WIDTH && y >= 0 && y < BOARD_HEIGHT)
                grid[y][topX] = '*';  //top side
            if (bottomX >= 0 && bottomX < BOARD_WIDTH && y + height - 1 >= 0 && y + height - 1 < BOARD_HEIGHT)
                grid[y + height - 1][bottomX] = '*';  //bottom side
        }

        for (int j = 0; j < height; ++j) {
            int leftY = y + j;
            int rightY = y + j;
            if (x >= 0 && x < BOARD_WIDTH && leftY >= 0 && leftY < BOARD_HEIGHT)
                grid[leftY][x] = '*';  //left side
            if (x + width - 1 >= 0 && x + width - 1 < BOARD_WIDTH && rightY >= 0 && rightY < BOARD_HEIGHT)
                grid[rightY][x + width - 1] = '*';  //right side
        }
    }
};

class Line : public Shape {
private:
    int x, y;    // Starting coordinates of the line
    int length;  // Length of the line
    Board& board;

public:
    Line(Board& board, int x, int y, int length)
        : board(board) {
        this->x = x;
        this->y = y;
        this->length = length;
    }

    void draw() override {
        vector<vector<char>>& grid = board.getGrid();

        if (length <= 0) return;

        for (int i = 0; i < length; ++i) {
            int drawX = x + i; //looping for each next x
            if (drawX >= 0 && drawX < BOARD_WIDTH && y >= 0 && y < BOARD_HEIGHT) {
                grid[y][drawX] = '*';
            }
        }
    }
};

int main()
{
    Board board;

    board.add(make_unique<Triangle>(board, 1, 5, 3)); //a triangle cropped to the left
    board.add(make_unique<Circle>(board, 10, 5, 5));
    board.add(make_unique<Rectangle>(board, 3, 6, 8, 5));
    board.add(make_unique<Line>(board, -1, 12, 15));

    board.draw();

    return 0;
}
