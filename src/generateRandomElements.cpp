#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <random>
#include <string>

// Define Elements Type
enum class ElementType
{
    LINEAR_TRIANGLE,
    LINEAR_QUAD,
    LINEAR_TETRA,
    LINEAR_HEXA,
    LINEAR_PYRAMID,
    LINEAR_PRISM,
    QUADRATIC_TRIANGLE,
    QUADRATIC_QUAD,
    QUADRATIC_TETRA,
    QUADRATIC_HEX
};

// Generate random node coordination
std::vector<std::vector<double>> generateNodes(int numNodes, double minCoord, double maxCoord)
{
    std::vector<std::vector<double>> nodes(numNodes, std::vector<double>(3, 0.0));
    for (int i = 0; i < numNodes; ++i)
    {
        nodes[i][0] = minCoord + static_cast<double>(rand()) / RAND_MAX * (maxCoord - minCoord); // x
        nodes[i][1] = minCoord + static_cast<double>(rand()) / RAND_MAX * (maxCoord - minCoord); // y
        nodes[i][2] = minCoord + static_cast<double>(rand()) / RAND_MAX * (maxCoord - minCoord); // z
    }
    return nodes;
}

// quadratic points moved at 1/4 edge length area
double random_disturb_num(double low, double up, double scale = 0.15)
{
    std::random_device rd;
    std::mt19937 gen(rd());

    std::uniform_real_distribution<double> dist(-1.0, 1.0);
    double rand_num = dist(gen);

    return std::abs(up - low) * scale * rand_num;
}

