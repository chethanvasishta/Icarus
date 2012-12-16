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

    DominanceNode<T>* getParent();
    void addChild(DominanceNode<T>* node);
    void removeChild(DominanceNode<T>* node);
    std::set<DominanceNode<T>*>& getChildren(); 
    std::set<DominanceNode<T>*>& getDominators(); //where should we allocate the Dominator set?
    T* getActualNode() { return m_actualNode; }
private:
    T* m_actualNode; 
    std::set<DominanceNode<T>*> m_children;

    //private constructors
    DominanceNode<T>();
    DominanceNode<T>(DominanceNode<T>&);

};

template<class T>
class DominanceTree {
public:
    DominanceTree() {
        m_root = 0;
    }
    ~DominanceTree() {}
    DominanceNode<T>* findNode(T* key);
    void setRoot(T* node);
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
}

template<class T>
DominanceNode<T>* DominanceTree<T>::findNode(T* key){
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
DominanceNode<T>* DominanceNode<T>::getParent(){
}

template<class T>
void DominanceNode<T>::addChild(DominanceNode<T>* node){
}

template<class T>
void DominanceNode<T>::removeChild(DominanceNode<T>* node){
}

template<class T>
std::set<DominanceNode<T>*>& DominanceNode<T>::getChildren(){
}

template<class T>
std::set<DominanceNode<T>*>& DominanceNode<T>::getDominators(){
}
#endif //DOMINANCE_TREE
