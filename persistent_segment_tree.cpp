#include <iostream>
#include <vector>
#include <algorithm>
#include <map>
using namespace std;

struct PersistentSegmentTree {

    struct Node {
        int data;
        Node* left;
        Node* right;
        Node(int d = 0, Node* l = nullptr, Node* r = nullptr):
            data(d), left(l), right(r) {}
    };

    int NMAX = 0;
    vector<Node*> vroots;

    void build(Node* root, int l, int r) {
        if (l + 1 == r) return;
        int mid = (r + l) / 2;
        root->left = new Node();
        root->right = new Node();
        build(root->left, l, mid);
        build(root->right, mid, r);
    }

    PersistentSegmentTree(int N = 0): NMAX(N) {
        Node* root = new Node();
        build(root, 0, NMAX);
        vroots.push_back(root);
    }

    int query(Node* root, int l, int r, int ql, int qr) {
        if (r <= ql or qr <= l) return 0;
        if (ql <= l and r <= qr) return root->data;
        int mid = (r + l) / 2;
        int a = query(root->left, l, mid, ql, qr);
        int b = query(root->right, mid, r, ql, qr);
        return a + b;
    }

    int search_minimal_r(int lset, int k) { // l-=1
        int l = lset;
        int r = vroots.size() - 1;
        Node* root = vroots[r];
        int q = query(root, 0, NMAX, l, r);
        if (q < k) return 0;
        while (l + 1 < r) {
            int mid = (r + l) / 2;
            root = vroots[mid];
            q = query(root, 0, NMAX, lset, mid);
            // cout << "  [" << l+1 << ", " << r << "] -> " << q << "\n";
            if (q >= k) r = mid;
            else l = mid;
        } return r;
    }

    void modify(Node* root, Node* last, int l, int r, int pos, int val) {
        if (l + 1 == r) {
            root->data = val;
            return;
        }
        int mid = (r + l) / 2;
        if (pos < mid) {
            root->left = new Node();
            root->right = last->right;
            modify(root->left, last->left, l, mid, pos, val);
        } else {
            root->left = last->left;
            root->right = new Node();
            modify(root->right, last->right, mid, r, pos, val);
        }
        root->data = root->left->data + root->right->data;
    }

    void change_position(int before, int after) {
        Node* temp = new Node();
        Node* last = vroots[(int)vroots.size() - 1];
        modify(temp, last, 0, NMAX, before, 0);
        Node* root = new Node();
        modify(root, temp, 0, NMAX, after, 1);
        vroots.push_back(root);
    }

};

int position[1 << 20] = {0};


int main () {
    // ifstream cin("input.txt"); ofstream cout("output.txt");
    ios_base::sync_with_stdio(false); cin.tie(nullptr);

    int n, m; cin >> n >> m;
    for (int i = 0; i < (1<<20); ++i) position[i] = -1;

    PersistentSegmentTree pst(n);

    for(int i = 0; i < n; i++) {
        int a; cin >> a;
        if (position[a] == -1) pst.change_position(i, i);
        else pst.change_position(position[a], i);
        position[a] = i;
    }

    int p = 0;
    int q; cin >> q;
    while (q--) {
        int x, y; cin >> x >> y;
        int l = (x + p) % n + 1;
        int k = (y + p) % m + 1;
        // cout << " =======\n";
        p = pst.search_minimal_r(l - 1, k);
        cout << p << "\n";
    }

    return 0;
}
