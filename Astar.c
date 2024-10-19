#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

#define ROW 5
#define COL 5

typedef struct Node {
    int x, y;
    int g, h, f;
    struct Node* parent;
} Node;

typedef struct {
    Node** nodes;
    int size;
} MinHeap;

Node* createNode(int x, int y, int g, int h) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->x = x;
    newNode->y = y;
    newNode->g = g;
    newNode->h = h;
    newNode->f = g + h;
    newNode->parent = NULL;
    return newNode;
}

MinHeap* createMinHeap(int capacity) {
    MinHeap* heap = (MinHeap*)malloc(sizeof(MinHeap));
    heap->nodes = (Node**)malloc(sizeof(Node*) * capacity);
    heap->size = 0;
    return heap;
}

void swap(Node** a, Node** b) {
    Node* temp = *a;
    *a = *b;
    *b = temp;
}

void heapify(MinHeap* heap, int index) {
    int smallest = index;
    int leftChild = 2 * index + 1;
    int rightChild = 2 * index + 2;

    if (leftChild < heap->size && heap->nodes[leftChild]->f < heap->nodes[smallest]->f)
        smallest = leftChild;

    if (rightChild < heap->size && heap->nodes[rightChild]->f < heap->nodes[smallest]->f)
        smallest = rightChild;

    if (smallest != index) {
        swap(&heap->nodes[smallest], &heap->nodes[index]);
        heapify(heap, smallest);
    }
}

Node* extractMin(MinHeap* heap) {
    if (heap->size == 0)
        return NULL;

    Node* root = heap->nodes[0];
    heap->nodes[0] = heap->nodes[heap->size - 1];
    heap->size--;
    heapify(heap, 0);

    return root;
}

void insertMinHeap(MinHeap* heap, Node* node) {
    heap->size++;
    int i = heap->size - 1;
    heap->nodes[i] = node;

    while (i != 0 && heap->nodes[(i - 1) / 2]->f > heap->nodes[i]->f) {
        swap(&heap->nodes[i], &heap->nodes[(i - 1) / 2]);
        i = (i - 1) / 2;
    }
}

int heuristic(int x, int y, int endX, int endY) {
    return abs(x - endX) + abs(y - endY);  
}

int isValid(int x, int y, int grid[ROW][COL], int closedSet[ROW][COL]) {
    return (x >= 0 && x < ROW && y >= 0 && y < COL && grid[x][y] != 1 && !closedSet[x][y]);
}

void aStar(int grid[ROW][COL], int startX, int startY, int endX, int endY) {
    MinHeap* openList = createMinHeap(ROW * COL);
    int closedSet[ROW][COL] = {0};
    int moves[4][2] = {{0, 1}, {1, 0}, {0, -1}, {-1, 0}};

    Node* startNode = createNode(startX, startY, 0, heuristic(startX, startY, endX, endY));
    insertMinHeap(openList, startNode);

    while (openList->size > 0) {
        Node* currentNode = extractMin(openList);

        if (currentNode->x == endX && currentNode->y == endY) {
            printf("Path found!\n");
            Node* pathNode = currentNode;
            while (pathNode) {
                printf("(%d, %d) ", pathNode->x, pathNode->y);
                pathNode = pathNode->parent;
            }
            printf("\n");
            return;
        }

        closedSet[currentNode->x][currentNode->y] = 1;

        for (int i = 0; i < 4; i++) {
            int newX = currentNode->x + moves[i][0];
            int newY = currentNode->y + moves[i][1];

            if (isValid(newX, newY, grid, closedSet)) {
                int gCost = currentNode->g + 1;
                int hCost = heuristic(newX, newY, endX, endY);
                Node* neighbor = createNode(newX, newY, gCost, hCost);
                neighbor->parent = currentNode;

                insertMinHeap(openList, neighbor);
            }
        }
    }

    printf("No path found.\n");
}

int main() {
    int grid[ROW][COL] = {
        {0, 0, 0, 0, 0},
        {0, 1, 1, 1, 0},
        {0, 0, 0, 0, 0},
        {0, 1, 1, 1, 0},
        {0, 0, 0, 0, 0}
    };

    int startX = 0, startY = 0;
    int endX = 4, endY = 4;

    aStar(grid, startX, startY, endX, endY);

    return 0;
}
