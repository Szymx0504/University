import math
from collections import defaultdict
import matplotlib.pyplot as plt

def get_chars_words_entropies(data, max_order=10):
    chars_counts = [defaultdict(int) for _ in range(max_order+1)]
    chars_frequencies = [defaultdict(float) for _ in range(max_order+1)]
    chars_entropies = []

    for i in range(len(data)):
        for order in range(max_order+1):
            start_idx = i - order
            if start_idx < 0:
                break
            subtext = data[start_idx:i+1]
            if " " in subtext:
                break
            chars_counts[order][subtext] += 1

    for order in range(max_order+1):
        order_chars_counts = sum(chars_counts[order].values())
        for subtext, occurences in chars_counts[order].items():
            chars_frequencies[order][subtext] = occurences / order_chars_counts

    for order in range(max_order+1):
        if order == 0:
            entropy = 0
            
            for probability in chars_frequencies[order].values():
                if probability == 0:
                    continue
                entropy -= probability * math.log2(probability)
            chars_entropies.append(entropy)
        else:
            conditional_entropy = 0
            cond_entropies = defaultdict(float)
            for subtext, subtext_count in chars_counts[order].items():
                context = subtext[:-1] # x
                context_count = chars_counts[order-1][context]
                y_given_context = subtext_count / context_count
                if y_given_context > 0:
                    cond_entropies[context] -= y_given_context * math.log2(y_given_context)
            for context, condEnt in cond_entropies.items():
                context_probability = chars_frequencies[order-1][context]
                conditional_entropy += context_probability * condEnt
            chars_entropies.append(conditional_entropy)

    

    words_counts = [defaultdict(int) for _ in range(max_order+1)]
    words_frequencies = [defaultdict(float) for _ in range(max_order+1)]
    words_entropies = []

    data = data.split()
    for i in range(len(data)):
        for order in range(max_order+1):
            start_idx = i - order
            if start_idx < 0:
                break
            sub_words = tuple(data[start_idx:i+1])
            words_counts[order][sub_words] += 1
    for order in range(max_order+1):
        order_words_counts = sum(words_counts[order].values())
        for sub_words, occurences in words_counts[order].items():
            words_frequencies[order][sub_words] = occurences / order_words_counts
    for order in range(max_order+1):
        if order == 0:
            entropy = 0
            for probability in words_frequencies[order].values():
                if probability == 0:
                    continue
                entropy -= probability * math.log2(probability)
            words_entropies.append(entropy)
        else:
            conditional_entropy = 0
            cond_entropies = defaultdict(float)
            for sub_words, sub_words_count in words_counts[order].items():
                context = sub_words[:-1]
                context_count = words_counts[order-1][context]
                y_given_context = sub_words_count / context_count
                if y_given_context > 0:
                    cond_entropies[context] -= y_given_context * math.log2(y_given_context)
            for context, condEnt in cond_entropies.items():
                context_prob = words_frequencies[order-1][context]
                conditional_entropy += context_prob * condEnt
            words_entropies.append(conditional_entropy)

    return chars_entropies, words_entropies

def plot(chars_entropies, words_entropies, save_file):
    plt.figure(figsize=(10, 6))
    plt.plot(chars_entropies, "o-", label="Characters", linewidth=2, markersize=8)
    plt.plot(words_entropies, "s-", label="Words", linewidth=2, markersize=8)
    plt.xlabel("Order")
    plt.ylabel("Entropy")
    plt.legend()
    plt.grid(True, alpha=0.3)
    plt.xticks(range(len(chars_entropies)))
    plt.savefig(save_file, dpi=300, bbox_inches='tight')
    

file = open("./norm_wiki_en.txt", "r")
data = file.readline()
file.close()
chars_entropies, words_entropies = get_chars_words_entropies(data, 10)
print("Results for English:")
for i in range(len(chars_entropies)):
    print(f"Conditional entropy of order {i} for characters: {chars_entropies[i]}")
print()
for i in range(len(words_entropies)):
    print(f"Conditional entropy of order {i} for words: {words_entropies[i]}")
print()
plot(chars_entropies, words_entropies, "english_plot.png")

file = open("./norm_wiki_la.txt", "r")
data = file.readline()
file.close()
chars_entropies, words_entropies = get_chars_words_entropies(data, 10)
print("Results for Latin:")
for i in range(len(chars_entropies)):
    print(f"Conditional entropy of order {i} for characters: {chars_entropies[i]}")
print()
for i in range(len(words_entropies)):
    print(f"Conditional entropy of order {i} for words: {words_entropies[i]}")
print()
plot(chars_entropies, words_entropies, "latin_plot.png")


for i in range(6):
    file = open(f"./sample{i}.txt", "r")
    data = file.readline()
    file.close()
    chars_entropies, words_entropies = get_chars_words_entropies(data, 10)
    plot(chars_entropies, words_entropies, f"entropy_plot{i}.png")

# my guesses are:

# sample0.txt: not a real language
# Word entropy barely decreases from order 0 to 1. In real languages, knowing the previous word should make the next word much more predictable. Here it doesn't help much

# sample1.txt: a real language
# Looks like a real language - word entropy drops a lot from order 0 to 1 and both character and word entropy descrease smoothly with more context. That's what we expect

# sample2.txt: not a real language
# Similarly to sample0 - knowing the previous word doesn't help predict the next word enough. Real languages have stronger word-to-word connections

# sample3.txt: a real language
# Shows typical language patterns. Big decrease in word entropy from order 0 to 1 and gradual decrease for characters. Looks natural

# sample4.txt: not a real language
# The character entropy stays almost the same from order 0 to 3 which doesn't make sense. In real languages each additional character of context should make the next character more predictable so entropy should keep decreasing
# Also the word entropy drops way too much from order 0 to 1. It's unnatural for real languages

# sample5.txt: not a real language
# Word entropy goes almost to zero at order 1. This means words are completely predictable from context which doesn't happen in real languages