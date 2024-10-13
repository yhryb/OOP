#include <iostream>
#include <vector>
#include <memory>
using namespace std;

// Define the size of the board
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

// Struct to define the board
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
    Triangle(Board& board, int x, int y, int height)
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

int main()
{
    Board board;

    board.add(std::make_unique<Triangle>(board, 40, 12, 10));

    board.draw();

    return 0;
}
