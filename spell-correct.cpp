#include <iostream>
#include <fstream>
#include <string>
#include <regex>
#include <locale>         // std::locale, std::tolower
#include <algorithm>
#include <unordered_map>
#include <unordered_set>

using namespace std;

const std::string kAlphabet = "abcdefghijklmnopqrstuvwxyz";

void readWordsFromFile(const char* filename, unordered_map<string, int>& word_cout_map)
{
	ifstream file(filename);
	string word;
	regex rgx("[a-zA-Z]+");
	//map<string, int> word_cout_map;
	if(file.is_open())
	{
		/*
		file.seekg(0, std::ios::end);
		word.reserve(file.tellg());
		file.seekg(0, std::ios::beg);

		word.assign((std::istreambuf_iterator<char>(file)),
            std::istreambuf_iterator<char>());
		*/
		while (file >> word)
		{
			smatch match;
			//cout << "[" << word << "]" << endl;

			while (regex_search(word, match, rgx))
			{
				string tmpstr = match[0];
				std::transform(tmpstr.begin(), tmpstr.end(), tmpstr.begin(), ::tolower);
				//cout << tmpstr << endl;
				word_cout_map[tmpstr] += 1;
				word = word.substr(match.prefix().length() + match[0].length());
			}

		}
	}

}

void edits1(const string& error, unordered_set<string>& wordSet)
{
	wordSet.clear();
	std::string word;
	int len = error.size();
    int len_alphabet = kAlphabet.length();
	//deletes
	for (int i = 0; i < len; ++i)
    {
        word = error;
        word.erase(i, 1);
        wordSet.insert(word);
    }
	//transposes
	for (int i = 0; i < len - 1; ++i)
    {
        word = error;
        std::swap(word[i], word[i + 1]);
        wordSet.insert(word);
    }
	//replaces
	for (int i = 0; i < len; ++i)
    {
        for (int j = 0; j < len_alphabet; ++j)
        {
            word = error;
            word[i] = kAlphabet[j];
            wordSet.insert(word);
        }
    }
	//inserts
	for (int i = 0; i < len + 1; ++i)
    {
        for (int j = 0; j < len_alphabet; ++j)
        {
            word = error;
            word.insert(i, 1, kAlphabet[j]);
            wordSet.insert(word);
        }
    }
}

void edits2(const std::string &error, std::unordered_set<std::string> &words2)
{
    words2.clear();
    std::unordered_set<std::string> words1;
    edits1(error, words1);
    std::unordered_set<std::string> tmp_words2;
    for (const auto &word1 : words1)
    {
        edits1(word1, tmp_words2);
        for (const auto &word : tmp_words2)
        {
            words2.insert(word);
        }
    }
}

string MostFrequent(const unordered_map<string, int>& dic, std::unordered_set<std::string> &candidates)
{
    std::string answer;
    size_t max_count = 0;
    for (const auto &word : candidates)
    {
        if (dic.find(word) != dic.end() && dic.at(word) > max_count)
        {
            max_count = dic.at(word);
            answer = word;
        }
    }
    return answer;
}

string correct(const unordered_map<string, int>& dic, const string& error)
{
	if (dic.find(error) != dic.end())
        return error;
	std::unordered_set<std::string> candidates;
	edits1(error, candidates);
	string answer = MostFrequent(dic, candidates);
	if (!answer.empty())
        return answer;
	edits2(error, candidates);
    answer = MostFrequent(dic, candidates);
    if (!answer.empty())
        return answer;
    return error;

}

int main(int arg_num, char* args[])
{
	if (arg_num != 2)
	{
		cout << "USAGE: ./spell-correct [WORD]" << endl;
		return -1;
	}
	unordered_map<string, int> NWORDS;
	readWordsFromFile("big.txt", NWORDS);
	cout << "NWORDS size=" << NWORDS.size() << endl;
	string error = args[1];
	cout << error << " ==> " << correct(NWORDS, error) << endl;
	return 0;
}
