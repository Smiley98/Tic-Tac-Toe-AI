#include <cassert>
#include <vector>
#include <algorithm>
#include <iostream>

//Decided not to over-engineer this ;)
struct Node {
    int value = 0;
    Node* left = nullptr;
    Node* right = nullptr;
};

//Simple binary tree with min-max support!
struct Tree {
    Node* m_root = nullptr;
};

//Assuming "values" are the leaves of a perfect binary tree, this will return the optimal value a maximizer can obtain.
int minmmax(int currentDepth, int maxDepth, std::vector<int>& values, int index, bool isMax)
{
    if (currentDepth == maxDepth)
        return values[index];

    if (isMax)
        return std::max(
            minmmax(currentDepth + 1, maxDepth, values, index * 2,     false),
            minmmax(currentDepth + 1, maxDepth, values, index * 2 + 1, false)
        );
    else
        return std::min(
            minmmax(currentDepth + 1, maxDepth, values, index * 2,     true),
            minmmax(currentDepth + 1, maxDepth, values, index * 2 + 1, true)
        );
}

//Useful for calculating the depth of a perfect  binary tree given the number of leaves.
int log2(int n) { return (n == 1) ? 0 : 1 + log2(n / 2); }

int main()                                  //12        <- max
                                    //5           12    <- min
{                               //5,    9,    12,    23 <- max
    std::vector<int> scores = { 3, 5, 2, 9, 12, 5, 23, 23 };
    assert(scores.size() % 2 == 0);
    printf("Optimal value is %i.\n", minmmax(0, log2(scores.size()), scores, 0, true));

    return getchar();
}
