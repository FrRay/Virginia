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
    with open(plaintext_file, 'w') as f:
        f.write(''.join(plaintext))
decrypt('cipher.txt', 'decrypted.txt', 'Cookie')