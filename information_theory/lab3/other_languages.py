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
            subtext = data[start_idx:i+1] #include i
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

    

    words_counts = [defaultdict(int) for _ in range(max_order+1)] # keys will be tuples of words
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

languages = ["eo", "et", "ht", "nv", "so"]
for i in range(5):
    file = open(f"./norm_wiki_{languages[i]}.txt", "r")
    data = file.readline()
    file.close()
    chars_entropies, words_entropies = get_chars_words_entropies(data, 10)
    plot(chars_entropies, words_entropies, f"{languages[i]}_plot.png")