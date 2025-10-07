import random

# TASK 1

def load_file(filename: str) -> str:
    file = open(filename, "r")
    text = file.read()
    return text

# TASK 2

def text_approximation(probabilities: dict[str, float], length: int = 100000) -> str:
    chars = list(probabilities.keys())
    probs = list(probabilities.values())

    text = "".join(random.choices(chars, weights=probs, k=length))

    return text

def avg_word_len(text: str) -> int:
    word_lengths = [len(word) for word in text.split(" ")]
    return sum(word_lengths)/len(word_lengths)

probabilities = {chr(i): 1/27 for i in range(97, 123)}
probabilities[" "] = 1/27
text = text_approximation(probabilities)

print("average length:", avg_word_len(text))
# geometric distribution, (1-p)/p where p=1/27 is 26 - average length of a word (proven by practical experiments)

# TASK 3

def count_frequencies(filenames: list[str]) -> dict[str, float]:
    dictionary = {}
    for filename in filenames:
        for char in load_file(filename):
            if dictionary.get(char):
                dictionary[char] += 1
            else:
                dictionary[char] = 1
    total_len = sum(dictionary.values())
    for key in dictionary:
        dictionary[key] /= total_len
    return dictionary

results = count_frequencies(["norm_hamlet.txt", "norm_romeo_and_juliet.txt", "norm_wiki_sample.txt"])
frequencies = sorted([(key, value) for key, value in results.items()], key=lambda row: row[1])
# print(frequencies)
# most probable: e, a, t, i
# least probable: q, z, 6, 7

# TASK 4

text = text_approximation(results)
print("average length:", avg_word_len(text))

# TASK 5

def following_probabilities(predecessor: str, filenames: list[str]) -> dict[str, float]:
    counts = {chr(i): 0 for i in range(97, 123)} | {" ": 0} | {str(i): 0 for i in range(10)}
    for filename in filenames:
        loaded_file = load_file(filename)
        length = len(loaded_file)
        for i in range(length):
            if loaded_file[i] == predecessor and i+1 < length:
                counts[loaded_file[i+1]] += 1
    total_len = sum(counts.values())
    return {key: value/total_len for key, value in counts.items()}

for char, prob in frequencies[-2:]:
    results = following_probabilities(char, ["norm_hamlet.txt", "norm_romeo_and_juliet.txt", "norm_wiki_sample.txt"])
    freq = sorted([(key, value) for key, value in results.items()], key=lambda row: row[1], reverse=True)
    print(f"Most probable chars after '{char}':", freq[:5])

# Task 6

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
    
    return following_counts

def markov_approximation(filenames: list[str], order: int, length: int = 100000) -> str:
    probabilities = calculate_probs(filenames, order)

    cur_char = random.choice([chr(i) for i in range(97, 123)])
    text = cur_char

    for i in range(length):
        cur_char = random.choices(list(probabilities[cur_char].keys()), weights=list(probabilities[cur_char].values()),k=1)[0]
        text += cur_char
    
    return text

result = markov_approximation(["norm_hamlet.txt", "norm_romeo_and_juliet.txt", "norm_wiki_sample.txt"], 1)
print(result)