void addQuadraticNodes(std::vector<std::vector<double>>& randomNodes, ElementType elmType)
{
    switch(elmType)
    {
        case ElementType::QUADRATIC_TRIANGLE:
        {
            int qdrNds = 3;
            std::vector<std::vector<double>> quadraticNodes(qrdNds, std::vector<double>(3, 0.0));
            for (int i = 0; i < qdrNds; ++i)
            {
                quadraticNodes[i][0] = 0.5 * (randomNodes[i][0] + randomNodes[(i+1)%qdrNds][0]) + random_disturb_num(randomNodes[i][0], randomNodes[(i+1)%qdrNds][0]);
                quadraticNodes[i][1] = 0.5 * (randomNodes[i][1] + randomNodes[(i+1)%qdrNds][1]) + random_disturb_num(randomNodes[i][1], randomNodes[(i+1)%qdrNds][1]);
                quadraticNodes[i][2] = 0.5 * (randomNodes[i][2] + randomNodes[(i+1)%qdrNds][2]) + random_disturb_num(randomNodes[i][2], randomNodes[(i+1)%qdrNds][2]);
            }
            randomNodes.insert(randomNodes.end(), quadraticNodes.begin(), quadraticNodes.end());
            break;
        }
        case ElementType::QUADRATIC_QUAD:
        {
            int qdrNds = 4;
            std::vector<std::vector<double>> quadraticNodes(qrdNds, std::vector<double>(3, 0.0));
            for (int i = 0; i < qdrNds; ++i)
            {
                quadraticNodes[i][0] = 0.5 * (randomNodes[i][0] + randomNodes[(i+1)%qdrNds][0]) + random_disturb_num(randomNodes[i][0], randomNodes[(i+1)%qdrNds][0]);
                quadraticNodes[i][1] = 0.5 * (randomNodes[i][1] + randomNodes[(i+1)%qdrNds][1]) + random_disturb_num(randomNodes[i][1], randomNodes[(i+1)%qdrNds][1]);
                quadraticNodes[i][2] = 0.5 * (randomNodes[i][2] + randomNodes[(i+1)%qdrNds][2]) + random_disturb_num(randomNodes[i][2], randomNodes[(i+1)%qdrNds][2]);
            }
            randomNodes.insert(randomNodes.end(), quadraticNodes.begin(), quadraticNodes.end());
            break;
        }
        case ElementType::QUADRATIC_TETRA:
        {
            int qdrNds = 6;
            std::vector<std::vector<double>> quadraticNodes(qrdNds, std::vector<double>(3, 0.0));
            for (int i = 0; i < 4; ++i)
            {
                quadraticNodes[i][0] = 0.5 * (randomNodes[i][0] + randomNodes[(i+1)%4][0]) + random_disturb_num(randomNodes[i][0], randomNodes[(i+1)%4][0]);
                quadraticNodes[i][1] = 0.5 * (randomNodes[i][1] + randomNodes[(i+1)%4][1]) + random_disturb_num(randomNodes[i][1], randomNodes[(i+1)%4][1]);
                quadraticNodes[i][2] = 0.5 * (randomNodes[i][2] + randomNodes[(i+1)%4][2]) + random_disturb_num(randomNodes[i][2], randomNodes[(i+1)%4][2]);
            }

            quadraticNodes[4][0] = 0.5 * (randomNodes[1][0] + randomNodes[3][0]) + random_disturb_num(randomNodes[1][0], randomNodes[3][0]);
            quadraticNodes[4][1] = 0.5 * (randomNodes[1][1] + randomNodes[3][1]) + random_disturb_num(randomNodes[1][1], randomNodes[3][1]);
            quadraticNodes[4][2] = 0.5 * (randomNodes[1][2] + randomNodes[3][2]) + random_disturb_num(randomNodes[1][2], randomNodes[3][2]);

            quadraticNodes[5][0] = 0.5 * (randomNodes[2][0] + randomNodes[3][0]) + random_disturb_num(randomNodes[2][0], randomNodes[3][0]);
            quadraticNodes[5][1] = 0.5 * (randomNodes[2][1] + randomNodes[3][1]) + random_disturb_num(randomNodes[2][1], randomNodes[3][1]);
            quadraticNodes[5][2] = 0.5 * (randomNodes[2][2] + randomNodes[3][2]) + random_disturb_num(randomNodes[2][2], randomNodes[3][2]);

            randomNodes.insert(randomNodes.end(), quadraticNodes.begin(), quadraticNodes.end());
            break;
        }
        case ElementType::QUADRATIC_HEX:
        {
            int qdrNds = 12;
            std::vector<std::vector<double>> quadraticNodes(qrdNds, std::vector<double>(3, 0.0));
            for (int i = 0; i < 4; ++i)
            {
                quadraticNodes[i][0] = 0.5 * (randomNodes[i][0] + randomNodes[(i+1)%4][0]) + random_disturb_num(randomNodes[i][0], randomNodes[(i+1)%4][0]);
                quadraticNodes[i][1] = 0.5 * (randomNodes[i][1] + randomNodes[(i+1)%4][1]) + random_disturb_num(randomNodes[i][1], randomNodes[(i+1)%4][1]);
                quadraticNodes[i][2] = 0.5 * (randomNodes[i][2] + randomNodes[(i+1)%4][2]) + random_disturb_num(randomNodes[i][2], randomNodes[(i+1)%4][2]);
            }

            for (int i = 0; i < 4; ++i)
            {
                quadraticNodes[i+4][0] = 0.5 * (randomNodes[i][0] + randomNodes[(i+4)%8][0]) + random_disturb_num(randomNodes[i][0], randomNodes[(i+4)%8][0]);
                quadraticNodes[i+4][1] = 0.5 * (randomNodes[i][1] + randomNodes[(i+4)%8][1]) + random_disturb_num(randomNodes[i][1], randomNodes[(i+4)%8][1]);
                quadraticNodes[i+4][2] = 0.5 * (randomNodes[i][2] + randomNodes[(i+4)%8][2]) + random_disturb_num(randomNodes[i][2], randomNodes[(i+4)%8][2]);
            }

            for (int i = 0; i < 4; ++i)
            {
                quadraticNodes[i+8][0] = 0.5 * (randomNodes[i+4][0] + randomNodes[(i+1)%4+4][0]) + random_disturb_num(randomNodes[i+4][0], randomNodes[(i+1)%4+4][0]);
                quadraticNodes[i+8][1] = 0.5 * (randomNodes[i+4][1] + randomNodes[(i+1)%4+4][1]) + random_disturb_num(randomNodes[i+4][1], randomNodes[(i+1)%4+4][1]);
                quadraticNodes[i+8][2] = 0.5 * (randomNodes[i+4][2] + randomNodes[(i+1)%4+4][2]) + random_disturb_num(randomNodes[i+4][2], randomNodes[(i+1)%4+4][2]);
            }
            randomNodes.insert(randomNodes.end(), quadraticNodes.begin(), quadraticNodes.end());
            break;
        }
    }
}

