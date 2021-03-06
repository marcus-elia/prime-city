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
    PlotNetwork pn = PlotNetwork(512, 8);
    std::vector<int> nodeIDs{1,2,3,5,6,7,8,9,11,12,13,16,21,24,25,26,29,30,31,32,39,40,41,45,46,47,49,53,57,58,59,60,61,62,63};
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
    std::cout << "Test passed." << std::endl;
    return true;
}


/*
 * + - - - - - - - - + - - - - - - - - +
 * | X s o o X o o o | o o o o o o X F |
 * | o o X o o o X X | o X X X X o o o |
 * | o X X X X o X X | o X X X X X X X |
 * | o o o X X o o o | o X X X X X X X |
 * | o X X X X X X o | o X X X X X X X |
 * | o o X X X o o o | o X X X X X X X |
 * | X o X X X o X X | o X X X X X X X |
 * | X o o o o o o o | o X X X X X X X |
 * + - - - - - - - - + - - - - - - - - +
 */
bool testBFSexample2()
{
    std::cout << "Testing Breadth First Search on Example 2" << std::endl;
    PlotNetwork pn = PlotNetwork(512, 8);
    std::vector<int> nodeIDs{1,2,3,5,6,7,8,9,11,12,13,16,21,24,25,26,29,30,31,32,39,40,41,45,46,47,49,53,57,58,59,60,61,62,63,
                             320,321,322,323,324,325,327,328,333,334,335,336,344,352,360,368,376};
    for(int id : nodeIDs)
    {
        pn.addNode(PlotNode(id, 512, 8));
    }

    std::vector<PlotNode*> observedPath = pn.getShortestPath(1, 327);
    std::vector<int> expectedPathIDs{1,2,3,11,12,13,5,6,7,320,321,322,323,324,325,333,334,335,327};

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
    std::cout << "Test passed." << std::endl;
    return true;
}

/*
 * + - - - - - - - - +
 * | X s o o X X X X |
 * | o o X o X X X X |
 * | o X X X X X X X |
 * | o o o X X X X X |
 * | o X X X X X X X |
 * | o o X X X X X X |
 * | X o X X X X X X |
 * | X o o o o X o F |
 * + - - - - - - - - +
 */
// When it is impossible to reach
bool testBFSexample3()
{
    std::cout << "Testing Breadth First Search on Example 3" << std::endl;
    PlotNetwork pn = PlotNetwork(512, 8);
    std::vector<int> nodeIDs{1,2,3,8,9,11,16,24,25,26,32,40,41,49,57,58,59,60,62,63};
    for(int id : nodeIDs)
    {
        pn.addNode(PlotNode(id, 512, 8));
    }

    std::vector<PlotNode*> observedPath = pn.getShortestPath(1, 63);
    std::vector<int> expectedPathIDs{1,9,8,16,24,32,40,41,49,57,58,59,60};

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
    std::cout << "Test passed." << std::endl;
    return true;
}

/*
 * + - - - - - - - - +
 * | X s o o X X X X |
 * | o o X o o X X X |
 * | o X X X o X X X |
 * | o o o X o X X X |
 * | o X X X X X X X |
 * | o o X X X X X X |
 * | X o X X X X X X |
 * | X o o o o X o F |
 * + - - - - - - - - +
 */
// When it is impossible to reach
bool testBFSexample4()
{
    std::cout << "Testing Breadth First Search on Example 4" << std::endl;
    PlotNetwork pn = PlotNetwork(512, 8);
    std::vector<int> nodeIDs{1,2,3,8,9,11,12,16,20,24,25,26,28,32,40,41,49,57,58,59,60,62,63};
    for(int id : nodeIDs)
    {
        pn.addNode(PlotNode(id, 512, 8));
    }

    std::vector<PlotNode*> observedPath = pn.getShortestPath(1, 63, 6);
    std::vector<int> expectedPathIDs{1,2,3,11,12,20};

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
    std::cout << "Test passed." << std::endl;
    return true;
}

