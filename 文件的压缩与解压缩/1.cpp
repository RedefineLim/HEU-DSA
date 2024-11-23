#include <iostream>
#include <fstream>
#include <unordered_map>
#include <vector>
#include <queue>
#include <string>

//霍夫曼编码用于文件压缩:使用频率高的字符使用较短的编码

//读取文件, 统计每个词出现的频率
void countFrequency(const std::string& filename, std::unordered_map<char, int>& freq) {
    std::ifstream file(filename);
    char ch;
    while (file >> std::noskipws >> ch) {
        freq[ch]++;
    }
    file.close();
}

// 哈夫曼树节点
struct HuffmanNode {
    char ch;
    int freq;
    HuffmanNode* left;
    HuffmanNode* right;
    HuffmanNode(char c, int f) : ch(c), freq(f), left(nullptr), right(nullptr) {}
};

// 比较函数，用于优先队列
struct Compare {
    bool operator()(HuffmanNode* l, HuffmanNode* r) {
        return l->freq > r->freq;
    }
};

// 构建哈夫曼树
//使用堆排序找出出现次数少的节点进行合并
HuffmanNode* buildHuffmanTree(const std::unordered_map<char, int>& freq) {
    std::priority_queue<HuffmanNode*, std::vector<HuffmanNode*>, Compare> minHeap;

    for (const auto& pair : freq) {
        minHeap.push(new HuffmanNode(pair.first, pair.second));
    }

    while (minHeap.size() > 1) { // 确保 minHeap 中至少有两个节点
        HuffmanNode* left = minHeap.top(); minHeap.pop();
        HuffmanNode* right = minHeap.top(); minHeap.pop();

        HuffmanNode* newNode = new HuffmanNode('\0', left->freq + right->freq);
        newNode->left = left;
        newNode->right = right;
        minHeap.push(newNode);
    }

    return minHeap.empty() ? nullptr : minHeap.top(); // 如果 minHeap 为空，返回 nullptr
}


// 为每个字符生成对应的哈夫曼树编码
void generateCodes(HuffmanNode* root, std::string str, std::unordered_map<char, std::string>& huffmanCode) {
    if (!root) return;

    if (!root->left && !root->right) {
        huffmanCode[root->ch] = str;
    }

    generateCodes(root->left, str + "0", huffmanCode);
    generateCodes(root->right, str + "1", huffmanCode);
}

//  函数将生成的霍夫曼编码保存到文件中，以便在解压缩时使用
void saveHuffmanCodes(const std::unordered_map<char, std::string>& huffmanCode, const std::string& filename) {
    std::ofstream file(filename);
    for (const auto& pair : huffmanCode) {
        file << pair.first << " " << pair.second << "\n";
    }
    file.close();
}

// 使用霍夫曼编码对原始文件进行编码，将编码后的字符串写入新文件中，实现文件压缩
void compressFile(const std::string& srcFilename, const std::string& dstFilename, const std::unordered_map<char, std::string>& huffmanCode) {
    std::ifstream srcFile(srcFilename);
    std::ofstream dstFile(dstFilename, std::ios::binary);
    char ch;
    std::string encodedStr;
    while (srcFile >> std::noskipws >> ch) {
        encodedStr += huffmanCode.at(ch);
    }

    // 将编码后的字符串写入压缩文件
    dstFile << encodedStr;
    srcFile.close();
    dstFile.close();
}

// 根据霍夫曼编码和霍夫曼树对压缩文件进行解码，还原出原始文件内容
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

// 主函数
int main() {
    // 输入文件名
    std::string filename = "input.txt";
    std::cout << "正在压缩文件: "<< filename << std::endl;
    std::cout << "读取文件中" <<  std::endl;

    // 统计词频
    std::cout << "统计词频中" << std::endl;
    std::unordered_map<char, int> freq;
    countFrequency(filename, freq);

    // 构建哈夫曼树
    std::cout << "构建哈夫曼树中" << std::endl;
    HuffmanNode* root = buildHuffmanTree(freq);

    // 生成哈夫曼编码
    std::unordered_map<char, std::string> huffmanCode;
    generateCodes(root, "", huffmanCode);

    // 保存哈夫曼编码到文件
    std::cout << "保存字典 保存编码表" << std::endl;
    saveHuffmanCodes(huffmanCode, "HufCode.txt");

    // 压缩文件
    std::cout << "保存压缩文件" << std::endl;
    compressFile(filename, "CodeFile.dat", huffmanCode);

    // 解压缩文件
    decompressFile("CodeFile.dat", "output.txt", root);

    std::cout << "文件压缩和解压缩完成!" << std::endl;

    return 0;
}
