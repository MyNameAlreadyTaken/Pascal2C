#pragma once
#define BUFFER_SIZE 21
#define DELIMITER_SIZE 20

#define ISCHAR 0
#define ISSEPRATE 1
#define ISDIGIT 2
#define ISDELIMITER 3
#define ISFLLOWT 4

#define UNKNOWWRONG 1
#define INVALIDATEFLLOWT 2
#define INVALIDATEDELIMITER 3
#define INVALIDATESTR 4

struct typeCode {
	typeCode* next;
	char* key;
	int value;
};

struct typeValue {
	int value;
	char* key;
	int line;
};