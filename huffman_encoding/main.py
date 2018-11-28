def create_table(message, length):

    # char and freq dictionary
    char_and_freq_dict = {} 
    for c in message:
        if c not in char_and_freq_dict:
            char_and_freq_dict.update({c: 1})
        else:
            char_and_freq_dict[c] = char_and_freq_dict[c] + 1
    # if message is hello  then char_and_freq_dictionary will contain {'h': 1, 'l': 2, 'e': 1, 'o': 1}

    # create a 2d list with each row as [char, freq, code]
    # for now code will be empty
    table = sorted(char_and_freq_dict.items(), key=lambda x: x[1])
    # for key in char_and_freq_dict:
    #     table.append([key, char_and_freq_dict[key],])
    print(table)

if __name__ == "__main__":
    message = input("Enter a string: ")
    length = len(message)

    # create the table
    create_table(message, length)