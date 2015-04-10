driver: driver.c
	gcc -Wall -g -c manager.c -o manager
	gcc -Wall -g driver.c manager -o driver

clean: 
	rm -f manager
	rm -f driver
	rm -fr driver.dSYM/* 
	rmdir driver.dSYM
	rm -f testFile.txt
	rm -fr testDirectory/* 
	rmdir testDirectory