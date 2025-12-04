#include "TextStatsTool.h"
#include <iostream>
#include <sstream>
#include <string>
#include <unordered_map>
#include <vector>
#include <algorithm>
#include <cctype>
#include <set>
#include <limits>

static inline std::string to_lower(const std::string& s) {
    std::string o; o.reserve(s.size());
    for (char c : s) o.push_back((char)std::tolower((unsigned char)c));
    return o;
}

static inline bool is_word_char(char c) {
    return std::isalnum((unsigned char)c) || c == '\''; // keep contractions as part of word
}

static std::vector<std::string> tokenize_words(const std::string& text) {
    std::vector<std::string> words;
    std::string cur;
    for (size_t i = 0; i < text.size(); ++i) {
        char c = text[i];
        if (is_word_char(c)) cur.push_back(c);
        else {
            if (!cur.empty()) {
                words.push_back(cur);
                cur.clear();
            }
        }
    }
    if (!cur.empty()) words.push_back(cur);
    return words;
}

static const std::set<std::string> STOPWORDS = {
    "the","and","is","in","it","of","to","a","an","that","this","on","for","with","as","are","was","were","be","by","or","from","at","which","but","not","they","their","i","you","he","she","we","his","her","them"
};

void TextStatsTool::run() {
    std::cout << "\n=== Text Stats ===\n";
    std::cout << "请输入多行文本，完成后输入一个空行（直接回车）结束输入。\n";
    std::cout << "输入命令: summary (显示统计) ; quit (退出)\n";

    // read multi-line input until an empty line or 'quit' typed alone
    std::string line;
    std::string text;
    while (true) {
        std::string chunk;
        bool anyInput = false;
        while (true) {
            if (!std::getline(std::cin, line)) break;
            if (line.empty()) break; // empty line ends text input
            if (line == "quit") return;
            if (line == "summary") {
                // user wants to show summary of current collected text
                break;
            }
            anyInput = true;
            text += line;
            text.push_back('\n');
        }

        // calculate stats for current 'text' (could be empty)
        int chars_total = (int)text.size();
        int chars_no_space = 0;
        for (char c : text) if (!std::isspace((unsigned char)c)) ++chars_no_space;

        auto words = tokenize_words(text);
        int word_count = (int)words.size();

        // sentence count by counting '.', '!', '?'
        int sentence_count = 0;
        for (char c : text) if (c == '.' || c == '!' || c == '?') ++sentence_count;
        if (sentence_count == 0 && word_count > 0) sentence_count = 1; // approximate

        double avg_word_len = 0.0;
        if (word_count > 0) {
            int total_letters = 0;
            for (auto &w : words) total_letters += (int)w.size();
            avg_word_len = (double)total_letters / word_count;
        }

        // frequency (lowercased), filter stopwords
        std::unordered_map<std::string, int> freq;
        for (auto &w : words) {
            std::string lw = to_lower(w);
            if (STOPWORDS.count(lw)) continue;
            ++freq[lw];
        }

        // top N
        std::vector<std::pair<std::string,int>> freqv;
        freqv.reserve(freq.size());
        for (auto &p : freq) freqv.emplace_back(p.first, p.second);
        std::sort(freqv.begin(), freqv.end(), [](const auto& a, const auto& b){
            if (a.second != b.second) return a.second > b.second;
            return a.first < b.first;
        });

        // output summary
        std::cout << "\n--- Summary ---\n";
        std::cout << "Characters (total): " << chars_total << "\n";
        std::cout << "Characters (no spaces): " << chars_no_space << "\n";
        std::cout << "Words: " << word_count << "\n";
        std::cout << "Sentences: " << sentence_count << "\n";
        std::cout << "Average word length: " << (word_count>0 ? avg_word_len : 0.0) << "\n";

        int topN = 8;
        std::cout << "Top " << topN << " words (excluding common stopwords):\n";
        for (int i = 0; i < (int)freqv.size() && i < topN; ++i) {
            std::cout << "  " << freqv[i].first << " : " << freqv[i].second << "\n";
        }
        if (freqv.empty()) std::cout << "  (no words or only stopwords found)\n";

        // clear text buffer and ask user whether to continue or quit
        text.clear();
        std::cout << "\n继续输入新文本或输入 'quit' 返回主菜单。\n";
    }
}

