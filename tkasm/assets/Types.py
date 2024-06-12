from enum import Enum

class TypeClasses(Enum):
    INT = "int"
    UINT = "uint"
    FLOAT = "float"
    UNKNOWN = "unknown"
    
    def biggerTypeClass(class1: str, class2: str):
        if class1 == class2:
            return class1
        
        if class1 == "float" or class2 == "float":
            return "float"
        
        if class1 == "int" or class2 == "int":
            return "int"        
        
        if class1 == "uint" or class2 == "uint":
            return "uint"
        
        return "unknown"

class Type():
    def __init__(self, value: str, size: int, type_class: str, isArray: bool):
        self.value = value
        self.size = size
        self.type_class = type_class
        self.isArray = isArray

class Types(Enum):
    CHAR         = Type(r"%char",   1, TypeClasses.INT.value, False)
    INT8         = Type(r"%int8",   1, TypeClasses.INT.value, False)
    INT16        = Type(r"%int16",  2, TypeClasses.INT.value, False)
    INT32        = Type(r"%int32",  4, TypeClasses.INT.value, False)
    INT64        = Type(r"%int64",  8, TypeClasses.INT.value, False)
    
    BOOL         = Type(r"%bool",   1, TypeClasses.UINT.value, False)
    UINT8        = Type(r"%uint8",  1, TypeClasses.UINT.value, False)
    UINT16       = Type(r"%uint16", 2, TypeClasses.UINT.value, False)
    UINT32       = Type(r"%uint32", 4, TypeClasses.UINT.value, False)
    UINT64       = Type(r"%uint64", 8, TypeClasses.UINT.value, False)
    
    FLOAT        = Type(r"%float",  4, TypeClasses.FLOAT.value, False)
    DOUBLE       = Type(r"%double", 8, TypeClasses.FLOAT.value, False)

    ARRAY_CHAR   = Type(r"%array.char",   1, TypeClasses.INT.value, True)
    ARRAY_INT8   = Type(r"%array.int8",   1, TypeClasses.INT.value, True)
    ARRAY_INT16  = Type(r"%array.int16",  2, TypeClasses.INT.value, True)
    ARRAY_INT32  = Type(r"%array.int32",  4, TypeClasses.INT.value, True)
    ARRAY_INT64  = Type(r"%array.int64",  8, TypeClasses.INT.value, True)
    
    ARRAY_BOOL   = Type(r"%array.bool",   1, TypeClasses.UINT.value, True)
    ARRAY_UINT8  = Type(r"%array.uint8",  1, TypeClasses.UINT.value, True)
    ARRAY_UINT16 = Type(r"%array.uint16", 2, TypeClasses.UINT.value, True)
    ARRAY_UINT32 = Type(r"%array.uint32", 4, TypeClasses.UINT.value, True)
    ARRAY_UINT64 = Type(r"%array.uint64", 8, TypeClasses.UINT.value, True)
    
    ARRAY_FLOAT  = Type(r"%array.float",  4, TypeClasses.FLOAT.value, True)
    ARRAY_DOUBLE = Type(r"%array.double", 8, TypeClasses.FLOAT.value, True)
    
    UNKNOWN       = Type(r"%unknown",  0, TypeClasses.UNKNOWN.value, False)
    
    def biggerType(type1: Type, type2: Type) -> Type:
        if(type1.size > type2.size):
            return type1
        
        return type2
    
    def bestType(type1: Type, type2: Type) -> Type:
        
        if(type1.isArray != type2.isArray):
            return Types.UNKNOWN
        
        if type1 == Types.UNKNOWN or type2 == Types.UNKNOWN:
            return Types.UNKNOWN
        
        if type1 == Types.DOUBLE or type2 == Types.DOUBLE:
            return Types.DOUBLE
        
        if type1 == Types.FLOAT or type2 == Types.FLOAT:
            return Types.FLOAT
        
        biggestType: Type = Types.biggerType(type1, type2)
        biggestTypeClass: str = TypeClasses.biggerTypeClass(type1.type_class, type2.type_class)
        
        classTypes: list[Type] = []
        for type in Types:
            if type.value.type_class == biggestTypeClass and type.value.isArray == biggestType.isArray:
                classTypes.append(type)
        
        for type in classTypes:
            if type.value.size >= biggestType.size:
                return type
        
        return Types.UNKNOWN
             
    def getType(rawType: str) -> Type:
        
        for type in Types:
            if type.value.value == rawType:
                return type
            
        return Types.UNKNOWN
        
    
if __name__ == "__main__":
    for type in Types:
        print(type.value.value)
    