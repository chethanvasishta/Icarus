#ifndef TYPE_H
#define TYPE_H

class Type{
public:
	enum TypeID {
		VoidTy=0,
		BoolTy,
		CharTy,	
		IntegerTy,
		FloatTy,
		DoubleTy,
		ArrayTy,
		StructTy
	};
	explicit Type(TypeID id) : m_typeID(id){}
	TypeID getTypeID() { return m_typeID; }

	const Type &operator=(const Type &); //do not implement this
	Type(const Type&); //do not implement this
private:
	TypeID m_typeID;	
};

class ArrayType : public Type {
public:
	explicit ArrayType(TypeID eltTypeID, int nelem): Type(ArrayTy), m_eltTypeID(eltTypeID), m_numElements(nelem){}
	const ArrayType &operator=(const ArrayType &); //do not implement this
	ArrayType(const ArrayType&); //do not implement this
	
	TypeID getEltTypeID() { return m_eltTypeID; }
	int getNumElements() { return m_numElements; }
private:
	TypeID m_eltTypeID;
	int m_numElements;
};

#endif //TYPE_H
