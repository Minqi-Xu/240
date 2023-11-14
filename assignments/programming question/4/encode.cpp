#include <iostream>
#include <vector>
#include<string>
using namespace std;

// your code goes here

// trie structure
struct trie_node {
    int code;
    trie_node* children[26] = {NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,     // idx 0 --> a, idx 1 --> b, idx 2 --> c, etc.
                               NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL, NULL,
                               NULL, NULL, NULL, NULL, NULL, NULL};
};

void initialize_trie (trie_node* n) {       // initialize the dictionary
    n->code = -1;
    for(int i = 0; i < 26; i++) {
        n->children[i] = new trie_node;
        n->children[i]->code = i;
        for(int j = 0; j < 26; j++) {
            n->children[i]->children[j] = new trie_node;
            n->children[i]->children[j]->code = (i + 1) * 26 + j;
        }
    }
}

void output(int n, string& coded_s) {
    int bits[12];
    for(int i = 0; i < 12; i++)
        bits[i] = 0;
    int counts = 0;
    while(n > 0) {
        bits[counts] = n % 2;
        counts++;
        n = n / 2;
    }
    for(int i = 11; i >= 0; i--) {
        if(bits[i] == 0)
            coded_s += "0";
        else
            coded_s += "1";
    }
}

void LZW_encode_modified(const string& s, string& coded_s)
{
    trie_node* t = new trie_node;
    initialize_trie(t);
    int idx = 702;
    int i = 0;
    while(i < s.size()) {
        trie_node* v = t;
        int c = s[i] - 97;
        while(i < s.size() && v->children[c] != NULL) {
            v = v->children[c];
            i++;
            if(i < s.size())
                c = s[i] - 97;
        }
        output(v->code, coded_s);
        if(i < s.size()) {
            v->children[c] = new trie_node;
            v->children[c]->code = idx;
            idx++;
            if(i + 1 < s.size()) {
                int c2 = s[i + 1] - 97;
                v->children[c]->children[c2] = new trie_node;
                v->children[c]->children[c2]->code = idx;
                idx++;
            }
        }
    }
}


int main()
{
    string s;
    string coded_s = "";
    getline(cin, s);

    LZW_encode_modified(s, coded_s);

    cout << coded_s;
}
