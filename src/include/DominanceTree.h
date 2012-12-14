#ifndef DOMINANCE_TREE
#define DOMINANCE_TREE
/* Dominator Tree
 * A tree
 * Each node is a basic block
 * Parent of a node is the immediate dominator of the node
 * Recusively iterating up the tree from the node gets the dominators of the node
 */

#include <set>

template<class T> //T is usually a BasicBlock
class DominatorNode {
public:
    typedef typename std::set<DominatorNode<T>*>::iterator node_iterator;

    DominatorNode<T>* getParent();
    void addChild(DominatorNode<T>* node);
    void removeChild(DominatorNode<T>* node);
    std::set<DominatorNode<T>*>& getChildren();    
    std::set<DominatorNode<T>*>& getDominators(); //where should we allocate the Dominator set?
    T* getActualNode() { return m_actualNode; }
private:
    std::set<DominatorNode<T>*> m_children; 
    T* m_actualNode;        
};

template<class T>
class DominatorTree {
    public:
        DominatorTree() {
            m_root = 0;
        }
        ~DominatorTree() {}
        DominatorNode<T>* findNode(T* key);
        void setRoot(T* node); 
    private:
        DominatorNode<T>* m_root;
};


#endif //DOMINANCE_TREE
