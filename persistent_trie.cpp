#include <iostream>
#include <vector>
using namespace std;

// /* ... 

static inline unsigned long long Hashed(long long x) {
    unsigned long long z = (unsigned long long)x + 0x9E3779B97F4A7C15ULL;
    z = (z ^ (z >> 30)) * 0xBF58476D1CE4E5B9ULL;
    z = (z ^ (z >> 27)) * 0x94D049BB133111EBULL;
    z = z ^ (z >> 31);
    return z;
}

//   ... */


struct PersistentTrie {
    
    struct TrieNode {
        int count;
        unsigned long long hashmark;
        TrieNode* child[2]; // zero (0) | one (1)
        TrieNode(int c = 0): count(c), hashmark(0ULL), child{nullptr, nullptr} {}
    };

    vector<TrieNode*> vroot;

    PersistentTrie() {
        TrieNode* root = new TrieNode();
        vroot.push_back(root);
    }

    bool match(int number, int nbit) { return ((1 << nbit) & number);  }

    void insert(TrieNode* root, TrieNode* last, int number) {
        unsigned long long mymark = Hashed(number);
        for (int i = 30; i >= 0; i--) {
            root->count = last ? last->count + 1 : 1;
            root->hashmark = last ? last->hashmark ^ mymark : mymark;

            int myself = match(number, i);
            int sibling = myself ^ 1;
            
            root->child[myself] = new TrieNode();
            root->child[sibling] = last ? last->child[sibling] : nullptr;
            
            last = last ? last->child[myself] : nullptr;
            root = root->child[myself];
        }
        root->count = last ? last->count + 1 : 1;
        root->hashmark = last ? last->hashmark ^ mymark : mymark;
    }

    void remove(TrieNode* root, TrieNode* last, int number) {
        unsigned long long mymark = Hashed(number);
        for (int i = 30; i >= 0; i--) {
            root->count = last->count - 1;
            root->hashmark = last->hashmark ^ mymark;

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
        root->hashmark = last->hashmark ^ mymark;
    }



    int special_query(TrieNode* A, TrieNode* B) {
        unsigned long long markA = A ? A->hashmark : 0ULL;
        unsigned long long markB = B ? B->hashmark : 0ULL;

        if (markA == markB) return 0;

        int number = 0;
        for (int i = 30; i >= 0; i--) {
            TrieNode* A0 = A ? A->child[0] : nullptr;
            TrieNode* B0 = B ? B->child[0] : nullptr;
            unsigned long long markA0 = A0 ? A0->hashmark : 0ULL;
            unsigned long long markB0 = B0 ? B0->hashmark : 0ULL;
            
            if (markA0 != markB0) {
                A = A0;
                B = B0;
            } else {
                A = A ? A->child[1] : nullptr;
                B = B ? B->child[1] : nullptr;
                number |= (1 << i);
            }
        }

        return number;
    }


    int Que (int l, int r) {
        TrieNode* A = vroot[r];
        TrieNode* B = vroot[l - 1];
        return special_query(A, B);
    }


    bool dummy_exists(int version = -1, int number = 0) {
        TrieNode* root = vroot[version];
        for (int i = 30; i >= 0; i--) {
            if (root == nullptr) return false;
            int myself = match(number, i);
            root = root->child[myself];
        }
        return root != nullptr;
    }

    void Upd(int number) {
        int version = vroot.size() - 1;
        TrieNode* root = new TrieNode();
        if (dummy_exists(version, number))
            remove(root, vroot[version], number);
        else
            insert(root, vroot[version], number);
        vroot.push_back(root);
    }


    /* *************************************************** */
    /* dummy functions for testing... (maybe not so dummy) */
    /* *************************************************** */


    void add(int number) {
        TrieNode* root = new TrieNode();
        TrieNode* last = vroot[(int)vroot.size() - 1];
        insert(root, last, number);
        vroot.push_back(root);
    }

    void rem(int number) {
        TrieNode* root = new TrieNode();
        TrieNode* last = vroot[(int)vroot.size() - 1];
        remove(root, last, number);
        vroot.push_back(root);
    }

};


/* void dummy_test() {
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
} */


int main () {
    // ifstream cin("input.txt"); ofstream cout("output.txt");
    ios_base::sync_with_stdio(false); cin.tie(nullptr);

    /* dummy_test(); */

    int n; cin >> n;

    PersistentTrie Kur;

    for (int i = 0; i < n; i++) {
        int a; cin >> a;
        Kur.Upd(a);
    }

    int ans = 0;

    int q; cin >> q;
    while (q--) {
        int l, r; cin >> l >> r;
        l ^= ans;
        r ^= ans;
        ans = Kur.Que(l, r);
        cout << ans << "\n";
    }

    return 0;
}
