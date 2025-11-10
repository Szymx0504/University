import random

def load_file(filename: str) -> str:
    file = open(filename, "r")
    text = file.read()
    return text

def avg_word_len(text: str) -> int:
    word_lengths = [len(word) for word in text.split(" ")]
    return sum(word_lengths)/len(word_lengths)

def calculate_probs(filenames: list[str], order: int) -> dict[str, float]:
    chars = [chr(i) for i in range(97, 123)] + [str(i) for i in range(10)] + [" "]
    following_counts: dict[str, dict[str, int]] = {}

    for filename in filenames:
        content = load_file(filename)
        length = len(content)
        for i in range(length-order):
            context = content[i:i+order]
            next_char = content[i+order]
            if not following_counts.get(context):
                following_counts[context] = {c: 0 for c in chars}
            following_counts[context][next_char] += 1
    
    for context in following_counts:
        total_sum = sum(following_counts[context].values())
        if total_sum == 0:
            n = len(following_counts[context])
            for next_char in following_counts[context]:
                following_counts[context][next_char] = 1/n
        else:
            for next_char in following_counts[context]:
                following_counts[context][next_char] /= total_sum

    return following_counts

def markov_approximation(filenames: list[str], order: int, length: int = 1000, context: str = "", seed_order: int = 0) -> str:
    probabilities = calculate_probs(filenames, order)
    
    if context:
        cur_context = context[-order:]
        text = context
    else:
        if order == 1:
            cur_context = random.choice([chr(i) for i in range(97, 123)])
            text = cur_context
        else:
            cur_context = markov_approximation(filenames, seed_order or order-1, order)
            text = cur_context

    for i in range(length-len(cur_context)):
        if not probabilities.get(cur_context):
            cur_context = random.choice(list(probabilities.keys()))

        new_char = random.choices(list(probabilities[cur_context].keys()), weights=list(probabilities[cur_context].values()),k=1)[0]
        cur_context = cur_context[1:] + new_char
        text += new_char
    
    return text

result0 = markov_approximation(["norm_hamlet.txt", "norm_romeo_and_juliet.txt", "norm_wiki_sample.txt"], 1)
print(result0)

print("---------------------------------------------------------")

result1 = markov_approximation(["norm_hamlet.txt", "norm_romeo_and_juliet.txt", "norm_wiki_sample.txt"], 3, seed_order=1)
print(result1)

print("---------------------------------------------------------")

result2 = markov_approximation(["norm_hamlet.txt", "norm_romeo_and_juliet.txt", "norm_wiki_sample.txt"], 3)
print(result2)

print("---------------------------------------------------------")

result3 = markov_approximation(["norm_hamlet.txt", "norm_romeo_and_juliet.txt", "norm_wiki_sample.txt"], 5, context="probability")
print(result3)

print("---------------------------------------------------------")

print("avg word len in 1st order approximation:", avg_word_len(result0))
print("avg word len in 3rd order approximation with 1st order seed:", avg_word_len(result1))
print("avg word len in 3rd order approximation:", avg_word_len(result2))
print("avg word len in 5th order approximation:", avg_word_len(result3))