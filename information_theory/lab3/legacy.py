# entropies = [
#     # { # rank 0
#     #     chr(a): None for a in range(97, 123)
#     # },
#     # {
#     #     chr(a): {chr(b): None for b in range(97, 123)} for a in range(97, 123) 
#     # } # etc...
# ] # index = rank

# chars_counts = [
#     # {chr(a): 0 for a in range(97, 123)},
#     # {chr(a)+chr(b): 0 for a in range(97, 123) for b in range(97, 123)}
#     # etc...
# ]

# first chars
# chars_counts = []
# for word in data:
#     accumulated_text = ""
#     for i in range(len(word)):
#         char = word[i]
#         accumulated_text += char
#         for j in range(min(5, i)): # up to 5th order
#             text = accumulated_text[-j-1:]
#             main_key = len(text) - 1
#             if len(chars_counts) - 1 < main_key:
#                 chars_counts.append({})
#             chars_counts[main_key][text] = chars_counts[main_key].get(text, 0) + 1
# chars_frequencies = []
# for i in range(len(chars_counts)):
#     chars_frequencies.append({})
#     len_occurences = sum(chars_counts[i].values())
#     for key in chars_counts[i]:
#         chars_frequencies[i][key] = chars_counts[i][key] / len_occurences
# entropies = []
# for i in range(len(chars_frequencies)):
#     entropies.append({})
#     for key in chars_frequencies[i]:
#         entropies[i][key] = -chars_frequencies[i][key] * math.log2(chars_frequencies[i][key])

# print(entropies)



    
# my guesses are:
# sample0.txt doesn't represent a real language because a conditional entropy of order 1 for words is almost the same as for order 0
# sample1.txt represents a real language because its graph of conditional entropies for words and characters looks almost like for English and Latin
# sample2.txt doesn't represent a real language because a conditional entropy of order 1 for words is only slightly lower than for order 0 (should be substantially lower)
# sample3.txt represents a real language because its graph of conditional entropies for words and characters looks almost like for English and Latin - 
# sample4.txt doesn't represent a real language because the conditional entropy of order 1 for words is over 6 times lower than that of order 0 as well as conditional entropy for characters is almost the same for orders from 0 to 3
# sample5.txt doesn't represent a real language because the conditional entropy of order 1 for words is roughly 0 - which doesn't make sense for a language
