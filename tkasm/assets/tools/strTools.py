def split_without_brackets(text: str) -> list[str]:
    result = []
    current_word = ""
    in_square_brackets = False
    in_round_brackets = False
    in_string = False

    text = text.replace("\n", "").replace("\t", "")

    for char in text:
        if char == '(':
            in_square_brackets = True
        elif char == ')':
            in_square_brackets = False
            
        elif char == '[':
            in_round_brackets = True
        elif char == ']':
            in_round_brackets = False
            
        elif char == '"':
            in_string = not in_string
            
        elif char == ' ' and not in_string and not in_square_brackets and not in_round_brackets:
            if current_word:
                result.append(current_word)
            current_word = ""
            continue
            
        current_word += char

    if current_word:
        result.append(current_word)

    return result

def containsBrackets(line: str) -> bool:
    return line.find('(') != -1 and line.find(')') != -1

def filter_out_comments(parts: list[str]) -> list[str]:
    if parts.__len__() == 0:
        return []
    
    if parts[0].startswith("//"):
        return []
    
    for i, part in enumerate(parts):
        if part.startswith("//"):
            return parts[:i-1]
        
    return parts