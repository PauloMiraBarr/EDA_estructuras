#include <iostream>
#include <vector>

using namespace std;

template <typename data_type>
struct PersistentStack {
    
    struct StackNode {
        data_type data;
        StackNode* next;
        StackNode(data_type data, StackNode* next): 
            data(data), next(next) {}
    };

    vector<StackNode*> version_roots;
    PersistentStack() {
        version_roots.push_back(nullptr);
    }

    void update_push(int version, data_type data) {
        version_roots.emplace_back(new StackNode(data, version_roots[version]));
    }

    void update_pop(int version) {
        version_roots.emplace_back(version_roots[version] -> next);
    }

    data_type top(int version) {
        if (!version) return 0;
        return version_roots[version] -> data;
    }

    void print(int version) {
        StackNode* top = version_roots[version];
        while (top != nullptr) {
            cout << top -> data << " ";
            top = top -> next;
        }
        cout << "-- END OF STACK --\n";
    }
};

int main () {
    // ifstream cin("input.txt"); ofstream cout("output.txt");
    ios_base::sync_with_stdio(false); cin.tie(nullptr);

    PersistentStack<long long> ps;
    ps.update_push(0, 0);

    int q; cin >> q;
    while (q--) {
        int t, m; cin >> t >> m;
        t += 1;
        if (m) {
            ps.update_push(t, 1LL * m + ps.top(t));
        } else {
            ps.update_pop(t);
        }
    }

    long long ans = 0;
    for (PersistentStack<long long>::StackNode *u: ps.version_roots) {
        if (u) {
            ans += u -> data;
            // cout << u -> data << " ";
        }
    }

    cout << ans << "\n";

    return 0;
}
