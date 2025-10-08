def calculate_probs(filenames: list[str], order: int) -> dict[str, float]:
    chars = [chr(i) for i in range(97, 123)] + [str(i) for i in range(10)] + [" "]
    following_counts = {}
    if order == 1:
        for char in chars:
            following_counts[char] = {chr(i): 0 for i in range(97, 123)} | {" ": 0} | {str(i): 0 for i in range(10)}

        for filename in filenames:
            content = load_file(filename)
            length = len(content)
            for i in range(length):
                if i+1 < length:
                    following_counts[content[i]][content[i+1]] += 1
        
        for char in following_counts:
            total_sum = sum(following_counts[char].values())
            if total_sum == 0:
                n = len(following_counts[char])
                for key in following_counts[char]:
                    following_counts[char][key] = 1/n
            else:
                for key in following_counts[char]:
                    following_counts[char][key] /= total_sum
    elif order == 3:
        for char1 in chars:
            following_counts[char1] = {chr(i): 0 for i in range(97, 123)} | {" ": 0} | {str(i): 0 for i in range(10)}
            for char2 in chars:
                following_counts[char1][char2] = {chr(i): 0 for i in range(97, 123)} | {" ": 0} | {str(i): 0 for i in range(10)}
                for char3 in chars:
                    following_counts[char1][char2][char3] = {chr(i): 0 for i in range(97, 123)} | {" ": 0} | {str(i): 0 for i in range(10)}
        
        for filename in filenames:
            content = load_file(filename)
            length = len(content)
            for i in range(length):
                if i+3 < length:
                    following_counts[content[i]][content[i+1]][content[i+2]][content[i+3]] += 1
        
        for char1 in following_counts:
            for char2 in following_counts[char1]:
                for char3 in following_counts[char1][char2]:
                    total_sum = sum(following_counts[char1][char2][char3].values())
                    if total_sum == 0:
                        n = len(following_counts[char1][char2][char3])
                        for key in following_counts[char1][char2][char3]:
                            following_counts[char1][char2][char3][key] = 1/n
                    else:
                        for key in following_counts[char1][char2][char3]:
                            following_counts[char1][char2][char3][key] /= total_sum
    
    return following_counts