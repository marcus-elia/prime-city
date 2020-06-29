#include "testPathFinding.h"


/*
 * + - - - - - - - - +
 * | X s o o X o o o |
 * | o o X o o o X X |
 * | o X X X X o X X |
 * | o o o X X o o o |
 * | o X X X X X X o |
 * | o o X X X o o o |
 * | X o X X X o X X |
 * | X o o o o o o F |
 * + - - - - - - - - +
 */

bool testBFSexample1()
{
    std::cout << "Testing Breadth First Search on Example 1" << std::endl;
    PlotNetwork pn;
    std::vector<int> nodeIDs{1,2,3,5,6,7,8,9,11,12,16,21,24,25,26,29,30,31,32,39,40,41,45,46,47,49,53,57,58,59,60,61,62,63};
    for(int id : nodeIDs)
    {
        pn.addNode(PlotNode(id, 512, 8));
    }

    std::vector<PlotNode*> observedPath = pn.getShortestPath(1, 63);
    std::vector<int> expectedPathIDs{1,9,8,16,24,32,40,41,49,57,58,59,60,61,62,63};

    if(observedPath.size() != expectedPathIDs.size())
    {
        std::cout << "Test FAILED" << std::endl;
        std::cout << "Expected path of size " << expectedPathIDs.size() << ", but got path of size " << observedPath.size() << std::endl;
        return false;
    }

    for(int i = 0; i < expectedPathIDs.size(); i++)
    {
        if(expectedPathIDs[i] != observedPath[i]->getID())
        {
            std::cout << "Test FAILED" << std::endl;
            std::cout << "Expected entry to be " << expectedPathIDs[i] << ", but got " << observedPath[i]->getID() << std::endl;
            return false;
        }
    }
    return true;
}