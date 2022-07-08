
#include<iostream> 
#include<stack>
#include<string>
#include<random>
#include<chrono>
//#include"bst.h"


using namespace std;
mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());

//Threaded AVL Tree Class
template <typename T>
class avl {
	struct Node
	{
	public:
		T key;
		Node* left;
		Node* right;
		Node* next;
		int h;

	}* root, *first;
	int n;

public:
	avl() {
		root=first = NULL;
		n = 0;
	}

	class iterator {
		Node* itr;

	public:

		iterator() {
			itr = NULL;
		}

		iterator(const Node*& node) {
			itr = node;
		}

		//return the next smallest number 
		Node* operator ++() {
			if (itr->next != NULL) {
				itr = itr->next;
			}
			return itr;
		}

		void operator = (avl<T>::Node* node) {
			itr = node;
		}

		bool operator != (avl<T>::Node* obj) 
		{
			if (itr == obj) {
				return false;
			}
			return true;
		}

		friend ostream& operator<<(ostream& os, avl<T>::iterator obj) {
			if (obj.itr != NULL) {
				os << obj.itr->key;
				obj.itr = obj.itr->next;
			}
			return os;
		}		
	};

	int height(Node* N)
	{
		if (N == NULL)
			return 0;
		return N->h;
	}

	int max(int a, int b)
	{
		return (a > b) ? a : b;
	}

	Node* begin() {
		return first;
	}

	Node* end() {
		Node* curr = root;
		while (curr->right != NULL) {
			curr = curr->right;
		}
		return curr;
	}

	Node* getMin() {
		return first;
	}

	void deleteMin() {
		if(first!=NULL)
			deleteNode(first->key);
	}

	T getMinKey() {
		return first->key;
	}

	int getKey(Node* node) {
		return node->key;
	}
	bool isLeaf(Node* node) {
		if (node->left != NULL || node->right != NULL) {
			return false;
		}	
		return true;
	}

	Node* newNode(Node* parent,const T& key)
	{
		Node* node = new Node();
		node->key = key;
		node->left = NULL;
		node->right = NULL;
		node->next = NULL;
		node->h = 1;

		return(node);
	}

	Node* rightRotation(Node* y)
	{
		Node* x = y->left;
		Node* T2 = x->right;

		x->right = y; 
		y->left = T2;

		y->h = max(height(y->left), height(y->right)) + 1;

		x->h = max(height(x->left), height(x->right)) + 1;
		
		//updates the next
		x->next = y->left;
		y->left->next = y;		

		return x;
	}

	Node* leftRotation(Node* y)
	{
		Node* x = y->right;
		Node* T2 = x->left;


		x->left = y;
		y->next = x; //update next
		y->right = T2;


		y->h = max(height(y->left), height(y->right)) + 1;

		x->h = max(height(x->left), height(x->right)) + 1;

		x->left->next =x;	//updates the next

		// Return new root 
		return x;
	}

	// Get Balance factor of node N 
	int bf(Node* N)
	{
		if (N == NULL)
			return 0;
		return height(N->left) - height(N->right);
	}
	
	//wrapper insert
	void insert(int key) {
		stack<Node*> ancestors;
		Node* pnode=NULL;
		ancestors.push(pnode);
		insert(ancestors, pnode, root, key);
	}

	Node* insert(stack<Node*>& ancestors, Node* pnode, Node*& node, const T& key)
	{
		//Normal Insertation
		if (node == NULL) {
			node = newNode(pnode, key);
			//ancestors.push(node);
			return node;
		}

		if (key < node->key)
		{
			ancestors.push(node);
			pnode = node;
			node->left = insert(ancestors, pnode, node->left, key);
			if(ancestors.top()!=NULL && isLeaf(node->left))
				node->left->next = ancestors.top();		//updates the next according to in-order traversal
			ancestors.pop();
		}
		else if (key > node->key)
		{
			ancestors.push(node);
			pnode = node;
			node->right = insert(ancestors, pnode, node->right, key);
			node->next = node->right;	//the middle node's next is the right node
			ancestors.pop();
			if(ancestors.top()!=NULL && ancestors.top()->key>node->right->key && isLeaf(node->right))
				node->right->next = ancestors.top(); //give the grandparent of the 
			
			
		}
		else {
			return node;
		}
		
		node->h = 1 + max(height(node->left), height(node->right));

		int balance = bf(node);

		if (balance > 1 && key < node->left->key) //left left
			return rightRotation(node);

		if (balance < -1 && key > node->right->key) //right right
			return leftRotation(node);

		if (balance > 1 && key > node->left->key)	//left-right case
		{
			node->left = leftRotation(node->left);
			return rightRotation(node);
		}

		if (balance < -1 && key < node->right->key)		//right left
		{
			node->right = rightRotation(node->right);
			return leftRotation(node);
		}

		first = minValueNode(root);	//update first
		return node;
	}

