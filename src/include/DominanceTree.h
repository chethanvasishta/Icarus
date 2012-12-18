#ifndef DOMINANCE_TREE
#define DOMINANCE_TREE
/* Dominator Tree
 * A tree
 * Each node is a basic block
 * Parent of a node is the immediate dominator of the node
 * Recusively iterating up the tree from the node gets the dominators of the node
 */

#include <set>
#include <cstddef>
#include <queue>

template<class T> //T is usually a BasicBlock
class DominanceNode {
public:
    typedef typename std::set<DominanceNode<T>*>::iterator node_iterator;

    DominanceNode<T>(T* node) { m_actualNode = node; }

    DominanceNode<T>* getParent() { return m_parent; }
    void setParent(DominanceNode<T>* parent) { m_parent = parent; }

    void addChild(DominanceNode<T>* node);
    void removeChild(DominanceNode<T>* node);

    std::set<DominanceNode<T>*>& getChildren(); 
    std::set<DominanceNode<T>*>& getDominators(); //where should we allocate the Dominator set?
    T* getActualNode() { return m_actualNode; }
private:
    T* m_actualNode; 
    std::set<DominanceNode<T>*> m_children;
    DominanceNode<T>* m_parent;

    //private constructors
    DominanceNode<T>();
    DominanceNode<T>(DominanceNode<T>&);

};

template<class T>
class DominanceTree {
public:
    DominanceTree() { m_root = NULL; }
    ~DominanceTree() {}
    DominanceNode<T>* findNode(T* key);
    void setRoot(T* node);
    int depth(T*);
    void setIDom(T* node, T* dom);
    void print();
private:
    DominanceNode<T>* m_root;

    DominanceNode<T>* getNewNode(T* node){
        return new DominanceNode<T>(node);
    }
};

template<class T>
void DominanceTree<T>::setRoot(T* node) {
    m_root = findNode(node);
    if(m_root == NULL)
        m_root = getNewNode(node);
    m_root->setParent(NULL);
}

template<class T>
DominanceNode<T>* DominanceTree<T>::findNode(T* key){
    if(m_root == NULL)
        return NULL;
    std::queue<DominanceNode<T>* > visitQ;
    visitQ.push(m_root);
    while(!visitQ.empty()){
        DominanceNode<T>* node = visitQ.front();
        visitQ.pop();
        if(node->getActualNode() == key)
            return node;
        std::set<DominanceNode<T>*>& children = node->getChildren();
        for(typename std::set<DominanceNode<T>*>::iterator iter = children.begin();
                                          iter != children.end(); ++iter){
            visitQ.push(*iter);
        }
    }
    return NULL;
}

template<class T>
int DominanceTree<T>::depth(T* key){
    DominanceNode<T>* node = findNode(key);
    int depth = 0;
    while(node != NULL){
        node = node->getParent();
        ++depth;
    }
    return depth;
}

template<class T>
void DominanceTree<T>::setIDom(T* node, T* idom){
    DominanceNode<T>* domNode = findNode(node);
    DominanceNode<T>* domIdom = findNode(idom);
    if(domNode == NULL)
        domNode = getNewNode(node);
    else //we need to disavow from the old parent
        domNode->getParent()->removeChild(domNode);
    domIdom->addChild(domNode);
}

template<class T>
void DominanceNode<T>::addChild(DominanceNode<T>* node){
    m_children.insert(node);
    node->setParent(this);
}

template<class T>
void DominanceNode<T>::removeChild(DominanceNode<T>* node){
    m_children.erase(m_children.find(node));
    node->setParent(NULL);
}

template<class T>
std::set<DominanceNode<T>*>& DominanceNode<T>::getChildren(){
    return m_children;
}

template<class T>
std::set<DominanceNode<T>*>& DominanceNode<T>::getDominators(){
}

//specialization for basic block
template<> void DominanceTree<llvm::BasicBlock>::print();

#endif //DOMINANCE_TREE
