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
cryption [DEL] [IDENTIFIER] [KEY]
```

where:

* ```[ADD]``` tells cryption that you want to create a private key

* ```[GET]``` tells cryption that you want to retrieve a private key

* ```[DEL]``` tells cryption to delete a private key it previously created

* ```[IDENTIFIER]``` sets the name of the private key in order to retrieve it later

* ```[MESSAGE]``` is the message or password you want to encrypt

* ```[KEY]``` is the key used for encrypting ```[MESSAGE]```

## how it works

cryption first XORs ```[MESSAGE]```` using ```[KEY]```. then it base64-encrypts the result.

## example

Adding a private key

```
C:\Users\SnaiLegacy>cryption add test TestPassw0rd123!@# testKey
message successfully encrypted.

C:\Users\SnaiLegacy>
```

Retrieving a private key

```
C:\Users\SnaiLegacy>cryption get test testKey
TestPassw0rd123!@#

C:\Users\SnaiLegacy>
```

Deleting a private key

```
C:\Users\SnaiLegacy>cryption del test
private key successfully deleted.

C:\Users\SnaiLegacy>
```