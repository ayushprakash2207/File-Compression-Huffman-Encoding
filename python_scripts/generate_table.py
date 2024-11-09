import pandas as pd
import numpy as np  # Import numpy for handling NaN values

# Load the CSV file with custom headers 
data = pd.read_csv('Encode_new.csv', header=None, skiprows=2, converters={'Code[26:0]': lambda x: int(x, 16)}, sep=',')
data.columns = [
    "Sample in Buffer", "Sample in Window", "TRIGGER", "Encode_en1", "Encode_we1", 
    "Encode_addr1[7:0]", "Encode_din1[31:0]", "Length[4:0]", "Code[26:0]", "Code_new[7:0]", "start_1", 
    "start", "done_OBUF", "data_addr1[16:0]"
]

# Filter data where Encode_we1 is 1
filtered_data = data[data['Encode_we1'] == 1]

# Extract data from the matching rows
if not filtered_data.empty:
    # Convert 'Code[26:0]' to binary string
    filtered_data.loc[:, "Code[26:0]"] = filtered_data["Code[26:0]"].apply(
        lambda x: bin(x)[2:].zfill(27)
    )

    # Convert 'Encode_addr1[7:0]' to integer
    filtered_data.loc[:, "Encode_addr1[7:0]"] = filtered_data["Encode_addr1[7:0]"].apply(
        lambda x: int(x, 16) if isinstance(x, str) else x
    )

    # Convert 'Length[4:0]' from hexadecimal to decimal
    filtered_data.loc[:, "Length[4:0]"] = filtered_data["Length[4:0]"].apply(lambda x: int(x, 16))

    # Create a new DataFrame with the extracted data
    new_table = pd.DataFrame({
        "ASCII_Symbol": filtered_data["Encode_addr1[7:0]"],
        "Length[4:0]": filtered_data["Length[4:0]"],
        "Codeword": filtered_data["Code[26:0]"]
    })

    # Write the encoding table to a new CSV file
    new_table.to_csv("encoding_table.csv", index=False) 

    print("\nEncoding table written to encoding_table.csv successfully!")
else:
    print("No rows match the filtering criteria.")

# Calculate and print the average length of the codewords
if not new_table.empty:
    average_length = new_table["Length[4:0]"].mean()
    print(f"\nAverage codeword length: {average_length} bits")