std::vector<std::vector<std::vector<double>>> getStandardElement(ElementType type, double minCoord, double maxCoord) {
    switch (type) {
        case ElementType::LINEAR_TRIANGLE: {
            std::vector<std::vector<std::vector<double>>> elements;
            // ET
            std::vector<std::vector<double>> nodes(3, std::vector<double>(3, 0.0));
            nodes[0][0] = minCoord; nodes[0][1] = minCoord; nodes[0][2] = minCoord;
            nodes[1][0] = maxCoord; nodes[1][1] = minCoord; nodes[1][2] = maxCoord;
            nodes[2][0] = minCoord; nodes[2][1] = maxCoord; nodes[2][2] = maxCoord;
            elements.push_back(nodes);
            // RT
            std::vector<std::vector<double>> nodes1(3, std::vector<double>(3, 0.0));
            nodes1[0][0] = minCoord; nodes1[0][1] = minCoord; nodes1[0][2] = minCoord;
            nodes1[1][0] = maxCoord; nodes1[1][1] = minCoord; nodes1[1][2] = minCoord;
            nodes1[2][0] = minCoord; nodes1[2][1] = maxCoord; nodes1[2][2] = maxCoord;
            elements.push_back(nodes1);
            return elements;
        }
        case ElementType::LINEAR_QUAD: {
            std::vector<std::vector<std::vector<double>>> elements;
            // square
            std::vector<std::vector<double>> nodes(4, std::vector<double>(3, 0.0));
            nodes[0][0] = minCoord; nodes[0][1] = minCoord; nodes[0][2] = minCoord;
            nodes[1][0] = maxCoord; nodes[1][1] = minCoord; nodes[1][2] = minCoord;
            nodes[2][0] = maxCoord; nodes[2][1] = minCoord; nodes[2][2] = maxCoord;
            nodes[3][0] = minCoord; nodes[3][1] = minCoord; nodes[3][2] = maxCoord;
            elements.push_back(nodes);
            // 10:1 rectangle
            std::vector<std::vector<double>> nodes1(4, std::vector<double>(3, 0.0));
            nodes1[0][0] = minCoord; nodes1[0][1] = minCoord; nodes1[0][2] = minCoord;
            nodes1[1][0] = maxCoord; nodes1[1][1] = minCoord; nodes1[1][2] = minCoord;
            nodes1[2][0] = maxCoord; nodes1[2][1] = minCoord; nodes1[2][2] = minCoord + 0.1 * (maxCoord - minCoord);
            nodes1[3][0] = minCoord; nodes1[3][1] = minCoord; nodes1[3][2] = minCoord + 0.1 * (maxCoord - minCoord);
            elements.push_back(nodes1);
            // tri-rectangle
            std::vector<std::vector<double>> nodes2(4, std::vector<double>(3, 0.0));
            nodes1[0][0] = minCoord; nodes1[0][1] = minCoord; nodes1[0][2] = minCoord;
            nodes1[1][0] = maxCoord; nodes1[1][1] = minCoord; nodes1[1][2] = minCoord;
            nodes2[2][0] = 0.5 * (minCoord + maxCoord); nodes2[2][1] = 0.5 * (minCoord + maxCoord); nodes1[2][2] = 0.5 * (minCoord + maxCoord);
            nodes1[3][0] = minCoord; nodes1[3][1] = maxCoord; nodes1[3][2] = maxCoord;
            elements.push_back(nodes1);
            return elements;
        }
        case ElementType::LINEAR_TETRA: {
            std::vector<std::vector<std::vector<double>>> elements;
            // regular tetra
            std::vector<std::vector<double>> nodes(4, std::vector<double>(3, 0.0));
            nodes[0][0] = minCoord; nodes[0][1] = minCoord; nodes[0][2] = minCoord;
            nodes[1][0] = maxCoord; nodes[1][1] = minCoord; nodes[1][2] = maxCoord;
            nodes[2][0] = maxCoord; nodes[2][1] = maxCoord; nodes[2][2] = minCoord;
            nodes[3][0] = minCoord; nodes[3][1] = maxCoord; nodes[3][2] = maxCoord;
            elements.push_back(nodes);
            // right angle tetra
            std::vector<std::vector<double>> nodes1(4, std::vector<double>(3, 0.0));
            nodes1[0][0] = minCoord; nodes1[0][1] = minCoord; nodes1[0][2] = minCoord;
            nodes1[1][0] = maxCoord; nodes1[1][1] = minCoord; nodes1[1][2] = minCoord;
            nodes1[2][0] = minCoord; nodes1[2][1] = maxCoord; nodes1[2][2] = minCoord;
            nodes1[3][0] = minCoord; nodes1[3][1] = minCoord; nodes1[3][2] = maxCoord;
            elements.push_back(nodes1);
            return elements;
        }
        case ElementType::LINEAR_HEXA: {
            std::vector<std::vector<std::vector<double>>> elements;
            // Cube
            std::vector<std::vector<double>> nodes(8, std::vector<double>(3, 0.0));
            nodes[0][0] = minCoord; nodes[0][1] = minCoord; nodes[0][2] = minCoord;
            nodes[1][0] = maxCoord; nodes[1][1] = minCoord; nodes[1][2] = minCoord;
            nodes[2][0] = maxCoord; nodes[2][1] = maxCoord; nodes[2][2] = minCoord;
            nodes[3][0] = minCoord; nodes[3][1] = maxCoord; nodes[3][2] = minCoord;
            nodes[4][0] = minCoord; nodes[4][1] = minCoord; nodes[4][2] = maxCoord;
            nodes[5][0] = maxCoord; nodes[5][1] = minCoord; nodes[5][2] = maxCoord;
            nodes[6][0] = maxCoord; nodes[6][1] = maxCoord; nodes[6][2] = maxCoord;
            nodes[7][0] = minCoord; nodes[7][1] = maxCoord; nodes[7][2] = maxCoord;
            elements.push_back(nodes);
            // 10:10:1 Cube
            std::vector<std::vector<double>> nodes1(8, std::vector<double>(3, 0.0));
            nodes1[0][0] = minCoord; nodes1[0][1] = minCoord; nodes1[0][2] = minCoord;
            nodes1[1][0] = maxCoord; nodes1[1][1] = minCoord; nodes1[1][2] = minCoord;
            nodes1[2][0] = maxCoord; nodes1[2][1] = maxCoord; nodes1[2][2] = minCoord;
            nodes1[3][0] = minCoord; nodes1[3][1] = maxCoord; nodes1[3][2] = minCoord;
            nodes1[4][0] = minCoord; nodes1[4][1] = minCoord; nodes1[4][2] = minCoord + 0.1 * (maxCoord - minCoord);
            nodes1[5][0] = maxCoord; nodes1[5][1] = minCoord; nodes1[5][2] = minCoord + 0.1 * (maxCoord - minCoord);
            nodes1[6][0] = maxCoord; nodes1[6][1] = maxCoord; nodes1[6][2] = minCoord + 0.1 * (maxCoord - minCoord);
            nodes1[7][0] = minCoord; nodes1[7][1] = maxCoord; nodes1[7][2] = minCoord + 0.1 * (maxCoord - minCoord);
            elements.push_back(nodes1);
            return elements;
        }
        case ElementType::LINEAR_PYRAMID: {
            std::vector<std::vector<std::vector<double>>> elements;
            //
            std::vector<std::vector<double>> nodes(5, std::vector<double>(3, 0.0));
            nodes[0][0] = minCoord; nodes[0][1] = minCoord; nodes[0][2] = minCoord;
            nodes[1][0] = maxCoord; nodes[1][1] = minCoord; nodes[1][2] = minCoord;
            nodes[2][0] = maxCoord; nodes[2][1] = maxCoord; nodes[2][2] = minCoord;
            nodes[3][0] = minCoord; nodes[3][1] = maxCoord; nodes[3][2] = minCoord;
            nodes[4][0] = 0.5 * (minCoord + maxCoord); nodes[4][1] = 0.5 * (minCoord + maxCoord); nodes[4][2] = maxCoord;
            elements.push_back(nodes);
            // right angle pyramid
            std::vector<std::vector<double>> nodes1(5, std::vector<double>(3, 0.0));
            nodes1[0][0] = minCoord; nodes1[0][1] = minCoord; nodes1[0][2] = minCoord;
            nodes1[1][0] = maxCoord; nodes1[1][1] = minCoord; nodes1[1][2] = minCoord;
            nodes1[2][0] = maxCoord; nodes1[2][1] = maxCoord; nodes1[2][2] = minCoord;
            nodes1[3][0] = minCoord; nodes1[3][1] = maxCoord; nodes1[3][2] = minCoord;
            nodes1[4][0] = minCoord; nodes1[4][1] = minCoord; nodes1[4][2] = maxCoord;
            elements.push_back(nodes1);
            return elements;
        }
        case ElementType::LINEAR_PRISM: {
            std::vector<std::vector<std::vector<double>>> elements;
            // Isosceles prism
            std::vector<std::vector<double>> nodes(6, std::vector<double>(3, 0.0));
            nodes[0][0] = minCoord; nodes[0][1] = minCoord; nodes[0][2] = minCoord;
            nodes[1][0] = maxCoord; nodes[1][1] = minCoord; nodes[1][2] = minCoord;
            nodes[2][0] = 0.5 * (minCoord + maxCoord); nodes[2][1] = maxCoord; nodes[2][2] = minCoord;
            nodes[3][0] = minCoord; nodes[3][1] = minCoord; nodes[3][2] = maxCoord;
            nodes[4][0] = maxCoord; nodes[4][1] = minCoord; nodes[4][2] = maxCoord;
            nodes[5][0] = 0.5 * (minCoord + maxCoord); nodes[5][1] = maxCoord; nodes[5][2] = maxCoord;
            elements.push_back(nodes);
            // right angle prism
            std::vector<std::vector<double>> nodes1(6, std::vector<double>(3, 0.0));
            nodes1[0][0] = minCoord; nodes1[0][1] = minCoord; nodes1[0][2] = minCoord;
            nodes1[1][0] = maxCoord; nodes1[1][1] = minCoord; nodes1[1][2] = minCoord;
            nodes1[2][0] = minCoord; nodes1[2][1] = maxCoord; nodes1[2][2] = minCoord;
            nodes1[3][0] = minCoord; nodes1[3][1] = minCoord; nodes1[3][2] = maxCoord;
            nodes1[4][0] = maxCoord; nodes1[4][1] = minCoord; nodes1[4][2] = maxCoord;
            nodes1[5][0] = minCoord; nodes1[5][1] = maxCoord; nodes1[5][2] = maxCoord;
            elements.push_back(nodes1);
            return elements;
        }
    }
}

