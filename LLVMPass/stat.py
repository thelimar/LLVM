import re
import matplotlib.pyplot as plt
from collections import Counter

with open("instr.txt", "r") as file:
    text = file.read()
words = re.findall(r'\w+', text)
words = [word.lower() for word in words]
filtered_words = [word for word in words if word]

word_counter = Counter(filtered_words)
top_words = word_counter.most_common(10)
top_words, top_frequencies = zip(*top_words)

plt.figure(figsize=(18, 9))
plt.bar(top_words, top_frequencies)
plt.ylabel("Frequncy")
plt.xlabel("Inst name")
plt.title("Most frequent instr for WS = 1")
plt.tight_layout()
plt.savefig("fig1.png")


bigrams = [filtered_words[i] + "\n" + filtered_words[i + 1] for i in range(len(filtered_words) - 1)]
bigram_counter = Counter(bigrams)
top_bigrams = bigram_counter.most_common(10)
top_bigram_words, top_bigram_frequencies = zip(*top_bigrams)

plt.figure(figsize=(18, 9))
plt.bar(top_bigram_words, top_bigram_frequencies)
plt.ylabel("Frequency")
plt.xlabel("Inst name")
plt.title("Most frequent instr for WS = 2")
plt.savefig("fig2.png")

trigrams = [filtered_words[i] + "\n" + filtered_words[i + 1] + "\n" + filtered_words[i + 2] for i in range(len(filtered_words) - 2)]
trigram_counter = Counter(trigrams)
top_trigrams = trigram_counter.most_common(10)
top_trigram_words, top_trigram_frequencies = zip(*top_trigrams)

plt.figure(figsize=(18, 9))
plt.bar(top_trigram_words, top_trigram_frequencies)
plt.ylabel("Frequency")
plt.xlabel("Inst name")
plt.title("Most frequent instr for WS = 3")
plt.savefig("fig3.png")

quadgrams = [filtered_words[i] + "\n" + filtered_words[i + 1] + "\n" + filtered_words[i + 2] + "\n" + filtered_words[i + 3] for i in range(len(filtered_words) - 3)]
quadgram_counter = Counter(quadgrams)
top_quadgrams = quadgram_counter.most_common(10)
top_quadgram_words, top_quadgram_frequencies = zip(*top_quadgrams)

plt.figure(figsize=(18, 9))
plt.bar(top_quadgram_words, top_quadgram_frequencies)
plt.ylabel("Frequency")
plt.xlabel("Inst name")
plt.title("Most frequent instr for WS = 4")
plt.savefig("fig4.png")

pentagrams = [filtered_words[i] + "\n" + filtered_words[i + 1] + "\n" + filtered_words[i + 2] + "\n" + filtered_words[i + 3] + "\n" + filtered_words[i + 4] for i in range(len(filtered_words) - 4)]
pentagram_counter = Counter(pentagrams)
top_pentagrams = pentagram_counter.most_common(10)
top_pentagram_words, top_pentagram_frequencies = zip(*top_pentagrams)

plt.figure(figsize=(18, 9))
plt.bar(top_pentagram_words, top_pentagram_frequencies)
plt.ylabel("Frequency")
plt.xlabel("Inst name")
plt.title("Most frequent instr for WS = 5")
plt.savefig("fig5.png")