bool testGetPlotIDsBetween()
{
    bool passed = true;
    std::cout << "\nTesting GetPlotIDsBetween" << std::endl;
    PlotNetwork network;
    std::vector<std::vector<int>> exp;
    std::vector<std::vector<int>> obs;
    std::vector<std::string> cases;

    cases.push_back(" end is the same as start");
    obs.push_back(network.getPlotIDsBetween(22, 22));
    exp.push_back(std::vector<int>{22});

    cases.push_back(" end is to the right of start");
    obs.push_back(network.getPlotIDsBetween(1, 7));
    exp.push_back(std::vector<int>{1,2,3,4,5,6,7});

    cases.push_back(" end is to the left of start");
    obs.push_back(network.getPlotIDsBetween(7, 1));
    exp.push_back(std::vector<int>{7,6,5,4,3,2,1});

    cases.push_back(" end is below start");
    obs.push_back(network.getPlotIDsBetween(1, 17));
    exp.push_back(std::vector<int>{1,9,17});

    cases.push_back(" end is above start");
    obs.push_back(network.getPlotIDsBetween(17, 1));
    exp.push_back(std::vector<int>{17,9,1});

    cases.push_back(" end is up and right from start");
    obs.push_back(network.getPlotIDsBetween(25, 14));
    exp.push_back(std::vector<int>{25,17,26,18,27,19,11,28,20,12,21,13,22,14});

    cases.push_back(" end is down and left from start");
    obs.push_back(network.getPlotIDsBetween(14, 25));
    exp.push_back(std::vector<int>{14,22,13,21,12,20,28,11,19,27,18,26,17,25});

    cases.push_back(" end is up and left from start");
    obs.push_back(network.getPlotIDsBetween(30, 9));
    exp.push_back(std::vector<int>{30,22,29,21,28,20,12,27,19,11,18,10,17,9});

    cases.push_back(" end is down and right from start");
    obs.push_back(network.getPlotIDsBetween(9, 30));
    exp.push_back(std::vector<int>{9,17,10,18,11,19,27,12,20,28,21,29,22,30});

    cases.push_back(" end is up and right at a 45 degree angle");
    obs.push_back(network.getPlotIDsBetween(56, 42));
    exp.push_back(std::vector<int>{56,48,57,49,41,50,42});

    cases.push_back(" end is down and left at a 45 degree angle");
    obs.push_back(network.getPlotIDsBetween(42, 56));
    exp.push_back(std::vector<int>{42,50,41,49,57,48,56});

    cases.push_back(" end is up and left at a 45 degree angle");
    obs.push_back(network.getPlotIDsBetween(58, 40));
    exp.push_back(std::vector<int>{58,50,57,49,41,48,40});

    cases.push_back(" end is down and right at a 45 degree angle");
    obs.push_back(network.getPlotIDsBetween(40, 58));
    exp.push_back(std::vector<int>{40,48,41,49,57,50,58});

    cases.push_back(" end is down and left at a small angle");
    obs.push_back(network.getPlotIDsBetween(71, 72));
    exp.push_back(std::vector<int>{71,79,70,78,69,77,68,76,67,75,66,74,65,73,64,72});

    cases.push_back(" end is up and right with a slope of 2");
    obs.push_back(network.getPlotIDsBetween(2977, 2962));
    exp.push_back(std::vector<int>{2977,2969,2961,2978,2970,2962});

    for(int j = 0; j < cases.size(); j++)
    {
        if(obs[j].size() != exp[j].size())
        {
            std::cout << "Test FAILED when " + cases[j] << std::endl;
            std::cout << "Expected vec of size " << exp[j].size() << ", but got vec of size " << obs[j].size() << std::endl;
            passed = false;
        }
        else
        {
            for(int i = 0; i < obs[j].size(); i++)
            {
                if(exp[j][i] != obs[j][i])
                {
                    std::cout << "Test FAILED when " + cases[j] << std::endl;
                    std::cout << "Expected entry " << exp[j][i] << ", but got " << obs[j][i] << std::endl;
                    passed = false;
                }
            }
        }
    }

    if(passed)
    {
        std::cout << "All tests passed." << std::endl;
    }
    return passed;
}

/*
 * + - - - - - - - - +
 * | X s o o X X X X |
 * | X o o o o X X X |
 * | X X o o o X X X |
 * | X X X X o X X X |
 * | X X X X o X X X |
 * | X X X X F X X X |
 * | X X X X X X X X |
 * | X X X X X X X X |
 * + - - - - - - - - +
 */
bool testClippingExample1()
{
    std::cout << "\nTesting Clip Path on Example 1" << std::endl;
    PlotNetwork pn = PlotNetwork(512, 8);
    std::vector<int> nodeIDs{1,2,3,9,10,11,12,18,19,20,28,36,44};
    for(int id : nodeIDs)
    {
        pn.addNode(PlotNode(id, 512, 8));
    }

    std::vector<int> observedClippedPath = pn.clipPath(pn.breadthFirstSearch(1, 44));
    std::vector<int> expectedPath{44,20,1};
    if(observedClippedPath.size() != expectedPath.size())
    {
        std::cout << "Test FAILED" << std::endl;
        std::cout << "Expected path of size " << expectedPath.size() << ", but got path of size " << observedClippedPath.size() << std::endl;
        return false;
    }

    for(int i = 0; i < expectedPath.size(); i++)
    {
        if(expectedPath[i] != observedClippedPath[i])
        {
            std::cout << "Test FAILED" << std::endl;
            std::cout << "Expected entry to be " << expectedPath[i] << ", but got " << observedClippedPath[i]<< std::endl;
            return false;
        }
    }
    std::cout << "Test passed." << std::endl;
    return true;
}

