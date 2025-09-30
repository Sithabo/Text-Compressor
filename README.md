# Text Compression with Byte Pair Encoding

This C++ project implements a basic text compression algorithm inspired by Byte Pair Encoding (BPE). The program reads a block of text, learns a set of merge rules by identifying the most frequent pairs of adjacent characters (or symbols), and then applies these rules to compress the text. It can also decompress the text back to its original form.

## How It Works

The core logic revolves around the `Text2Compress` class:

1.  **Initialization**: The program reads an initial text sequence. Each character is converted to its ASCII value and stored in an integer array.
2.  **Training**: The `train` method iteratively finds the most frequent pair of adjacent symbols in the current sequence.
    *   It creates a new symbol (with an ID greater than any existing symbol).
    *   It creates a rule to merge the pair into this new symbol (e.g., `A B -> Z`).
    *   It replaces all occurrences of the pair in the sequence with the new symbol.
    *   This process is repeated for a specified number of merges (`k`).
3.  **Compression**: After training, the learned rules are applied to the original text to produce the final compressed sequence.
4.  **Decompression**: The `decode` method reverses the process. It applies the learned rules in reverse order, expanding the merged symbols back into their original pairs until the text is fully restored.

## How to Compile and Run

### Compilation

You can compile the project using a standard C++ compiler like g++.

```bash
g++ project1.cpp -o compressor
```

### Execution

The program reads from standard input. You can run it and type the input directly, but it's more convenient to redirect input from a file.

An example `input1.txt` is provided. To run the program with this input:

```bash
# On Windows
.\compressor.exe < input1.txt

# On macOS/Linux
./compressor < input1.txt
```

## Input Format

The program expects a specific input format:

1.  **First Line**: Two integers separated by a space:
    *   `k`: The number of merge operations to perform during the training phase.
    *   `numLines`: The number of lines of text that follow, which will be used as the training data.
2.  **Training Text**: The next `numLines` of text.
3.  **Decompression Data (Optional)**: The current implementation in `main` also includes a `decode` step that reads from the same input stream. The `input1.txt` file contains sample data for this, which includes:
    *   The number of rules to read.
    *   The rule definitions (e.g., `32 116 128`).
    *   The compressed sequence to be decoded.

The program will output the learned rules, the compressed sequence, and the final decompressed text.
