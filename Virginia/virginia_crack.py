import math
from collections import defaultdict

def decrypt(ciphertext_file, plaintext_file, key):
    with open(ciphertext_file, 'r', encoding='utf-8') as f:
        ciphertext = f.read().upper()
    key = key.upper()
    key_len = len(key)
    plaintext = []
    key_index = 0
    for char in ciphertext:
        if char.isalpha():
            shift = ord(key[key_index % key_len]) - ord('A')
            decrypted_char = chr((ord(char) - ord('A') - shift) % 26 + ord('A'))
            plaintext.append(decrypted_char)
            key_index += 1
        else:
            plaintext.append(char)
    with open(plaintext_file, 'w', encoding='utf-8') as f:
        f.write(''.join(plaintext))
def kasiski_test(ciphertext, seq_length=3):
    sequences = defaultdict(list)
    for i in range(len(ciphertext) - seq_length + 1):
        seq = ciphertext[i:i+seq_length]
        sequences[seq].append(i)
    distances = []
    for seq, positions in sequences.items():
        if len(positions) > 1:
            for j in range(1, len(positions)):
                distances.append(positions[j] - positions[0])
    factors = defaultdict(int)
    for d in distances:
        for i in range(2, int(math.sqrt(d)) + 1):
            if d % i == 0:
                factors[i] += 1
                if i != d // i:
                    factors[d // i] += 1
    if not factors:
        return None
    return max(factors, key=factors.get)
def frequency_analysis(group):
    english_freq = [
        0.08167, 0.01492, 0.02782, 0.04258, 0.12702, 0.02228, 0.02015,
        0.06094, 0.06966, 0.00153, 0.00772, 0.04025, 0.02406, 0.06749,
        0.07507, 0.01929, 0.00095, 0.05987, 0.06327, 0.09056, 0.02758,
        0.00978, 0.02360, 0.00150, 0.01974, 0.00074
    ]
    counts = [0] * 26
    for char in group:
        if char.isalpha():
            counts[ord(char) - ord('A')] += 1
    best_shift = 0
    min_chi = float('inf')
    for shift in range(26):
        chi = 0.0
        total = sum(counts)
        for i in range(26):
            expected = english_freq[(i - shift) % 26] * total
            observed = counts[i]
            if expected > 0:
                chi += (observed - expected) ** 2 / expected
        if chi < min_chi:
            min_chi = chi
            best_shift = shift
    return chr(best_shift + ord('A'))
def crack(ciphertext_file, plaintext_file):
    with open(ciphertext_file, 'r', encoding='utf-8') as f:
        ciphertext = f.read().upper()
    key_length = kasiski_test(ciphertext)
    if not key_length:
        key_length = 1  
    groups = [''] * key_length
    for i, char in enumerate(ciphertext):
        if char.isalpha():
            groups[i % key_length] += char
    key = ''.join([frequency_analysis(group) for group in groups])
    decrypt(ciphertext_file, plaintext_file, key)
    return key
key = crack('cipher.txt', 'cracked.txt')
print(f"破解的密钥: {key}")