std::vector<std::vector<std::vector<double>>> generateElements(ElementType type, int numElements, double minCoord, double maxCoord) {
    std::vector<std::vector<std::vector<double>>> elements = getStandardElement(type, minCoord, maxCoord);
    for (int i = 2; i < numElements; ++i) {
        std::vector<std::vector<double>> randomNodes;
        switch (type) {
            case ElementType::LINEAR_TRIANGLE: {
                randomNodes = generateNodes(3, minCoord, maxCoord);
                break;
            }
            case ElementType::LINEAR_QUAD: {
                randomNodes = generateNodes(4, minCoord, maxCoord);
                break;
            }
            case ElementType::LINEAR_TETRA: {
                randomNodes = generateNodes(4, minCoord, maxCoord);
                break;
            }
            case ElementType::LINEAR_HEXA: {
                randomNodes = generateNodes(8, minCoord, maxCoord);
                break;
            }
            case ElementType::LINEAR_PYRAMID: {
                randomNodes = generateNodes(5, minCoord, maxCoord);
                break;
            }
            case ElementType::LINEAR_PRISM: {
                randomNodes = generateNodes(6, minCoord, maxCoord);
                break;
            }
            case ElementType::QUADRATIC_TRIANGLE: {
                randomNodes = generateNodes(3, minCoord, maxCoord);
                addQuadraticNodes(randomNodes, ElementType::QUADRATIC_TRIANGLE);
                break;
            }
            case ElementType::QUADRATIC_QUAD: {
                randomNodes = generateNodes(4, minCoord, maxCoord);
                addQuadraticNodes(randomNodes, ElementType::QUADRATIC_QUAD);
                break;
            }
            case ElementType::QUADRATIC_TETRA: {
                randomNodes = generateNodes(4, minCoord, maxCoord);
                addQuadraticNodes(randomNodes, ElementType::QUADRATIC_TETRA);
                break;
            }
            case ElementType::QUADRATIC_HEX: {
                randomNodes = generateNodes(8, minCoord, maxCoord);
                addQuadraticNodes(randomNodes, ElementType::QUADRATIC_HEX);
                break;
            }
        }
        try {
            elements.push_back(randomNodes);
        }
        catch (const std::bad_alloc& e)
        {
            std::cerr << e.what() << std::endl;
        }

    }
    return elements;
}

