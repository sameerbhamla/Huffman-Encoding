#include <iostream>
#include <fstream>
#include <string>
#include <cmath>

using namespace std;

//making listNode class
class listNode {
public:
    int data;
    listNode* next;

    listNode(int data) {
        this->data = data;
        this->next = NULL;
    }
};

//Making Linked List Queue
class LLQueue {
public:
    listNode* head;
    listNode* tail;

    LLQueue() {

        head = new listNode(-1);
        tail = head;
    }

    //insertQ method
    void insertQ(int table, int index, listNode* newNode) {
        tail->next = newNode;
        tail = newNode;
    }

    //if Q is not empty delete and return node after the dummy
    listNode* deleteQ(int table, int index) {
        if (isEmpty()) {
            return NULL;
        }
        listNode* nodeToDelete = head->next;
        head->next = nodeToDelete->next;
        if (head->next == NULL) {
            tail = head;
        }
        return nodeToDelete;
    }

    //checks if queue in empty
    bool isEmpty() {
        return (tail == head);
    }

    void printQueue(int whichTable, int index, ofstream& outFile) {
        outFile << "Printing one Queue" << endl;
        outFile << "Table [" << whichTable << "][" << index << "]: ";
        listNode* curr = head->next;
        while (curr != NULL) {
            if (curr->next != NULL) {
                outFile << "(" << curr->data << ", " << curr->next->data << ") -> ";
            }
            else {
                outFile << "(" << curr->data << ", NULL) -> NULL" << endl;
            }
            curr = curr->next;
        }
    }
};

class RadixSort {
public:
    const int tableSize = 10;
    LLQueue hashTable[2][10];
    int data;
    int currentTable;
    int previousTable;
    int maxDigits;
    int offSet;
    int digitPosition;
    int currentDigit;

    RadixSort() {
        for (int i = 0; i < 2; i++) {
            for (int j = 0; j < tableSize; j++) {
                hashTable[i][j] = LLQueue();
            }
        }
    }

    void preProcessing(ifstream& inFile, ofstream& deBugFile) {
        deBugFile << "*** Performing firstReading" << endl;
        //step 0
        int positiveNum = 0;
        int negativeNum = 0;
        //step 1 and 2
        while (inFile >> data) {
            if (data > positiveNum) {
                positiveNum = data;
            }
            if (data < negativeNum) {
                negativeNum = data;
            }
        }
        // step 3
        offSet = abs(negativeNum);
        maxDigits = ceil(log10(positiveNum + offSet));

        deBugFile << "negativeNum is " << negativeNum << endl;
        deBugFile << "positiveNum is " << positiveNum << endl;
        if (offSet < 0) {
            deBugFile << "offSet is " << offSet << endl;
        }
        else {
            deBugFile << "There is no offset, no negative numbers" << endl;
        }
        inFile.clear();
        inFile.seekg(0, ios::beg);
    }

    void RSort(ifstream& inFile, ofstream& deBugFile, ofstream& outFile) {

        deBugFile << "Entered RSort Method" << endl;
        preProcessing(inFile, deBugFile);
        deBugFile << "preProccessing has finished" << endl;
        currentTable = 0;
        previousTable = 1;

        //inserting each data from file
        for (int i = 0; i < maxDigits; i++) {
            while (inFile >> data) {
                int hashIndex = getDigit(data + offSet, i);
                listNode* newNode = new listNode(data);
                hashTable[currentTable][hashIndex].insertQ(currentTable, hashIndex, newNode);
            }
            //printing table after each iteration
            printTable(currentTable, deBugFile);

            for (int j = 0; j < tableSize; j++) {
                while (!hashTable[previousTable][j].isEmpty()) {
                    listNode* nodeToMove = hashTable[previousTable][j].deleteQ(previousTable, j);
                    int hashIndex = getDigit(nodeToMove->data + offSet, i);
                    hashTable[currentTable][hashIndex].insertQ(currentTable, hashIndex, nodeToMove);
                }
            }

            //print whole table
            printTable(currentTable, outFile);

            currentDigit++;
            swap(currentTable, previousTable);
            printSortedData(outFile);

        }

    }

    //getDigit method
    int getDigit(int number, int digitPosition) {
        return (number / (int)pow(10, digitPosition)) % 10;
    }

    //Printing the queue in each table index
    void printTable(int whichTable, ofstream& outFile) {
        for (int i = 0; i < tableSize; i++) {
            hashTable[whichTable][i].printQueue(whichTable, i, outFile);
        }
    }

    void printSortedData(ofstream& outFile) {
        outFile << "Entered printSortedData Method" << endl;
        int count = 0;
        for (int i = 0; i < tableSize; i++) {
            while (!hashTable[previousTable][i].isEmpty()) {
                listNode* nodeToDelete = hashTable[previousTable][i].deleteQ(previousTable, i);
                if (count % 10 == 0 && count != 0) {
                    outFile << endl;
                }
                outFile << nodeToDelete->data << " ";
                count++;
                delete nodeToDelete;
            }
        }
    }
};

int main(int argc, char** argv) {
    if (argc != 4) {
        cout << "Only allowed three arguments";
        return 1;
    }
    //opening files
    ifstream inFile(argv[1]);
    ofstream outFile(argv[2]);
    ofstream deBugFile(argv[3]);

    RadixSort rs;
    rs.RSort(inFile, deBugFile, outFile);

    //closing files
    inFile.close();
    deBugFile.close();
    outFile.close();
    return 0;
}
