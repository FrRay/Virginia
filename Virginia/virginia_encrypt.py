def encrypt(plaintext_file, ciphertext_file, key):
    with open(plaintext_file, 'r', encoding='utf-8') as f:
        plaintext = f.read().upper()
    key = key.upper()
    key_len = len(key)
    ciphertext = []
    key_index = 0
    for char in plaintext:
        if char.isalpha():
            shift = ord(key[key_index % key_len]) - ord('A')
            encrypted_char = chr((ord(char) - ord('A') + shift) % 26 + ord('A'))
            ciphertext.append(encrypted_char)
            key_index += 1
        else:
            ciphertext.append(char)
    with open(ciphertext_file, 'w') as f:
        f.write(''.join(ciphertext))

encrypt('plain.txt', 'cipher.txt', 'Cookie')