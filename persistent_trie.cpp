#include <iostream>
#include <vector>
using namespace std;


struct PersistentTrie {
    
    struct TrieNode {
        int count;
        TrieNode* child[2]; // zero (0) | one (1)
        TrieNode(int c = 0): count(c), child{nullptr, nullptr} {}
    };

    vector<TrieNode*> vroot;

    PersistentTrie() {
        TrieNode* root = new TrieNode();
        vroot.push_back(root);
    }

    bool match(int number, int nbit) { return ((1 << nbit) & number);  }

    void insert(TrieNode* root, TrieNode* last, int number) {
        for (int i = 30; i >= 0; i--) {
            root->count = last ? last->count + 1 : 1;
            int myself = match(number, i);
            int sibling = myself ^ 1;
            
            root->child[myself] = new TrieNode();
            root->child[sibling] = last ? last->child[sibling] : nullptr;
            
            last = last ? last->child[myself] : nullptr;
            root = root->child[myself];
        }
        root->count = last ? last->count + 1 : 1; 
    }

    void add(int number) {
        TrieNode* root = new TrieNode();
        TrieNode* last = vroot[(int)vroot.size() - 1];
        insert(root, last, number);
        vroot.push_back(root);
    }

    void remove(TrieNode* root, TrieNode* last, int number) {
        for (int i = 30; i >= 0; i--) {
            root->count = last->count - 1;
            int myself = match(number, i);
            int sibling = myself ^ 1;

            root->child[sibling] = last->child[sibling];

            if (last->child[myself]->count > 1) {
                root->child[myself] = new TrieNode();
                root = root->child[myself];
                last = last->child[myself];
                continue;
            }

            root->child[myself] = nullptr;
            return;
        }
        root->count = last->count - 1;
    }

    void rem(int number) {
        TrieNode* root = new TrieNode();
        TrieNode* last = vroot[(int)vroot.size() - 1];
        remove(root, last, number);
        vroot.push_back(root);
    }


    // dummy functions for testing... (maybe not so dummy)

    bool dummy_exists(int version = -1, int number = 0) {
        TrieNode* root = vroot[version];
        for (int i = 30; i >= 0; i--) {
            if (root == nullptr) return false;
            int myself = match(number, i);
            root = root->child[myself];
        }
        return root != nullptr;
    }

};


void dummy_test() {
    cout << "(...) compiled check\n";

    PersistentTrie Kur;

    Kur.add(3);
    Kur.add(1);
    Kur.add(4);
    Kur.add(6);
    cout << "(...) add check\n";

    cout << "Exists 3 in version 1?    : " << (Kur.dummy_exists(1, 3) ? "Yes" : "No ") << " | Ex: Yes\n";
    cout << "Exists 1 in version 1?    : " << (Kur.dummy_exists(1, 1) ? "Yes" : "No ") << " | Ex: No\n";
    cout << "Exists 4 in version 3?    : " << (Kur.dummy_exists(3, 4) ? "Yes" : "No ") << " | Ex: Yes\n";
    cout << "Exists 6 in last version? : " << (Kur.dummy_exists(4, 6) ? "Yes" : "No ") << " | Ex: Yes\n";
    cout << "Exists 2 in last version? : " << (Kur.dummy_exists(4, 2) ? "Yes" : "No ") << " | Ex: No\n";
    cout << "Exists 4 in last version? : " << (Kur.dummy_exists(4, 4) ? "Yes" : "No ") << " | Ex: Yes\n";

    Kur.rem(6);
    cout << "(...) rem from last version check\n";

    cout << "Exists 6 in last version? : " << (Kur.dummy_exists(5, 6) ? "Yes" : "No ") << " | Ex: No\n";
    cout << "Exists 4 in last version? : " << (Kur.dummy_exists(5, 4) ? "Yes" : "No ") << " | Ex: Yes\n";
}


int main () {

    dummy_test();

    return 0;
}
