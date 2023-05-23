#include <iostream>
#include <cstdlib>
#include <limits>
#include <ctime>
#include <vector>
#include <limits>
using namespace std;


template <class T> 
struct Node{
 public:
	Node<T>* right;
	Node<T>*down;
	Node<T>* left;
	Node<T>* up;
	T value;
	
	Node(const T& value){
	    this->value = value;
	    this->right = NULL;
	    this->down = NULL;
	    this->left = NULL;
	    this->up = NULL;		
	}
	
	void printvalue(){
		        if (value < std::numeric_limits<T>::max() && value > std::numeric_limits<T>::min()) {cout << value << " ";}
	}
};


template <class T> class skipList{
	public:
	  skipList();
      void add(T value);		
  	  void printData();	  
  	 Node<T>* insert_to_level(
      T N, int level, Node<T>* down);  // Returns the pointer to the up node 	  
	  void erase(T value);
	  void addLevel();
      bool Search(T value);	  
      Node<T>* SearchFirstNode(T val);
      Node<T>* SearchNode(T value);
      Node<T>* insertAfter(T val , Node<T>* t , Node<T>* c);
        int Levels;  // Number of Levels in SkipList, Accessed via Heads
      Node<T>** heads;
      vector<Node<T>*> Heads;  // Head of each Linked List in Skiplist
};


// Constructor
template <class T>
skipList<T>::skipList() {
  srand(static_cast<unsigned int>(time(NULL)));
  Node<T>* Head1 = new Node<T>(std::numeric_limits<T>::min());

  Node<T>* Tail1 = new Node<T>(std::numeric_limits<T>::max());

  Head1->right = Tail1;

  Tail1->left = Head1;

  Heads.push_back(Head1);
}


template <class T>
Node<T>* skipList<T>::insertAfter(T val , Node<T>* t , Node<T>* c)
{
    Node<T>* newNode = new Node<T>(val);
    newNode->right = t->right;
    newNode->left  = t;
    t->right = newNode;
    newNode->right->left = newNode;
    newNode->down = c;
    return newNode;
}

template <class T>
Node<T>* skipList<T>::SearchFirstNode(T val){	
	//	Node<T>* topleftnode = heads[levelAmount-1];
	Node<T>* topleftnode =Heads[Heads.size() - 1];
  	Node<T>* pt = new Node<T>(std::numeric_limits<T>::min());
	pt = topleftnode;	
	  while (pt != NULL) {
	    if (pt->value == val) {
	      break;
	    } else if (val > pt->value && val >= pt->right->value) {
	      pt = pt->right;
	    } else if (val > pt->value && val < pt->right->value) {
	      pt = pt->down;
	    }
	  }
	  return pt;	
}

template <class T>
Node<T>* skipList<T>::SearchNode(T val)
{
    //	Node<T>* topleftnode = heads[levelAmount-1];
    Node<T>* topleftnode =Heads[Heads.size() - 1];
    do {
        if(topleftnode->down != NULL) {
            topleftnode = topleftnode->down;
        }
        while(val >= topleftnode->right->value) {
            topleftnode= topleftnode->right;
        }
    } while(topleftnode->down != NULL);
    return topleftnode;
}


template <class T>
bool skipList<T>::Search(T val){
	//	Node<T>* topleftnode = heads[levelAmount-1];
    Node<T>* topleftnode =Heads[Heads.size() - 1];
    do {
        if(topleftnode->down != NULL) {
            topleftnode = topleftnode->down;
        }
        while(val >= topleftnode->right->value) {
            topleftnode= topleftnode->right;
        }
    } while(topleftnode->down != NULL);
    
	if(val == topleftnode->value){
		return true;
	}else{
		return false;
	}
}

template <class T>
void skipList<T>::erase(T val){
	Node<T>* pt = SearchFirstNode(val);
	while (pt != NULL) {
	    Node<T>* temp = pt->down;
		if (pt->down != NULL) pt->down->up = NULL;
		  if (pt->up != NULL) pt->up->down = NULL;
		  Node<T>* Next = pt->right;
		   Node<T>* Prev = pt->left;
		  Prev->right = Next;
		  delete (pt);
		  Next->left = Prev;
	    pt = temp;
	  }	
}

template <class T>
void skipList<T>::addLevel(){

	 
	 Node<T>* firsthead = new Node<T>(std::numeric_limits<T>::min());
  
 	 Node<T>* tirsttail = new Node<T>(std::numeric_limits<T>::max());

 	 firsthead->right = tirsttail;
	
  	 tirsttail->left = firsthead;

  	  Heads[Heads.size() - 1]->up = firsthead;
  	  
      firsthead->down = Heads[Heads.size() - 1];
    
	  Heads.push_back(firsthead);

}

