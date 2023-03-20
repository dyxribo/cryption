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
OR 

```
cryption [DEL] [IDENTIFIER]
```

where:

* ```[ADD]``` tells cryption that you want to create a private key

* ```[GET]``` tells cryption that you want to retrieve a private key

* ```[DEL]``` tells cryption to delete a private key it previously created

* ```[IDENTIFIER]``` sets the name of the private key in order to retrieve it later

* ```[MESSAGE]``` is the message or password you want to encrypt

* ```[KEY]``` is the key used for encrypting ```[MESSAGE]```

## how it works

cryption first XORs ```[MESSAGE]``` using ```[KEY]```. 
then, it base64-encrypts the result, and saves the file in a folder called "pk" located in the same directory as cryption.

suggestion: add cryption to your windows PATH variable in order to call cryption from anywhere, and make keys with ease.

## example

What an encrypted message looks like

```""%/#;@:x(1>> 0!$```

Adding a private key

```
C:\Users\dyxribo>cryption add test TestPassw0rd123!@# testKey
message successfully encrypted.

C:\Users\dyxribo>
```

Retrieving a private key

```
C:\Users\dyxribo>cryption get test testKey
TestPassw0rd123!@#

C:\Users\dyxribo>
```

Deleting a private key

```
C:\Users\SnaiLegacy>cryption del test
private key successfully deleted.

C:\Users\dyxribo>
```
