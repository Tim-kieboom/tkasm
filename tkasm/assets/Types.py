from enum import Enum

class TypeClasses(Enum):
    INT = "integer"
    UINT = "uinteger"
    FLOAT = "float"
    VOID = "void"
    UNKNOWN = "unknown"
    
def biggerTypeClass(class1: str, class2: str):
    if class1 == class2:
        return class1
    
    if class1 == "unknown" and class2 == "unknown":
        return "unknown"
    
    if class1 == "void" or class2 == "void":
        return "void"
    
    if class1 == "float" or class2 == "float":
        return "float"
    
    if class1 == "integer" or class2 == "integer":
        return "integer"        
    
    if class1 == "uinteger" or class2 == "uinteger":
        return "uinteger"
    
    return "unknown"

class Type():
    def __init__(self, value: str, size: int, type_class: str, isArray: bool):
        self.value = value
        self.size = size
        self.type_class = type_class
        self.isArray = isArray

CHAR         = Type("char",   1, TypeClasses.INT.value, False)
INT8         = Type("int8",   1, TypeClasses.INT.value, False)
INT16        = Type("int16",  2, TypeClasses.INT.value, False)
INT32        = Type("int32",  4, TypeClasses.INT.value, False)
INT64        = Type("int64",  8, TypeClasses.INT.value, False)

BOOL         = Type("bool",   1, TypeClasses.UINT.value, False)
UINT8        = Type("uint8",  1, TypeClasses.UINT.value, False)
UINT16       = Type("uint16", 2, TypeClasses.UINT.value, False)
UINT32       = Type("uint32", 4, TypeClasses.UINT.value, False)
UINT64       = Type("uint64", 8, TypeClasses.UINT.value, False)

FLOAT        = Type("float",  4, TypeClasses.FLOAT.value, False)
DOUBLE       = Type("double", 8, TypeClasses.FLOAT.value, False)

STRING       = Type("string", 0, TypeClasses.INT.value, True)
ARRAY_STRING = Type("array.string", 0, TypeClasses.INT.value, True)

ARRAY_CHAR   = Type("array.char",   1, TypeClasses.INT.value, True)
ARRAY_INT8   = Type("array.int8",   1, TypeClasses.INT.value, True)
ARRAY_INT16  = Type("array.int16",  2, TypeClasses.INT.value, True)
ARRAY_INT32  = Type("array.int32",  4, TypeClasses.INT.value, True)
ARRAY_INT64  = Type("array.int64",  8, TypeClasses.INT.value, True)

ARRAY_BOOL   = Type("array.bool",   1, TypeClasses.UINT.value, True)
ARRAY_UINT8  = Type("array.uint8",  1, TypeClasses.UINT.value, True)
ARRAY_UINT16 = Type("array.uint16", 2, TypeClasses.UINT.value, True)
ARRAY_UINT32 = Type("array.uint32", 4, TypeClasses.UINT.value, True)
ARRAY_UINT64 = Type("array.uint64", 8, TypeClasses.UINT.value, True)

ARRAY_FLOAT  = Type("array.float",  4, TypeClasses.FLOAT.value, True)
ARRAY_DOUBLE = Type("array.double", 8, TypeClasses.FLOAT.value, True)


VOID          = Type("void",        0, TypeClasses.VOID.value, False)
UNKNOWN       = Type("unknown",     0, TypeClasses.UNKNOWN.value, False)
    
ALLTYPES = [CHAR, INT8, INT16, INT32, INT64, BOOL, UINT8, UINT16, UINT32, UINT64, FLOAT, DOUBLE, STRING, ARRAY_STRING, ARRAY_CHAR, ARRAY_INT8, ARRAY_INT16, ARRAY_INT32, ARRAY_INT64, ARRAY_BOOL, ARRAY_UINT8, ARRAY_UINT16, ARRAY_UINT32, ARRAY_UINT64, ARRAY_FLOAT, ARRAY_DOUBLE, VOID, UNKNOWN]
    
def biggerType(type1: Type, type2: Type) -> Type:
    if(type1.size > type2.size):
        return type1
    
    return type2

def bestType(type1: Type, type2: Type) -> Type:
    
    if(type1.isArray != type2.isArray):
        return UNKNOWN
    
    if type1 == VOID or type2 == VOID:
        return VOID
    
    if type1 == UNKNOWN or type2 == UNKNOWN:
        return UNKNOWN
    
    if type1 == DOUBLE or type2 == DOUBLE:
        return DOUBLE
    
    if type1 == FLOAT or type2 == FLOAT:
        return FLOAT
    
    biggestType: Type = biggerType(type1, type2)
    biggestTypeClass: str = TypeClasses.biggerTypeClass(type1.type_class, type2.type_class)
    
    classTypes: list[Type] = []
    for type in ALLTYPES:
        if type.value.type_class == biggestTypeClass and type.value.isArray == biggestType.isArray:
            classTypes.append(type)
    
    for type in classTypes:
        if type.value.size >= biggestType.size:
            return type
    
    return UNKNOWN
            
def getType(rawType: str) -> Type:
    
    if rawType.__contains__("[]"):
        rawType = rawType.replace("[]", "")
        return getType("array."+rawType)
    
    if rawType =="int":
        return INT32
    
    if rawType =="uint":
        return UINT32
    
    for type in ALLTYPES:
        if type.value == rawType:
            return type
        
    return UNKNOWN
  
def isType(rawType: str) -> bool:
    return getType(rawType) != UNKNOWN and getType(rawType) != VOID
  
def type_to_asmType(type: Type) -> str:
    size: int = type.size * 8
    
    if size == 0:
        return ""
    
    elif size == 8:
        return "BYTE"
    elif size == 16:
        return "WORD"
    elif size == 32:
        return "DWORD"
    elif size == 64:
        return "QWORD"
    
if __name__ == "__main__":
    for type in ALLTYPES:
        print(type.value.value)
    