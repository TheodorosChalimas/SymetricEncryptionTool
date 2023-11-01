#README for assigment2 course HPY414

Code in C programming language that utilizes a symmetric encryption tool using the EVP and CMAC API's from OpenSSL.

Folder contains:
	-assign_1.c
	"Contains the C code used for utilizing the toolkit."
	
	-Makefile
    	"make all <--make an executable called assign_1."
	
	-txt files
	"Used to evaluate the implementation."


TASKS:	1. >>./assign_1 -i encryptme_256.txt -o ciphertext.txt -p TUC2014030157 -b 256 -e
		The above command produces a ciphertext.txt file wich is then tested using the below command:
	   >>./assign_1 -i ciphertext.txt -o test.txt -p TUC2014030157 -b 256 -d
		After this we can see that the new test.txt file contains the information of the original eencryptme_256.txt.	
				-------------------------------
	2. >>./assign_1 -i hpy414_decryptme_128.txt -o hpy414_encryptme_128.txt -p hpy414 -b 128 -d
		The above comand produces a hpy414_encryptme_128.txt that is for some reason not FULLY decrepted(only a part of it is)!!
		My thoughts are that there is propably something wrong with the EVP_DecryptFinal_ex() and the final block was not decrypted/written but in task 1 i tried it with the file wich i had encrypted and it seemed to work fine.
		Whichever the case is if you are reading this i have not yet found where i (or my VM perhaps) messed up.
				-------------------------------
	3. >>./assign_1 -i signme_128.txt -o verifyme_128.txt -p TUC2014030157 -b 128 -s
		The above command produces a verifyme_128.txt file wich is the encryption and the CMAC concatenated and then tested using the below command:
	   >>./assign_1 -i verifyme_128.txt -o verifiedTest.txt -p TUC2014030157 -b 128 -v
		After this we can see that the new verifiedTest.txt file contains the information of the original signme_128.txt since the verify was succesfull!!	
				-------------------------------
	4. >>./assign_1 -i hpy414_verifyme_128.txt -o verifiedTest2.txt -p hpy414 -b 128 -v
		The above comand wasnt able to verify the hpy414_verifyme_128.txt because of the failure in the decryption function and since the CMAC was not the same.
	   >>./assign_1 -i hpy414_verifyme_256.txt -o verifiedTest2.txt -p hpy414 -b 256 -v
		The above comand wasnt able to verify the hpy414_verifyme_256.txt because of the failure in the decryption function and since the CMAC was not the same.


After running each command succesfull or not the program prints usefull information on the screen as such:
--------------------INFO-----------------------
The size of the input file is X bytes long
                HEX KEY:
XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX XX

-----(More depending on the selected mode)-----

--File xxxxxx.txt renewed-- 


There are comments on the code for further code explanation.
- Chalimas Theodoros <thodorischa@gmail.com>
 