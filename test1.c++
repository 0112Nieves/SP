#include <iostream>
#include <vector>
#include <utility>  // 包含对 std::pair 的支持

int main() {
    // 创建一个 vector，其中每个元素都是一个包含两个 string 元素的 pair
    std::vector<std::pair<std::string, std::string>> stringPairs;

    // 向 vector 中添加一些数据
    stringPairs.push_back(std::make_pair("Hello", "World"));
    stringPairs.push_back(std::make_pair("OpenAI", "GPT-3"));

    // 遍历 vector 并输出每个 pair 中的两个字符串
    for (const auto& pair : stringPairs) {
        std::cout << "First: " << pair.first << ", Second: " << pair.second << std::endl;
    }

    return 0;
}
