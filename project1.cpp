#include <iostream>
#include <string>
using namespace std;

class Text2Compress
{
protected:
    int _seq[200000];    // stores the current sequence
    int _length;         // length of sequence
    int _freq[640][640]; // static matrix 128+k by 128+k
    int _rules[512][3];  // merge rules: a b -> z
    int _ruleCount;      // number of rules learned
    int _maxSymbol;      // highest assigned symbol ID

public:
    Text2Compress();
    void initialize(int k, int numLines); // read input text
    void train(int k);                    // perform k merges
    void encode();                        // apply learned merges
    void decode();                        // optional: expand compressed form
    void displaySequence();               // print current sequence
    void displayRules();                  // print learned rules
};

Text2Compress::Text2Compress()
{
    _length = 0;
    _ruleCount = 0;
    _maxSymbol = 127; // ASCII symbols range from 0 to 127

    for (int i = 0; i < 640; ++i)
    {
        for (int j = 0; j < 640; ++j)
        {
            _freq[i][j] = 0;
        }
    }
}

void Text2Compress::initialize(int k, int numLines)
{
    _length = 0; // Initialize length once
    std::string line;
    for (int i = 0; i < numLines; i++)
    {
        std::getline(std::cin, line);
        for (int j = 0; j < line.length(); j++)
        {
            _seq[_length] = line[j];
            _length++;
        }
    }
}

void Text2Compress::train(int k)
{
    for (int i = 0; i < k; ++i)
    {
        for (int row = 0; row < 640; ++row)
        {
            for (int col = 0; col < 640; ++col)
            {
                _freq[row][col] = 0;
            }
        }

        for (int j = 0; j < _length - 1; ++j)
        {
            int current_symbol = _seq[j];
            int next_symbol = _seq[j + 1];
            _freq[current_symbol][next_symbol]++;
        }

        int max_frequency = 0;
        int most_frequent_A = -1;
        int most_frequent_B = -1;

        int max_freq_range = _maxSymbol + 1;
        if (max_freq_range > 640)
        {
            max_freq_range = 640;
        }

        for (int row = 0; row < max_freq_range; ++row)
        {
            for (int col = 0; col < max_freq_range; ++col)
            {
                if (_freq[row][col] > max_frequency)
                {
                    max_frequency = _freq[row][col];
                    most_frequent_A = row;
                    most_frequent_B = col;
                }
                else if (_freq[row][col] == max_frequency && max_frequency > 0)
                {
                    if (row < most_frequent_A || (row == most_frequent_A && col < most_frequent_B))
                    {
                        most_frequent_A = row;
                        most_frequent_B = col;
                    }
                }
            }
        }

        if (max_frequency > 0)
        {
            int newSymbol = _maxSymbol + 1;
            _rules[_ruleCount][0] = most_frequent_A;
            _rules[_ruleCount][1] = most_frequent_B;
            _rules[_ruleCount][2] = newSymbol;
            _maxSymbol = newSymbol;
            _ruleCount++;

            int newLength = 0;
            int tempSeq[200000];

            for (int j = 0; j < _length; ++j)
            {
                if (j < _length - 1 && _seq[j] == most_frequent_A && _seq[j + 1] == most_frequent_B)
                {
                    tempSeq[newLength] = newSymbol;
                    newLength++;
                    j++;
                }
                else
                {
                    tempSeq[newLength] = _seq[j];
                    newLength++;
                }
            }

            for (int j = 0; j < newLength; ++j)
            {
                _seq[j] = tempSeq[j];
            }
            _length = newLength;
        }
        else
        {
            break;
        }
    }
}

void Text2Compress::displayRules()
{
    for (int i = 0; i < _ruleCount; ++i)
    {
        std::cout << _rules[i][0] << " " << _rules[i][1] << " " << _rules[i][2] << std::endl;
    }
}

void Text2Compress::displaySequence()
{
    for (int i = 0; i < _length; ++i)
    {
        std::cout << _seq[i] << " ";
    }
    std::cout << std::endl;
}

void Text2Compress::decode()
{
    int tempSeq[200000];

    // Apply the rules in reverse order
    for (int i = _ruleCount - 1; i >= 0; --i)
    {
        int originalA = _rules[i][0];
        int originalB = _rules[i][1];
        int newSymbol = _rules[i][2];

        int tempLength = 0;

        for (int j = 0; j < _length; ++j)
        {
            if (_seq[j] == newSymbol)
            {
                tempSeq[tempLength++] = originalA;
                tempSeq[tempLength++] = originalB;
            }
            else
            {
                tempSeq[tempLength++] = _seq[j];
            }
        }

        for (int k = 0; k < tempLength; ++k)
        {
            _seq[k] = tempSeq[k];
        }
        _length = tempLength;
    }

    for (int i = 0; i < _length; ++i)
    {
        cout << (char)_seq[i];
    }
    cout << endl;
}

int main()
{
    int k, numLines;
    // First row: k and number of lines of input
    cin >> k >> numLines;
    cout << "Successfully read k: " << k << " and numLines: " << numLines << endl; // Add this line

    while (cin.peek() == '\r' || cin.peek() == '\n')
    {
        cin.get(); // eat carriage returns and newlines
    }
    // Step 2: Create a Text2Compress object
    Text2Compress compressor;
    // Step 1: Read lines of input text
    // Store each character’s ASCII code (0–127) into the sequence array
    compressor.initialize(k, numLines);
    // Step 3: Train with k merges
    compressor.train(k);
    // Step 4: Display the learned rules
    cout << "Rules learned from Compression:" << endl;
    compressor.displayRules();
    // Step 5: Display the compressed sequence
    cout << "Compressed sequence:" << endl;
    compressor.displaySequence();
    // Step 6: Process decompression lines (triplets + sequence)
    cout << "Decompressed Text:" << endl;
    compressor.decode();
    // Step7: print the compressed text

    // You will write code to handle that part
    return 0;
}