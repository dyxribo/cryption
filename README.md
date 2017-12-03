# cryption
simple command-line Base64 x XOR encryption for Windows

## usage
Open command prompt as administrator, and run:
```
cryption [ADD] [IDENTIFIER] [MESSAGE] [KEY]
```
OR 

```
cryption [GET] [IDENTIFIER] [KEY]
```

where:

* ```[ADD]``` tells cryption that you want to create a private key

* ```[GET]``` tells cryption that you want to retrieve a private key

* ```[IDENTIFIER]``` sets the name of the private key in order to retrieve it later

* ```[MESSAGE]``` is the message or password you want to encrypt

* ```[KEY]``` is the key used for encrypting ```[MESSAGE]```

## how it works

cryption first XORs ```[MESSAGE]```` using ```[KEY]```. then it base64-encrypts the result.

## example

Adding a private key

```
C:\Users\SnaiLegacy>cryption add test ThisIsATestMessageAndOrPassPhrase testkey
message successfully encrypted.

C:\Users\SnaiLegacy>
```

Retrieving a private key

```
C:\Users\SnaiLegacy>cryption get test testkey
before decryption:

"""U4%&J&+),% #%<P V9%2=704<!:        "

after decryption:

"ThisIsATestMessageAndOrPassPhrase"

message decrypted.


C:\Users\SnaiLegacy>
```

currently wondering if i should take out the retrieval message and just put the unencrypted string for development purposes, such as if someone wanted to use the output automatically from another program. i'll probably do it anyway.
