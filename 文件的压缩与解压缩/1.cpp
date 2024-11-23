#include <iostream>
#include <fstream>
#include <unordered_map>
#include <vector>
#include <queue>
#include <string>

//���������������ļ�ѹ��:ʹ��Ƶ�ʸߵ��ַ�ʹ�ý϶̵ı���

//��ȡ�ļ�, ͳ��ÿ���ʳ��ֵ�Ƶ��
void countFrequency(const std::string& filename, std::unordered_map<char, int>& freq) {
    std::ifstream file(filename);
    char ch;
    while (file >> std::noskipws >> ch) {
        freq[ch]++;
    }
    file.close();
}

// ���������ڵ�
struct HuffmanNode {
    char ch;
    int freq;
    HuffmanNode* left;
    HuffmanNode* right;
    HuffmanNode(char c, int f) : ch(c), freq(f), left(nullptr), right(nullptr) {}
};

// �ȽϺ������������ȶ���
struct Compare {
    bool operator()(HuffmanNode* l, HuffmanNode* r) {
        return l->freq > r->freq;
    }
};

// ������������
//ʹ�ö������ҳ����ִ����ٵĽڵ���кϲ�
HuffmanNode* buildHuffmanTree(const std::unordered_map<char, int>& freq) {
    std::priority_queue<HuffmanNode*, std::vector<HuffmanNode*>, Compare> minHeap;

    for (const auto& pair : freq) {
        minHeap.push(new HuffmanNode(pair.first, pair.second));
    }

    while (minHeap.size() > 1) { // ȷ�� minHeap �������������ڵ�
        HuffmanNode* left = minHeap.top(); minHeap.pop();
        HuffmanNode* right = minHeap.top(); minHeap.pop();

        HuffmanNode* newNode = new HuffmanNode('\0', left->freq + right->freq);
        newNode->left = left;
        newNode->right = right;
        minHeap.push(newNode);
    }

    return minHeap.empty() ? nullptr : minHeap.top(); // ��� minHeap Ϊ�գ����� nullptr
}


// Ϊÿ���ַ����ɶ�Ӧ�Ĺ�����������
void generateCodes(HuffmanNode* root, std::string str, std::unordered_map<char, std::string>& huffmanCode) {
    if (!root) return;

    if (!root->left && !root->right) {
        huffmanCode[root->ch] = str;
    }

    generateCodes(root->left, str + "0", huffmanCode);
    generateCodes(root->right, str + "1", huffmanCode);
}

//  ���������ɵĻ��������뱣�浽�ļ��У��Ա��ڽ�ѹ��ʱʹ��
void saveHuffmanCodes(const std::unordered_map<char, std::string>& huffmanCode, const std::string& filename) {
    std::ofstream file(filename);
    for (const auto& pair : huffmanCode) {
        file << pair.first << " " << pair.second << "\n";
    }
    file.close();
}

// ʹ�û����������ԭʼ�ļ����б��룬���������ַ���д�����ļ��У�ʵ���ļ�ѹ��
void compressFile(const std::string& srcFilename, const std::string& dstFilename, const std::unordered_map<char, std::string>& huffmanCode) {
    std::ifstream srcFile(srcFilename);
    std::ofstream dstFile(dstFilename, std::ios::binary);
    char ch;
    std::string encodedStr;
    while (srcFile >> std::noskipws >> ch) {
        encodedStr += huffmanCode.at(ch);
    }

    // ���������ַ���д��ѹ���ļ�
    dstFile << encodedStr;
    srcFile.close();
    dstFile.close();
}

// ���ݻ���������ͻ���������ѹ���ļ����н��룬��ԭ��ԭʼ�ļ�����
void decompressFile(const std::string& encodedFilename, const std::string& outputFilename, HuffmanNode* root) {
    std::ifstream encodedFile(encodedFilename, std::ios::binary);
    std::ofstream outputFile(outputFilename);
    HuffmanNode* currentNode = root;
    char bit;
    while (encodedFile.get(bit)) {
        currentNode = (bit == '0') ? currentNode->left : currentNode->right;
        if (!currentNode->left && !currentNode->right) {
            outputFile.put(currentNode->ch);
            currentNode = root;
        }
    }
    encodedFile.close();
    outputFile.close();
}

// ������
int main() {
    // �����ļ���
    std::string filename = "input.txt";
    std::cout << "����ѹ���ļ�: "<< filename << std::endl;
    std::cout << "��ȡ�ļ���" <<  std::endl;

    // ͳ�ƴ�Ƶ
    std::cout << "ͳ�ƴ�Ƶ��" << std::endl;
    std::unordered_map<char, int> freq;
    countFrequency(filename, freq);

    // ������������
    std::cout << "��������������" << std::endl;
    HuffmanNode* root = buildHuffmanTree(freq);

    // ���ɹ���������
    std::unordered_map<char, std::string> huffmanCode;
    generateCodes(root, "", huffmanCode);

    // ������������뵽�ļ�
    std::cout << "�����ֵ� ��������" << std::endl;
    saveHuffmanCodes(huffmanCode, "HufCode.txt");

    // ѹ���ļ�
    std::cout << "����ѹ���ļ�" << std::endl;
    compressFile(filename, "CodeFile.dat", huffmanCode);

    // ��ѹ���ļ�
    decompressFile("CodeFile.dat", "output.txt", root);

    std::cout << "�ļ�ѹ���ͽ�ѹ�����!" << std::endl;

    return 0;
}
