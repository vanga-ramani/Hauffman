#include<bits/stdc++.h>
using namespace std;

class huffmanNode {
    public:
    huffmanNode *left;
    huffmanNode *right;
    int freq;
    char letter; // num
    huffmanNode(int key, char c) {
        freq = key;
        letter = c;
        left = NULL;
        right = NULL;
    }
};

struct PriorityQueue {
private:
    vector<huffmanNode*>A;

    void heapify_down(int i) {
        int left = 2 * i + 1;
        int right = 2 * i + 2;
        int smallest = i;

        if (left < A.size() && A[left]->freq < A[i]->freq) {
            smallest = left;
        }

        if (right < A.size() && A[right]->freq < A[smallest]->freq) {
            smallest = right;
        }

        if (smallest != i) {
            swap(A[i], A[smallest]);
            heapify_down(smallest);
        }
    }

    void heapify_up(int i) {
        int parent = (i - 1) / 2;
        if (i && A[parent]->freq > A[i]->freq) {
            swap(A[i], A[parent]);
            heapify_up(parent);
        }
    }

public:
    int size() {
        return A.size();
    }

    bool empty() {
        return A.size() == 0;
    }

    void push(huffmanNode* key) {
        A.push_back(key);
        int index = A.size() - 1;
        heapify_up(index);
    }

    void pop() {
        if (A.size() == 0) {
            return;
        }

        A[0] = A.back();
        A.pop_back();

        heapify_down(0);
    }

    huffmanNode* top() {
        if (A.size() != 0) {
            return A.at(0);
        }

        huffmanNode* node = new huffmanNode(0, '#');
        return node;
    }
};

class comparenode
{
public:
    bool operator()(huffmanNode *nodeA, huffmanNode *nodeB) {
        return nodeA->freq > nodeB->freq;
    }
};

map<char, pair<int, string>>freq;
huffmanNode* root;

void decodeData(huffmanNode* root) {
    ifstream file;
    string encodedString;
    file.open("encoded_data.txt");
    file >> encodedString;
    file.close();
    string decodedMessage = "";
    huffmanNode* node = root;
    for (char c: encodedString) {
        if (c == '0') node = node->left;
        else node = node->right;

        if (node -> left == NULL and node->right == NULL) {
            decodedMessage += node -> letter;
            node = root;
        }
    }
    ofstream fileWrite;
    fileWrite.open("target_message.txt");
    fileWrite << decodedMessage;
    fileWrite << endl;
    fileWrite.close();
    return;
}

void generateCodes(huffmanNode *root, vector<int>&arr, int top) {
    if (root->left) {
        arr[top] = 0;
        generateCodes(root->left, arr, top + 1);
    } if (root->right) {
        arr[top] = 1;
        generateCodes(root->right, arr, top + 1);
    }

    if (root->left == NULL && root->right == NULL) {
        ofstream file;
        file.open("huffmanTable.txt", std::ios_base::app);
        file << root->letter << " ";
        string binary = "";
        for (int i = 0; i < top; i++) {
            binary += to_string(arr[i]);
            file << arr[i];
        }
        freq[root -> letter].second = binary;
        file << endl;
        file.close();
    }
    return;
}

huffmanNode* generateTree(struct PriorityQueue pq) {
    while (pq.size() != 1) {
        huffmanNode *store_left = pq.top();
        pq.pop();
        huffmanNode *store_right = pq.top();
        pq.pop();
        huffmanNode* node = new huffmanNode(store_left->freq + store_right->freq, '*');
        node->left = store_left;
        node->right = store_right;
        pq.push(node);
    }
    return pq.top();
}

void encodeData(string message) {
    struct PriorityQueue pq;
    for(auto it: freq) {
        huffmanNode* node = new huffmanNode(it.second.first, it.first);
        pq.push(node);
    }
    ofstream file;
    file.open("huffmanTable.txt");
    file << pq.size();
    file << endl;
    file.close();

    root = generateTree(pq);

    vector<int>arr(pq.size());
    generateCodes(root, arr, 0);
    file.open("encoded_data.txt");
    for(char c: message) {
        string codedString = "";
        codedString += freq[c].second;
        file << codedString;
    }
    file << endl;
    file.close();
    return;
}

void generateFreq(string message) {
    for (char c: message) {
        freq[c].first++;
    }
}


int main() {
    ifstream file;
    file.open("message.txt");
    string message;
    file >> message;
    file.close();
    generateFreq(message);
    encodeData(message);
    decodeData(root);
    return 0;
}



