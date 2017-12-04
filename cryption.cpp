#include <iostream>
#include <string>
#include <fstream>
#include <errno.h>
#include <unistd.h>
#include <Windows.h>

// error codes.
int NOT_ENOUGH_ARGS = 0;
int INCORRECT_ARGS = 1;
int NO_PKDIR = 2;
int ERROR_CREATING_PKDIR = 3;
int ERROR_GETTING_CWD = 4;
int ERROR_OPENING_FILE = 5;
int FILE_DOES_NOT_EXIST = 6;
int DELETE_FILE_ERROR = 7;

using namespace std;


// characters used for the b64 encryption.
static const string base64_chars =
    "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
    "abcdefghijklmnopqrstuvwxyz"
    "0123456789+/";

// check for b64 legitimacy.
static inline bool is_base64(unsigned char c)
{
    return (isalnum(c) || (c == '+') || (c == '/'));
}

// encode a c string to b64.
string base64_encode(unsigned char const *bytes_to_encode, unsigned int in_len)
{
    string ret;
    int i = 0;
    int j = 0;
    unsigned char char_array_3[3];
    unsigned char char_array_4[4];

    while (in_len--)
    {
        char_array_3[i++] = *(bytes_to_encode++);
        if (i == 3)
        {
            char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
            char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
            char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
            char_array_4[3] = char_array_3[2] & 0x3f;

            for (i = 0; (i < 4); i++)
                ret += base64_chars[char_array_4[i]];
            i = 0;
        }
    }

    if (i)
    {
        for (j = i; j < 3; j++)
            char_array_3[j] = '\0';

        char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
        char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
        char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
        char_array_4[3] = char_array_3[2] & 0x3f;

        for (j = 0; (j < i + 1); j++)
            ret += base64_chars[char_array_4[j]];

        while ((i++ < 3))
            ret += '=';
    }

    return ret;
}

// decodes a b64 encoded string.
string base64_decode(string const &encoded_string)
{
    int in_len = encoded_string.size();
    int i = 0;
    int j = 0;
    int in_ = 0;
    unsigned char char_array_4[4], char_array_3[3];
    string ret;

    while (in_len-- && (encoded_string[in_] != '=') && is_base64(encoded_string[in_]))
    {
        char_array_4[i++] = encoded_string[in_];
        in_++;
        if (i == 4)
        {
            for (i = 0; i < 4; i++)
                char_array_4[i] = base64_chars.find(char_array_4[i]);

            char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
            char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
            char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

            for (i = 0; (i < 3); i++)
                ret += char_array_3[i];
            i = 0;
        }
    }

    if (i)
    {
        for (j = i; j < 4; j++)
            char_array_4[j] = 0;

        for (j = 0; j < 4; j++)
            char_array_4[j] = base64_chars.find(char_array_4[j]);

        char_array_3[0] = (char_array_4[0] << 2) + ((char_array_4[1] & 0x30) >> 4);
        char_array_3[1] = ((char_array_4[1] & 0xf) << 4) + ((char_array_4[2] & 0x3c) >> 2);
        char_array_3[2] = ((char_array_4[2] & 0x3) << 6) + char_array_4[3];

        for (j = 0; (j < i - 1); j++)
            ret += char_array_3[j];
    }

    return ret;
}

// encrypts a string with b64 first, then encrypts the b64 encoded string with a XOR encryption.
string encrypt(string msg, string const &key)
{
    if (key.empty())
        return msg;

    unsigned char *tmp = (unsigned char *)&msg.c_str()[0];
    string b64EncodedString = base64_encode(tmp, msg.size());

    for (string::size_type i = 0; i < b64EncodedString.size(); ++i)
    {
        b64EncodedString[i] ^= key[i % key.size()];
    }

    return b64EncodedString;
}

//decrypts the XOR encoded string that was encrypted by encrypt(); then decrypts the resulting b64 string into plaintext.
string decrypt(string msg, string const &key)
{
    for (string::size_type i = 0; i < msg.size(); ++i)
    {
        msg[i] ^= key[i % key.size()];
    }

    string decodedB64 = base64_decode(msg);
    return decodedB64;
}

// sets the executable working directory (windows only, only reason it's not cross platform. i'll get a PC for GNU/linux soon, probably).
void setWorkingDirectory()
{
    TCHAR pBuf[MAX_PATH];
    int bytes = GetModuleFileName(NULL, pBuf, MAX_PATH);
    string s;

    if (bytes == 0)
    {
        cout << "GetModuleFileName failed" << endl;
        exit(ERROR_GETTING_CWD);
    }
    else
    {
        s = pBuf;
        s = s.substr(0, s.find_last_of("\\")) + "\\";
        _chdir(s.c_str());
    }
}

// creates the private key directory in order to store private keys saved by the program.
// once again, need a linux PC to test on more than one platform
void createPrivateKeyDirectory()
{
    int directoryStatus = _mkdir("pk");

    switch (errno)
    {
    case 0:
        cout << "successfully created private key directory.\n";
        break;
    case EEXIST:
        break;
    default:
        cerr << "problem creating private key directory. check permissions.\n";
        exit(ERROR_CREATING_PKDIR);
    }
}

