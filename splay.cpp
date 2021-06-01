#include <iostream>

using namespace std;

// структура данных, которая представляет узел в дереве
struct Node {
	int data; // содержит ключ
	Node *parent; // указатель на родителя
	Node *left; // указатель на левый дочерний элемент
	Node *right; // указатель на правый дочерний элемент
};

typedef Node *NodePtr;

// класс SplayTree реализует операции в дереве Splay
class SplayTree {
private:
	NodePtr root;

	void preOrderHelper(NodePtr node) {
		if (node != nullptr) {
			cout<<node->data<<" ";
			preOrderHelper(node->left);
			preOrderHelper(node->right);
		}
	}

	void inOrderHelper(NodePtr node) {
		if (node != nullptr) {
			inOrderHelper(node->left);
			cout<<node->data<<" ";
			inOrderHelper(node->right);
		}
	}

	void postOrderHelper(NodePtr node) {
		if (node != nullptr) {
			postOrderHelper(node->left);
			postOrderHelper(node->right);
			cout<<node->data<<" ";
		}
	}

	NodePtr searchTreeHelper(NodePtr node, int key) {
		if (node == nullptr || key == node->data) {
			return node;
		}

		if (key < node->data) {
			return searchTreeHelper(node->left, key);
		}
		return searchTreeHelper(node->right, key);
	}

	void deleteNodeHelper(NodePtr node, int key) {
		NodePtr x = nullptr;
		NodePtr t, s;
		while (node != nullptr){
			if (node->data == key) {
				x = node;
			}

			if (node->data <= key) {
				node = node->right;
			} else {
				node = node->left;
			}
		}

		if (x == nullptr) {
			cout<<"Couldn't find key in the tree"<<endl;
			return;
		}
		split(x, s, t); // разделить дерево
		if (s->left){ // удалить x
			s->left->parent = nullptr;
		}
		root = join(s->left, t);
		delete(s);
		s = nullptr;
	}

	void printHelper(NodePtr root, string indent, bool last) {
		//  распечатать древовидную структуру на экране
	   	if (root != nullptr) {
		   cout<<indent;
		   if (last) {
		      cout<<"└────";
		      indent += "     ";
		   } else {
		      cout<<"├────";
		      indent += "|    ";
		   }

		   cout<<root->data<<endl;

		   printHelper(root->left, indent, false);
		   printHelper(root->right, indent, true);
		}
	}

	// повернуть влево на узле x
	void leftRotate(NodePtr x) {
		NodePtr y = x->right;
		x->right = y->left;
		if (y->left != nullptr) {
			y->left->parent = x;
		}
		y->parent = x->parent;
		if (x->parent == nullptr) {
			this->root = y;
		} else if (x == x->parent->left) {
			x->parent->left = y;
		} else {
			x->parent->right = y;
		}
		y->left = x;
		x->parent = y;
	}

	// повернуть вправо на узле x
	void rightRotate(NodePtr x) {
		NodePtr y = x->left;
		x->left = y->right;
		if (y->right != nullptr) {
			y->right->parent = x;
		}
		y->parent = x->parent;
		if (x->parent == nullptr) {
			this->root = y;
		} else if (x == x->parent->right) {
			x->parent->right = y;
		} else {
			x->parent->left = y;
		}
		y->right = x;
		x->parent = y;
	}

	// воспроизведение
	void splay(NodePtr x) {
		while (x->parent) {
			if (!x->parent->parent) {
				if (x == x->parent->left) {
					// поворот зигзагообразных стей
					rightRotate(x->parent);
				} else {
					// Вращение загообразного загообразного
					leftRotate(x->parent);
				}
			} else if (x == x->parent->left && x->parent == x->parent->parent->left) {
				// zig-zig
				rightRotate(x->parent->parent);
				rightRotate(x->parent);
			} else if (x == x->parent->right && x->parent == x->parent->parent->right) {
				// zag-zag
				leftRotate(x->parent->parent);
				leftRotate(x->parent);
			} else if (x == x->parent->right && x->parent == x->parent->parent->left) {
				// zig-zag
				leftRotate(x->parent);
				rightRotate(x->parent);
			} else {
				// zag-zig
				rightRotate(x->parent);
				leftRotate(x->parent);
			}
		}
	}