template <class T>
Node<T>* skipList<T>::insert_to_level(T data, int level, Node<T>* Down) {
  int i = level;
  Node<T>* N = new Node<T>(data);
  N->down = Down;
  Node<T>* pt = new Node<T>(0);
  pt = Heads[i];

  while (pt->right != NULL && data > pt->right->value) {
    pt = pt->right;
  }
  N->left = pt;
  N->right = pt->right;
  pt->right = N;
  N->right->left = N;
  return N;
}


template <class T>
void skipList<T>::add(T val)
{
    Node<T>* placetoaddinbottomlevel = SearchNode(val);
    Node<T>* newNode = insertAfter(val , placetoaddinbottomlevel , NULL);

    int i = 0;
    int Coin_Toss = rand() % 2;
    while(Coin_Toss  == 0) {
        i += 1;
        if(i >= Heads.size()) {
            addLevel();
        }
        while(placetoaddinbottomlevel->up == NULL) {
            placetoaddinbottomlevel = placetoaddinbottomlevel->left;
        }
        placetoaddinbottomlevel = placetoaddinbottomlevel->up;
        Node<T>* newNode_ = insertAfter(val , placetoaddinbottomlevel , newNode);
        newNode->up = newNode_;
        newNode = newNode_;
        Coin_Toss = rand() % 2;
    }
}

/*
// Insert to skip list function
template <class T>
void skipList<T>::add(T data) {
  int i = 0;
  Node<T>* Down =
      insert_to_level(data, i, NULL);  // Insert data to bottom level
  // Now Decide Whether to Create Top Layer and Insert;
  int Coin_Toss = rand() % 2;
  while (Coin_Toss == 0) {
    i++;
    if (Levels < i) {
      Levels += 1;
      Node<T>* NewHead = new Node<T>(std::numeric_limits<T>::min());
      Node<T>* NewTail = new Node<T>(std::numeric_limits<T>::max());
      NewHead->right = NewTail;
      NewTail->left = NewHead;
      Heads[i - 1]->up = NewHead;
      NewHead->down = Heads[i - 1];
      Heads.push_back(NewHead);
    }
    Node<T>* N = insert_to_level(data, i, Down);
    Down->up = N;
    Down = N;
    Coin_Toss = rand() % 2;
  }
  return;
}
*/

/*template <class T>
void skipList<T>::add(T val){
	Node<T>* placetoaddinbottomlevel = SearchNode(val);
	Node<T>* newNode = new Node<T>(val);
	Node<T>* down;
	if(val == placetoaddinbottomlevel->value){
		return;
	}else{
		//added in level 1;
		newNode->right = placetoaddinbottomlevel->right;
		newNode->left  = placetoaddinbottomlevel;
		placetoaddinbottomlevel->right = newNode;
		newNode->right->left = newNode;
		down = newNode;
	}
	int level = 0;
	int Coin_Toss = rand() % 2;
	while(Coin_Toss  == 0){
		level += 1;
		if(level > Heads.size()){
			addLevel();
		}
		while(placetoaddinbottomlevel->up == NULL){
			placetoaddinbottomlevel = placetoaddinbottomlevel->left;
		}
		placetoaddinbottomlevel = placetoaddinbottomlevel->up;
		Node<T>* newNode_ = new Node<T>(val);
		newNode_->right = placetoaddinbottomlevel->right;
		newNode_->left  = placetoaddinbottomlevel;
		placetoaddinbottomlevel->right = newNode_;
		newNode_->right->left = newNode_;
		newNode_->down = down;
		down->up = newNode_;
		down = newNode_;		
		Coin_Toss = rand() % 2;
	}
}
*/

template <class T>
void skipList<T>::printData(){
  for (int i = 0; i != Heads.size(); i++) {
    cout << " LEVEL : " << i << endl;
    Node<T>* pt = new Node<T>(std::numeric_limits<T>::min());
    pt = Heads[i];
    while (pt != NULL) {
      pt->printvalue();
      pt = pt->right;
    }
    cout << endl;
  }
}

int main() { 

  skipList<int> *mySL = new skipList<int>();
  
  cout << "SKIP LIST DATA : " << endl;
  mySL->add(4);
  mySL->add(5);
  mySL->add(6);
  mySL->add(2);
  mySL->add(3);
  mySL->add(7); 
  mySL->add(10);
  mySL->add(311);
  mySL->add(0); 
  mySL->add(8);
  mySL->add(9);
  mySL->add(11);
  mySL->printData();
  //mySL->Search(4);
  cout<<"\n";
  if(mySL->Search(6)){
  	cout<<1<<": found"<<"\n";
  }
    if(!mySL->Search(46)){
  	cout<<1<<": not found"<<"\n";
  }

  int i = 5; // Node to be deleted
  
  mySL->erase(i); 
  cout <<"\n \n \n AFTER DELETION OF : " << i << endl;
  cout << "\n \n \n " << endl;
  mySL->printData();


  mySL->erase(7);
  cout <<"\n \n \n AFTER DELETION OF : " << 7 << endl;
  cout << "\n \n \n " << endl;
  mySL->printData();

  cout<< endl;

	
	
 return 0;
}