	Node* minValueNode(Node* node)
	{
		Node* current = node;

		while (current->left != NULL)
			current = current->left;

		return current;
	}

	//wrapper delete
	void deleteNode(const T& key) {
		root = deleteNode(root, key);
	}

	Node* deleteNode(Node* root, const T& key)
	{

		if (root == NULL)
			return root;

		if (key < root->key)
			root->left = deleteNode(root->left, key);

		else if (key > root->key)
			root->right = deleteNode(root->right, key);

		else
		{
			if ((root->left == NULL) || (root->right == NULL))
			{
				Node* temp = root->left ?
					root->left :
					root->right;

				if (temp == NULL)
				{
					temp = root;
					root = NULL;
				}
				else
					*root = *temp;

				free(temp);
			}
			else
			{

				Node* temp = minValueNode(root->right);
				root->key = temp->key;
				//root->next=temp->next;

				root->right = deleteNode(root->right,temp->key);
			}
		}


		if (root == NULL)
			return root;

		root->h = 1 + max(height(root->left), height(root->right));

		int balance = bf(root);
		
		if (balance > 1 && bf(root->left) >= 0)
			return rightRotation(root);

		if (balance > 1 && bf(root->left) < 0)
		{
			root->left = leftRotation(root->left);
			return rightRotation(root);
		}

		if (balance < -1 && bf(root->right) <= 0)
			return leftRotation(root);

		if (balance < -1 && bf(root->right) > 0)
		{
			root->right = rightRotation(root->right);
			return leftRotation(root);
		}
		first = minValueNode(root);	//update first
		return root;
	}


	void InOrder(Node* root)
	{
		if (root != NULL)
		{
			InOrder(root->left);
			cout << root->key << " ";
			InOrder(root->right);
		}
	}

	void printInorder() {
		InOrder(root);
	}

	void printTree(Node* root, int sCount)
	{
		if (root == NULL) {
			return;
		}

		sCount = sCount+8;
		printTree(root->right, sCount);  
		cout << endl;
		for (int i = 10; i < sCount; i++) {
			cout << " ";
		}
		cout << root->key << endl;

		printTree(root->left, sCount);
	}

	//Q5
	//print nodes in tree form
	void printTree()
	{
		printTree(root, 0);
	}

	//update key
	void updateKey(T p1, T p2) {
		deleteNode(p1);
		insert(p2);
	}
};


