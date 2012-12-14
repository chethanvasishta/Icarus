#include <DominanceTree.h>
#include <queue>
#include <set>
#include <cstddef>

template<class T>
void DominatorTree<T>::setRoot(T* node){
   m_root = getNode(node);
}

template<class T>
DominatorNode<T>* DominatorTree<T>::findNode(T* key){
    std::queue<DominatorNode<T>* > visitQ;
    visitQ.push_back(m_root);
    while(visitQ.size() != 0){
        DominatorNode<T>* node = visitQ.pop_front();
        if(node->getActualNode() == key)
            return key;
        std::set<DominatorNode<T>*>& children = node->getChildren();
        for(typename std::set<DominatorNode<T>*>::iterator iter = children.begin();
                                          iter != children.end(); ++iter){
            visitQ.push_back(*iter);
        }
    }
    return NULL;
}

template<class T>
DominatorNode<T>* DominatorNode<T>::getParent(){
}

template<class T>
void DominatorNode<T>::addChild(DominatorNode<T>* node){
}

template<class T>
void DominatorNode<T>::removeChild(DominatorNode<T>* node){
}

template<class T>
std::set<DominatorNode<T>*>& DominatorNode<T>::getChildren(){
}

template<class T>
std::set<DominatorNode<T>*>& DominatorNode<T>::getDominators(){
}
