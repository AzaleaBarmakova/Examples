#include <iostream>
#include <cstdlib>
#include <vector>

using namespace std;

struct Treap {
    int y;   
    int size;  
    int val; 
	Treap *l, *r; 
};
Treap *new_Treap(int val) {
    Treap *result = new Treap;
    result->y = rand();
    result->size = 1;     
    result->val = val;
    result->l = result->r = nullptr; 
    return result;
}
int get_size(Treap *t) {
    if (t == nullptr) { 
		return 0; 
	} 
    return t->size; 
}
void update_size(Treap *t) {
    if (t == nullptr) { 
		return; 
	}
    t->size = 1 + get_size(t->l) + get_size(t->r);
}
Treap *Merge(Treap *t1, Treap *t2) {
    if (t1 == nullptr) { 
		return t2; 
	}
    if (t2 == nullptr) {
		return t1; 
	}
    if (t1->y > t2->y) {
        t1->r = Merge(t1->r, t2);
        update_size(t1);
        return t1;
    }
    else {
        t2->l = Merge(t1, t2->l);
        update_size(t2);
        return t2;
    }
}
void Split(Treap *t, int x, Treap *&t1, Treap *&t2) {
    if (t == nullptr) {
        t1 = t2 = nullptr;
        return;
    }
    if (get_size(t->l) < x) {
        Split(t->r, x - get_size(t->l) - 1, t->r, t2);
        t1 = t;
    }
    else {
        Split(t->l, x, t1, t->l);
        t2 = t;
    }
    update_size(t);
}
Treap *insert(Treap *t, int pos, int val) {
    Treap *t1, *t2;
    Split(t, pos, t1, t2);
    Treap* new_tree = new_Treap(val);
    return Merge(Merge(t1, new_tree), t2);
}
Treap* erase(Treap *t, int pos) {
    Treap *t1, *t2, *t3, *t4;
    Split(t, pos, t1, t2);
    Split(t2, pos + 1, t3, t4);
    t = Merge(t1, t4);
    delete t3;
    return t;
}
int get_value(Treap *t, int pos) {
    int index = get_size(t->l);
    if (pos < index)
		return get_value(t->l, pos);
    else 
		if (pos == index)
			return t->val;
		else
			return get_value(t->r, pos - index - 1);
}
Treap *to_front(Treap *t, int l, int r){
    Treap *t1, *t2, *t3, *t4;
    Split(t, r + 1, t1, t2);
    Split(t1, l, t3, t4);
    return Merge(Merge(t4, t3), t2);
}
void print_tree(Treap *t) {
    if (t == nullptr) { 
		return;
	}
    print_tree(t->l);
    cout << t->val << " ";
    print_tree(t->r);
}
Treap *build_Treap(vector<int>& v) {
    Treap *result = nullptr;
    for (int i = 0; i < v.size(); ++i)
        result = Merge(result, new_Treap(v[i]));
    return result;
}

int main() {
    int n, m, l, r;
	cin >> n >> m;
	vector<int> v(n);
	Treap *t = NULL;
	for (int i = 0; i < n; ++i)
		v[i] = i + 1;
	t = build_Treap(v);
	for (int i = 0; i < m; ++i) {
		cin >> l >> r;
		to_front(t, l - 1, r - 1);
	}
	print_tree(t);
    return 0;
}