	// соединяет два дерева s и t
	NodePtr join(NodePtr s, NodePtr t){
		if (!s) {
			return t;
		}

		if (!t) {
			return s;
		}
		NodePtr x = maximum(s);
		splay(x);
		x->right = t;
		t->parent = x;
		return x;
	}

	// разделяет дерево на s и t
	void split(NodePtr &x, NodePtr &s, NodePtr &t) {
		splay(x);
		if (x->right) {
			t = x->right;
			t->parent = nullptr;
		} else {
			t = nullptr;
		}
		s = x;
		s->right = nullptr;
		x = nullptr;
	}

public:
	SplayTree() {
		root = nullptr;
	}

	// Предзаказ обхода
	// Узел->Левое поддерево->Правое поддерево
	void preorder() {
		preOrderHelper(this->root);
	}

	// Поход в заказе
	// Левое поддерево -> Узел -> Правое поддерево
	void inorder() {
		inOrderHelper(this->root);
	}

	// Потерп. после заказа
	// Левое поддерево -> Правое поддерево -> Узел
	void postorder() {
		postOrderHelper(this->root);
	}

	// найти в дереве ключ k
	// вернуть соответствующий узел
	NodePtr searchTree(int k) {
		NodePtr x = searchTreeHelper(this->root, k);
		if (x) {
			splay(x);
		}
		return x;
	}

	// найти узел с минимальным ключом
	NodePtr minimum(NodePtr node) {
		while (node->left != nullptr) {
			node = node->left;
		}
		return node;
	}

	// найти узел с максимальным ключом
	NodePtr maximum(NodePtr node) {
		while (node->right != nullptr) {
			node = node->right;
		}
		return node;
	}

	// найти преемника данного узла
	NodePtr successor(NodePtr x) {
		// если правое поддерево не равно нулю,
		// преемник является самым левым узлом в
		// правое поддерево
		if (x->right != nullptr) {
			return minimum(x->right);
		}

		// иначе это самый низкий предок x, чей
		// левый дочерний элемент также является предком x.
		NodePtr y = x->parent;
		while (y != nullptr && x == y->right) {
			x = y;
			y = y->parent;
		}
		return y;
	}

	// найти предшественника данного узла
	NodePtr predecessor(NodePtr x) {
		// если левое поддерево не равно нулю,
		// предшественник является самым правым узлом в
		// левое поддерево
		if (x->left != nullptr) {
			return maximum(x->left);
		}

		NodePtr y = x->parent;
		while (y != nullptr && x == y->left) {
			x = y;
			y = y->parent;
		}

		return y;
	}

	// вставить ключ к дереву в соответствующее положение
	void insert(int key) {
		// обычная вставка BST
		NodePtr node = new Node;
		node->parent = nullptr;
		node->left = nullptr;
		node->right = nullptr;
		node->data = key;
		NodePtr y = nullptr;
		NodePtr x = this->root;

		while (x != nullptr) {
			y = x;
			if (node->data < x->data) {
				x = x->left;
			} else {
				x = x->right;
			}
		}

		// y является родителем x
		node->parent = y;
		if (y == nullptr) {
			root = node;
		} else if (node->data < y->data) {
			y->left = node;
		} else {
			y->right = node;
		}

		// воспроизвести узел
		splay(node);
	}

	NodePtr getRoot(){
		return this->root;
	}

	// удалить узел из дерева
	void deleteNode(int data) {
		deleteNodeHelper(this->root, data);
	}

	// распечатать древовидную структуру на экране
	void prettyPrint() {
		printHelper(this->root, "", true);
	}

};

int main() {
	SplayTree bst;
	bst.insert(33);
	bst.insert(44);
	bst.insert(67);
	bst.insert(5);
	bst.insert(89);
	bst.insert(41);
	bst.insert(98);
	bst.insert(1);
	bst.prettyPrint();
	bst.searchTree(33);
	bst.searchTree(44);
	bst.prettyPrint();
	bst.deleteNode(89);
	bst.deleteNode(67);
	bst.deleteNode(41);
	bst.deleteNode(5);
	bst.prettyPrint();
	bst.deleteNode(98);
	bst.deleteNode(1);
	bst.deleteNode(44);
	bst.deleteNode(33);
	bst.prettyPrint();
	return 0;
}