int main() {
    std::srand(static_cast<unsigned int>(std::time(0)));

    ElementType type = ElementType::QUADRATIC_TRIANGLE;
    int numElements = 10;
    double minCoord = 0.0;
    double maxCoord = 10.0;

    std::vector<std::vector<std::vector<double>>> element = generateElements(type, numElements, minCoord, maxCoord);

    std::ofstream outFile;

    outFile.open("D:\\Metric.txt");
    if(!outFile.is_open()) {
        std::cerr << "Failed to open the file." <<std::endl;
        return 1;
    }

    for (int i = 0; i < (int)element.size(); ++i) {
        for (int j = 0; j < (int)element[i].size(); ++j) {
            outFile << "{" << element[i][j][0] << "," << element[i][j][1] << "," << element[i][j][2] << "}";
        }
        outFile << std::endl;
    }
    outFile.close();

    for (int i = 0; i < (int)element.size(); ++i)
    {
        std::string filePath = "D:\\RandomElement" + std::to_string(i) + ".nas";
        outFile.open(filePath.c_str());
        if(!outFile.is_open())
        {
            std::cerr << "Failed to open the file!" << std::endl;
            return 1;
        }
        outFile << "BEGIN BULK" <<std::endl;
        for (int j = 0; j < (int)element[i].size(); ++j)
        {
            outFile << "GRID," << j+1 << ",," << element[i][j][0] << "," << element[i][j][1] << "," << element[i][j][2] << std::endl;
        }
        switch(type)
        {
            case ElementType::LINEAR_TRIANGLE:
                outFile << "CTRIA3,1,1,1,2,3" << std::endl; 
                break;
            case ElementType::LINEAR_QUAD:
                outFile << "CQUAD4,1,1,1,2,3,4" << std::endl;
                break;
            case ElementType::LINEAR_TETRA:
                outFile << "CTETRA,1,1,1,2,3,4" << std::endl;
                break;
            case ElementType::LINEAR_HEX:
                outFile << "CHEXA,1,1,1,2,3,4,5,6,7,8" << std::endl;
                break;
            case ElementType::QUADRATIC_TRIANGLE:
                outFile << "CTRIA6,1,1,1,2,3,4,5,6" << std::endl;
                break;
            case ElementType::QUADRATIC_QUAD:
                outFile << "CQUAD8,1,1,1,2,3,4,5,6,7,8" << std::endl;
                break;
            case ElementType::QUADRATIC_TETRA:
                outFile << "CTETRA,1,1,1,2,3,4,5,6,7,8,9,10" << std::endl;
                break;
            case ElementType::QUADRATIC_HEX:
                outFile << "CHEXA,1,1,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,18,20" << std::endl;
                break;
        }
        outFile << "ENDDATA" << std::endl;
        outFile.close();
     }
    std::cout << "Data has been written to the file." << std::endl;
    return 0;
}