// Just a straight line
bool testClippingExample2()
{
    std::cout << "\nTesting Clip Path on Example 2" << std::endl;
    PlotNetwork pn = PlotNetwork(512, 8);
    std::vector<int> nodeIDs{8,9,10,11,12,13,14,15};
    for(int id : nodeIDs)
    {
        pn.addNode(PlotNode(id, 512, 8));
    }

    std::vector<int> observedClippedPath = pn.clipPath(pn.breadthFirstSearch(15, 8));
    std::vector<int> expectedPath{8,15};
    if(observedClippedPath.size() != expectedPath.size())
    {
        std::cout << "Test FAILED" << std::endl;
        std::cout << "Expected path of size " << expectedPath.size() << ", but got path of size " << observedClippedPath.size() << std::endl;
        return false;
    }

    for(int i = 0; i < expectedPath.size(); i++)
    {
        if(expectedPath[i] != observedClippedPath[i])
        {
            std::cout << "Test FAILED" << std::endl;
            std::cout << "Expected entry to be " << expectedPath[i] << ", but got " << observedClippedPath[i]<< std::endl;
            return false;
        }
    }
    std::cout << "Test passed." << std::endl;
    return true;
}

/*
 * + - - - - - - - - +
 * | X s o o o X X X |
 * | X o o o o X X X |
 * | X X o o o X X X |
 * | X X X X o X X X |
 * | X X X X o X X X |
 * | X o o o o X X X |
 * | o o o X X X X X |
 * | F o X X X X X X |
 * + - - - - - - - - +
 */
bool testClippingExample3()
{
    std::cout << "\nTesting Clip Path on Example 3" << std::endl;
    PlotNetwork pn = PlotNetwork(512, 8);
    std::vector<int> nodeIDs{1,2,3,4,9,10,11,12,18,19,20,28,36,41,42,43,44,48,49,50,56,57};
    for(int id : nodeIDs)
    {
        pn.addNode(PlotNode(id, 512, 8));
    }

    std::vector<int> observedClippedPath = pn.clipPath(pn.breadthFirstSearch(1, 56));
    std::vector<int> expectedPath{56,42,44,20,1};
    if(observedClippedPath.size() != expectedPath.size())
    {
        std::cout << "Test FAILED" << std::endl;
        std::cout << "Expected path of size " << expectedPath.size() << ", but got path of size " << observedClippedPath.size() << std::endl;
        return false;
    }

    for(int i = 0; i < expectedPath.size(); i++)
    {
        if(expectedPath[i] != observedClippedPath[i])
        {
            std::cout << "Test FAILED" << std::endl;
            std::cout << "Expected entry to be " << expectedPath[i] << ", but got " << observedClippedPath[i]<< std::endl;
            return false;
        }
    }
    std::cout << "Test passed." << std::endl;
    return true;
}

/*
 * + - - - - - - - - +
 * | X X X X X X X X |
 * | X X X X X X X X |
 * | X X X X X X X X |
 * | X X X X X X X X |
 * | X X X X X X X F |
 * | X X X X X X o o |
 * | X X X X X o o X |
 * | X X X X s o X X |
 * + - - - - - - - - +
 */
bool testClippingExample4()
{
    std::cout << "\nTesting Clip Path on Example 4" << std::endl;
    PlotNetwork pn = PlotNetwork(512, 8);
    std::vector<int> nodeIDs{60,61,53,54,46,47,39};
    for(int id : nodeIDs)
    {
        pn.addNode(PlotNode(id, 512, 8));
    }

    std::vector<int> observedClippedPath = pn.clipPath(pn.breadthFirstSearch(60, 39));
    std::vector<int> expectedPath{39,47,46,54,53,61,60};
    if(observedClippedPath.size() != expectedPath.size())
    {
        std::cout << "Test FAILED" << std::endl;
        std::cout << "Expected path of size " << expectedPath.size() << ", but got path of size " << observedClippedPath.size() << std::endl;
        return false;
    }

    for(int i = 0; i < expectedPath.size(); i++)
    {
        if(expectedPath[i] != observedClippedPath[i])
        {
            std::cout << "Test FAILED" << std::endl;
            std::cout << "Expected entry to be " << expectedPath[i] << ", but got " << observedClippedPath[i]<< std::endl;
            return false;
        }
    }
    std::cout << "Test passed." << std::endl;
    return true;
}