//PRIORITY QUEUE Using AVL Tree Class
template <typename T>
 class priority_queue {

	avl<T> q;
	struct Node
	{
	public:
		T key;
		Node* left;
		Node* right;
		Node* next;
		int h;
	};

public:
	priority_queue() {
		
	}
	void enqueue(const T& el) {
		q.insert(el);
	}
	void dequeue() {
		q.deleteMin();
	}
	const T& peek() {
		return q.getMinKey() ;
	}
	//changes the priority of p1 to p2
	void updatePriority(T p1, T p2) {
		q.updateKey(p1, p2);
	}
	void print() {
		q.printInorder();
		q.printTree();
	}
};



 //GLOBAL FUNCTION TO SORT ARRAY OF TYPE T USING PRIORITY QUEUE
 template <typename T>
 void sort(T array, int n) {	//works in nlog n
	 priority_queue<T> que;

	 for (int i = 0; i < n; i++) {
		 que.enqueue(array[i]);
	 }
	 for (int i = 0; i < n; i++) {
		 array[i] = que.peek();
		 que.dequeue();
	 }
 }


 //AVL TREE CLASS
 template <typename T>
 class AVL {

	 struct treeNode {
		 T data;
		 treeNode* left, * right;
		 int h; //height of the node

		 treeNode(const T& new_data, treeNode* lc = NULL, treeNode* rc = NULL, int nh = 0) {
			 data = new_data;
			 left = lc;
			 right = rc;
			 h = nh;
		 }
	 } *root;
	 int n;//size of the tree

	 int height(treeNode* ptr) {
		 if (ptr == NULL)
			 return -1;
		 else
			 return ptr->h;
	 }


	 int bf(treeNode* ptr) {
		 return height(ptr->right) - height(ptr->left);
	 }

	 int max(int a, int b)
	 {
		 return (a > b) ? a : b;
	 }
	 bool updateHeight(treeNode* ptr) {
		 int old_h = height(ptr);
		 int new_h = max(height(ptr->left), height(ptr->right)) + 1;
		 ptr->h = new_h;
		 return (old_h != new_h);

	 }


	 bool getAccess(const T& key, treeNode*& ptr, stack<treeNode*>& ancestors)
	 {
		 ptr = root;

		 while (ptr != NULL && ptr->data != key) {
			 ancestors.push(ptr);
			 if (key < ptr->data)
				 ptr = ptr->left;
			 else
				 ptr = ptr->right;
		 }

		 return (ptr != NULL);
	 }

	 int childCount(treeNode* ptr) {
		 if (ptr == NULL || (ptr->left == NULL && ptr->right == NULL))
			 return 0;
		 else if (ptr->left != NULL && ptr->right != NULL)
			 return 2;
		 else return 1;

	 }

	 void eraseCase0(treeNode* ptr, treeNode* pptr) {

		 if (ptr == root) {
			 //cout<<"Root updated..."<<endl;
			 root = NULL;
		 }
		 else if (ptr == pptr->left) {
			 pptr->left = NULL;
		 }
		 else {
			 pptr->right = NULL;
		 }

		 delete ptr;
	 }

	 void eraseCase1(treeNode* ptr, treeNode* pptr) {
		 treeNode* cptr;
		 if (ptr->right != NULL)
			 cptr = ptr->right;
		 else
			 cptr = ptr->left;
		 //ptr: node, pptr: parent of ptr, cptr: child of ptr
		 if (ptr == root) {
			 //cout<<"Root updated..."<<endl;
			 root = cptr;
			 delete ptr;
		 }
		 else {
			 if (ptr == pptr->left)//ptr is left child of pptr
				 pptr->left = cptr;
			 else//ptr is right child of pptr
				 pptr->right = cptr;


			 delete ptr;
		 }
	 }

	 void eraseCase2(treeNode* ptr, treeNode*& pptr, stack<treeNode*>& ancestors) {

		 treeNode* nptr = ptr->right, * pnptr = ptr;
		 ancestors.push(ptr);
		 while (nptr->left != NULL) {
			 pnptr = nptr;
			 ancestors.push(nptr);
			 nptr = nptr->left;
		 }

		 swap(ptr->data, nptr->data);

		 if (childCount(nptr) == 0) {
			 eraseCase0(nptr, pnptr);
		 }
		 else {
			 eraseCase1(nptr, pnptr);
		 }
	 }


	 bool rr(treeNode* ptr, treeNode* pptr) {
		 //right rotate around ptr
		 treeNode* cptr = ptr->left;

		 if (cptr == NULL)
			 return false;
		 else {
			 //what is the node itself is the root
			 //promote cptr
			 if (pptr == NULL) {
				 root = cptr;
			 }
			 else {
				 if (ptr == pptr->left)
					 pptr->left = cptr;
				 else
					 pptr->right = cptr;
			 }


			 //adjust children
			 treeNode* rc_cptr = cptr->right;
			 cptr->right = ptr;
			 ptr->left = rc_cptr;

			 //update heights of ptr and cptr
			 updateHeight(ptr);
			 updateHeight(cptr);

			 return true;
		 }
	 }


	 bool rl(treeNode* ptr, treeNode* pptr) {
		 //left rotate around ptr

		 treeNode* cptr = ptr->right;

		 if (cptr == NULL)
			 return false;
		 else {
			 if (pptr == NULL) {
				 root = cptr;
			 }
			 else {
				 if (ptr == pptr->left)
					 pptr->left = cptr;
				 else
					 pptr->right = cptr;
			 }


			 treeNode* lc_cptr = cptr->left;
			 cptr->left = ptr;
			 ptr->right = lc_cptr;

			 //update heights of ptr and cptr
			 updateHeight(ptr);
			 updateHeight(cptr);
			 return true;
		 }
	 }



 public:
	 AVL() {
		 root = NULL;
		 n = 0;
	 }

	 int height() {
		 return height(root);
	 }

	 bool insert(const T& key) {

		 treeNode* ptr = NULL;
		 stack<treeNode*> ancestors;
		 if (getAccess(key, ptr, ancestors))
			 return false;
		 else {
			 //being here means ptr is NULL
			 treeNode* pptr = ancestors.size() == 0 ? NULL : ancestors.top();

			 if (pptr == NULL) {//empty tree
				 root = new treeNode(key, NULL, NULL, 0);
			 }
			 else if (key < pptr->data) {//new node is a left child
				 pptr->left = new treeNode(key, NULL, NULL, 0);
			 }
			 else {
				 pptr->right = new treeNode(key, NULL, NULL, 0);
			 }

			 //now update heights and check balance factors of the ancestors
			 while (!ancestors.empty()) {
				 treeNode* anc = ancestors.top();
				 ancestors.pop();

				 //update the height of the next ancestor
				 updateHeight(anc);

				 //is there imbalance on this ancestor?
				 if (bf(anc) > 1 || bf(anc) < -1) {
					 //Yes there is imbalance.
					 //Identify the type of imbalance and resolve it

					 //get parent pointer, required by rotation methods
					 treeNode* panc = ancestors.empty() ? NULL : ancestors.top();

					 if (bf(anc) == 2 && bf(anc->right) == 1) {
						 //right-right
						 rl(anc, panc);
					 }
					 else
						 if (bf(anc) == -2 && bf(anc->left) == -1) {
							 //left-left
							 rr(anc, panc);
						 }
						 else
							 if (bf(anc) == 2 && bf(anc->right) == -1) {
								 //right-left
								 rr(anc->right, anc);//make it a right-right imbalance
								 rl(anc, panc);
							 }
							 else
								 if (bf(anc) == -2 && bf(anc->left) == 1) {
									 //left-right
									 rl(anc->left, anc);//make it a left-left imbalance
									 rr(anc, panc);
								 }
				 }
			 }

			 n++;
			 return true;
		 }
	 }

	 bool find(const T& key) {
		 return getAccess(key);
	 }

	 bool erase(const T& key) {

		 treeNode* ptr;
		 stack<treeNode*> ancestors;

		 if (!getAccess(key, ptr, ancestors))
			 return false;
		 else {
			 treeNode* pptr = ancestors.empty() ? NULL : ancestors.top();
			 //case 0: ptr has 0 children
			 if (childCount(ptr) == 0) {
				 //cout<<"case 0"<<endl;
				 eraseCase0(ptr, pptr);
				 //case 1: ptr has 1 children
			 }
			 else if (childCount(ptr) == 1) {
				 //cout<<"case 1"<<endl;
				 eraseCase1(ptr, pptr);
				 //case 2: ptr has 2 children
			 }
			 else {
				 //cout<<"case 2"<<endl;
				 eraseCase2(ptr, pptr, ancestors);
			 }

			 //now update heights and check balance factors of the ancestors
			 while (!ancestors.empty()) {
				 treeNode* anc = ancestors.top();
				 ancestors.pop();

				 //will the height of anc change?
				 updateHeight(anc);
				 //height is changing: is there imbalance?
				 if (bf(anc) > 1 || bf(anc) < -1) {
					 //Yes there is imbalance.
					 //Identify the type of imbalance and resolve it

					 //get parent pointer, required by rotation methods
					 treeNode* panc = ancestors.empty() ? NULL : ancestors.top();


					 if (bf(anc) == 2 && bf(anc->right) >= 0) {
						 //right-right
						 rl(anc, panc);
					 }
					 else
						 if (bf(anc) == -2 && bf(anc->left) <= 0) {
							 //left-left
							 rr(anc, panc);
						 }
						 else if (bf(anc) == 2 && bf(anc->right) == -1) {
								 //right-left
								 rr(anc->right, anc);
								 rl(anc, panc);
						 }else if (bf(anc) == -2 && bf(anc->left) == 1) {
									 //left right
									 rl(anc->left, anc);
									 rr(anc, panc);
						 }
				 }
			 }
			 n--;
			 return true;
		 }

	 }

	 int size() { return n; }

	 void InOrder(treeNode* root)
	 {
		 if (root != NULL)
		 {
			 InOrder(root->left);
			 cout << root->key << " ";
			 InOrder(root->right);
		 }
	 }

	 void printInorder() {
		 InOrder(root);
	 }

	 ~AVL()
	 {
		 destructor(root);
	 }

	 void destructor(treeNode* n)
	 {
		 if (n != NULL)
		 {
			 destructor(n->left);
			 destructor(n->right);
			 delete n;
		 }
	 }
 };
 
 /*
  //TreeNode of rbst for experiment (e)
	treeNode(const T& new_data, treeNode* lc = NULL, treeNode* rc = NULL) {
			 data = new_data;
			 left = lc;
			 right = rc;
			 chaos = rand();
	}

 //TreeNode of rbst for experiment (g)
	treeNode(const T& new_data, treeNode* lc = NULL, treeNode* rc = NULL) {
			 data = new_data;
			 left = lc;
			 right = rc;
			 int ran = rand();
			 int r = rand() % 5;
			 r = ran / pow(10, r);
			 chaos = r;	
	}
 */
 

 //Randomized Binary Search Tree Class
 template<typename T>
 class rbst {
	 struct treeNode {
		 T data;
		 treeNode* left, * right;
		 double chaos;

		 treeNode(const T& new_data, treeNode* lc = NULL, treeNode* rc = NULL) {
			 data = new_data;
			 left = lc;
			 right = rc;
			 chaos = uniform_real_distribution<double>(0, 1)(rng);
		 }


	 } *root;
	 
	 int n;		//size of the tree

	 bool getAccess(const T& key, treeNode*& ptr , treeNode*& pptr , stack<treeNode*>& ancestors)
	 {
		 ptr = root;
		 pptr = NULL;

		 while (ptr != NULL && ptr->data != key) {
			 ancestors.push(ptr);
			 pptr = ptr;//pptr is the parent of next ptr
			 if (key < ptr->data)
				 ptr = ptr->left;
			 else
				 ptr = ptr->right;
		 }

		 return (ptr != NULL);
	 }
	
	 int childCount(treeNode* ptr) {
		 if (ptr == NULL || (ptr->left == NULL && ptr->right == NULL))
			 return 0;
		 else if (ptr->left != NULL && ptr->right != NULL)
			 return 2;
		 else return 1;

	 }

	 void eraseCase0(treeNode* ptr, treeNode* pptr) {
		 delete ptr;
		 if (ptr == root) {
			 //cout<<"Root updated..."<<endl;
			 root = NULL;
		 }
		 else if (ptr == pptr->left) {
			 pptr->left = NULL;
		 }
		 else {
			 pptr->right = NULL;
		 }
	 }

	 void eraseCase1(treeNode* ptr, treeNode* pptr) {
		 treeNode* cptr;
		 if (ptr->right != NULL)
			 cptr = ptr->right;
		 else
			 cptr = ptr->left;
		 //ptr: node, pptr: parent of ptr, cptr: child of ptr
		 if (ptr == root) {
			 //cout<<"Root updated..."<<endl;
			 root = cptr;
			 delete ptr;
		 }
		 else {
			 if (ptr == pptr->left)//ptr is left child of pptr
				 pptr->left = cptr;
			 else//ptr is right child of pptr
				 pptr->right = cptr;

			 delete ptr;
		 }
	 }

	 void eraseCase2(treeNode* ptr, treeNode* pptr) {

		 treeNode* nptr = ptr->right, * pnptr = ptr;

		 while (nptr->left != NULL) {
			 pnptr = nptr;
			 nptr = nptr->left;
		 }

		 swap(ptr->data, nptr->data);

		 if (childCount(nptr) == 0) {
			 eraseCase0(nptr, pnptr);
		 }
		 else {
			 eraseCase1(nptr, pnptr);
		 }
	 }

	 void compareChaos(treeNode*& ptr, treeNode*& pptr, stack<treeNode*>& ancestors) {
		 while (ptr != root && ptr->chaos > pptr->chaos) {
			 ancestors.pop();

			 if (pptr != NULL && pptr->left == ptr)
				 rr(pptr, ancestors.top());

			 else if (pptr != NULL && pptr->right==ptr)	
				 rl(pptr, ancestors.top());

			 pptr = ancestors.top();

		 }
	 }

	 bool rr(treeNode* ptr, treeNode* pptr) {
		 //right rotate around ptr
		 treeNode* cptr = ptr->left;

		 if (cptr == NULL)
			 return false;
		 else {
			 //what is the node itself is the root
			 //promote cptr
			 if (pptr == NULL) {
				 root = cptr;
			 }
			 else {
				 if (ptr == pptr->left)
					 pptr->left = cptr;
				 else
					 pptr->right = cptr;
			 }


			 //adjust children
			 treeNode* rc_cptr = cptr->right;
			 cptr->right = ptr;
			 ptr->left = rc_cptr;
			 return true;
		 }
	 }

	 bool rl(treeNode* ptr, treeNode* pptr) {
		 //left rotate around ptr

		 treeNode* cptr = ptr->right;

		 if (cptr == NULL)
			 return false;
		 else {
			 if (pptr == NULL) {
				 root = cptr;
			 }
			 else {
				 if (ptr == pptr->left)
					 pptr->left = cptr;
				 else
					 pptr->right = cptr;
			 }

			 treeNode* lc_cptr = cptr->left;
			 cptr->left = ptr;
			 ptr->right = lc_cptr;
			 return true;
		 }
	 }

	 

 public:

	 rbst() {
		 root = NULL;
		 n = 0;
	 }

	 bool insert(const T& key) {
		 treeNode* ptr, * pptr;
		 stack<treeNode*> ancestors;
		 ancestors.push(NULL);

		 if (getAccess(key, ptr, pptr,ancestors))
			 return false;
		 else {
			 //ptr is null
			 if (pptr == NULL) {	//empty tree, because pptr==NULL
				 root = new treeNode(key, NULL, NULL);
			 }
			 else if (key < pptr->data) {
				 pptr->left = new treeNode(key, NULL, NULL);
				 ptr = pptr->left;
				 compareChaos(ptr, pptr, ancestors);
				
			 }
			 else {
				 pptr->right = new treeNode(key, NULL, NULL);
				 ptr = pptr->right;
				 compareChaos(ptr, pptr, ancestors);

			 }
			 n++;
			 return true;
		 }
	 }
	 
	 //Wrapper For Height 
	 int height() {
		 int h = height(root);
		 return h;
	 }

	 int height(treeNode* ptr) {
		 if (ptr == nullptr) {
			 return -1;
		 }
		 else {
			 int lh = height(ptr->left);
			 int rh = height(ptr->right);
			 if (lh > rh) {
				 return (lh + 1);
			 }
			 else {
				 return (rh + 1);
			 }
		 }
	 }

	 void printTree(treeNode* root, int sCount)
	 {
		 if (root == NULL) {
			 return;
		 }

		 sCount = sCount + 8;
		 printTree(root->right, sCount);
		 cout << endl;
		 for (int i = 10; i < sCount; i++) {
			 cout << " ";
		 }
		 cout << root->data << endl;

		 printTree(root->left, sCount);
	 }

	 void printTree(){
		 printTree(root, 0);
	 }

 };


