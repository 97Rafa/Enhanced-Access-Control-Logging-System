all: logger acmonitor test_aclog

logger: logger.c
	gcc -Wall -fPIC -shared -o logger.so logger.c -lcrypto -ldl 

acmonitor: acmonitor.c 
	gcc acmonitor.c -o acmonitor

test_aclog: test_aclog.c 
	gcc test_aclog.c -o test_aclog

run: logger.so test_aclog
	LD_PRELOAD=./logger.so ./test_aclog

rsa:
	gcc rsa_assign_1.c -o rsa_assign_1 -lgmp

generate: rsa_assign_1
	./rsa_assign_1 -g 

encrypt: rsa_assign_1
	./rsa_assign_1 -i file_logging.log -o encryptedFile.log -k public.key -e


decrypt: rsa_assign_1
	./rsa_assign_1 -i encryptedFile.log -o decryptedFile.log -k private.key -d

clean:
	rm -rf logger.so
	rm -rf test_aclog
	rm -rf acmonitor
	rm -rf file*
	rm -rf private.key public.key 
	rm -rf rsa_assign_1
	rm -rf *.log