/*        40                  69
 * + - - - - - - - - + - - - - - - - - +
 * | X X X X X X X X | X X X X X X X X |
 * | X X X X X X X X | X X X X X X X X |
 * | X X X X X X X X | X X X X X X X X |
 * | X X X X X X F o | B s X X X X X X |
 * | X X X X X X o o | o o X X X X X X |
 * | X X X X X X X X | X X X X X X X X |
 * | X X X X X X X X | X X X X X X X X |
 * | X X X X X X X X | X X X X X X X X |
 * + - - - - - - - - + - - - - - - - - +
 */
bool testClippingExample5()
{
    std::cout << "\nTesting Clip Path on Example 5" << std::endl;
    PlotNetwork pn = PlotNetwork(512, 8);
    std::vector<int> nodeIDs{2590,2591,2598,2599,4441,4448,4449};
    for(int id : nodeIDs)
    {
        pn.addNode(PlotNode(id, 512, 8));
    }

    std::vector<int> observedClippedPath = pn.clipPath(pn.breadthFirstSearch(4441, 2590));
    std::vector<int> expectedPath{2590,2599,4449,4441};
    if(observedClippedPath.size() != expectedPath.size())
    {
        std::cout << "Test FAILED" << std::endl;
        std::cout << "Expected path of size " << expectedPath.size() << ", but got path of size " << observedClippedPath.size() << std::endl;
        return false;
    }

    for(int i = 0; i < expectedPath.size(); i++)
    {
        if(expectedPath[i] != observedClippedPath[i])
        {
            std::cout << "Test FAILED" << std::endl;
            std::cout << "Expected entry to be " << expectedPath[i] << ", but got " << observedClippedPath[i]<< std::endl;
            return false;
        }
    }
    std::cout << "Test passed." << std::endl;
    return true;
}

/*
 * + - - - - - - - - +
 * | X o o o o X X X |
 * | X o o o o X X X |
 * | X X o o o X X X |
 * | X o X X o X o o |
 * | X X X X o o o X |
 * | X o o o o X X o |
 * | o o o X X X X X |
 * | o o X X X X X X |
 * + - - - - - - - - +
 */
bool testHasLineOfSight()
{
    std::cout << "\nTesting hasLineOfSight" << std::endl;
    PlotNetwork pn = PlotNetwork(512, 8);
    std::vector<int> nodeIDs{1,2,3,4,9,10,11,12,18,19,20,25,28,30,31,36,37,38,41,42,43,44,47,48,49,50,56,57};
    for(int id : nodeIDs)
    {
        pn.addNode(PlotNode(id, 512, 8));
    }

    int startID, endID;
    std::vector<int> startIDs;
    std::vector<int> endIDs;
    std::vector<bool> exp;
    std::vector<bool> obs;

    startID = 56;
    endID = 57;
    startIDs.push_back(startID);
    endIDs.push_back(endID);
    exp.push_back(true);
    obs.push_back(pn.hasLineOfSight(startID, endID));

    startID = 38;
    endID = 47;
    startIDs.push_back(startID);
    endIDs.push_back(endID);
    exp.push_back(false);
    obs.push_back(pn.hasLineOfSight(startID, endID));

    startID = 47;
    endID = 38;
    startIDs.push_back(startID);
    endIDs.push_back(endID);
    exp.push_back(false);
    obs.push_back(pn.hasLineOfSight(startID, endID));

    startID = 31;
    endID = 37;
    startIDs.push_back(startID);
    endIDs.push_back(endID);
    exp.push_back(false);
    obs.push_back(pn.hasLineOfSight(startID, endID));

    startID = 30;
    endID = 36;
    startIDs.push_back(startID);
    endIDs.push_back(endID);
    exp.push_back(false);
    obs.push_back(pn.hasLineOfSight(startID, endID));

    startID = 36;
    endID = 30;
    startIDs.push_back(startID);
    endIDs.push_back(endID);
    exp.push_back(false);
    obs.push_back(pn.hasLineOfSight(startID, endID));

    startID = 1;
    endID = 12;
    startIDs.push_back(startID);
    endIDs.push_back(endID);
    exp.push_back(true);
    obs.push_back(pn.hasLineOfSight(startID, endID));

    for(int i = 0; i < startIDs.size(); i++)
    {
        if(exp[i] != obs[i])
        {
            std::cout << "Test FAILED" << std::endl;
            std::cout << "Expected " << exp[i] << ", for start =  " << startIDs[i] << " and end = " << endIDs[i] << std::endl;
            return false;
        }
    }
    std::cout << "All tests passed." << std::endl;
    return true;
}