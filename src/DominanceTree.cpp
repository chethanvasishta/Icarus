#include <DominanceTree.h>

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