int main()
{
	//Q1
	
		//TEST CODE FOR THREADED AVL
		avl<int> obj;
	
		//Insertion of the data
		obj.insert( 9);
		obj.insert(5);
		obj.insert(10);
		obj.insert(0);
		obj.insert(6);
		obj.insert(11);
		obj.insert(-1);
		obj.insert(1);
		//obj.insert(2);

		//INORDER
		cout << " AVL TREE STRUCTURE " " IN INORDER TRAVERSAL \n";
		obj.printInorder();
		cout << endl;
	
		//TREE FORM
		cout << "\nAvl tree Structure In Tree Form\n\n";
		obj.printTree();

		//ITERATOR
		cout << "\n\nAvl tree Structure Through ITERATOR\n";
		avl<int>::iterator itr;

		for (itr = obj.begin(); itr != obj.end(); ++itr) {
			cout << itr<<" ";
		}

		//TEST CODE FOR DELETION OF THE DATA
		obj.deleteNode(9);
	
		cout << "\n On deleting 9  \n";
		obj.printInorder();
		cout << endl;

		cout << "\nAvl tree Structure In Tree Form\n\n";
		obj.printTree();
	

	//Q2
	/*	
		//TEST CODE FOR THR PRIORITY QUEUE
		cout << endl << "priority queue : " << endl;

		priority_queue<int> obj1;
		obj1.enqueue(9);
		obj1.enqueue(5);
		obj1.enqueue(10);
		obj1.enqueue(0);
		obj1.enqueue(6);
		obj1.enqueue(11);
		obj1.enqueue(-1);
		obj1.enqueue(1);

		cout<<"obj1.peek = "<<obj1.peek();

		obj1.dequeue();
		obj1.print();
		
		obj1.updatePriority(10,12);
		obj1.print();
	*/
	
	//Q3
	
		//TEST CODE FOR rbst
		rbst<int> r;
		int keys[] = { 15, 10, 20, 8, 12};

		for (int i = 0; i < 5; i++)
			r.insert(keys[i]);

		cout << "\n\nPinting the rbst: \n\n";
		r.printTree();
		cout << endl;
	

	/*
		//(e)Experiment
		rbst<int> rtree;
		AVL<int> atree;	//unthreaded avl tree

		//make 10 million insertions

		//insertions for random tree
		for (int i = 0; i < 10000000; i++) {
			rtree.insert(i);
		}

		//insertions for avl tree
		for (int i = 0; i < 10000000; i++) {
			atree.insert(i);
		}

		int rbstHeight = rtree.height();
		int abstHeight = atree.height();

		int idealHeight=log2(10000000);
	

		cout << endl<<"The rbst Height = " << rbstHeight<<endl;
		cout << endl << "The abst Height = " << abstHeight << endl;
		cout<<endl<<"The Ideal Height = "<<idealHeight<<endl;

		int x = (rbstHeight) / (idealHeight); 
		int y = (abstHeight) / (idealHeight); 

	
	*/
	
	/*
		
		//f(i)
		the worst caase of a rbst is O(n) because we can get very unclucky while getting the random 
		number in a way that it can increse the deapth of the tree, it may look like linked list and
		then it will take O(n) time to the left or right for the isertion in the worst worst case

		//f(ii) Output

		The rbst Height = 328

		The abst Height = 23

		The Ideal Height = 23

		Distance Of The Ideal of avl : 1times
		Distance from the ideal of rbst : 14times
		
	*/
	

	//(g)
	//cout << "\n\nExperiment G: \n\n";
	/*
		Experiment G:

		The rbst Height = 113

		The abst Height = 23

		The Ideal Height = 23

		Distance Of The Ideal of avl: 1times
		Distance from the ideal of rbst: 4times
	*/
	
	rbst<int> rtree;
	AVL<int> atree;	//unthreaded avl tree

	//Experiment (h)
	//Make 10 million insertions
	

	//RBST Insertion
	auto start = chrono::steady_clock::now(); //stores the current time in start
	for (int i = 0; i < 100; i++) {
		rtree.insert(i);
	}
	auto end = chrono::steady_clock::now();//stores the current time in end

	//Time For RBST Insertion
	cout << "\nElapsed time for RBST in seconds : "
		<< chrono::duration_cast<chrono::nanoseconds>(end - start).count()
		<< " nanosec\n";


	////AVL Insertion
	//start = chrono::steady_clock::now(); //stores the current time in start
	//for (int i = 0; i < 10000000; i++) {
	//	atree.insert(i);
	//}
	//end = chrono::steady_clock::now();//stores the current time in end

	////Time For AVL Insertion
	//cout << "\nElapsed time for AVL Tree in seconds : "
	//	<< chrono::duration_cast<chrono::seconds>(end - start).count()
	//	<< " sec\n";


	////Heights
	//int rbstHeight = rtree.height();	
	//int abstHeight = atree.height();	
 //   int idealHeight=log2(10000000);
	//
	////Heights Print
	//cout << endl<<"The rbst Height = " << rbstHeight;
	//cout << endl << "The abst Height = " << abstHeight;
	//cout<<endl<<"The Ideal Height = "<<idealHeight<<endl;

	////Differnces With Ideal Hight
	//int x = (rbstHeight) / (idealHeight); 
	//int y = (abstHeight) / (idealHeight); 

	//cout << endl << "Distance Of The Ideal of avl: " << y;
	//cout<<"times \nDistance from the ideal of rbst: " << x << "times";
	
	/*
		//Experiment (h) Output
		Elapsed time for RBST in seconds : 327 sec

		Elapsed time for AVL Tree in seconds : 653 sec

		The rbst Height = 58
		The abst Height = 23
		The Ideal Height = 23

		Distance Of The Ideal of avl: 1times
		Distance from the ideal of rbst: 2times
	*/

	return 0;
}

