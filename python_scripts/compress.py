import csv

def load_encoding_table(csv_file):
    """
    Load the encoding table from the provided CSV file.
    Each row contains 'ASCII_Symbol', 'Length', and 'Codeword'.
    """
    encoding_table = {}
    
    with open(csv_file, mode='r') as file:
        reader = csv.DictReader(file)
        for row in reader:
            symbol = chr(int(row['ASCII_Symbol']))  # Convert ASCII value to character
            length = int(row['Length[4:0]'])
            codeword = row['Codeword'][:length]  # Slice codeword to the correct length
            encoding_table[symbol] = codeword
    
    return encoding_table

def write_encoding_table(file, encoding_table):
    """
    Write the encoding table to the output file in a format that can be parsed later.
    Convert strings to bytes before writing in binary mode.
    """
    for symbol, codeword in encoding_table.items():
        # Write each symbol, its length, and codeword in bytes
        line = f"{ord(symbol)},{len(codeword)},{codeword}\n"
        file.write(line.encode())  # Convert to bytes before writing

def compress_file(input_file, output_file, encoding_table):
    """
    Compress the input text file based on the encoding table,
    then write the encoding table and compressed binary data to the output file.
    """
    with open(input_file, 'r') as file:
        text = file.read()
    
    compressed_data = ""

    # Encode each character using the encoding table
    for char in text:
        if char in encoding_table:
            compressed_data += encoding_table[char]
        else:
            # Handle unencoded characters (e.g., use a default code or skip)
            compressed_data += '1111'  # Placeholder for unencoded characters (modify as needed)

    # Open the output file in binary mode to write both table and data
    with open(output_file, 'wb') as file:
        # Write the encoding table as a header in the output file
        write_encoding_table(file, encoding_table)
        
        # Add a separator to mark the end of the table and start of compressed data
        file.write(b"\n---END-OF-TABLE---\n")
        
        # Write the compressed binary data
        byte_data = int(compressed_data, 2).to_bytes((len(compressed_data) + 7) // 8, byteorder='big')
        file.write(byte_data)

    print(f"Compression complete. Compressed data and encoding table saved to {output_file}")

# Example usage
csv_file = 'encoding_table.csv'  # Path to the CSV file containing the encoding table
input_file = 'data_100KB.txt'       # Path to the input text file
output_file = 'compressed_with_table.bin'  # Path to save the compressed binary file

# Load the encoding table from the CSV
encoding_table = load_encoding_table(csv_file)

# Compress the file based on the encoding table
compress_file(input_file, output_file, encoding_table)