// user error!
void showUsageError()
{
    cerr << "Usage: "
         << "cryption "
         << "[ADD] "
         << "[IDENTIFIER] "
         << "[MESSAGE] "
         << "[KEY] " << endl
         << "OR" << endl
         << "cryption "
         << "[GET] "
         << "[IDENTIFIER] "
         << "[KEY]" << endl
         << "OR" << endl
         << "cryption "
         << "[DEL] "
         << "[IDENTIFIER] "
         << "[KEY]" << endl;
}

// moves into the private key directory to check for the specified private key (name).
// exits the program if it doesnt. also checks if the private key directory exists.
void checkForPrivateKey(string name)
{
    if (_chdir("pk"))
    {
        if (errno == ENOENT)
        {
            cout << "private key directory does not exist, so there's no keys to load; creating directory anyway...\n";
            createPrivateKeyDirectory();
        }
    }
    else
    {
        if (access(name.c_str(), F_OK) == -1)
        {
            cerr << "that private key does not exist.\n";
            exit(FILE_DOES_NOT_EXIST);
        }
    }
    _chdir("..");
}

int main(int numArgs, char *argVector[])
{
    // make sure there is the minimum amount of arguments at least to do a delete operation
    if (numArgs < 3)
    {
        showUsageError();
        return NOT_ENOUGH_ARGS;
    }

    // make all arguments lowercase for simplicity reasons
    for (int i = 0; i < numArgs; i++)
    {
        string currentArg = argVector[i];
        for (int j = 0; j < currentArg.length(); j++)
        {
            currentArg[j] = tolower(currentArg[j]);
        }
    }

    // create a variable for the operation name for ease of access
    string operationName = argVector[1];

    // check for argument errors; make sure the operation name isn't something other than "add", "get", or "del"
    if (!(operationName == "get") && !(operationName == "add") && !(operationName == "del"))
    {
        showUsageError();
        return INCORRECT_ARGS;
    }

    // sets a variable based on whether the operation is get or add
    bool isGet = (operationName == "get");
    bool isDel = (operationName == "del");

    //sets the working directory for file operations
    setWorkingDirectory();

    // if the operation is a "get" operation (for getting the encrypted keys)
    if (isGet)
    {
         // if there are less than 4 args, get operations cant be run
        if (numArgs < 4)
        {
            cout << "not enough args for \"get\"." << endl;
            showUsageError();
            return NOT_ENOUGH_ARGS;
        }

        // check if the private key (pk) specified by the user exists
        checkForPrivateKey((string)argVector[2]);

        // load the file
        ifstream storedEncrypt("pk\\" + (string)argVector[2]);

        // make a variable for storing the characters from the file
        string line;

        // if the file is open...
        if (storedEncrypt.is_open())
        {
            // read all the characters from the file
            string contents = "";
            while (getline(storedEncrypt, line))
            {
                contents = contents + line;
            }

            // print out the encrypted and decrypted message to the user;
            // if decrypt is given the wrong key, the message will not be correct
            cout << "\"" << decrypt(contents, argVector[3]) << "\"" << endl;
            storedEncrypt.close();
        }
        else // if the file is not open...
        {
            // just put out an error message
            cerr << "unable to open file.\n";
            return ERROR_OPENING_FILE;
        }
    }
    else if (!isGet && !isDel) // if the operation is a "add" operation (for setting the encrypted keys)
    {
        // if there are less than 5 args, add operations cant be run
        if (numArgs < 5)
        {
            cout << "not enough args for \"add\"." << endl;
            showUsageError();
            return NOT_ENOUGH_ARGS;
        }

        // check for pk directory, create if if it doesn't exist
        createPrivateKeyDirectory();

        // create a new private key
        ofstream encryptedStore("pk\\" + (string)argVector[2]);

        // if the file is open
        if (encryptedStore.is_open())
        {
            // encrypt message and save it to the file
            encryptedStore << encrypt(argVector[3], argVector[4]);
            encryptedStore.close();
            cout << "message successfully encrypted.\n";
        }
        else // but if the file is closed
        {
            // error tiiiime!
            cerr << "unable to open file.\n";
            return ERROR_OPENING_FILE;
        }
    }
    else // otherwise, the operation is definitely a "del" (delete) operation
    {
        // check to see if the pk even exists before starting
        checkForPrivateKey((string)argVector[2]);
        
        // store the relative file path of the file to delete as a const char for remove()
        string fileToDelete = ("pk\\" + (string)argVector[2]);
        
        // attempt to delete the key
        if (remove(fileToDelete.c_str()) == 0)
        {
            cout << "private key successfully deleted." << endl;
        }
        else // if it doesn't work, what do we do?
        {
            // that's right, throw that error like a football at the superbowl!
            cerr << "there was a problem deleting the private key (" << fileToDelete << ").";
            return DELETE_FILE_ERROR;
        }
    }
    // program complete with no errors!
    return 0;
}