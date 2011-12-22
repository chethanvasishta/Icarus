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
private:
	TypeID m_typeID;	
};

#endif //TYPE_H
